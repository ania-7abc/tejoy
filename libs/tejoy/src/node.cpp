// node.cpp
#include <tejoy/node.hpp>
#include <tejoy/events/message.hpp>
#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/detail/modules/update_manager_module.hpp>
#include <tejoy/detail/modules/update_sort_module.hpp>
#include <tejoy/detail/modules/ack_module.hpp>

namespace tejoy
{
  Node::Node(std::string data_path, uint16_t port, size_t max_attempts) : storage_(data_path),
                                                                          bus_(),
                                                                          module_manager_(bus_),
                                                                          port_(port),
                                                                          request_data_subs_()
  {
    storage_.load();

    storage_.data.emplace("next_pkg_id", nlohmann::json::object());

    request_data_subs_.push_back(bus_.make_subscriber<events::RequestPort>(
        [this](auto &e)
        { e.promise.set_value(port_); }));
    request_data_subs_.push_back(bus_.make_subscriber<events::RequestIp>(
        [this](auto &e)
        { e.promise.set_value("127.0.0.1"); }));
    request_data_subs_.push_back(bus_.make_subscriber<events::SendMessageRequest>(
        [this](auto &e)
        {
          nlohmann::json update;
          auto &next_pkg_id = storage_.data.at("next_pkg_id");
          next_pkg_id.emplace(e.to.ip, 0);

          uint32_t pkg_id = next_pkg_id[e.to.ip].template get<uint32_t>();
          next_pkg_id[e.to.ip] = pkg_id + 1;

          update["pkg_id"] = pkg_id;
          update["type"] = "message";
          update["data"] = {{"text", e.text}};
          bus_.publish(std::make_shared<events::detail::SendUpdateRequest>(update, e.to));
        }));

    module_manager_.create_module<detail::modules::NetworkModule>(port_, storage_);
    module_manager_.create_module<detail::modules::UpdateManagerModule>(storage_);
    module_manager_.create_module<detail::modules::UpdateSortModule>();
    module_manager_.create_module<detail::modules::AckModule>(max_attempts);
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
