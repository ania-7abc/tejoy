// network_module.hpp
#pragma once

#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/packet_events.hpp>

#include <cstdint>
#include <string>

#include <storage/storage.hpp>
#include <udp/udp.hpp>

namespace tejoy::detail::modules
{
  class NetworkModule : public tejoy::detail::modules::Module
  {
  public:
    NetworkModule(event_system::EventBus &bus, nlohmann::json &config, uint16_t port);
    void on_start() override;
    void on_stop() override;

  private:
    void on_send_packet_request(const tejoy::events::detail::SendPacketRequest &e);
    void on_network_message(const std::string &message, const std::string &ip, uint16_t port);

    UDP udp_;

    bool print_;
    bool simulate_loss_;
    size_t loss_percent_;
  };
} // namespace tejoy::detail::modules
