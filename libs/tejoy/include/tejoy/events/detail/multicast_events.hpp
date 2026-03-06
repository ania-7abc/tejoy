// multicast_events.hpp
#pragma once

#include <string>

#include <event_system/event.hpp>

namespace tejoy::events::detail
{

  struct JoinMulticastGroupRequest : event_system::Event
  {
    JoinMulticastGroupRequest(std::string ip) : ip(std::move(ip)) {}
    std::string ip;
  };

  struct LeaveMulticastGroupRequest : event_system::Event
  {
    LeaveMulticastGroupRequest(std::string ip) : ip(std::move(ip)) {}
    std::string ip;
  };

} // namespace tejoy::events::detail
