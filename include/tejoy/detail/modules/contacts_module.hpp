// contacts_module.hpp
#pragma once
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/contacts.hpp>
#include <tejoy/events/detail/updates.hpp>
#include <unordered_map>

namespace tejoy::detail::modules
{
class ContactsModule : public Module
{
  public:
	explicit ContactsModule(std::shared_ptr<event_system::EventBus> bus, nlohmann::json &config);

	~ContactsModule() override;
	ContactsModule(const ContactsModule &) = default;
	ContactsModule(ContactsModule &&) = default;
	auto operator=(const ContactsModule &) -> ContactsModule & = delete;
	auto operator=(ContactsModule &&) -> ContactsModule & = delete;

	void run_subscribes() override;
	static auto priority() -> int
	{
		return 0;
	}

  private:
	void on_add_contact_request(const events::AddContactRequest &event);
	void on_request_contacts(const events::RequestContacts &event);

	void on_send_update_request(const events::detail::SendUpdateRequest &event);
	void on_update_send_error(const events::UpdateSendError &event);
	void on_update_received(const events::detail::UpdateReceived &event);
	void on_request_history(const events::RequestHistory &event);

	std::unordered_map<User, events::History> histories;
};
} // namespace tejoy::detail::modules
