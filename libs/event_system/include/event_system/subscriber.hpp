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
            bus_.subscribe(weak_from_this(), handler, senderFilter);
        }

        template <typename EventType, typename... Args>
        void publish(Args &&...args)
        {
            auto event = std::make_shared<EventType>(std::forward<Args>(args)...);
            event->set_sender(this);
            bus_.publish(event);
        }

    private:
        EventBus &bus_;
    };

} // namespace event_system
