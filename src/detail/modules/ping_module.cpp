// ping_module.cpp
#include <nlohmann/json.hpp>
#include <tejoy/detail/modules/ping_module.hpp>
#include <tejoy/detail/update_types.hpp>
#include <tejoy/events/detail/updates.hpp>

namespace tejoy::detail::modules
{

PingModule::~PingModule()
{
	for (const auto &request : requests)
		request.on_error(std::make_exception_ptr(std::runtime_error("ping module stop")));
}

void PingModule::run_subscribes()
{
	subscribe_update(UpdateTypes::PING, [this](auto &event) -> auto { on_ping(event); });
	subscribe_update(UpdateTypes::PONG, [this](auto &event) -> auto { on_pong(event); });
	subscribe<events::RequestPing>([this](auto &event) -> auto { on_ping_request(event); });
	subscribe<events::UpdateSendError>([this](auto &event) -> auto { on_update_send_error(event); });
}

void PingModule::on_ping(const events::detail::UpdateReceived &event) const
{
	publish<events::detail::SendUpdateRequest>(nlohmann::json::object(), UpdateTypes::PONG, event.sender);
}

void PingModule::on_pong(const events::detail::UpdateReceived &event)
{
	const auto iterator = std::ranges::find_if(requests, [&](const auto &req) -> auto {
		return req.ping_user.ip == event.sender.ip && req.ping_user.port == event.sender.port;
	});
	if (iterator != requests.end())
	{
		iterator->on_result(event.sender);
		requests.erase(iterator);
	}
}

void PingModule::on_ping_request(const events::RequestPing &event)
{
	requests.push_back(event);
	publish<events::detail::SendUpdateRequest>(nlohmann::json::object(), UpdateTypes::PING, event.ping_user);
}

void PingModule::on_update_send_error(const events::UpdateSendError &event)
{
	if (event.update.update.at("type").get<std::string>() != UpdateTypes::PING)
		return;

	const auto iterator = std::ranges::find_if(requests, [&](const auto &req) -> auto {
		return req.ping_user.ip == event.update.recipient.ip && req.ping_user.port == event.update.recipient.port;
	});
	if (iterator != requests.end())
	{
		iterator->on_error(std::make_exception_ptr(events::PingFailedError("ping failed")));
		requests.erase(iterator);
	}
}

} // namespace tejoy::detail::modules
