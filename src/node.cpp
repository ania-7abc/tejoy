/**
 * @file node.cpp
 * @copyright Copyright (c) 2026 ania-7abc SPDX-License-Identifier: MIT
 */

#include <tejoy/node.hpp>
#include <utility>

namespace tejoy
{
    Node::Node(boost::asio::any_io_executor executor, nlohmann::json& config)
        : executor_(std::move(executor)), event_bus_(executor_), config_(config),

          network_(executor_, event_bus_, config_["network"])
    {
    }

    auto Node::get_bus() -> event_bus::EventBus&
    {
        return event_bus_;
    }
}
