/**
 * @file node.hpp
 * @brief Defines the Node class
 * @copyright Copyright (c) 2026 ania-7abc SPDX-License-Identifier: MIT
 */

#pragma once

#include <boost/asio/any_io_executor.hpp>
#include <event_bus/event_bus.hpp>
#include <nlohmann/json.hpp>

/**
 * @namespace tejoy
 * @brief Main namespace of the tejoy library
 */
namespace tejoy
{
    /**
     * @class Node
     * @brief Represents 1 device on the network
     */
    class Node
    {
        /// Executor for all asynchronous operations
        boost::asio::any_io_executor executor_;
        /// Event bus for modules communication
        event_bus::EventBus event_bus_;
        /// Config for modules settings. Should live longer than this object
        nlohmann::json& config_;

    public:
        /**
         * @brief Constructor. Builds ready-to-use Node
         * @param executor Executor for performing asynchronous tasks
         * @param config Mutating config for setting up Node.
         *               Should live longer than this object.
         *               If the required keys are missing, they will be created.
         *               May contain: network
         */
        Node(boost::asio::any_io_executor executor, nlohmann::json& config);

        /**
         * @brief Getter. Returns a reference to the internal event bus
         * @return Non-const reference to the internal event bus
         */
        auto get_bus() -> event_bus::EventBus&;
    };
}
