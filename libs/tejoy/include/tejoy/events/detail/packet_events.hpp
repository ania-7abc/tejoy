// packet_events.hpp
#pragma once

#include <tejoy/user.hpp>

#include <cstdint>
#include <string>

#include <nlohmann/json.hpp>
#include <event_system/event.hpp>

namespace tejoy::events::detail
{

  struct PacketReceived : event_system::Event
  {
    PacketReceived(std::string message, std::string ip, uint16_t port)
        : message(std::move(message)), ip(std::move(ip)), port(port) {}
    std::string message;
    std::string ip;
    uint16_t port;
  };

  struct SendPacketRequest : event_system::Event
  {
    SendPacketRequest(std::string message, std::string ip, uint16_t port)
        : message(std::move(message)), ip(std::move(ip)), port(port) {}
    std::string message;
    std::string ip;
    uint16_t port;
  };

  struct UpdateReceived : event_system::Event
  {
    UpdateReceived(nlohmann::json update, tejoy::User from)
        : update(std::move(update)), from(std::move(from)) {}
    nlohmann::json update;
    tejoy::User from;
  };

  struct SendAckUpdateRequest : event_system::Event
  {
    SendAckUpdateRequest(const UpdateReceived &e)
        : pkg_id(e.update.at("pkg_id").get<uint32_t>()), to(e.from) {}
    uint32_t pkg_id;
    tejoy::User to;
  };

  struct SendUpdateRequest : event_system::Event
  {
    SendUpdateRequest(nlohmann::json update, tejoy::User to)
        : update(std::move(update)), to(std::move(to)) {}
    nlohmann::json update;
    tejoy::User to;
  };

} // namespace tejoy::events::detail
