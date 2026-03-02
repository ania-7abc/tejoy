// MIT License
//
// Copyright (c) 2026 Anya Baykina Dmitrievna
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// network_module.cpp
#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/events/data_request.hpp>
#include <iostream>

namespace tejoy::detail::modules
{

    NetworkModule::NetworkModule(event_system::EventBus &bus, uint16_t port, Storage &storage) : Module(bus), udp_(port), storage_(storage) {}

    void NetworkModule::on_start()
    {
        storage_.data.emplace("network", nlohmann::json::object());
        storage_.data["network"].emplace("loss", nlohmann::json::object());
        storage_.data["network"].emplace("print", false);
        storage_.data["network"]["loss"].emplace("enable", false);
        storage_.data["network"]["loss"].emplace("every", 3);
        storage_.data["network"]["loss"].emplace("print", true);

        print_ = storage_.data["network"]["print"].get<bool>();
        simulate_loss_ = storage_.data["network"]["loss"]["enable"].get<bool>();
        loss_every_ = storage_.data["network"]["loss"]["every"].get<size_t>();
        print_loss_ = storage_.data["network"]["loss"]["print"].get<bool>();

        udp_.start([this](auto &message, auto &ip, auto port)
                   { onNetworkMessage(message, ip, port); });
        subscribe<tejoy::events::detail::SendPacketRequest>([this](auto &e)
                                                            { onSendPacketRequest(e); });
    }

    void NetworkModule::on_stop()
    {
        udp_.stop();
    }

    void NetworkModule::onSendPacketRequest(const tejoy::events::detail::SendPacketRequest &e)
    {
        static size_t packet_counter = 0;
        if (simulate_loss_ && (++packet_counter % loss_every_ == 0))
        {
            if (print_loss_)
                std::cout << "Simulated loss message \"" << e.message << "\" to " << e.ip << ":" << e.port << std::endl;
            return;
        }
        udp_.send(e.message, e.ip, e.port);
        if (print_)
            std::cout << "Sent message \"" << e.message << "\" to " << e.ip << ":" << e.port << std::endl;
    }

    void NetworkModule::onNetworkMessage(const std::string &message, const std::string &ip, uint16_t port)
    {
        publish<tejoy::events::detail::PacketReceived>(message, ip, port);
    }
} // namespace tejoy::detail::modules
