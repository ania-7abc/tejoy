// packet_events.hpp
#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>
#include <secret_box.hpp>
#include <event_system/event.hpp>
#include <tejoy/user.hpp>

struct PacketReceivedEvent : event_system::Event
{
    PacketReceivedEvent(std::string message, std::string ip, uint16_t port)
        : message(std::move(message)), ip(std::move(ip)), port(port) {}
    std::string message;
    std::string ip;
    uint16_t port;
};

struct NeedSendPacketEvent : event_system::Event
{
    NeedSendPacketEvent(std::string message, std::string ip, uint16_t port)
        : message(std::move(message)), ip(std::move(ip)), port(port) {}
    std::string message;
    std::string ip;
    uint16_t port;
};

struct UpdateReceivedEvent : event_system::Event
{
    UpdateReceivedEvent(nlohmann::json update, tejoy::User from)
        : update(update), from(from) {}
    const nlohmann::json update;
    const tejoy::User from;
};

struct NeedSendUpdateEvent : event_system::Event
{
    NeedSendUpdateEvent(nlohmann::json update, tejoy::User to)
        : update(update), to(to) {}
    const nlohmann::json update;
    const tejoy::User to;
};
