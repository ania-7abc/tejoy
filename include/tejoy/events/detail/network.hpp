/**
 * @file network.hpp
 * @brief This file defines events for communicating with the network
 * @copyright Copyright (c) 2026 ania-7abc SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <boost/asio/ip/udp.hpp>

/**
 * @namespace tejoy::events
 * @brief Namespace for tejoy events
 */
namespace tejoy::events
{
    /**
     * @namespace tejoy::events::detail
     * @brief Namespace for internal events
     */
    namespace detail
    {
        /**
         * @namespace tejoy::events::detail::requests
         * @brief Namespace for action/information request events
         */
        namespace requests
        {
            /**
             * @struct SendDatagram
             * @brief Request to send a datagram
             */
            struct SendDatagram
            {
                /// Datagram recipient
                boost::asio::ip::udp::endpoint recipient;
                /// Datagram body
                std::string data;
            };
        }

        /**
         * @struct DatagramReceived
         * @brief Event indicating the receipt of a datagram
         */
        struct DatagramReceived
        {
            /// Datagram sender
            boost::asio::ip::udp::endpoint sender;
            /// Datagram body
            std::string data;
        };
    }
}
