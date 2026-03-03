// event.hpp
#pragma once

namespace event_system
{

  class Subscriber;

  class Event
  {
  public:
    virtual ~Event() = default;
    Subscriber *sender() const { return sender_; }
    void set_sender(Subscriber *s) { sender_ = s; }

  private:
    Subscriber *sender_ = nullptr;
  };

} // namespace event_system
