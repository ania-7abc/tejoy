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

// subscriber.hpp
#pragma once
#include <event_system/event_bus.hpp>

namespace event_system
{

    class Subscriber : public std::enable_shared_from_this<Subscriber>
    {
    public:
        explicit Subscriber(EventBus &bus) : bus_(bus) {}
        virtual ~Subscriber() = default;

        Subscriber(const Subscriber &) = delete;
        Subscriber &operator=(const Subscriber &) = delete;

    protected:
        template <typename EventType>
        void subscribe(std::function<void(const EventType &)> handler,
                       Subscriber *senderFilter = nullptr)
        {
            static_assert(std::is_base_of_v<Event, EventType>,
                          "EventType must derive from Event");

            bus_.subscribe(weak_from_this(), handler, senderFilter);
        }

        template <typename EventType, typename... Args>
        void publish(Args &&...args)
        {
            static_assert(std::is_base_of_v<Event, EventType>,
                          "EventType must derive from Event");
            bus_.publish(args...);
        }

    private:
        EventBus &bus_;
    };

} // namespace event_system
