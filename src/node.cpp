// node.cpp
#include <nlohmann/json.hpp>
#include <tejoy/detail/modules/ack_module.hpp>
#include <tejoy/detail/modules/contacts_module.hpp>
#include <tejoy/detail/modules/discovery_module.hpp>
#include <tejoy/detail/modules/log_module.hpp>
#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/detail/modules/ping_module.hpp>
#include <tejoy/detail/modules/update_manager_module.hpp>
#include <tejoy/node.hpp>

namespace tejoy
{

Node::Node(nlohmann::json &data) : data_(data), module_manager_(bus_, data_)
{
    module_manager_.create_module<detail::modules::AckModule>("/ack"_json_pointer);
    module_manager_.create_module<detail::modules::ContactsModule>("/contacts"_json_pointer);
    module_manager_.create_module<detail::modules::DiscoveryModule>("/discovery"_json_pointer);
    module_manager_.create_module<detail::modules::LogModule>("/log"_json_pointer);
    module_manager_.create_module<detail::modules::NetworkModule>("/network"_json_pointer);
    module_manager_.create_module<detail::modules::PingModule>("/ping"_json_pointer);
    module_manager_.create_module<detail::modules::UpdateManagerModule>("/update_manager"_json_pointer);
    module_manager_.start_all();

    uch_ = std::make_shared<UserCodeHelper>(bus_);
    uch_->start();
}

auto Node::get_event_bus() -> EventBus &
{
    return bus_;
}

auto Node::get_uch() const -> UserCodeHelper &
{
    return *uch_;
}

} // namespace tejoy
