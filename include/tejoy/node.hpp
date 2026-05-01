// node.hpp
#pragma once
#include <event_system/event_bus.hpp>
#include <memory>
#include <tejoy/detail/modules/module_manager.hpp>
#include <tejoy/user_code_helper.hpp>

namespace tejoy
{

using EventBus = event_system::EventBus;
class Node
{
    std::shared_ptr<EventBus> bus_;
    std::shared_ptr<nlohmann::json> data_;
    detail::modules::ModuleManager module_manager_;
    std::shared_ptr<UserCodeHelper> uch_;

  public:
    explicit Node(std::shared_ptr<nlohmann::json> data);

    auto get_event_bus() -> EventBus &;
    auto get_uch() const -> UserCodeHelper &;
};

} // namespace tejoy
