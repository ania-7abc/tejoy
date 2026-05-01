// others.hpp
#pragma once
#include <event_system/event.hpp>
#include <tejoy/user.hpp>

namespace tejoy::events
{

struct RequestI : event_system::Event
{
    explicit RequestI(std::function<void(User)> on_result) : on_result(std::move(on_result))
    {
    }
    std::function<void(User)> on_result;
};

} // namespace tejoy::events
