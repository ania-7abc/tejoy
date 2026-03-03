// message.hpp
#pragma once

#include <tejoy/user.hpp>

#include <string>

#include <event_system/event.hpp>

namespace tejoy::events
{

  struct SendMessageRequest : event_system::Event
  {
    SendMessageRequest(std::string text, User to) : text(std::move(text)), to(std::move(to)) {}
    std::string text;
    tejoy::User to;
  };

} // namespace tejoy::events
