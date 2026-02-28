// subscriber.hpp
#pragma once
#include <eventbus/event_bus.hpp>

namespace eventbus
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

        EventBus &bus() { return bus_; }
        const EventBus &bus() const { return bus_; }

    private:
        EventBus &bus_;
    };

} // namespace eventbus
