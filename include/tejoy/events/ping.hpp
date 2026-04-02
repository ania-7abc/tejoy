// ping.hpp
#pragma once
#include <event_system/event.hpp>
#include <tejoy/user.hpp>

namespace tejoy::events
{

struct PingRequest : event_system::Event
{
    explicit PingRequest(User ping_user) : ping_user(std::move(ping_user))
    {
    }
    User ping_user;
};

struct PingOk : event_system::Event
{
    explicit PingOk(User ping_user) : ping_user(std::move(ping_user))
    {
    }
    User ping_user;
};

struct PingFailed : event_system::Event
{
    explicit PingFailed(User ping_user) : ping_user(std::move(ping_user))
    {
    }
    User ping_user;
};

} // namespace tejoy::events
