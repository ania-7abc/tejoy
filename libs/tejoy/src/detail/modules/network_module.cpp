// network_module.cpp

#include <tejoy/detail/modules/network_module.hpp>
#include <tejoy/events/data_requests.hpp>
#include <tejoy/events/detail/multicast_events.hpp>

#include <iostream>

namespace tejoy::detail::modules
{

  void NetworkModule::on_start()
  {
    srand(time(nullptr));

    config_.emplace("loss", nlohmann::json::object());
    config_.emplace("print", false);
    config_["loss"].emplace("enable", false);
    config_["loss"].emplace("percent", 40);

    print_ = config_["print"].get<bool>();
    simulate_loss_ = config_["loss"]["enable"].get<bool>();
    loss_percent_ = config_["loss"]["percent"].get<int>();

    std::promise<uint16_t> promise;
    auto ft = promise.get_future();
    publish<events::RequestPort>(promise);

    udp_.emplace(ft.get());
    udp_.value().set_callback([this](auto &message, auto &ip, auto port)
                              { on_network_message(message, ip, port); });
    subscribe<events::detail::SendPacketRequest>([this](auto &e)
                                                 { on_send_packet_request(e); });

    subscribe<events::detail::JoinMulticastGroupRequest>([this](auto &e)
                                                         { udp_.value().join_multicast_group(e.ip); });
    subscribe<events::detail::LeaveMulticastGroupRequest>([this](auto &e)
                                                          { udp_.value().leave_multicast_group(e.ip); });
  }

  void NetworkModule::on_stop()
  {
    udp_.value().stop();
    udp_.reset();
  }

  void NetworkModule::on_send_packet_request(const events::detail::SendPacketRequest &e)
  {
    if (rand() % 100 < loss_percent_)
    {
      if (print_)
        std::cout << "Not sent message \"" << e.message << "\" to " << e.ip << ":" << e.port << std::endl;
      return;
    }
    udp_.value().send(e.message, e.ip, e.port);
    if (print_)
      std::cout << "Sent message \"" << e.message << "\" to " << e.ip << ":" << e.port << std::endl;
  }

  void NetworkModule::on_network_message(const std::string &message, const std::string &ip, uint16_t port)
  {
    publish<events::detail::PacketReceived>(message, ip, port);
  }

} // namespace tejoy::detail::modules
