// log.hpp
#pragma once
#include <event_system/event.hpp>

namespace tejoy::events
{

struct LogEvent : event_system::Event
{
    explicit LogEvent(std::string event_type, std::string from)
        : event_type(std::move(event_type)), from(std::move(from))
    {
    }
    std::string event_type;
    std::string from;
};

} // namespace tejoy::events
