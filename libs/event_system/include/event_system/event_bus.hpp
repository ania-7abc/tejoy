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
