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
    explicit Module(std::shared_ptr<event_system::EventBus> bus, nlohmann::json &config)
        : Subscriber(std::move(bus)), config_(config)
    {
    }

    virtual void run_subscribes()
    {
    }

  protected:
    nlohmann::json &config_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
    void subscribe_update(const std::string_view &update_type,
                          const std::function<void(const events::detail::UpdateReceived &)> &handler)
    {
        subscribe<events::detail::UpdateReceived>(
            [update_type, handler](const events::detail::UpdateReceived &event) -> void {
                if (event.type == update_type)
                    handler(event);
            });
    }
};

} // namespace tejoy::detail::modules
