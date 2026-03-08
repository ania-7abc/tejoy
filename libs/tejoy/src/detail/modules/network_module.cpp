// network_module.cpp

#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/events/data_requests.hpp>
#include <tejoy/events/detail/multicast_events.hpp>

#include <iostream>

namespace tejoy::detail::modules
{

void NetworkModule::on_start()
{
    srand(time(nullptr));

    config_.emplace("print", false);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    config_.emplace("port", static_cast<uint16_t>(5768));
    config_["loss"].emplace("enable", false);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    config_["loss"].emplace("percent", 40);

    print_ = config_["print"].get<bool>();
    simulate_loss_ = config_["loss"]["enable"].get<bool>();
    loss_percent_ = config_["loss"]["percent"].get<int>();
    uint16_t port = config_["port"].get<uint16_t>();

    udp_.emplace(port);
    udp_.value().set_callback([this](auto &message, auto &sender_ip, auto sender_port) {
        on_network_message(message, sender_ip, sender_port);
    });
    subscribe<events::detail::SendPacketRequest>([this](auto &event) { on_send_packet_request(event); });

    subscribe<events::detail::JoinMulticastGroupRequest>(
        [this](auto &event) { udp_.value().join_multicast_group(event.multicast_ip); });
    subscribe<events::detail::LeaveMulticastGroupRequest>(
        [this](auto &event) { udp_.value().leave_multicast_group(event.multicast_ip); });

    subscribe<events::RequestPort>([port](auto &event) { event.promise.set_value(port); });
    subscribe<events::RequestIp>([](auto &event) { event.promise.set_value("127.0.0.1"); });
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
            std::cout << "Not sent message \"" << event.message << "\" recipient " << event.recipient_ip << ":"
                      << event.recipient_port << '\n';
        }
        return;
    }
    udp_.value().send(event.message, event.recipient_ip, event.recipient_port);
    if (print_)
    {
        std::cout << "Sent message \"" << event.message << "\" recipient " << event.recipient_ip << ":"
                  << event.recipient_port << '\n';
    }
}

void NetworkModule::on_network_message(const std::string &message, const std::string &sender_ip, uint16_t sender_port)
{
    publish<events::detail::PacketReceived>(message, sender_ip, sender_port);
}

} // namespace tejoy::detail::modules
