/**
 * @file network.hpp
 * @brief This file defines a network module
 * @copyright Copyright (c) 2026 ania-7abc SPDX-License-Identifier: MIT
 */

#pragma once

#include <boost/asio/any_io_executor.hpp>
#include <event_bus/event_bus.hpp>
#include <nlohmann/json.hpp>

#include <random>
#include <boost/asio/ip/udp.hpp>
#include <tejoy/events/detail/network.hpp>

/**
 * @namespace tejoy::detail
 * @brief Namespace for internal implementation details with unstable API
 */
namespace tejoy::detail // NOLINT
{
    /**
     * @namespace tejoy::detail::modules
     * @brief Namespace for tejoy modules
     */
    namespace modules
    {
        /**
         * @class Network
         * @brief Network operations module
         */
        class Network
        {
            /// Executor for all asynchronous operations
            boost::asio::any_io_executor executor_;
            /// Event bus for communication. Should live longer than this object
            event_bus::EventBus& event_bus_;
            /// Config for modules settings. Should live longer than this object
            nlohmann::json& config_;

            /// Socket for communicating with the network
            boost::asio::ip::udp::socket socket_;
            /// Sender endpoint. Filled in when a datagram is received
            boost::asio::ip::udp::endpoint sender_endpoint_;
            /// Buffer for data. Filled in when a datagram is received
            std::array<uint16_t, 1 << 16> buffer_ = {};

            /// Network port
            uint16_t port_ = 5768;
            /// Traffic logging flag. Filled from the config
            bool log_ = false;
            /// Datagram loss percentage for debugging. Filled from the config
            int lost_percent_ = 0;

            /// Random generator
            std::mt19937 rnd_;

            /// Boost datagram acceptance handler
            void on_datagram_received() noexcept;
            /// tejoy request handler for sending a datagram
            void on_send_datagram_request(const events::detail::requests::SendDatagram& request) noexcept;

        public:
            /**
             * @brief Constructor. Builds ready-to-use Network
             * @param executor Executor for asynchronous operations
             * @param event_bus Event bus for communication with other modules. Should live longer than this object
             * @param config Module configuration. Should live longer than this object.
             *               May contain: port, log, lost_percent
             */
            Network(boost::asio::any_io_executor executor, event_bus::EventBus& event_bus, nlohmann::json& config);
        };
    }
}
