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

class AckModule : public tejoy::detail::modules::Module
{
  public:
    using tejoy::detail::modules::Module::Module;
    void on_start() override;
    void on_stop() override;

  private:
    struct PendingUpdate
    {
        tejoy::events::detail::SendUpdateRequest event;
        size_t attempts{};
        std::unique_ptr<boost::asio::steady_timer> timer;
        uint32_t pkg_id{};
        explicit PendingUpdate(tejoy::events::detail::SendUpdateRequest event) : event(std::move(event))
        {
        }
    };

    void on_send_update_request(const tejoy::events::detail::SendUpdateRequest &event);
    void on_ack_received(const tejoy::events::detail::UpdateReceived &event);

    void on_update_received(const tejoy::events::detail::UpdateReceived &event) const;

    void start_timer(PendingUpdate &update);
    void handle_timeout(uint32_t pkg_id);

    boost::asio::io_context io_context_;
    std::unique_ptr<boost::asio::io_context::work> work_guard_;
    std::thread io_thread_;
    std::unordered_map<uint32_t, PendingUpdate> pending_;
    std::mutex pending_mutex_;
    size_t max_attempts_{};
    size_t retry_interval_ms_{};
};

} // namespace tejoy::detail::modules
