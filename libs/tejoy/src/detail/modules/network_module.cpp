// network_module.cpp

#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/events/data_requests.hpp>

#include <iostream>

namespace tejoy::detail::modules
{

  NetworkModule::NetworkModule(event_system::EventBus &bus, nlohmann::json &config, uint16_t port) : Module(bus, config), udp_(port) {}

  void NetworkModule::on_start()
  {
    config_.emplace("loss", nlohmann::json::object());
    config_.emplace("print", false);
    config_["loss"].emplace("enable", false);
    config_["loss"].emplace("percent", 40);

    print_ = config_["print"].get<bool>();
    simulate_loss_ = config_["loss"]["enable"].get<bool>();
    loss_percent_ = config_["loss"]["percent"].get<int>();

    udp_.start([this](auto &message, auto &ip, auto port)
               { on_network_message(message, ip, port); });
    subscribe<events::detail::SendPacketRequest>([this](auto &e)
                                                 { on_send_packet_request(e); });
  }

  void NetworkModule::on_stop()
  {
    udp_.stop();
  }

  void NetworkModule::on_send_packet_request(const events::detail::SendPacketRequest &e)
  {
    if (rand() % 100 < loss_percent_)
    {
      if (print_)
        std::cout << "Not sent message \"" << e.message << "\" to " << e.ip << ":" << e.port << std::endl;
      return;
    }
    udp_.send(e.message, e.ip, e.port);
    if (print_)
      std::cout << "Sent message \"" << e.message << "\" to " << e.ip << ":" << e.port << std::endl;
  }

  void NetworkModule::on_network_message(const std::string &message, const std::string &ip, uint16_t port)
  {
    publish<events::detail::PacketReceived>(message, ip, port);
  }

} // namespace tejoy::detail::modules
