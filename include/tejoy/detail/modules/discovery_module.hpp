// discover_module.hpp
#pragma once
#include <nlohmann/json.hpp>
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/updates.hpp>

namespace tejoy::detail::modules
{

class DiscoveryModule : public Module
{
  public:
    explicit DiscoveryModule(event_system::EventBus &bus, nlohmann::json &config);
    void run_subscribes() override;
    ~DiscoveryModule() override;
    static auto priority() -> int
    {
        return 0;
    }

  private:
    void on_dis_find_received(const events::detail::UpdateReceived &event) const;
    void on_dis_ok_received(const events::detail::UpdateReceived &event) const;
    void on_timer();
    void start_timer();

    boost::asio::io_context io_context_{};
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
    std::jthread io_thread_;

    boost::asio::steady_timer timer_;

    std::string discovery_ip_;
    uint16_t port_{};
    size_t ping_interval_s_{};
    bool anonymous_{};
    bool discover_i_{};

    User i_;
};

} // namespace tejoy::detail::modules
