// node.hpp
#pragma once
#include <event_system/event_bus.hpp>
#include <memory>
#include <tejoy/detail/modules/module_manager.hpp>
#include <tejoy/user_code_helper.hpp>
#include <vector>

namespace tejoy
{

using EventBus = event_system::EventBus;
class Node
{
  private:
    nlohmann::json &data_;
    detail::modules::ModuleManager module_manager_;
    std::shared_ptr<UserCodeHelper> uch_;
    EventBus bus_;

  public:
    explicit Node(nlohmann::json &data);
    ~Node();
    Node(const Node &) = delete;
    auto operator=(const Node &) -> Node & = delete;
    Node(Node &&) = delete;
    auto operator=(Node &&) -> Node & = delete;

    auto get_event_bus() -> EventBus &;
    auto get_uch() -> UserCodeHelper &;
};

} // namespace tejoy
