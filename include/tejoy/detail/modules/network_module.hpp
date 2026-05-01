// network_module.hpp
#pragma once
#include <random>
#include <string>
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/packets.hpp>
#include <udp/udp.hpp>

namespace tejoy::detail::modules
{
class NetworkModule : public Module
{
  public:
    explicit NetworkModule(event_system::EventBus &bus, nlohmann::json &config);
    void run_subscribes() override;
    static auto priority() -> int
    {
        return 100; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    }

  private:
    void on_send_packet_request(const events::detail::SendPacketRequest &event);
    void on_network_message(const std::string &message, const std::string &sender_ip, uint16_t sender_port) const;

    std::mt19937 gen_;
    std::bernoulli_distribution loss_dist_;

    bool print_{};
    uint16_t port_{};
    float loss_percent_{};

    UDP udp_;
};
} // namespace tejoy::detail::modules
