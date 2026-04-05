// discover.hpp
#pragma once
#include <event_system/event.hpp>
#include <future>
#include <string>
#include <tejoy/user.hpp>

namespace tejoy::events
{

struct DiscoveredNewNode : event_system::Event
{
    explicit DiscoveredNewNode(User node) : node(std::move(node))
    {
    }
    User node;
};

struct RequestDiscoveryIp : event_system::Event
{
    explicit RequestDiscoveryIp(std::promise<std::string> &promise) : promise(promise)
    {
    }
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
    std::promise<std::string> &promise;
};

} // namespace tejoy::events
