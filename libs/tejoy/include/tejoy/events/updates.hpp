// updates.hpp
#pragma once

#include <tejoy/user.hpp>

#include <string>

#include <event_system/event.hpp>

namespace tejoy::events
{

  struct MessageUpdateReceived : event_system::Event
  {
    MessageUpdateReceived(std::string text, uint32_t pkg_id, User from)
        : text(std::move(text)), pkg_id(pkg_id), from(std::move(from)) {}
    std::string text;
    uint32_t pkg_id;
    User from;
  };

  struct AckUpdateReceived : event_system::Event
  {
    AckUpdateReceived(uint32_t pkg_id, User from)
        : pkg_id(pkg_id), from(std::move(from)) {}
    uint32_t pkg_id;
    User from;
  };

  struct AlloUpdateReceived : event_system::Event
  {
    AlloUpdateReceived(User from) : from(std::move(from)) {}
    User from;
  };

  struct ImokUpdateReceived : event_system::Event
  {
    ImokUpdateReceived(User from) : from(std::move(from)) {}
    User from;
  };

  struct SendConfiguredUpdateRequest : event_system::Event
  {
    SendConfiguredUpdateRequest(nlohmann::json data, std::string type, User to, bool no_encrypt = false, bool no_ack = false)
        : data(std::move(data)), type(std::move(type)), to(std::move(to)), no_encrypt(no_encrypt), no_ack(no_ack) {}
    nlohmann::json data;
    std::string type;
    User to;
    bool no_encrypt;
    bool no_ack;
  };

} // namespace tejoy::events
