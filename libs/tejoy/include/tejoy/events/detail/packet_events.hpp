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

// packet_events.hpp
#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>
#include <event_system/event.hpp>
#include <tejoy/user.hpp>
#include <string>

namespace tejoy::events::detail {

struct PacketReceived : event_system::Event
{
    PacketReceived(std::string message, std::string ip, uint16_t port)
        : message(std::move(message)), ip(std::move(ip)), port(port) {}
    std::string message;
    std::string ip;
    uint16_t port;
};

struct SendPacketRequest : event_system::Event
{
    SendPacketRequest(std::string message, std::string ip, uint16_t port)
        : message(std::move(message)), ip(std::move(ip)), port(port) {}
    std::string message;
    std::string ip;
    uint16_t port;
};

struct UpdateReceived : event_system::Event
{
    UpdateReceived(nlohmann::json update, tejoy::User from)
        : update(update), from(from) {}
    const nlohmann::json update;
    const tejoy::User from;
};

struct SendUpdateRequest : event_system::Event
{
    SendUpdateRequest(nlohmann::json update, tejoy::User to)
        : update(update), to(to) {}
    const nlohmann::json update;
    const tejoy::User to;
};

} // namespace tejoy::events::detail
