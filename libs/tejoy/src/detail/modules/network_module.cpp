// network_module.cpp
#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/events/data_requests.hpp>
#include <iostream>

namespace tejoy::detail::modules
{

  NetworkModule::NetworkModule(event_system::EventBus &bus, uint16_t port, Storage &storage) : Module(bus), udp_(port), storage_(storage) {}

  void NetworkModule::on_start()
  {
    storage_.data.emplace("network", nlohmann::json::object());
    storage_.data["network"].emplace("loss", nlohmann::json::object());
    storage_.data["network"].emplace("print", false);
    storage_.data["network"]["loss"].emplace("enable", false);
    storage_.data["network"]["loss"].emplace("every", 3);
    storage_.data["network"]["loss"].emplace("print", true);

    print_ = storage_.data["network"]["print"].get<bool>();
    simulate_loss_ = storage_.data["network"]["loss"]["enable"].get<bool>();
    loss_every_ = storage_.data["network"]["loss"]["every"].get<size_t>();
    print_loss_ = storage_.data["network"]["loss"]["print"].get<bool>();

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
    static size_t packet_counter = 0;
    if (simulate_loss_ && (++packet_counter % loss_every_ == 0))
    {
      if (print_loss_)
        std::cout << "Simulated loss message \"" << e.message << "\" to " << e.ip << ":" << e.port << std::endl;
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
