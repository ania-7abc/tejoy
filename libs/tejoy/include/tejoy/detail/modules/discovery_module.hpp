// discover_module.hpp
#pragma once

#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/updates.hpp>

#include <nlohmann/json.hpp>

namespace tejoy::detail::modules
{

  class DiscoveryModule : public tejoy::detail::modules::Module
  {
  public:
    explicit DiscoveryModule(event_system::EventBus &bus, nlohmann::json &config);
    void on_start() override;
    void on_stop() override;

  private:
    void on_allo_received(const tejoy::events::AlloUpdateReceived &e);
    void on_imok_received(const tejoy::events::ImokUpdateReceived &e);
    void on_timer();
    void start_timer();

    boost::asio::io_context io_context_;
    std::unique_ptr<boost::asio::io_context::work> work_guard_;
    std::thread io_thread_;

    boost::asio::steady_timer timer_;

    std::string discovery_ip_;
    uint16_t discovery_port_;
    size_t ping_interval_s_;
    bool anonymous_;
  };

} // namespace tejoy::detail::modules
