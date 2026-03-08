// update_sorter_module.cpp

#include <tejoy/detail/modules/update_sorter_module.hpp>
#include <tejoy/events/errors.hpp>
#include <tejoy/events/updates.hpp>

#include <algorithm>
#include <iostream>

#include <nlohmann/json.hpp>

namespace tejoy::detail::modules
{

void UpdateSorterModule::on_start()
{
    last_ids_ = boost::circular_buffer<uint32_t>(
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        config_.emplace("last_ids_buffer_size", static_cast<std::size_t>(10)).first.value().get<std::size_t>());
    subscribe<events::detail::UpdateReceived>([this](auto &event) { on_update_received(event); });
}

void UpdateSorterModule::on_stop()
{
}

void UpdateSorterModule::on_update_received(const events::detail::UpdateReceived &event)
{
    std::string type = event.update.at("type").get<std::string>();
    uint32_t pkg_id = event.update.at("pkg_id").get<uint32_t>();
    if (type == "ack")
    {
        publish<events::AckUpdateReceived>(pkg_id, event.sender);
    }

    if (std::find(last_ids_.begin(), last_ids_.end(), pkg_id) != last_ids_.end())
    {
        return;
    }
    last_ids_.push_back(pkg_id);

    if (type == "message")
    {
        publish<events::MessageUpdateReceived>(event.update.at("data").at("text").get<std::string>(), pkg_id,
                                               event.sender);
    }
    else if (type == "allo")
    {
        publish<events::AlloUpdateReceived>(event.sender);
    }
    else if (type == "imok")
    {
        publish<events::ImokUpdateReceived>(event.sender);
    }
    else
    {
        publish<events::InvalidUpdateError>(pkg_id, type, "Invalid update type");
    }
}

} // namespace tejoy::detail::modules
