// ack_module.cpp
#include <nlohmann/json.hpp>
#include <tejoy/detail/modules/ack_module.hpp>
#include <tejoy/events/errors.hpp>

namespace tejoy::detail::modules
{

void AckModule::on_start()
{
    config_.emplace("max_attempts", static_cast<std::size_t>(3)).first.value().get_to(max_attempts_);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    config_.emplace("retry_interval_ms", static_cast<std::size_t>(2000)).first.value().get_to(retry_interval_ms_);

    work_guard_ = std::make_unique<boost::asio::io_context::work>(io_context_);
    io_thread_ = std::thread([this] { io_context_.run(); });

    subscribe<events::detail::SendUpdateRequest>([this](const auto &event) { on_send_update_request(event); });
    subscribe<events::detail::UpdateReceived>([this](const auto &event) { on_update_received(event); });
    subscribe_update(detail::UpdateTypes::ACK, [this](const auto &event) { on_ack_received(event); });
}

void AckModule::on_stop()
{
    work_guard_.reset();
    io_context_.stop();
    if (io_thread_.joinable())
    {
        io_thread_.join();
    }
}

void AckModule::on_send_update_request(const events::detail::SendUpdateRequest &event)
{
    if (event.update.value("no_ack", false))
    {
        return;
    }

    uint32_t pkg_id = event.update.at("pkg_id").get<uint32_t>();

    std::lock_guard<std::mutex> lock(pending_mutex_);
    auto found_update = pending_.find(pkg_id);
    if (found_update == pending_.end())
    {
        PendingUpdate update(event);
        update.attempts = 1;
        update.pkg_id = pkg_id;
        start_timer(update);
        pending_.emplace(pkg_id, std::move(update));
    }
    else
    {
        auto &update = found_update->second;
        if (++update.attempts < max_attempts_)
        {
            start_timer(update);
        }
        else
        {
            pending_.erase(found_update);
            publish<events::UpdateSendError>(pkg_id, "Max attempts exceeded");
        }
    }
}

void AckModule::on_ack_received(const events::detail::UpdateReceived &event)
{
    std::lock_guard<std::mutex> lock(pending_mutex_);
    auto found_update = pending_.find(event.update.at("data").at("pkg_id").get<uint32_t>());
    if (found_update != pending_.end())
    {
        found_update->second.timer->cancel();
        pending_.erase(found_update);
    }
}

void AckModule::on_update_received(const events::detail::UpdateReceived &event) const
{
    if (event.update.value("no_ack", false) || event.update.at("type").get<std::string>() == detail::UpdateTypes::ACK)
    {
        return;
    }

    publish<events::detail::SendConfiguredUpdateRequest>(
        nlohmann::json({{"pkg_id", event.update.at("pkg_id").get<uint32_t>()}}), detail::UpdateTypes::ACK, event.sender,
        true);
}

void AckModule::start_timer(PendingUpdate &update)
{
    if (!update.timer)
    {
        update.timer = std::make_unique<boost::asio::steady_timer>(io_context_);
    }
    update.timer->expires_after(std::chrono::milliseconds(retry_interval_ms_));
    update.timer->async_wait([this, pkg_id = update.pkg_id](const auto &error_code) {
        if (!error_code)
        {
            handle_timeout(pkg_id);
        }
    });
}

void AckModule::handle_timeout(uint32_t pkg_id)
{
    std::lock_guard<std::mutex> lock(pending_mutex_);
    auto found_update = pending_.find(pkg_id);
    if (found_update != pending_.end())
    {
        publish<events::detail::SendUpdateRequest>(found_update->second.event.update,
                                                   found_update->second.event.recipient);
    }
}

} // namespace tejoy::detail::modules
