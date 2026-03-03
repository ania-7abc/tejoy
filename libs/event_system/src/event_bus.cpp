// event_bus.cpp

#include <event_system/event_bus.hpp>

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
