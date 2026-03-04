// module.hpp
#pragma once

#include <event_system/event_bus.hpp>
#include <event_system/subscriber.hpp>
#include <nlohmann/json.hpp>

namespace tejoy::detail::modules
{

  class Module : public event_system::Subscriber
  {
  public:
    explicit Module(event_system::EventBus &bus, nlohmann::json &config) : event_system::Subscriber(bus), config_(config) {}
    virtual ~Module() = default;
    virtual void on_start() {}
    virtual void on_stop() {}

  protected:
    nlohmann::json &config_;
  };

} // namespace tejoy::detail::modules
