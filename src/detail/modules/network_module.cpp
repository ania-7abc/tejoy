// network_module.cpp
#include <iostream>
#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/events/data_requests.hpp>
#include <tejoy/events/detail/multicast.hpp>

namespace tejoy::detail::modules
{

void NetworkModule::on_start()
{
    srand(time(nullptr));

    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    config_.emplace("print", false).first.value().get_to(print_);
    config_.emplace("port", static_cast<uint16_t>(5768)).first.value().get_to(port_);
    config_["loss"].emplace("enable", false).first.value().get_to(simulate_loss_);
    config_["loss"].emplace("percent", 40).first.value().get_to(loss_percent_);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

    udp_.emplace(port_);
    udp_.value().set_callback([this](auto &message, auto &sender_ip, auto sender_port) {
        on_network_message(message, sender_ip, sender_port);
    });

    subscribe<events::detail::SendPacketRequest>([this](auto &event) { on_send_packet_request(event); });

    subscribe<events::RequestPort>([this](auto &event) { event.promise.set_value(port_); });
    subscribe<events::RequestIp>([](auto &event) { event.promise.set_value("127.0.0.1"); });

    subscribe<events::detail::JoinMulticastGroup>(
        [this](auto &event) { udp_.value().join_multicast_group(event.multicast_ip); });
    subscribe<events::detail::LeaveMulticastGroup>(
        [this](auto &event) { udp_.value().leave_multicast_group(event.multicast_ip); });
}

void NetworkModule::on_stop()
{
    udp_.value().stop();
    udp_.reset();
}

void NetworkModule::on_send_packet_request(const events::detail::SendPacketRequest &event)
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    if (simulate_loss_ && rand() % 100 < loss_percent_)
    {
        if (print_)
        {
            std::cout << "Not sent message \"" << event.message << "\" to " << event.recipient_ip << ":"
                      << event.recipient_port << '\n';
        }
        return;
    }
    udp_.value().send(event.message, event.recipient_ip, event.recipient_port);
    if (print_)
    {
        std::cout << "Sent message \"" << event.message << "\" to " << event.recipient_ip << ":" << event.recipient_port
                  << '\n';
    }
}

void NetworkModule::on_network_message(const std::string &message, const std::string &sender_ip, uint16_t sender_port)
{
    if (print_)
    {
        std::cout << "Received message \"" << message << "\" from " << sender_ip << ":" << sender_port << '\n';
    }
    publish<events::detail::PacketReceived>(message, sender_ip, sender_port);
}

} // namespace tejoy::detail::modules
