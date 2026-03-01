// MIT License
// 
// Copyright (c) 2026 ania_7 (Anya Baykina Dmitrievna)
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

// network_module.hpp
#pragma once
#include <udp.hpp>
#include <string>
#include <cstdint>
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/packet_events.hpp>

namespace tejoy::detail::modules
{
    class NetworkModule : public Module
    {
    public:
        NetworkModule(event_system::EventBus &bus, uint16_t port);
        void on_start() override;
        void on_stop() override;
        void onNeedSendPacket(const tejoy::events::detail::SendPacketRequest &e);
        void onNetworkMessage(const std::string &message, const std::string &ip, uint16_t port);

    private:
        UDP udp_;
    };
} // namespace tejoy::detail::modules
