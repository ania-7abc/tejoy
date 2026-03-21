// event_bus.hpp
#pragma once

#include <event_system/event.hpp>

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <typeindex>

#include <boost/asio.hpp>

namespace event_system
{

class Subscriber;

class EventBus
{
  public:
    using EventPtr = std::shared_ptr<const Event>;

    explicit EventBus();
    ~EventBus();
    EventBus(const EventBus &) = delete;
    auto operator=(const EventBus &) -> EventBus & = delete;
    EventBus(EventBus &&) = delete;
    auto operator=(EventBus &&) -> EventBus & = delete;

    template <typename EventType, typename T>
    void subscribe(std::weak_ptr<T> subscriber, std::function<void(const EventType &)> handler,
                   const Subscriber *sender_filter = nullptr)
    {
        static_assert(std::is_base_of_v<Event, EventType>, "EventType must derive from Event");
        subscribe_impl(
            std::type_index(typeid(EventType)),
            [handler = std::move(handler)](const EventPtr &event) {
                if (auto concrete = dynamic_cast<const EventType *>(event.get()))
                {
                    handler(*concrete);
                }
            },
            sender_filter, subscriber);
    }

    template <typename EventType>
    auto make_subscriber(std::function<void(const EventType &)> callback, const Subscriber *sender_filter = nullptr);

    template <typename EventType, typename... Args> void publish(const Subscriber *sender, Args &&...args)
    {
        static_assert(std::is_base_of_v<Event, EventType>, "EventType must derive from Event");

        auto event = std::make_shared<EventType>(std::forward<Args>(args)...);
        event->set_sender(sender);
        publish(event);
    }

    void publish(const EventPtr &event);

    void cleanup();

  private:
    struct Subscription
    {
        std::function<void(EventPtr)> handler;
        const Subscriber *sender_filter;
        std::weak_ptr<void> weak_subscriber;
    };

    void subscribe_impl(const std::type_index &eventType, std::function<void(EventPtr)> handler,
                        const Subscriber *senderFilter, std::weak_ptr<void> weakSubscriber);

    std::map<std::type_index, std::list<Subscription>> subscriptions_;
    std::mutex mutex_;

    boost::asio::io_context io_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
    std::thread thread_;
};

} // namespace event_system

#include <event_system/subscriber.hpp>

namespace event_system
{

template <typename EventType>
auto EventBus::make_subscriber(std::function<void(const EventType &)> callback, const Subscriber *sender_filter)
{
    static_assert(std::is_base_of_v<Event, EventType>, "EventType must derive from Event");
    auto sub = std::make_shared<Subscriber>(*this);
    sub->subscribe<EventType>(std::move(callback), sender_filter);
    return sub;
}

} // namespace event_system
