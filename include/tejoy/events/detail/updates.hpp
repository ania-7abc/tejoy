// updates.hpp
#pragma once
#include <event_system/event.hpp>
#include <string>
#include <tejoy/detail/update_types.hpp>
#include <tejoy/user.hpp>
#include <utility>

namespace tejoy::events::detail
{

struct SendUpdateRequest : event_system::Event
{
    SendUpdateRequest(nlohmann::json data, std::string_view type, User recipient, bool no_ack = false,
                      uint32_t pkg_id = 0)
        : data(std::move(data)), type(type), recipient(std::move(recipient)), no_ack(no_ack), pkg_id(pkg_id)
    {
    }
    nlohmann::json data;
    std::string_view type;
    User recipient;
    bool no_ack;
    uint32_t pkg_id;
};

struct SendRawUpdateRequest : event_system::Event
{
    SendRawUpdateRequest(nlohmann::json update, User recipient)
        : update(std::move(update)), recipient(std::move(recipient))
    {
    }
    nlohmann::json update;
    User recipient;
};

struct UpdateReceived : event_system::Event
{
    UpdateReceived(nlohmann::json data, std::string type, User sender, bool no_ack, uint32_t pkg_id)
        : data(std::move(data)), type(std::move(type)), sender(std::move(sender)), no_ack(no_ack), pkg_id(pkg_id)
    {
    }
    nlohmann::json data;
    std::string type;
    User sender;
    bool no_ack;
    uint32_t pkg_id;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UpdateReceived, data, type, sender, no_ack, pkg_id)
};

} // namespace tejoy::events::detail
