// update_sorter_module.cpp

#include <tejoy/detail/modules/update_sorter_module.hpp>
#include <tejoy/events/errors.hpp>
#include <tejoy/events/updates.hpp>

#include <algorithm>
#include <iostream>

#include <nlohmann/json.hpp>

namespace tejoy::detail::modules
{

  UpdateSorterModule::UpdateSorterModule(event_system::EventBus &bus, nlohmann::json &config) : Module(bus, config)
  {
    last_ids_ = boost::circular_buffer<uint32_t>(
        config.emplace("last_ids_buffer_size", (std::size_t)10).first.value().get<std::size_t>());
  }

  void UpdateSorterModule::on_start()
  {
    subscribe<events::detail::UpdateReceived>([this](auto &e)
                                              { on_update_received(e); });
  }

  void UpdateSorterModule::on_stop()
  {
  }

  void UpdateSorterModule::on_update_received(const events::detail::UpdateReceived &e)
  {
    std::string type = e.update.at("type").get<std::string>();
    uint32_t pkg_id = e.update.at("pkg_id").get<uint32_t>();
    if (type == "ack")
      publish<events::AckUpdateReceived>(pkg_id, e.from);

    if (std::find(last_ids_.begin(), last_ids_.end(), pkg_id) != last_ids_.end())
      return;
    last_ids_.push_back(pkg_id);

    if (type == "message")
      publish<events::MessageUpdateReceived>(e.update.at("data").at("text").get<std::string>(), pkg_id, e.from);
    else
      publish<events::InvalidUpdateError>(pkg_id, type, "Invalid update type");
  }

} // namespace tejoy::detail::modules
