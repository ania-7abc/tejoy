// node.cpp

#include <tejoy/detail/modules/ack_module.hpp>
#include <tejoy/detail/modules/discovery_module.hpp>
#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/detail/modules/update_manager_module.hpp>
#include <tejoy/detail/modules/update_sorter_module.hpp>
#include <tejoy/events/message.hpp>
#include <tejoy/events/updates.hpp>
#include <tejoy/node.hpp>
#include <utility>

namespace tejoy
{

Node::Node(std::string data_path, uint16_t port)
    : storage_(std::move(data_path)), module_manager_(bus_, storage_), port_(port)
{
    storage_.load();

    if (!storage_.data().contains("/node/contacts"_json_pointer))
    {
        storage_.data()["/node/contacts"_json_pointer] = nlohmann::json({});
    }

    request_data_subs_.push_back(bus_.make_subscriber<events::SendConfiguredUpdateRequest>([this](auto &event) {
        auto &contact =
            storage_.data().at("/node/contacts"_json_pointer).emplace(event.recipient.ip, nlohmann::json({})).first.value();
        uint32_t pkg_id = contact.value("last_pkg_id", static_cast<uint32_t>(0)) + 1;
        contact["last_pkg_id"] = pkg_id;

        nlohmann::json update({
            {"type", event.type},
            {"pkg_id", pkg_id},
            {"data", event.data},
            {"no_ack", event.no_ack},
            {"no_encrypt", event.no_encrypt},
        });
        bus_.publish(std::make_shared<events::detail::SendUpdateRequest>(update, event.recipient));
    }));
    request_data_subs_.push_back(bus_.make_subscriber<events::SendMessageRequest>([this](auto &event) {
        bus_.publish(
            std::make_shared<events::SendConfiguredUpdateRequest>(nlohmann::json({{"text", event.text}}), "message", event.recipient));
    }));

    module_manager_.create_module<detail::modules::NetworkModule>("/network"_json_pointer);
    module_manager_.create_module<detail::modules::UpdateManagerModule>("/update_manager"_json_pointer);
    module_manager_.create_module<detail::modules::UpdateSorterModule>("/update_sorter"_json_pointer);
    module_manager_.create_module<detail::modules::AckModule>("/ack"_json_pointer);
    module_manager_.create_module<detail::modules::DiscoveryModule>("/discovery"_json_pointer);
    module_manager_.start_all();
}

Node::~Node()
{
    module_manager_.stop_all();
    storage_.save();
}

auto Node::get_event_bus() -> EventBus &
{
    return bus_;
}

} // namespace tejoy
