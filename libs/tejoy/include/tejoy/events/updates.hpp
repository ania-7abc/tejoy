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

} // namespace tejoy::events
