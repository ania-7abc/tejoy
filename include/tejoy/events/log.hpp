// log.hpp
#pragma once
#include <event_system/event.hpp>

namespace tejoy::events
{

struct LogEvent : event_system::Event
{
    explicit LogEvent(std::type_index event_type, std::type_index from)
        : event_type(std::move(event_type)), from(std::move(from))
    {
    }
    std::type_index event_type;
    std::type_index from;
};

} // namespace tejoy::events
