// ack_module.cpp
#include <nlohmann/json.hpp>
#include <tejoy/detail/modules/ack_module.hpp>
#include <tejoy/events/errors.hpp>

namespace tejoy::detail::modules
{

AckModule::AckModule(event_system::EventBus &bus, nlohmann::json &config)
    : Module::Module(bus, config), io_context_(), work_guard_(boost::asio::make_work_guard(io_context_))
{
    config_.emplace("max_attempts", static_cast<std::size_t>(3)).first.value().get_to(max_attempts_);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    config_.emplace("retry_interval_ms", static_cast<std::size_t>(2000)).first.value().get_to(retry_interval_ms_);

    io_thread_ = std::jthread([this] { io_context_.run(); });
}

void AckModule::run_subscribes()
{
    subscribe<events::detail::SendRawUpdateRequest>([this](const auto &event) { on_send_update_request(event); });
    subscribe<events::detail::UpdateReceived>([this](const auto &event) { on_update_received(event); });
    subscribe_update(detail::UpdateTypes::ACK, [this](const auto &event) { on_ack_received(event); });
}

AckModule::~AckModule()
{
    work_guard_.reset();
    io_context_.stop();
    if (io_thread_.joinable())
        io_thread_.join();
}

void AckModule::on_send_update_request(const events::detail::SendRawUpdateRequest &event)
{
    if (event.update.value("no_ack", false))
        return;

    const uint32_t pkg_id = event.update.at("pkg_id").get<uint32_t>();

    std::scoped_lock lock(pending_mutex_);
    if (const auto found_update = pending_.find(pkg_id); found_update == pending_.end())
    {
        PendingUpdate update(event);
        update.attempts = 1;
        update.pkg_id = pkg_id;
        start_timer(update);
        pending_.try_emplace(pkg_id, std::move(update));
    }
    else if (auto &update = found_update->second; update.attempts++ < max_attempts_)
    {
        start_timer(update);
    }
    else
    {
        publish<events::UpdateSendError>(update.event, "Max attempts exceeded");
        pending_.erase(found_update);
    }
}
}

void AckModule::on_ack_received(const events::detail::UpdateReceived &event)
{
    std::scoped_lock lock(pending_mutex_);
    auto found_update = pending_.find(event.data.at("pkg_id").get<uint32_t>());
    if (found_update != pending_.end())
    {
        found_update->second.timer->cancel();
        pending_.erase(found_update);
    }
}

void AckModule::on_update_received(const events::detail::UpdateReceived &event) const
{
    if (event.no_ack || event.type == detail::UpdateTypes::ACK)
        return;

    publish<events::detail::SendUpdateRequest>(nlohmann::json({{"pkg_id", event.pkg_id}}), detail::UpdateTypes::ACK,
                                               event.sender, true);
}

void AckModule::start_timer(PendingUpdate &update)
{
    if (!update.timer)
        update.timer = std::make_unique<boost::asio::steady_timer>(io_context_);
    update.timer->expires_after(std::chrono::milliseconds(retry_interval_ms_));
    update.timer->async_wait([this, pkg_id = update.pkg_id](const auto &error_code) {
        if (!error_code)
            handle_timeout(pkg_id);
    });
}

void AckModule::handle_timeout(const uint32_t pkg_id)
{
    std::scoped_lock lock(pending_mutex_);
    auto found_update = pending_.find(pkg_id);
    if (found_update != pending_.end())
    {
        publish<events::detail::SendRawUpdateRequest>(found_update->second.event.update,
                                                      found_update->second.event.recipient);
    }
}

} // namespace tejoy::detail::modules
