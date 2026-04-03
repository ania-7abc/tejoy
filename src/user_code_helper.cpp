// user_code_helper.cpp
#include <tejoy/events/data_requests.hpp>
#include <tejoy/events/detail/updates.hpp>
#include <tejoy/events/discovery.hpp>
#include <tejoy/events/log.hpp>
#include <tejoy/events/message.hpp>
#include <tejoy/events/ping.hpp>
#include <tejoy/user_code_helper.hpp>
#include <utility>

namespace tejoy
{

void UserCodeHelper::start()
{
    subscribe<events::SendMessageRequest>([this](auto &event) {
        publish<events::detail::SendUpdateRequest>(nlohmann::json({{"text", event.text}}), "message", event.recipient);
    });

    subscribe<events::detail::UpdateReceived>([this](auto &event) {
        if (event.type == "message")
        {
            publish<events::MessageReceived>(event.data.at("text").template get<std::string>(), event.sender);
        }
    });
}

auto UserCodeHelper::get_ip() -> std::string
{
    auto promise = std::promise<std::string>();
    auto fut = promise.get_future();
    publish<tejoy::events::RequestIp>(promise);
    return fut.get();
}

auto UserCodeHelper::get_port() -> uint16_t
{
    auto promise = std::promise<uint16_t>();
    auto fut = promise.get_future();
    publish<tejoy::events::RequestPort>(promise);
    return fut.get();
}

auto UserCodeHelper::get_discovery_ip() -> std::string
{
    auto promise = std::promise<std::string>();
    auto fut = promise.get_future();
    publish<tejoy::events::RequestDiscoveryIp>(promise);
    return fut.get();
}

auto UserCodeHelper::get_i() -> User
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

void UserCodeHelper::ping(const User &ping_user)
{
    publish<events::PingRequest>(ping_user);
}

void UserCodeHelper::on_message(const std::function<void(const std::string &text, const User &from)> &handler)
{
    on<events::MessageReceived>([handler](auto &event) { handler(event.text, event.sender); });
}

void UserCodeHelper::on_discovered_node(const std::function<void(const tejoy::User &node)> &handler)
{
    on<events::DiscoveredNewNode>([handler](auto &event) { handler(event.node); });
}

void UserCodeHelper::on_update_send_error(std::function<void(const events::UpdateSendError &error)> handler)
{
    on<events::UpdateSendError>(std::move(handler));
}

void UserCodeHelper::on_log(const std::function<void(const std::string &event_type, const std::string &from)> &handler)
{
    on<events::LogEvent>([handler](auto &event) { handler(event.event_type, event.from); });
}

void UserCodeHelper::on_ping_ok(const std::function<void(const User &ping_user)> &handler)
{
    on<events::PingOk>([handler](auto &event) { handler(event.ping_user); });
}

void UserCodeHelper::on_ping_failed(const std::function<void(const User &ping_user)> &handler)
{
    on<events::PingFailed>([handler](auto &event) { handler(event.ping_user); });
}

} // namespace tejoy
