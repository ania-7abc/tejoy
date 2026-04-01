// user_code_helper.hpp
#pragma once
#include <event_system/subscriber.hpp>

namespace tejoy
{

class UserCodeHelper : public event_system::Subscriber
{
  private:
    friend class Node;
    using Subscriber::Subscriber;

    void start();

  public:
};

} // namespace tejoy
