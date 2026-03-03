// data_requests.hpp
#pragma once
#include <cstdint>
#include <future>
#include <event_system/event.hpp>
#include <tejoy/user.hpp>

namespace tejoy::events
{

  struct RequestPort : event_system::Event
  {
    RequestPort(std::promise<uint16_t> &promise) : promise(promise) {}
    std::promise<uint16_t> &promise;
  };

  struct RequestIp : event_system::Event
  {
    RequestIp(std::promise<std::string> &promise) : promise(promise) {}
    std::promise<std::string> &promise;
  };

  struct RequestI : event_system::Event
  {
    RequestI(std::promise<tejoy::User> &promise) : promise(promise) {}
    std::promise<tejoy::User> &promise;
  };

} // namespace tejoy::events
