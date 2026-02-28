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

// event.bus
#pragma once
#include <memory>
#include <functional>
#include <string>
#include <map>
#include <list>
#include <mutex>
#include <boost/asio.hpp>
#include <event_system/event.hpp>

class Subscriber;

namespace event_system
{

    class EventBus
    {
    public:
        using EventPtr = std::shared_ptr<const Event>;

        explicit EventBus(boost::asio::io_context &io);

        template <typename EventType, typename T>
        void subscribe(std::weak_ptr<T> subscriber,
                       std::function<void(const EventType &)> handler,
                       Subscriber *sender_filter = nullptr)
        {
            static_assert(std::is_base_of_v<Event, EventType>,
                          "EventType must derive from Event");
            subscribe_impl(EventType::static_type(), [handler = std::move(handler)](EventPtr e)
                           {
                             if (auto concrete = dynamic_cast<const EventType*>(e.get()))
                               handler(*concrete); }, sender_filter, subscriber);
        }

        void publish(EventPtr event);

        void cleanup();

    private:
        struct Subscription
        {
            std::function<void(EventPtr)> handler;
            Subscriber *sender_filter;
            std::weak_ptr<void> weak_subscriber;
        };

        void subscribe_impl(const std::string &eventType,
                            std::function<void(EventPtr)> handler,
                            Subscriber *sender_filter,
                            std::weak_ptr<void> weak_subscriber);

        boost::asio::io_context &io_;
        std::map<std::string, std::list<Subscription>> subscriptions_;
        std::mutex mutex_;
    };

} // namespace event_system
