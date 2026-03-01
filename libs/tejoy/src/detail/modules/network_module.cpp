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

namespace tejoy::detail::modules
{

    NetworkModule::NetworkModule(event_system::EventBus &bus, uint16_t port) : Module(bus), udp_(port) {}

    void NetworkModule::on_start()
    {
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
        udp_.send(e.message, e.ip, e.port);
    }

    void NetworkModule::onNetworkMessage(const std::string &message, const std::string &ip, uint16_t port)
    {
        publish<tejoy::events::detail::PacketReceived>(message, ip, port);
    }
} // namespace tejoy::detail::modules
