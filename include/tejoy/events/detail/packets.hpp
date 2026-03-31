// packets.hpp
#pragma once
#include <cstdint>
#include <event_system/event.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <tejoy/user.hpp>

namespace tejoy::events::detail
{

struct PacketReceived : event_system::Event
{
    PacketReceived(std::string message, std::string sender_ip, uint16_t sender_port)
        : message(std::move(message)), sender_ip(std::move(sender_ip)), sender_port(sender_port)
    {
    }
    std::string message;
    std::string sender_ip;
    uint16_t sender_port;
};

struct SendPacketRequest : event_system::Event
{
    SendPacketRequest(std::string message, std::string recipient_ip, uint16_t recipient_port)
        : message(std::move(message)), recipient_ip(std::move(recipient_ip)), recipient_port(recipient_port)
    {
    }
    std::string message;
    std::string recipient_ip;
    uint16_t recipient_port;
};

} // namespace tejoy::events::detail
