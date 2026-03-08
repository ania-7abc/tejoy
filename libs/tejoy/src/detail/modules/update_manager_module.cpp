// update_manager_module.cpp

#include <tejoy/detail/modules/update_manager_module.hpp>
#include <tejoy/events/data_requests.hpp>

#include <future>

#include <base64/base64.hpp>
#include <secret_box/secret_box.hpp>

namespace tejoy::detail::modules
{

void UpdateManagerModule::on_start()
{
    if (!config_.contains("i"))
    {
        std::promise<uint16_t> promise_port;
        std::future<uint16_t> fut_port = promise_port.get_future();
        publish<events::RequestPort>(promise_port);

        std::promise<std::string> promise_ip;
        std::future<std::string> fut_ip = promise_ip.get_future();
        publish<events::RequestIp>(promise_ip);

        i_ = User{.box = SecretBox(), .ip = fut_ip.get(), .port = fut_port.get()};
        config_["i"] = i_;
    }
    else
    {
        config_.at("i").get_to(i_);
    }

    always_no_encrypt_ = config_.emplace("always_no_encrypt", false).first.value().get<bool>();

    subscribe<events::detail::SendUpdateRequest>([this](auto &event) { on_send_update_request(event); });
    subscribe<events::detail::SendAckUpdateRequest>([this](auto &event) {
        nlohmann::json update({{"type", "ack"}, {"pkg_id", event.pkg_id}});
        on_send_update_request(events::detail::SendUpdateRequest(update, event.recipient));
    });
    subscribe<events::detail::PacketReceived>([this](auto &event) { on_packet_received(event); });
    subscribe<events::RequestI>([this](auto &event) { event.promise.set_value(i_); });
}

void UpdateManagerModule::on_stop()
{
}

void UpdateManagerModule::on_send_update_request(const events::detail::SendUpdateRequest &event)
{
    nlohmann::json packet;
    bool no_encrypt = false;

    no_encrypt |= always_no_encrypt_;
    no_encrypt |= !event.recipient.box.has_public_key();
    no_encrypt |= event.update.value("no_encrypt", false);

    if (!no_encrypt)
    {
        packet = nlohmann::json({{"no_encrypt", false},
                                 {"data", Base64::encode(i_.box.encrypt(event.update.dump(), event.recipient.box))}});
    }
    else
    {
        packet = nlohmann::json({{"no_encrypt", true}, {"data", event.update}});
    }
    packet.emplace("id", Base64::encode(i_.box.get_public_key()));
    publish<events::detail::SendPacketRequest>(packet.dump(), event.recipient.ip, event.recipient.port);
}

void UpdateManagerModule::on_packet_received(const events::detail::PacketReceived &event)
{
    nlohmann::json packet = nlohmann::json::parse(event.message.begin(), event.message.end());
    SecretBox from_box(Base64::decode(packet.at("id")));
    if (!packet.at("no_encrypt").get<bool>())
    {
        publish<events::detail::UpdateReceived>(
            nlohmann::json::parse(i_.box.decrypt(Base64::decode(packet.at("data")), from_box)),
            User{.box = from_box, .ip = event.sender_ip, .port = event.sender_port});
    }
    else
    {
        publish<events::detail::UpdateReceived>(
            packet.at("data"), User{.box = from_box, .ip = event.sender_ip, .port = event.sender_port});
    }
}

} // namespace tejoy::detail::modules
