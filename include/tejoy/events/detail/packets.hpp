// packets.hpp
#pragma once
#include <cstdint>
#include <event_system/event.hpp>
#include <string>

namespace tejoy::events::detail
{

struct PacketReceived : event_system::Event
{
    explicit PacketReceived() = default;
    explicit PacketReceived(std::string message, std::string sender_ip, const uint16_t sender_port)
        : message(std::move(message)), sender_ip(std::move(sender_ip)), sender_port(sender_port)
    {
    }
    std::string message;
    std::string sender_ip;
    uint16_t sender_port{};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(PacketReceived, message, sender_ip, sender_port)
};

struct SendPacketRequest : event_system::Event
{
    explicit SendPacketRequest() = default;
    explicit SendPacketRequest(std::string message, std::string recipient_ip, const uint16_t recipient_port)
        : message(std::move(message)), recipient_ip(std::move(recipient_ip)), recipient_port(recipient_port)
    {
    }
    std::string message;
    std::string recipient_ip;
    uint16_t recipient_port{};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SendPacketRequest, message, recipient_ip, recipient_port)
};

} // namespace tejoy::events::detail
