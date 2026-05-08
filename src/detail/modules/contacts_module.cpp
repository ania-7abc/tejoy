// contacts_module.cpp
#include <iostream>
#include <nlohmann/json.hpp>
#include <tejoy/detail/json_variant.hpp>
#include <tejoy/detail/modules/contacts_module.hpp>
#include <tejoy/detail/update_types.hpp>
#include <tejoy/events/ping.hpp>

namespace tejoy::detail::modules
{

ContactsModule::ContactsModule(std::shared_ptr<event_system::EventBus> bus, nlohmann::json &config)
	: Module(std::move(bus), config)
{
	histories = config_.get<decltype(histories)>();
}

ContactsModule::~ContactsModule()
{
	try
	{
		config_ = nlohmann::json(histories);
	}
	catch (nlohmann::json::type_error &e)
	{
		std::cerr << e.what() << '\n';
	}
	catch (std::bad_alloc &e)
	{
		std::cerr << e.what() << '\n';
	}
}

void ContactsModule::run_subscribes()
{
	subscribe<events::RequestContacts>([this](auto &event) -> auto { on_request_contacts(event); });
	subscribe<events::AddContactRequest>([this](auto &event) -> auto { on_add_contact_request(event); });
	subscribe<events::RequestHistory>([this](auto &event) -> auto { on_request_history(event); });
	subscribe<events::detail::SendUpdateRequest>([this](auto &event) -> auto { on_send_update_request(event); });
	subscribe<events::UpdateSendError>([this](auto &event) -> auto { on_update_send_error(event); });
	subscribe<events::detail::UpdateReceived>([this](auto &event) -> auto { on_update_received(event); });
}

void ContactsModule::on_add_contact_request(const events::AddContactRequest &event)
{
	publish<events::RequestPing>(
		event.contact,
		[this, event](const User &user) -> void {
			histories.try_emplace(user);
			event.on_ok(user);
		},
		[event](const std::exception_ptr &ptr) -> void { event.on_error(ptr); });
}

void ContactsModule::on_request_contacts(const events::RequestContacts &event)
{
	auto keys = histories | std::views::keys;
	event.on_result(std::vector(keys.begin(), keys.end()));
}

void ContactsModule::on_send_update_request(const events::detail::SendUpdateRequest &event)
{
	if (event.type != UpdateTypes::MESSAGE)
		return;
	if (histories.contains(event.recipient))
		histories[event.recipient].emplace_back(event);
}

void ContactsModule::on_update_send_error(const events::UpdateSendError &event)
{
	if (event.update.update.at("type").get<std::string>() != UpdateTypes::MESSAGE)
		return;
	if (histories.contains(event.update.recipient))
		histories[event.update.recipient].emplace_back(event);
}

void ContactsModule::on_update_received(const events::detail::UpdateReceived &event)
{
	if (event.type != UpdateTypes::MESSAGE)
		return;
	if (histories.contains(event.sender))
		histories[event.sender].emplace_back(event);
}

void ContactsModule::on_request_history(const events::RequestHistory &event)
{
	event.on_result(histories[event.user]);
}

} // namespace tejoy::detail::modules
