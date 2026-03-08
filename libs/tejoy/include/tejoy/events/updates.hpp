// updates.hpp
#pragma once

#include <tejoy/user.hpp>

#include <string>

#include <event_system/event.hpp>

namespace tejoy::events
{

struct MessageUpdateReceived : event_system::Event
{
    MessageUpdateReceived(std::string text, uint32_t pkg_id, User sender)
        : text(std::move(text)), pkg_id(pkg_id), sender(std::move(sender))
    {
    }
    std::string text;
    uint32_t pkg_id;
    User sender;
};

struct AckUpdateReceived : event_system::Event
{
    AckUpdateReceived(uint32_t pkg_id, User sender) : pkg_id(pkg_id), sender(std::move(sender))
    {
    }
    uint32_t pkg_id;
    User sender;
};

struct AlloUpdateReceived : event_system::Event
{
    explicit AlloUpdateReceived(User sender) : sender(std::move(sender))
    {
    }
    User sender;
};

struct ImokUpdateReceived : event_system::Event
{
    explicit ImokUpdateReceived(User sender) : sender(std::move(sender))
    {
    }
    User sender;
};

struct SendConfiguredUpdateRequest : event_system::Event
{
    SendConfiguredUpdateRequest(nlohmann::json data, std::string type, User recipient, bool no_encrypt = false,
                                bool no_ack = false)
        : data(std::move(data)), type(std::move(type)), recipient(std::move(recipient)), no_encrypt(no_encrypt),
          no_ack(no_ack)
    {
    }
    nlohmann::json data;
    std::string type;
    User recipient;
    bool no_encrypt;
    bool no_ack;
};

} // namespace tejoy::events
