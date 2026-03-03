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

// update_sort_module.cpp
#include <tejoy/detail/modules/update_sort_module.hpp>
#include <tejoy/events/updates.hpp>
#include <tejoy/events/errors.hpp>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <iostream>

namespace tejoy::detail::modules
{

    UpdateSortModule::UpdateSortModule(event_system::EventBus &bus) : Module(bus), last_ids_(10) {}

    void UpdateSortModule::on_start()
    {
        subscribe<tejoy::events::detail::UpdateReceived>([this](auto &e)
                                                         { onUpdateReceived(e); });
    }

    void UpdateSortModule::on_stop()
    {
    }

    void UpdateSortModule::onUpdateReceived(const tejoy::events::detail::UpdateReceived &e)
    {
        std::string type = e.update.at("type").get<std::string>();
        uint32_t pkg_id = e.update.at("pkg_id").get<uint32_t>();
        if (type == "ack")
            publish<tejoy::events::AckUpdateReceived>(pkg_id, e.from);

        if (std::find(last_ids_.begin(), last_ids_.end(), pkg_id) != last_ids_.end())
            return;
        last_ids_.push_back(pkg_id);

        if (type == "message")
            publish<tejoy::events::MessageUpdateReceived>(e.update.at("data").at("text").get<std::string>(), pkg_id, e.from);
        else
            publish<events::InvalidUpdateError>(pkg_id, type, "Invalid update type");
    }

} // namespace tejoy::detail::modules
