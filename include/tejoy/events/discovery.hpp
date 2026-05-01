// discovery.hpp
#pragma once
#include <event_system/event.hpp>
#include <string>
#include <tejoy/user.hpp>

namespace tejoy::events
{

struct DiscoveredNewNode : event_system::Event
{
    explicit DiscoveredNewNode() = default;
    explicit DiscoveredNewNode(const User &node) : node(std::move(node))
    {
    }
    User node;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DiscoveredNewNode, node)
};

struct RequestDiscoveryIp : event_system::Event
{
    explicit RequestDiscoveryIp(std::function<void(std::string)> on_result) : on_result(std::move(on_result))
    {
    }
    std::function<void(std::string)> on_result{};
};

} // namespace tejoy::events
