// discover_module.hpp
#include <tejoy/detail/modules/discovery_module.hpp>
#include <tejoy/events/detail/multicast.hpp>
#include <tejoy/events/discovery.hpp>
#include <tejoy/events/network.hpp>
#include <tejoy/events/others.hpp>

namespace tejoy::detail::modules
{

DiscoveryModule::DiscoveryModule(event_system::EventBus &bus, nlohmann::json &config)
    : Module(bus, config), timer_(io_context_)
{
}

void DiscoveryModule::on_start()
{
    config_.emplace("discovery_ip", "239.116.101.97").first.value().get_to(discovery_ip_);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    config_.emplace("ping_interval_s", static_cast<std::size_t>(15)).first.value().get_to(ping_interval_s_);
    config_.emplace("anonymous", false).first.value().get_to(anonymous_);
    bool send_allo = config_.emplace("send_allo", true).first.value().get<bool>();
    config_.emplace("discover_i", false).first.value().get_to(discover_i_);

    auto promise_port = std::promise<uint16_t>();
    auto future_port = promise_port.get_future();
    publish<events::RequestPort>(promise_port);
    port_ = future_port.get();

    auto promise_i = std::promise<User>();
    auto future_i = promise_i.get_future();
    publish<events::RequestI>(promise_i);
    i_ = future_i.get();

    work_guard_ = std::make_unique<boost::asio::io_context::work>(io_context_);
    io_thread_ = std::thread([this] { io_context_.run(); });

    timer_ = std::move(boost::asio::steady_timer(io_context_));
    if (send_allo)
        start_timer();

    subscribe<events::RequestDiscoveryIp>([this](auto &event) { event.promise.set_value(discovery_ip_); });

    subscribe_update(detail::UpdateTypes::DIS_FIND, [this](auto &event) { on_dis_find_received(event); });
    subscribe_update(detail::UpdateTypes::DIS_OK, [this](auto &event) { on_dis_ok_received(event); });

    publish<events::detail::JoinMulticastGroup>(discovery_ip_);
}

void DiscoveryModule::on_stop()
{
    publish<events::detail::LeaveMulticastGroup>(discovery_ip_);

    timer_.cancel();

    work_guard_.reset();
    io_context_.stop();
    if (io_thread_.joinable())
        io_thread_.join();
}

void DiscoveryModule::on_dis_find_received(const events::detail::UpdateReceived &event)
{
    if (event.sender.box.get_public_key() == i_.box.get_public_key() && !discover_i_)
        return;
    publish<events::DiscoveredNewNode>(event.sender);
    if (!anonymous_)
        publish<events::detail::SendUpdateRequest>(nlohmann::json::object(), detail::UpdateTypes::DIS_OK, event.sender);
}

void DiscoveryModule::on_dis_ok_received(const events::detail::UpdateReceived &event)
{
    publish<events::DiscoveredNewNode>(event.sender);
}

void DiscoveryModule::on_timer()
{
    User recipient{.ip = discovery_ip_, .port = port_};
    publish<events::detail::SendUpdateRequest>(nlohmann::json::object(), detail::UpdateTypes::DIS_FIND, recipient,
                                               true);
    start_timer();
}

void DiscoveryModule::start_timer()
{
    timer_.expires_after(std::chrono::seconds(ping_interval_s_));
    timer_.async_wait([this](auto error_code) {
        if (!error_code)
            on_timer();
    });
}

} // namespace tejoy::detail::modules
