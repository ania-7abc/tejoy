// node.hpp
#pragma once
#include <cstdint>
#include <event_system/event_bus.hpp>
#include <event_system/subscriber.hpp>
#include <memory>
#include <storage/storage.hpp>
#include <string>
#include <tejoy/detail/modules/module_manager.hpp>
#include <tejoy/events/data_requests.hpp>
#include <vector>

namespace tejoy
{

using EventBus = event_system::EventBus;
class Node
{
  private:
    Storage storage_;
    EventBus bus_;
    tejoy::detail::modules::ModuleManager module_manager_;
    std::vector<std::shared_ptr<event_system::Subscriber>> request_data_subs_;

  public:
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    explicit Node(std::string data_path);
    ~Node();
    Node(const Node &) = delete;
    auto operator=(const Node &) -> Node & = delete;
    Node(Node &&) = delete;
    auto operator=(Node &&) -> Node & = delete;

    auto get_event_bus() -> EventBus &;
};

} // namespace tejoy
