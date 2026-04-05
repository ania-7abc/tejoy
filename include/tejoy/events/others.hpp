// others.hpp
#pragma once
#include <event_system/event.hpp>
#include <future>
#include <tejoy/user.hpp>

namespace tejoy::events
{

struct RequestI : event_system::Event
{
    explicit RequestI(std::promise<User> &promise) : promise(promise)
    {
    }
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
    std::promise<User> &promise;
};

} // namespace tejoy::events
