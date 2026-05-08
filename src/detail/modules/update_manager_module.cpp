// update_manager_module.cpp
#include <base64/base64.hpp>
#include <future>
#include <secret_box/secret_box.hpp>
#include <tejoy/detail/modules/update_manager_module.hpp>
#include <tejoy/events/network.hpp>
#include <tejoy/events/others.hpp>

namespace tejoy::detail::modules
{

UpdateManagerModule::UpdateManagerModule(std::shared_ptr<event_system::EventBus> bus, nlohmann::json &config)
	: Module(std::move(bus), config)
{
	if (!config_.contains("i"))
	{
		auto promise_port = std::make_shared<std::promise<uint16_t>>();
		std::future<uint16_t> fut_port = promise_port->get_future();
		publish<events::RequestPort>(
			[promise_port](const uint16_t port) mutable -> void { promise_port->set_value(port); });

		auto promise_ip = std::make_shared<std::promise<std::string>>();
		std::future<std::string> fut_ip = promise_ip->get_future();
		publish<events::RequestIp>(
			[promise_ip](const std::string &real_ip) mutable -> void { promise_ip->set_value(real_ip); });

		i_ = User{.box = SecretBox(), .ip = fut_ip.get(), .port = fut_port.get()};
		config_.emplace("i", i_);
	}
	else
	{
		config_.at("i").get_to(i_);
	}

	config_.emplace("last_pkg_id", nlohmann::json::object());
	always_no_encrypt_ = config_.emplace("always_no_encrypt", false).first.value().get<bool>();
	last_id_iterators_ = boost::circular_buffer<std::set<Key>::iterator>(
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
		config_.emplace("dedup_buf_size", static_cast<std::size_t>(10)).first.value().get<std::size_t>());
}

void UpdateManagerModule::run_subscribes()
{
	subscribe<events::detail::SendUpdateRequest>([this](auto &event) -> auto { on_send_update_request(event); });
	subscribe<events::detail::SendRawUpdateRequest>([this](auto &event) -> auto { on_send_raw_update_request(event); });
	subscribe<events::detail::PacketReceived>([this](auto &event) -> auto { on_packet_received(event); });
	subscribe<events::RequestI>([this](auto &event) -> auto { event.on_result(i_); });
}

auto UpdateManagerModule::is_duplicate(const std::vector<uint8_t> &recipient_id, uint32_t pkg_id) -> bool
{
	const auto key = std::make_pair(recipient_id, pkg_id);

	auto [iterator, inserted] = last_ids_.insert(key);
	if (!inserted)
		return true;

	last_id_iterators_.push_back(iterator);
	if (last_id_iterators_.full())
		last_ids_.erase(last_id_iterators_.front());

	return false;
}

void UpdateManagerModule::on_send_update_request(const events::detail::SendUpdateRequest &event) const
{
	uint32_t pkg_id = event.pkg_id;
	if (pkg_id == 0)
	{
		std::string key =
			event.recipient.box.has_public_key() ? Base64::encode(event.recipient.box.get_public_key()) : "no_id";
		auto &last_pkg_id = config_.at("last_pkg_id");
		pkg_id = last_pkg_id.value(key, 0) + 1;
		last_pkg_id.update({{key, pkg_id}});
	}

	nlohmann::json update({
		{"data", event.data},
		{"type", event.type},
		{"pkg_id", pkg_id},
		{"no_ack", event.no_ack},
	});
	publish<events::detail::SendRawUpdateRequest>(update, event.recipient);
}

void UpdateManagerModule::on_send_raw_update_request(const events::detail::SendRawUpdateRequest &event) const
{
	nlohmann::json packet = event.update;
	bool no_encrypt = always_no_encrypt_ || !event.recipient.box.has_public_key();
	if (!no_encrypt)
		packet = Base64::encode(i_.box.encrypt(packet.dump(), event.recipient.box));
	packet = nlohmann::json(
		{{"no_encrypt", no_encrypt}, {"update", packet}, {"id", Base64::encode(i_.box.get_public_key())}});
	publish<events::detail::SendPacketRequest>(packet.dump(), event.recipient.ip, event.recipient.port);
}

void UpdateManagerModule::on_packet_received(const events::detail::PacketReceived &event)
{
	nlohmann::json packet = nlohmann::json::parse(event.message.begin(), event.message.end());
	const std::vector<uint8_t> pub_raw = Base64::decode(packet.at("id"));
	User sender{.box = SecretBox(pub_raw), .ip = event.sender_ip, .port = event.sender_port};

	nlohmann::json update = packet.at("update");
	if (!packet.at("no_encrypt").get<bool>())
		update = nlohmann::json::parse(i_.box.decrypt(Base64::decode(update), sender.box));

	if (is_duplicate(pub_raw, update.at("pkg_id")))
		return;

	nlohmann::json data = update.at("data");
	auto type = update.at("type").get<std::string>();
	auto no_ack = update.value("no_ack", false);
	auto pkg_id = update.at("pkg_id").get<uint32_t>();

	publish<events::detail::UpdateReceived>(data, type, sender, no_ack, pkg_id);
}

} // namespace tejoy::detail::modules
