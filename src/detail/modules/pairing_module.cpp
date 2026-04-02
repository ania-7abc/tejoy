// pairing_module.cpp
#include <iostream>
#include <nlohmann/json.hpp>
#include <tejoy/detail/modules/pairing_module.hpp>
#include <tejoy/detail/update_types.hpp>
#include <tejoy/events/detail/updates.hpp>

namespace tejoy::detail::modules
{

void PairingModule::on_start()
{
    subscribe_update(UpdateTypes::PING, [this](auto &event) { on_ping(event); });
    subscribe_update(UpdateTypes::PONG, [this](auto &event) { on_ping_ok(event); });
    subscribe<events::PingRequest>([this](auto &event) { on_ping_request(event); });
    subscribe<events::UpdateSendError>([this](auto &event) { on_update_send_error(event); });
}

void PairingModule::on_ping(const tejoy::events::detail::UpdateReceived &event)
{
    publish<events::detail::SendConfiguredUpdateRequest>(nlohmann::json::object(), UpdateTypes::PONG, event.sender);
}

void PairingModule::on_ping_ok(const tejoy::events::detail::UpdateReceived &event)
{
    publish<events::PingOk>(event.sender);
}

void PairingModule::on_ping_request(const events::PingRequest &event)
{
    publish<events::detail::SendConfiguredUpdateRequest>(nlohmann::json::object(), UpdateTypes::PING, event.ping_user);
}

void PairingModule::on_update_send_error(const events::UpdateSendError &event)
{
    if (event.update.update.at("type").get<std::string>() == UpdateTypes::PING)
    {
        publish<events::PingFailed>(event.update.recipient);
    }
}

} // namespace tejoy::detail::modules
