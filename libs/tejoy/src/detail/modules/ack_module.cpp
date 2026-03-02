#include <tejoy/detail/modules/ack_module.hpp>
#include <tejoy/events/errors.hpp>

namespace tejoy::detail::modules
{

    AckModule::AckModule(event_system::EventBus &bus, size_t max_attempts)
        : Module(bus), max_attempts_(max_attempts) {}

    void AckModule::on_start()
    {
        work_guard_ = std::make_unique<boost::asio::io_context::work>(io_context_);
        io_thread_ = std::thread([this]
                                 { io_context_.run(); });

        subscribe<events::detail::SendUpdateRequest>(
            [this](const auto &e)
            { on_send_update_request(e); });
        subscribe<events::detail::UpdateReceived>(
            [this](const auto &e)
            { on_update_received(e); });
        subscribe<events::AckUpdateReceived>(
            [this](const auto &e)
            { on_ack_received(e); });
    }

    void AckModule::on_stop()
    {
        work_guard_.reset();
        io_context_.stop();
        if (io_thread_.joinable())
            io_thread_.join();
    }

    void AckModule::on_send_update_request(const events::detail::SendUpdateRequest &e)
    {
        uint32_t pkg_id = e.update.at("pkg_id").get<uint32_t>();

        std::lock_guard<std::mutex> lock(pending_mutex_);
        auto it = pending_.find(pkg_id);
        if (it == pending_.end())
        {
            PendingUpdate pu(e);
            pu.attempts = 1;
            pu.pkg_id = pkg_id;
            start_timer(pu);
            pending_.emplace(pkg_id, std::move(pu));
        }
        else
        {
            auto &pu = it->second;
            if (++pu.attempts < max_attempts_)
                start_timer(pu);
            else
            {
                pending_.erase(it);
                publish<events::UpdateSendError>(pkg_id, "Max attempts exceeded");
            }
        }
    }

    void AckModule::on_ack_received(const events::AckUpdateReceived &e)
    {
        std::lock_guard<std::mutex> lock(pending_mutex_);
        auto it = pending_.find(e.pkg_id);
        if (it != pending_.end())
        {
            it->second.timer->cancel();
            pending_.erase(it);
        }
    }

    void AckModule::on_update_received(const events::detail::UpdateReceived &e)
    {
        std::string type = e.update.at("type").get<std::string>();
        if (type != "ack")
            publish<events::detail::SendAckUpdateRequest>(e);
    }

    void AckModule::start_timer(PendingUpdate &pu)
    {
        if (!pu.timer)
            pu.timer = std::make_unique<boost::asio::steady_timer>(io_context_);
        pu.timer->expires_after(RETRY_INTERVAL);
        pu.timer->async_wait([this, pkg_id = pu.pkg_id](const boost::system::error_code &ec)
                             {
                                 if (!ec)
                                     handle_timeout(pkg_id);
                             });
    }

    void AckModule::handle_timeout(uint32_t pkg_id)
    {
        std::lock_guard<std::mutex> lock(pending_mutex_);
        auto it = pending_.find(pkg_id);
        if (it != pending_.end())
            publish<events::detail::SendUpdateRequest>(it->second.event);
    }

} // namespace tejoy::detail::modules
