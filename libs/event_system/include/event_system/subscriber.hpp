// subscriber.hpp
#pragma once

#include <functional>
#include <memory>

namespace event_system
{

class EventBus;

class Subscriber : public std::enable_shared_from_this<Subscriber>
{
    friend class EventBus;

  public:
    explicit Subscriber(EventBus &bus) : bus_(bus)
    {
    }
    virtual ~Subscriber() = default;

    explicit Subscriber(const Subscriber &) = delete;
    virtual auto operator=(const Subscriber &) -> Subscriber & = delete;
    explicit Subscriber(Subscriber &&) = delete;
    virtual auto operator=(Subscriber &&) -> Subscriber & = delete;

  protected:
    template <typename EventType>
    void subscribe(std::function<void(const EventType &)> handler, const Subscriber *senderFilter = nullptr);

    template <typename EventType, typename... Args> void publish(Args &&...args) const;

    auto bus() -> EventBus &
    {
        return bus_;
    }

  private:
    EventBus &bus_;
};

} // namespace event_system

#include <event_system/event_bus.hpp>

namespace event_system
{

template <typename EventType>
void Subscriber::subscribe(std::function<void(const EventType &)> handler, const Subscriber *senderFilter)
{
    bus_.subscribe(weak_from_this(), std::move(handler), senderFilter);
}

template <typename EventType, typename... Args> void Subscriber::publish(Args &&...args) const
{
    auto event = std::make_shared<EventType>(std::forward<Args>(args)...);
    event->set_sender(this);
    bus_.publish(event);
}

} // namespace event_system
