// network_module.hpp
#pragma once
#include <cstdint>
#include <storage/storage.hpp>
#include <string>
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/packets.hpp>
#include <udp/udp.hpp>

namespace tejoy::detail::modules
{
class NetworkModule : public tejoy::detail::modules::Module
{
  public:
    using tejoy::detail::modules::Module::Module;
    void on_start() override;
    void on_stop() override;
    auto priority() const -> int override
    {
        return 100; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    }

  private:
    void on_send_packet_request(const tejoy::events::detail::SendPacketRequest &event);
    void on_network_message(const std::string &message, const std::string &sender_ip, uint16_t sender_port);

    std::optional<UDP> udp_;

    bool print_{};
    bool simulate_loss_{};
    size_t loss_percent_{};
};
} // namespace tejoy::detail::modules
