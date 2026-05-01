// discover_module.hpp
#include <iostream>
#include <tejoy/detail/modules/discovery_module.hpp>
#include <tejoy/events/detail/multicast.hpp>
#include <tejoy/events/discovery.hpp>
#include <tejoy/events/network.hpp>
#include <tejoy/events/others.hpp>

namespace tejoy::detail::modules
{

DiscoveryModule::DiscoveryModule(event_system::EventBus &bus, nlohmann::json &config)
    : Module(bus, config), work_guard_(boost::asio::make_work_guard(io_context_)), timer_(io_context_)
{
    config_.emplace("discovery_ip", "239.116.101.97").first.value().get_to(discovery_ip_);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    config_.emplace("ping_interval_s", static_cast<std::size_t>(15)).first.value().get_to(ping_interval_s_);
    config_.emplace("anonymous", false).first.value().get_to(anonymous_);
    const bool send_allo = config_.emplace("send_allo", true).first.value().get<bool>();
    config_.emplace("discover_i", false).first.value().get_to(discover_i_);

    auto promise_port = std::make_shared<std::promise<uint16_t>>();
    auto future_port = promise_port->get_future();
    publish<events::RequestPort>([promise_port](const uint16_t port) mutable { promise_port->set_value(port); });
    port_ = future_port.get();

    auto promise_i = std::make_shared<std::promise<User>>();
    auto future_i = promise_i->get_future();
    publish<events::RequestI>([promise_i](const User &user) mutable { promise_i->set_value(user); });
    i_ = future_i.get();

    io_thread_ = std::jthread([this] { io_context_.run(); });

    timer_ = boost::asio::steady_timer(io_context_);
    if (send_allo)
        start_timer();

    publish<events::detail::JoinMulticastGroup>(discovery_ip_);
}

void DiscoveryModule::run_subscribes()
{
    subscribe<events::RequestDiscoveryIp>([this](auto &event) { event.on_result(discovery_ip_); });
    subscribe_update(UpdateTypes::DIS_FIND, [this](auto &event) { on_dis_find_received(event); });
    subscribe_update(UpdateTypes::DIS_OK, [this](auto &event) { on_dis_ok_received(event); });
}

DiscoveryModule::~DiscoveryModule()
{
    publish<events::detail::LeaveMulticastGroup>(discovery_ip_);

    try
    {
        timer_.cancel();
    }
    catch (boost::system::system_error &e)
    {
        std::cerr << e.what() << std::endl;
    }

    work_guard_.reset();
    io_context_.stop();
    if (io_thread_.joinable())
        io_thread_.join();
}

void DiscoveryModule::on_dis_find_received(const events::detail::UpdateReceived &event) const
{
    if (event.sender.box.get_public_key() == i_.box.get_public_key() && !discover_i_)
        return;
    publish<events::DiscoveredNewNode>(event.sender);
    if (!anonymous_)
        publish<events::detail::SendUpdateRequest>(nlohmann::json::object(), UpdateTypes::DIS_OK, event.sender);
}

void DiscoveryModule::on_dis_ok_received(const events::detail::UpdateReceived &event) const
{
    publish<events::DiscoveredNewNode>(event.sender);
}

void DiscoveryModule::on_timer()
{
    User recipient{.ip = discovery_ip_, .port = port_};
    publish<events::detail::SendUpdateRequest>(nlohmann::json::object(), UpdateTypes::DIS_FIND, recipient, true);
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
