// contacts.hpp
#pragma once
#include <event_system/event.hpp>
#include <tejoy/events/detail/updates.hpp>
#include <tejoy/events/errors.hpp>
#include <tejoy/user.hpp>

namespace tejoy::events
{

struct RequestContacts : event_system::Event
{
    explicit RequestContacts(std::function<void(std::vector<User>)> on_result) : on_result(std::move(on_result))
    {
    }
    std::function<void(std::vector<User>)> on_result{};
};

struct AddContactRequest : event_system::Event
{
    explicit AddContactRequest(const User &contact, std::function<void(User user)> on_ok,
                               std::function<void()> on_error)
        : contact(std::move(contact)), on_ok(std::move(on_ok)), on_error(std::move(on_error))
    {
    }
    User contact;
    std::function<void(User user)> on_ok;
    std::function<void()> on_error;
};

using HistoryElement = std::variant<detail::UpdateReceived, detail::SendUpdateRequest, UpdateSendError>;
using History = std::vector<HistoryElement>;

struct RequestHistory : event_system::Event
{
    explicit RequestHistory(const User &user, std::function<void(History)> on_result)
        : user(std::move(user)), on_result(std::move(on_result))
    {
    }
    User user;
    std::function<void(History)> on_result{};
};

} // namespace tejoy::events
