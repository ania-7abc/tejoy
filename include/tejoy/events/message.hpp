// message.hpp
#pragma once

#include <tejoy/user.hpp>

#include <string>

#include <event_system/event.hpp>

namespace tejoy::events
{

struct SendMessageRequest : event_system::Event
{
    SendMessageRequest(std::string text, User recipient) : text(std::move(text)), recipient(std::move(recipient))
    {
    }
    std::string text;
    tejoy::User recipient;
};

struct MessageReceived : event_system::Event
{
    MessageReceived(std::string text, User sender) : text(std::move(text)), sender(std::move(sender))
    {
    }
    std::string text;
    tejoy::User sender;
};

} // namespace tejoy::events
