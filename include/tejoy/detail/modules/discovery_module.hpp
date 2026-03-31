// discover_module.hpp
#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>
#include <optional>
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/updates.hpp>

namespace tejoy::detail::modules
{

class DiscoveryModule : public tejoy::detail::modules::Module
{
  public:
    explicit DiscoveryModule(event_system::EventBus &bus, nlohmann::json &config);
    void on_start() override;
    void on_stop() override;

  private:
    void on_dis_find_received(const tejoy::events::detail::UpdateReceived &event);
    void on_dis_ok_received(const tejoy::events::detail::UpdateReceived &event);
    void on_timer();
    void start_timer();

    boost::asio::io_context io_context_;
    std::unique_ptr<boost::asio::io_context::work> work_guard_;
    std::thread io_thread_;

    boost::asio::steady_timer timer_;

    std::string discovery_ip_;
    uint16_t port_{};
    size_t ping_interval_s_{};
    bool anonymous_{};

    tejoy::User i_;
};

} // namespace tejoy::detail::modules
