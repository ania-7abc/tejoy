// module.hpp
#pragma once
#include <event_system/event_bus.hpp>
#include <event_system/subscriber.hpp>
#include <nlohmann/json.hpp>
#include <string_view>
#include <tejoy/events/detail/updates.hpp>

namespace tejoy::detail::modules
{

class Module : public event_system::Subscriber
{
  public:
    explicit Module(event_system::EventBus &bus, nlohmann::json &config) : Subscriber(bus), config_(config)
    {
    }

    virtual void run_subscribes()
    {
    }

  protected:
    nlohmann::json &config_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
    void subscribe_update(const std::string_view &update_type,
                          const std::function<void(const events::detail::UpdateReceived &)> &handler,
                          const std::type_index sender_filter = typeid(void))
    {
        subscribe<events::detail::UpdateReceived>(
            [update_type, handler](const events::detail::UpdateReceived &event) -> void {
                if (event.type == update_type)
                    handler(event);
            },
            sender_filter);
    }
};

} // namespace tejoy::detail::modules
