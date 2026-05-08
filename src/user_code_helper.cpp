// user_code_helper.cpp
#include <future>
#include <tejoy/detail/update_types.hpp>
#include <tejoy/events/detail/updates.hpp>
#include <tejoy/events/discovery.hpp>
#include <tejoy/events/log.hpp>
#include <tejoy/events/message.hpp>
#include <tejoy/events/network.hpp>
#include <tejoy/events/others.hpp>
#include <tejoy/events/ping.hpp>
#include <tejoy/user_code_helper.hpp>

namespace tejoy
{

void UserCodeHelper::start()
{
	subscribe<events::SendMessageRequest>([this](auto &event) -> void {
		publish<events::detail::SendUpdateRequest>(nlohmann::json({{"text", event.text}}), "message", event.recipient);
	});

	subscribe<events::detail::UpdateReceived>([this](auto &event) -> void {
		if (event.type == detail::UpdateTypes::MESSAGE)
			publish<events::MessageReceived>(event.data.at("text").template get<std::string>(), event.sender);
	});
}

auto UserCodeHelper::get_ip() const -> std::future<std::string>
{
	auto promise = std::make_shared<std::promise<std::string>>();
	auto fut = promise->get_future();
	publish<events::RequestIp>([promise](const std::string &real_ip) mutable -> void { promise->set_value(real_ip); });
	return fut;
}

auto UserCodeHelper::get_port() const -> std::future<uint16_t>
{
	auto promise = std::make_shared<std::promise<uint16_t>>();
	auto fut = promise->get_future();
	publish<events::RequestPort>([promise](const auto port) mutable -> void { promise->set_value(port); });
	return fut;
}

auto UserCodeHelper::get_discovery_ip() const -> std::future<std::string>
{
	auto promise = std::make_shared<std::promise<std::string>>();
	auto fut = promise->get_future();
	publish<events::RequestDiscoveryIp>(
		[promise](const std::string &real_ip) mutable -> void { promise->set_value(real_ip); });
	return fut;
}

auto UserCodeHelper::get_i() const -> std::future<User>
{
	auto promise = std::make_shared<std::promise<User>>();
	auto fut = promise->get_future();
	publish<events::RequestI>([promise](const User &user) mutable -> void { promise->set_value(user); });
	return fut;
}

auto UserCodeHelper::get_contacts() const -> std::future<std::vector<User>>
{
	auto promise = std::make_shared<std::promise<std::vector<User>>>();
	auto fut = promise->get_future();
	publish<events::RequestContacts>(
		[promise](const std::vector<User> &contacts) mutable -> void { promise->set_value(contacts); });
	return fut;
}

auto UserCodeHelper::get_history(const User &user) const -> std::future<events::History>
{
	auto promise = std::make_shared<std::promise<events::History>>();
	auto fut = promise->get_future();
	publish<events::RequestHistory>(
		user, [promise](const events::History &history) mutable -> void { promise->set_value(history); });
	return fut;
}

void UserCodeHelper::send_message(const std::string &text, const User &recipient) const
{
	publish<events::SendMessageRequest>(text, recipient);
}

auto UserCodeHelper::ping(const User &ping_user) const -> std::future<User>
{
	auto promise = std::make_shared<std::promise<User>>();
	auto fut = promise->get_future();
	publish<events::RequestPing>(
		ping_user, [promise](const User &real_ping_user) mutable -> void { promise->set_value(real_ping_user); },
		[promise](const auto &ptr) mutable -> void { promise->set_exception(ptr); });
	return fut;
}

auto UserCodeHelper::add_contact(User &contact) const -> std::future<bool>
{
	auto promise = std::make_shared<std::promise<bool>>();
	auto fut = promise->get_future();
	publish<events::AddContactRequest>(
		contact,
		[promise, &contact](const auto &user) mutable -> void {
			contact = user;
			promise->set_value(true);
		},
		[promise](const auto &ptr) mutable -> void { promise->set_exception(ptr); });
	return fut;
}

void UserCodeHelper::on_message(const std::function<void(const std::string &text, const User &from)> &handler)
{
	on<events::MessageReceived>([handler](auto &event) -> void { handler(event.text, event.sender); });
}

void UserCodeHelper::on_discovered_node(const std::function<void(const User &node)> &handler)
{
	on<events::DiscoveredNewNode>([handler](auto &event) -> void { handler(event.node); });
}

void UserCodeHelper::on_update_send_error(const std::function<void(const events::UpdateSendError &error)> &handler)
{
	on<events::UpdateSendError>(handler);
}

void UserCodeHelper::on_log(const std::function<void(const std::string &event_type, const std::string &from)> &handler)
{
	on<events::LogEvent>([handler](auto &event) -> void { handler(event.event_type, event.from); });
}

} // namespace tejoy
