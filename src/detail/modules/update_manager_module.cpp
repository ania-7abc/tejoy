// update_manager_module.cpp
#include <base64/base64.hpp>
#include <future>
#include <secret_box/secret_box.hpp>
#include <tejoy/detail/modules/update_manager_module.hpp>
#include <tejoy/events/data_requests.hpp>

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
    last_id_iterators_ = boost::circular_buffer<std::set<Key>::iterator>(
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        config_.emplace("dedup_buf_size", static_cast<std::size_t>(10)).first.value().get<std::size_t>());

    subscribe<events::detail::SendUpdateRequest>([this](auto &event) { on_send_update_request(event); });
    subscribe<events::detail::PacketReceived>([this](auto &event) { on_packet_received(event); });
    subscribe<events::RequestI>([this](auto &event) { event.promise.set_value(i_); });

    subscribe<events::detail::SendConfiguredUpdateRequest>([this](auto &event) {
        std::string key;
        if (event.recipient.box.has_public_key())
        {
            key = Base64::encode(event.recipient.box.get_public_key());
        }
        else
        {
            key = "no_id";
        }
        uint32_t pkg_id = config_.emplace("last_pkg_id", nlohmann::json::object())
                              .first.value()
                              .value(key, static_cast<uint32_t>(0)) +
                          1;
        config_["last_pkg_id"][key] = pkg_id;

        nlohmann::json update({
            {"type", event.type},
            {"pkg_id", pkg_id},
            {"data", event.data},
            {"no_ack", event.no_ack},
        });
        publish<events::detail::SendUpdateRequest>(update, event.recipient);
    });
}

void UpdateManagerModule::on_stop()
{
}

void UpdateManagerModule::on_send_update_request(const events::detail::SendUpdateRequest &event) const
{
    nlohmann::json packet;
    bool no_encrypt = false;

    no_encrypt |= always_no_encrypt_;
    no_encrypt |= !event.recipient.box.has_public_key();

    if (!no_encrypt)
    {
        packet = nlohmann::json({{"data", Base64::encode(i_.box.encrypt(event.update.dump(), event.recipient.box))}});
    }
    else
    {
        packet = nlohmann::json({{"data", event.update}});
    }
    packet.emplace("id", Base64::encode(i_.box.get_public_key()));
    publish<events::detail::SendPacketRequest>(packet.dump(), event.recipient.ip, event.recipient.port);
}

void UpdateManagerModule::on_packet_received(const events::detail::PacketReceived &event)
{
    nlohmann::json packet = nlohmann::json::parse(event.message.begin(), event.message.end());
    std::vector<uint8_t> pub_raw = Base64::decode(packet.at("id"));
    User sender{.box = SecretBox(pub_raw), .ip = event.sender_ip, .port = event.sender_port};

    nlohmann::json update;
    if (packet.at("data").is_string())
    {
        update = nlohmann::json::parse(i_.box.decrypt(Base64::decode(packet.at("data")), sender.box));
    }
    else
    {
        update = packet.at("data");
    }

    if (is_duplicate(pub_raw, update.at("pkg_id")))
    {
        return;
    }
    publish<events::detail::UpdateReceived>(update, sender);
}

auto UpdateManagerModule::is_duplicate(const std::vector<uint8_t> &recipient_id, uint32_t pkg_id) -> bool
{
    auto key = std::make_pair(recipient_id, pkg_id);

    auto [it, inserted] = last_ids_.insert(key);
    last_id_iterators_.push_back(it);

    if (!inserted)
    {
        return true;
    }

    if (last_id_iterators_.full())
    {
        auto oldest_it = last_id_iterators_.front();
        last_ids_.erase(oldest_it);
        last_id_iterators_.pop_front();
    }

    return false;
}

} // namespace tejoy::detail::modules
