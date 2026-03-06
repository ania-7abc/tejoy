// discover.hpp
#pragma once

#include <tejoy/user.hpp>

#include <event_system/event.hpp>

namespace tejoy::events
{

  struct DiscoveredNewNode : event_system::Event
  {
    DiscoveredNewNode(User node) : node(std::move(node)) {}
    User node;
  };

} // namespace tejoy::events
