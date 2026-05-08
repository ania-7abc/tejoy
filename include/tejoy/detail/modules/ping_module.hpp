// ping_module.hpp
#pragma once
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/errors.hpp>
#include <tejoy/events/ping.hpp>

namespace tejoy::detail::modules
{

class PingModule : public Module
{
  public:
	using Module::Module;

	~PingModule() override;
	PingModule(const PingModule &) = default;
	PingModule(PingModule &&) = default;
	auto operator=(const PingModule &) -> PingModule & = delete;
	auto operator=(PingModule &&) -> PingModule & = delete;

	void run_subscribes() override;
	static auto priority() -> int
	{
		return 0;
	}

  private:
	void on_ping(const events::detail::UpdateReceived &event) const;
	void on_pong(const events::detail::UpdateReceived &event);

	void on_ping_request(const events::RequestPing &event);
	void on_update_send_error(const events::UpdateSendError &event);

	std::vector<events::RequestPing> requests;
};

} // namespace tejoy::detail::modules
