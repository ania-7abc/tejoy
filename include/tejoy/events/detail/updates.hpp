// updates.hpp
#pragma once
#include <event_system/event.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <tejoy/detail/update_types.hpp>
#include <tejoy/user.hpp>
#include <utility>

namespace tejoy::events::detail
{

struct SendUpdateRequest : event_system::Event
{
    explicit SendUpdateRequest() = default;
    explicit SendUpdateRequest(nlohmann::json data, const std::string_view type, const User &recipient,
                               const bool no_ack = false, const uint32_t pkg_id = 0)
        : data(std::move(data)), type(type), recipient(std::move(recipient)), no_ack(no_ack), pkg_id(pkg_id)
    {
    }
    nlohmann::json data{};
    std::string type{};
    User recipient;
    bool no_ack{};
    uint32_t pkg_id{};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SendUpdateRequest, data, type, recipient, no_ack, pkg_id)
};

struct SendRawUpdateRequest : event_system::Event
{
    explicit SendRawUpdateRequest() = default;
    explicit SendRawUpdateRequest(nlohmann::json update, const User &recipient)
        : update(std::move(update)), recipient(std::move(recipient))
    {
    }
    nlohmann::json update{};
    User recipient;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SendRawUpdateRequest, update, recipient)
};

struct UpdateReceived : event_system::Event
{
    explicit UpdateReceived() = default;
    explicit UpdateReceived(nlohmann::json data, std::string type, const User &sender, const bool no_ack,
                            const uint32_t pkg_id)
        : data(std::move(data)), type(std::move(type)), sender(std::move(sender)), no_ack(no_ack), pkg_id(pkg_id)
    {
    }
    nlohmann::json data{};
    std::string type{};
    User sender;
    bool no_ack{};
    uint32_t pkg_id{};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UpdateReceived, data, type, sender, no_ack, pkg_id)
};

} // namespace tejoy::events::detail
