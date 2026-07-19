/**
 * @file network.cpp
 * @copyright Copyright (c) 2026 ania-7abc SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <tejoy/detail/modules/network.hpp>

namespace tejoy::detail::modules
{
    void Network::on_datagram_received() noexcept
    {
        const auto data = std::string(buffer_.begin(), buffer_.end());
        if (log_)
            std::cerr << "Received datagram from "
                << sender_endpoint_.address().to_string() << ":" << sender_endpoint_.port()
                << ": " << data << '\n';
        event_bus_.emit(events::detail::DatagramReceived{
            sender_endpoint_,
            data
        });
    }

    void Network::on_send_datagram_request(const events::detail::requests::SendDatagram& request) noexcept
    {
        if (log_)
        {
            if (!std::bernoulli_distribution(lost_percent_)(rnd_))
                std::cerr << "Sending datagram to ";
            else
                std::cerr << "Simulated datagram loss for ";
            std::cerr << request.recipient.address().to_string() << ":" << request.recipient.port()
                << ": " << request.data << '\n';
        }
        socket_.async_send_to(boost::asio::buffer(request.data), request.recipient,
                              [](const boost::system::error_code&, size_t)
                              {
                              });
    }

    Network::Network(boost::asio::any_io_executor executor, event_bus::EventBus& event_bus, nlohmann::json& config)
        : executor_(std::move(executor)), event_bus_(event_bus), config_(config), socket_(executor_),
          rnd_(std::random_device{}())
    {
        if (!config_.is_object())
            config_ = nlohmann::json::object();

        if (!config_["port"].is_number_unsigned())
            config["port"] = port_;
        config_["port"].get_to(port_);

        if (!config_["log"].is_boolean())
            config_["log"] = log_;
        config_["log"].get_to(log_);

        if (!config_["lost_percent"].is_number_unsigned())
            config["lost_percent"] = lost_percent_;
        config_["lost_percent"].get_to(lost_percent_);

        socket_.open(boost::asio::ip::udp::v4());
        socket_.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port_));

        socket_.async_receive_from(boost::asio::buffer(buffer_), sender_endpoint_,
                                   [this](const boost::system::error_code& ec, std::size_t)
                                   {
                                       if (!ec)
                                           on_datagram_received();
                                   });
        event_bus_.subscribe<events::detail::requests::SendDatagram>(
            [this](const auto& request)
            {
                on_send_datagram_request(request);
            });
    }
}
