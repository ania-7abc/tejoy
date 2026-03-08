// multicast_events.hpp
#pragma once

#include <string>

#include <event_system/event.hpp>

namespace tejoy::events::detail
{

struct JoinMulticastGroupRequest : event_system::Event
{
    explicit JoinMulticastGroupRequest(std::string multicast_ip) : multicast_ip(std::move(multicast_ip))
    {
    }
    std::string multicast_ip;
};

struct LeaveMulticastGroupRequest : event_system::Event
{
    explicit LeaveMulticastGroupRequest(std::string multicast_ip) : multicast_ip(std::move(multicast_ip))
    {
    }
    std::string multicast_ip;
};

} // namespace tejoy::events::detail
