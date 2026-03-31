// updates.hpp
#pragma once
#include <event_system/event.hpp>
#include <string>
#include <tejoy/detail/update_types.hpp>
#include <tejoy/user.hpp>

namespace tejoy::events::detail
{

struct SendConfiguredUpdateRequest : event_system::Event
{
    SendConfiguredUpdateRequest(nlohmann::json data, std::string_view type, User recipient, bool no_ack = false)
        : data(std::move(data)), type(type), recipient(std::move(recipient)), no_ack(no_ack)
    {
    }
    nlohmann::json data;
    std::string_view type;
    User recipient;
    bool no_ack;
};

struct UpdateReceived : event_system::Event
{
    UpdateReceived(nlohmann::json update, tejoy::User sender) : update(std::move(update)), sender(std::move(sender))
    {
    }
    nlohmann::json update;
    tejoy::User sender;
};

struct SendUpdateRequest : event_system::Event
{
    SendUpdateRequest(nlohmann::json update, tejoy::User recipient)
        : update(std::move(update)), recipient(std::move(recipient))
    {
    }
    nlohmann::json update;
    tejoy::User recipient;
};

} // namespace tejoy::events::detail
