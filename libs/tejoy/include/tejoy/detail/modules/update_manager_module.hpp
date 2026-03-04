// update_manager_module.hpp
#pragma once

#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/packet_events.hpp>
#include <tejoy/user.hpp>

#include <storage/storage.hpp>

namespace tejoy::detail::modules
{

  class UpdateManagerModule : public tejoy::detail::modules::Module
  {
  public:
    UpdateManagerModule(event_system::EventBus &bus, nlohmann::json &config);
    void on_start() override;
    void on_stop() override;

  private:
    void on_send_update_request(const tejoy::events::detail::SendUpdateRequest &e);
    void on_packet_received(const tejoy::events::detail::PacketReceived &e);

    User i_;
    bool encrypt_;
  };

} // namespace tejoy::detail::modules
