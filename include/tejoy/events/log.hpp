// log.hpp
#pragma once
#include <event_system/event.hpp>

namespace tejoy::events
{

struct LogEvent : event_system::Event
{
    explicit LogEvent() = default;
    explicit LogEvent(std::string event_type, std::string from)
        : event_type(std::move(event_type)), from(std::move(from))
    {
    }
    std::string event_type;
    std::string from;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(LogEvent, event_type, from)
};

} // namespace tejoy::events
