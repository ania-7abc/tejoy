// user_code_helper.hpp
#pragma once
#include <event_system/subscriber.hpp>

namespace tejoy
{

class UserCodeHelper : public event_system::Subscriber
{
  public:
    using Subscriber::Subscriber;

    void start();
};

} // namespace tejoy
