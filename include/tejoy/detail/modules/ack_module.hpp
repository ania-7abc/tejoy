// ack_module.hpp
#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/updates.hpp>
#include <thread>
#include <unordered_map>
#include <utility>

namespace tejoy::detail::modules
{

class AckModule : public Module
{
  public:
    explicit AckModule(event_system::EventBus &bus, nlohmann::json &config);
    ~AckModule() override;
    void run_subscribes() override;
    static auto priority() -> int
    {
        return 0;
    }

  private:
    struct PendingUpdate
    {
        events::detail::SendRawUpdateRequest event;
        size_t attempts{};
        std::unique_ptr<boost::asio::steady_timer> timer;
        uint32_t pkg_id{};
        explicit PendingUpdate(events::detail::SendRawUpdateRequest event) : event(std::move(event))
        {
        }
    };

    void on_send_update_request(const events::detail::SendRawUpdateRequest &event);
    void on_ack_received(const events::detail::UpdateReceived &event);

    void on_update_received(const events::detail::UpdateReceived &event) const;

    void start_timer(PendingUpdate &update);
    void handle_timeout(uint32_t pkg_id);

    boost::asio::io_context io_context_{};
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
    std::jthread io_thread_;
    std::unordered_map<uint32_t, PendingUpdate> pending_;
    std::mutex pending_mutex_;
    size_t max_attempts_{};
    size_t retry_interval_ms_{};
};

} // namespace tejoy::detail::modules
