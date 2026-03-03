// node.hpp
#pragma once
#include <tejoy/detail/modules/module_manager.hpp>
#include <tejoy/events/data_requests.hpp>

#include <string>
#include <cstdint>
#include <vector>
#include <memory>

#include <storage/storage.hpp>
#include <event_system/event_bus.hpp>
#include <event_system/subscriber.hpp>

namespace tejoy
{

  using EventBus = event_system::EventBus;
  class Node
  {
  private:
    Storage storage_;
    EventBus bus_;
    tejoy::detail::modules::ModuleManager module_manager_;
    uint16_t port_;
    std::vector<std::shared_ptr<event_system::Subscriber>> request_data_subs_;

  public:
    Node(std::string data_path, uint16_t port = 5768, size_t max_attempts = 3);
    ~Node();

    EventBus &get_event_bus();
  };

} // namespace tejoy
