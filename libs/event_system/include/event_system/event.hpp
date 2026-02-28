#pragma once
#include <string>

class Subscriber;

namespace event_system
{

    class Event
    {
    public:
        virtual ~Event() = default;
        virtual std::string type() const = 0;
        virtual Subscriber *sender() const = 0;
        virtual void set_sender(Subscriber *s) = 0;
    };

} // namespace eventbus
