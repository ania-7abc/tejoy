// data_requests.hpp
#pragma once

#include <tejoy/user.hpp>

#include <cstdint>
#include <future>

#include <event_system/event.hpp>

namespace tejoy::events
{

  struct RequestIp : event_system::Event
  {
    RequestIp(std::promise<std::string> &promise) : promise(promise) {}
    std::promise<std::string> &promise;
  };

  struct RequestPort : event_system::Event
  {
    RequestPort(std::promise<uint16_t> &promise) : promise(promise) {}
    std::promise<uint16_t> &promise;
  };

  struct RequestDiscoveryIp : event_system::Event
  {
    RequestDiscoveryIp(std::promise<std::string> &promise) : promise(promise) {}
    std::promise<std::string> &promise;
  };

  struct RequestDiscoveryPort : event_system::Event
  {
    RequestDiscoveryPort(std::promise<uint16_t> &promise) : promise(promise) {}
    std::promise<uint16_t> &promise;
  };

  struct RequestI : event_system::Event
  {
    RequestI(std::promise<tejoy::User> &promise) : promise(promise) {}
    std::promise<tejoy::User> &promise;
  };

} // namespace tejoy::events
