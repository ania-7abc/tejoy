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

// node.cpp
#include <tejoy/node.hpp>
#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/detail/modules/update_manager_module.hpp>

namespace tejoy
{
    Node::Node(std::string data_path, uint16_t port) : storage_(data_path),
                                                       bus_(),
                                                       module_manager_(bus_),
                                                       port_(port),
                                                       request_data_subs_()
    {
        storage_.load();

        request_data_subs_.push_back(bus_.make_subscriber<tejoy::events::RequestPort>(
            [this](const auto &e)
            { e.promise.set_value(port_); }));
        request_data_subs_.push_back(bus_.make_subscriber<tejoy::events::RequestIp>(
            [this](const auto &e)
            { e.promise.set_value("127.0.0.1"); }));

        module_manager_.create_module<detail::modules::NetworkModule>(port_);
        module_manager_.create_module<detail::modules::UpdateManagerModule>(storage_);
        module_manager_.start_all();
    }

    Node::~Node()
    {
        module_manager_.stop_all();
        storage_.save();
    }

    EventBus &Node::get_event_bus()
    {
        return bus_;
    }

} // namespace tejoy
