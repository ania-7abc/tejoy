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
    explicit Module(event_system::EventBus &bus, nlohmann::json &config)
        : event_system::Subscriber(bus), config_(config)
    {
    }
    ~Module() override = default;
    explicit Module(const Module &) = delete;
    auto operator=(const Module &) -> Module & = delete;
    explicit Module(Module &&) = delete;
    auto operator=(Module &&) -> Module & = delete;
    virtual void on_start()
    {
    }
    virtual void on_stop()
    {
    }
    virtual auto priority() const -> int
    {
        return 0;
    }

  protected:
    nlohmann::json &config_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
};

} // namespace tejoy::detail::modules
