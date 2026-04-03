// errors.hpp
#pragma once
#include <event_system/event.hpp>
#include <string>
#include <tejoy/events/detail/updates.hpp>

namespace tejoy::events
{

struct UpdateSendError : event_system::Event
{
    UpdateSendError(detail::SendRawUpdateRequest update, std::string reason)
        : update(std::move(update)), reason(std::move(reason))
    {
    }
    detail::SendRawUpdateRequest update;
    std::string reason;
};

} // namespace tejoy::events
