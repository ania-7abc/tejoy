// network.hpp
#pragma once
#include <cstdint>
#include <event_system/event.hpp>
#include <string>

namespace tejoy::events
{

struct RequestIp : event_system::Event
{
    explicit RequestIp(std::function<void(std::string)> on_result) : on_result(std::move(on_result))
    {
    }
    std::function<void(std::string)> on_result{};
};

struct RequestPort : event_system::Event
{
    explicit RequestPort(std::function<void(uint16_t)> on_result) : on_result(std::move(on_result))
    {
    }
    std::function<void(uint16_t)> on_result;
};

} // namespace tejoy::events
