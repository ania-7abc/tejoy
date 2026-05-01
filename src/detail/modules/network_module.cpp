// network_module.cpp
#include <iostream>
#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/events/detail/multicast.hpp>
#include <tejoy/events/network.hpp>

namespace tejoy::detail::modules
{

NetworkModule::NetworkModule(event_system::EventBus &bus, nlohmann::json &config)
    : Module(bus, config), port_(config_.emplace("port", static_cast<uint16_t>(5768)).first.value().get<uint16_t>()),
      udp_(port_)
{
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    config_.emplace("print", false).first.value().get_to(print_);
    config_["loss"].emplace("percent", 0).first.value().get_to(loss_percent_);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

    loss_dist_ = std::bernoulli_distribution(loss_percent_);

    udp_.set_callback([this](auto &message, auto &sender_ip, auto sender_port) {
        on_network_message(message, sender_ip, sender_port);
    });
}

void NetworkModule::run_subscribes()
{
    subscribe<events::detail::SendPacketRequest>([this](auto &event) { on_send_packet_request(event); });

    subscribe<events::RequestPort>([this](auto &event) { event.on_result(port_); });
    subscribe<events::RequestIp>([](auto &event) { event.on_result("127.0.0.1"); });

    subscribe<events::detail::JoinMulticastGroup>(
        [this](auto &event) { udp_.join_multicast_group(event.multicast_ip); });
    subscribe<events::detail::LeaveMulticastGroup>(
        [this](auto &event) { udp_.leave_multicast_group(event.multicast_ip); });
}

void NetworkModule::on_send_packet_request(const events::detail::SendPacketRequest &event)
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    if (loss_dist_(gen_))
    {
        if (print_)
        {
            std::cout << "Not sent message \"" << event.message << "\" to " << event.recipient_ip << ":"
                      << event.recipient_port << '\n';
        }
        return;
    }
    udp_.send(event.message, event.recipient_ip, event.recipient_port);
    if (print_)
    {
        std::cout << "Sent message \"" << event.message << "\" to " << event.recipient_ip << ":" << event.recipient_port
                  << '\n';
    }
}

void NetworkModule::on_network_message(const std::string &message, const std::string &sender_ip,
                                       uint16_t sender_port) const
{
    if (print_)
        std::cout << "Received message \"" << message << "\" from " << sender_ip << ":" << sender_port << '\n';
    publish<events::detail::PacketReceived>(message, sender_ip, sender_port);
}

} // namespace tejoy::detail::modules
