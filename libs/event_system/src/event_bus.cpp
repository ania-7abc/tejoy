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

// event_bus.cpp
#include <event_system/event_bus.hpp>
#include <typeindex>

namespace event_system
{

    EventBus::EventBus() : io_(), work_guard_(boost::asio::make_work_guard(io_)), thread_([this]()
                                                                                          { io_.run(); }) {}

    EventBus::~EventBus()
    {
        work_guard_.reset();
        io_.stop();
        if (thread_.joinable())
            thread_.join();
    }

    void EventBus::subscribe_impl(const std::type_index &eventType,
                                  std::function<void(EventPtr)> handler,
                                  Subscriber *senderFilter,
                                  std::weak_ptr<void> weakSubscriber)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        subscriptions_[eventType].push_back({std::move(handler),
                                             senderFilter,
                                             weakSubscriber});
    }

    void EventBus::publish(EventPtr event)
    {
        std::type_index eventType = std::type_index(typeid(*event));
        std::list<Subscription> subsCopy;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = subscriptions_.find(eventType);
            if (it != subscriptions_.end())
                subsCopy = it->second;
        }

        for (const auto &sub : subsCopy)
        {
            if (sub.sender_filter && sub.sender_filter != event->sender())
                continue;
            if (sub.weak_subscriber.expired())
                continue;
            boost::asio::post(io_, [handler = sub.handler, event]()
                              { handler(event); });
        }
    }

    void EventBus::cleanup()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto &[type, subs] : subscriptions_)
            subs.remove_if([](const Subscription &s)
                           { return s.weak_subscriber.expired(); });
    }

} // namespace event_system
