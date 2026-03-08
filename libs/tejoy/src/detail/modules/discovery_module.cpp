// discover_module.hpp

#include <tejoy/detail/modules/discovery_module.hpp>
#include <tejoy/events/data_requests.hpp>
#include <tejoy/events/detail/multicast_events.hpp>
#include <tejoy/events/detail/packet_events.hpp>
#include <tejoy/events/discovery.hpp>

namespace tejoy::detail::modules
{

DiscoveryModule::DiscoveryModule(event_system::EventBus &bus, nlohmann::json &config)
    : Module(bus, config), timer_(io_context_)
{
}

void DiscoveryModule::on_start()
{
    config_.emplace("discovery_ip", "239.116.101.97").first.value().get_to(discovery_ip_);
    if (!config_.contains("discovery_port"))
    {
        std::promise<uint16_t> promise;
        auto fut = promise.get_future();
        publish<events::RequestPort>(promise);

        config_["discovery_port"] = discovery_port_ = fut.get();
    }
    else
    {
        config_["discovery_port"].get_to(discovery_port_);
    }
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    config_.emplace("ping_interval_s", static_cast<std::size_t>(15)).first.value().get_to(ping_interval_s_);
    config_.emplace("anonymous", false).first.value().get_to(anonymous_);
    bool send_allo = config_.emplace("send_allo", true).first.value().get<bool>();

    work_guard_ = std::make_unique<boost::asio::io_context::work>(io_context_);
    io_thread_ = std::thread([this] { io_context_.run(); });

    timer_ = std::move(boost::asio::steady_timer(io_context_));
    if (send_allo)
    {
        start_timer();
    }

    subscribe<events::RequestDiscoveryIp>([this](auto &event) { event.promise.set_value(discovery_ip_); });
    subscribe<events::RequestDiscoveryPort>([this](auto &event) { event.promise.set_value(discovery_port_); });

    subscribe<events::AlloUpdateReceived>([this](auto &event) { on_allo_received(event); });
    subscribe<events::ImokUpdateReceived>([this](auto &event) { on_imok_received(event); });

    publish<events::detail::JoinMulticastGroupRequest>(discovery_ip_);
}

void DiscoveryModule::on_stop()
{
    publish<events::detail::LeaveMulticastGroupRequest>(discovery_ip_);

    timer_.cancel();

    work_guard_.reset();
    io_context_.stop();
    if (io_thread_.joinable())
    {
        io_thread_.join();
    }
}

void DiscoveryModule::on_allo_received(const events::AlloUpdateReceived &event)
{
    if (event.from.box.get_public_key() == i_.box.get_public_key())
    {
        return;
    }
    publish<events::DiscoveredNewNode>(event.from);
    if (!anonymous_)
    {
        publish<events::SendConfiguredUpdateRequest>(nlohmann::json::object(), "imok", event.from, false, true);
    }
}

void DiscoveryModule::on_imok_received(const events::ImokUpdateReceived &event)
{
    publish<events::DiscoveredNewNode>(event.from);
}

void DiscoveryModule::on_timer()
{
    User recipient{.ip = discovery_ip_, .port = discovery_port_};
    publish<events::SendConfiguredUpdateRequest>(nlohmann::json::object(), "allo", recipient, true, true);
    start_timer();
}

void DiscoveryModule::start_timer()
{
    timer_.expires_after(std::chrono::seconds(ping_interval_s_));
    timer_.async_wait([this](auto error_code) {
        if (!error_code)
        {
            on_timer();
        }
    });
}

} // namespace tejoy::detail::modules
