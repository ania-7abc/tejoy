// user_code_helper.cpp
#include <tejoy/events/data_requests.hpp>
#include <tejoy/events/detail/updates.hpp>
#include <tejoy/events/discovery.hpp>
#include <tejoy/events/log.hpp>
#include <tejoy/events/message.hpp>
#include <tejoy/user_code_helper.hpp>

namespace tejoy
{

void UserCodeHelper::start()
{
    subscribe<events::SendMessageRequest>([this](auto &event) {
        publish<events::detail::SendConfiguredUpdateRequest>(nlohmann::json({{"text", event.text}}), "message",
                                                             event.recipient);
    });

    subscribe<events::detail::UpdateReceived>([this](auto &event) {
        if (event.update.at("type").template get<std::string>() == "message")
        {
            publish<events::MessageReceived>(event.update.at("data").at("text").template get<std::string>(),
                                             event.sender);
        }
    });

    subscribe<events::MessageReceived>([this](auto &event) { on_message_handler(event.text, event.sender); });
    subscribe<events::DiscoveredNewNode>([this](auto &event) { on_discovered_node_handler(event.node); });
    subscribe<events::InvalidUpdateError>([this](auto &event) { on_invalid_update_error_handler(event); });
    subscribe<events::UpdateSendError>([this](auto &event) { on_update_send_error_handler(event); });
    subscribe<events::LogEvent>([this](auto &event) { on_log_handler(event.event_type, event.from); });
}

std::string UserCodeHelper::get_ip()
{
    auto promise = std::promise<std::string>();
    auto fut = promise.get_future();
    publish<tejoy::events::RequestIp>(promise);
    return fut.get();
}

uint16_t UserCodeHelper::get_port()
{
    auto promise = std::promise<uint16_t>();
    auto fut = promise.get_future();
    publish<tejoy::events::RequestPort>(promise);
    return fut.get();
}

std::string UserCodeHelper::get_discovery_ip()
{
    auto promise = std::promise<std::string>();
    auto fut = promise.get_future();
    publish<tejoy::events::RequestDiscoveryIp>(promise);
    return fut.get();
}

User UserCodeHelper::get_i()
{
    auto promise = std::promise<tejoy::User>();
    auto fut = promise.get_future();
    publish<tejoy::events::RequestI>(promise);
    return fut.get();
}

void UserCodeHelper::send_message(const std::string &text, const User &recipient)
{
    publish<events::SendMessageRequest>(text, recipient);
}

void UserCodeHelper::on_message(std::function<void(const std::string &, const User &)> handler)
{
    on_message_handler = std::move(handler);
}

void UserCodeHelper::on_discovered_node(std::function<void(const tejoy::User &)> handler)
{
    on_discovered_node_handler = std::move(handler);
}

void UserCodeHelper::on_invalid_update_error(std::function<void(const events::InvalidUpdateError &)> handler)
{
    on_invalid_update_error_handler = std::move(handler);
}

void UserCodeHelper::on_update_send_error(std::function<void(const events::UpdateSendError &)> handler)
{
    on_update_send_error_handler = std::move(handler);
}

void UserCodeHelper::on_log(std::function<void(const std::string &event_type, const std::string &from)> handler)
{
    on_log_handler = std::move(handler);
}

} // namespace tejoy
