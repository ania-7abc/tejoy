// pairing_module.hpp
#pragma once
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/errors.hpp>
#include <tejoy/events/ping.hpp>

namespace tejoy::detail::modules
{

class PairingModule : public tejoy::detail::modules::Module
{
  public:
    using tejoy::detail::modules::Module::Module;
    void on_start() override;

  private:
    void on_ping(const tejoy::events::detail::UpdateReceived &event);
    void on_ping_ok(const tejoy::events::detail::UpdateReceived &event);

    void on_ping_request(const events::PingRequest &event);
    void on_update_send_error(const events::UpdateSendError &event);
};

} // namespace tejoy::detail::modules
