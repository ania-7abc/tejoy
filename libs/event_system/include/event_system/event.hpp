// event.hpp
#pragma once

namespace event_system
{

class Subscriber;

class Event
{
  public:
    virtual ~Event() = default;
    Event() = default;
    Event(const Event &) = default;
    auto operator=(const Event &) -> Event & = default;
    Event(Event &&) = default;
    auto operator=(Event &&) -> Event & = default;

    [[nodiscard]] auto sender() const -> Subscriber *
    {
        return sender_;
    }
    void set_sender(Subscriber *sender)
    {
        sender_ = sender;
    }

  private:
    Subscriber *sender_ = nullptr;
};

} // namespace event_system
