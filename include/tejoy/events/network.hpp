// network.hpp
#pragma once
#include <cstdint>
#include <event_system/event.hpp>
#include <future>

namespace tejoy::events
{

struct RequestIp : event_system::Event
{
    explicit RequestIp(std::promise<std::string> &promise) : promise(promise)
    {
    }
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
    std::promise<std::string> &promise;
};

struct RequestPort : event_system::Event
{
    explicit RequestPort(std::promise<uint16_t> &promise) : promise(promise)
    {
    }
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
    std::promise<uint16_t> &promise;
};

} // namespace tejoy::events
