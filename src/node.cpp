// node.cpp

#include <nlohmann/json.hpp>
#include <tejoy/detail/modules/ack_module.hpp>
#include <tejoy/detail/modules/discovery_module.hpp>
#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/detail/modules/update_manager_module.hpp>
#include <tejoy/events/message.hpp>
#include <tejoy/node.hpp>
#include <utility>

namespace tejoy
{

Node::Node(nlohmann::json &data) : data_(data), module_manager_(bus_, data_)
{
    if (!data_.contains("/node/contacts"_json_pointer))
    {
        data_["/node/contacts"_json_pointer] = nlohmann::json({});
    }

    request_data_subs_.push_back(bus_.make_subscriber<events::SendMessageRequest>([this](auto &event) {
        bus_.publish(std::make_shared<events::detail::SendConfiguredUpdateRequest>(
            nlohmann::json({{"text", event.text}}), "message", event.recipient));
    }));

    module_manager_.create_module<detail::modules::NetworkModule>("/network"_json_pointer);
    module_manager_.create_module<detail::modules::UpdateManagerModule>("/update_manager"_json_pointer);
    module_manager_.create_module<detail::modules::AckModule>("/ack"_json_pointer);
    module_manager_.create_module<detail::modules::DiscoveryModule>("/discovery"_json_pointer);
    module_manager_.start_all();
}

Node::~Node()
{
    module_manager_.stop_all();
}

auto Node::get_event_bus() -> EventBus &
{
    return bus_;
}

} // namespace tejoy
