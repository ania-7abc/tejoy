// node.cpp

#include <tejoy/detail/modules/ack_module.hpp>
#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/detail/modules/update_manager_module.hpp>
#include <tejoy/detail/modules/update_sorter_module.hpp>
#include <tejoy/events/message.hpp>
#include <tejoy/node.hpp>

namespace tejoy
{

  Node::Node(std::string data_path, uint16_t port) : storage_(data_path),
                                                     bus_(),
                                                     module_manager_(bus_, storage_),
                                                     port_(port),
                                                     request_data_subs_()
  {
    storage_.load();

    if (!storage_.data.contains("/node/contacts"_json_pointer))
      storage_.data["/node/contacts"_json_pointer] = nlohmann::json({});

    request_data_subs_.push_back(bus_.make_subscriber<events::RequestPort>(
        [this](auto &e)
        { e.promise.set_value(port_); }));
    request_data_subs_.push_back(bus_.make_subscriber<events::RequestIp>(
        [this](auto &e)
        { e.promise.set_value("127.0.0.1"); }));
    request_data_subs_.push_back(bus_.make_subscriber<events::SendMessageRequest>(
        [this](auto &e)
        {
          auto &contact = storage_.data
                              .at("/node/contacts"_json_pointer)
                              .emplace(e.to.ip, nlohmann::json({}))
                              .first.value();
          uint32_t pkg_id = contact.value("last_pkg_id", (uint32_t)0) + 1;
          contact["last_pkg_id"] = pkg_id;

          nlohmann::json update = {{"pkg_id", pkg_id}, {"type", "message"}, {"data", {{"text", e.text}}}};
          bus_.publish(std::make_shared<events::detail::SendUpdateRequest>(update, e.to));
        }));

    module_manager_.create_module<detail::modules::NetworkModule>("/network"_json_pointer);
    module_manager_.create_module<detail::modules::UpdateManagerModule>("/update_manager"_json_pointer);
    module_manager_.create_module<detail::modules::UpdateSorterModule>("/update_sorter"_json_pointer);
    module_manager_.create_module<detail::modules::AckModule>("/ack"_json_pointer);
    module_manager_.start_all();
  }

  Node::~Node()
  {
    module_manager_.stop_all();
    storage_.save();
  }

  EventBus &Node::get_event_bus()
  {
    return bus_;
  }

} // namespace tejoy
