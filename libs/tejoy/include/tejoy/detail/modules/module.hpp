// module.hpp
#pragma once

#include <event_system/subscriber.hpp>

namespace tejoy::detail::modules
{

  class Module : public event_system::Subscriber
  {
  public:
    using Subscriber::Subscriber;
    virtual ~Module() = default;
    virtual void on_start() {}
    virtual void on_stop() {}
  };

} // namespace tejoy::detail::modules
