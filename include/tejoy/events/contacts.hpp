// contacts.hpp
#pragma once
#include <event_system/event.hpp>
#include <future>
#include <tejoy/events/detail/updates.hpp>
#include <tejoy/user.hpp>

namespace tejoy::events
{

struct RequestContacts : event_system::Event
{
    explicit RequestContacts(std::promise<std::vector<User>> &promise) : promise(promise)
    {
    }
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
    std::promise<std::vector<User>> &promise;
};

struct RequestHistory : event_system::Event
{
    explicit RequestHistory(std::promise<std::vector<detail::UpdateReceived>> &promise) : promise(promise)
    {
    }
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
    std::promise<std::vector<detail::UpdateReceived>> &promise;
};

struct AddContactRequest : event_system::Event
{
    explicit AddContactRequest(User contact) : contact(std::move(contact))
    {
    }
    User contact;
};

} // namespace tejoy::events
