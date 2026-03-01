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

// update_manager_module.hpp
#pragma once
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/packet_events.hpp>
#include <tejoy/user.hpp>
#include <storage.hpp>

namespace tejoy::detail::modules
{
    class UpdateManagerModule : public Module
    {
    public:
        UpdateManagerModule(event_system::EventBus &bus, Storage &storage);
        void on_start() override;
        void on_stop() override;

    private:
        void onSendUpdateRequest(const tejoy::events::detail::SendUpdateRequest &e);
        void onPacketReceived(const tejoy::events::detail::PacketReceived &e);

        Storage &storage_;
        User i_;
    };
} // namespace tejoy::detail::modules
