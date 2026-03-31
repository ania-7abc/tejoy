// multicast.hpp
#pragma once
#include <event_system/event.hpp>
#include <string>

namespace tejoy::events::detail
{

struct JoinMulticastGroup : event_system::Event
{
    explicit JoinMulticastGroup(std::string multicast_ip) : multicast_ip(std::move(multicast_ip))
    {
    }
    std::string multicast_ip;
};

struct LeaveMulticastGroup : event_system::Event
{
    explicit LeaveMulticastGroup(std::string multicast_ip) : multicast_ip(std::move(multicast_ip))
    {
    }
    std::string multicast_ip;
};

} // namespace tejoy::events::detail
