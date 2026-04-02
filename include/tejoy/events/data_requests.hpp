// data_requests.hpp
#pragma once

#include <tejoy/user.hpp>

#include <cstdint>
#include <future>

#include <event_system/event.hpp>

namespace tejoy::events
{

// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)

struct RequestIp : event_system::Event
{
    explicit RequestIp(std::promise<std::string> &promise) : promise(promise)
    {
    }
    std::promise<std::string> &promise;
};

struct RequestPort : event_system::Event
{
    explicit RequestPort(std::promise<uint16_t> &promise) : promise(promise)
    {
    }
    std::promise<uint16_t> &promise;
};

struct RequestDiscoveryIp : event_system::Event
{
    explicit RequestDiscoveryIp(std::promise<std::string> &promise) : promise(promise)
    {
    }
    std::promise<std::string> &promise;
};

struct RequestI : event_system::Event
{
    explicit RequestI(std::promise<tejoy::User> &promise) : promise(promise)
    {
    }
    std::promise<tejoy::User> &promise;
};

// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

} // namespace tejoy::events
