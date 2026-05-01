// message.hpp
#pragma once

#include <tejoy/user.hpp>

#include <string>

#include <event_system/event.hpp>

namespace tejoy::events
{

struct SendMessageRequest : event_system::Event
{
    explicit SendMessageRequest() = default;
    explicit SendMessageRequest(std::string text, const User &recipient)
        : text(std::move(text)), recipient(std::move(recipient))
    {
    }
    std::string text{};
    User recipient;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SendMessageRequest, text, recipient)
};

struct MessageReceived : event_system::Event
{
    explicit MessageReceived() = default;
    explicit MessageReceived(std::string text, const User &sender) : text(std::move(text)), sender(std::move(sender))
    {
    }
    std::string text{};
    User sender;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(MessageReceived, text, sender)
};

} // namespace tejoy::events
