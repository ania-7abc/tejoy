// update_manager_module.cpp

#include <tejoy/detail/modules/update_manager_module.hpp>
#include <tejoy/events/data_requests.hpp>

#include <future>

#include <base64/base64.hpp>
#include <secret_box/secret_box.hpp>

namespace tejoy::detail::modules
{

  UpdateManagerModule::UpdateManagerModule(event_system::EventBus &bus, nlohmann::json &config) : Module(bus, config) {}

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
      config_.at("i").get_to(i_);

    encrypt_ = config_.emplace("encrypt", true).first.value().get<bool>();

    subscribe<events::detail::SendUpdateRequest>([this](auto &e)
                                                 { on_send_update_request(e); });
    subscribe<events::detail::SendAckUpdateRequest>(
        [this](auto &e)
        {
          nlohmann::json update({{"type", "ack"}, {"pkg_id", e.pkg_id}});
          on_send_update_request(events::detail::SendUpdateRequest(update, e.to));
        });
    subscribe<events::detail::PacketReceived>([this](auto &e)
                                              { on_packet_received(e); });
    subscribe<events::RequestI>([this](auto &e)
                                { e.promise.set_value(i_); });
  }

  void UpdateManagerModule::on_stop()
  {
  }

  void UpdateManagerModule::on_send_update_request(const events::detail::SendUpdateRequest &e)
  {
    nlohmann::json packet;
    if (encrypt_)
      packet = nlohmann::json({{"data", Base64::encode(i_.box.encrypt(e.update.dump(), e.to.box))}});
    else
      packet = nlohmann::json({{"data", e.update}});
    packet.emplace("id", Base64::encode(i_.box.get_public_key()));
    publish<events::detail::SendPacketRequest>(packet.dump(), e.to.ip, e.to.port);
  }

  void UpdateManagerModule::on_packet_received(const events::detail::PacketReceived &e)
  {
    nlohmann::json packet = nlohmann::json::parse(e.message.begin(), e.message.end());
    SecretBox from_box(Base64::decode(packet.at("id")));
    if (encrypt_)
      publish<events::detail::UpdateReceived>(
          nlohmann::json::parse(i_.box.decrypt(Base64::decode(packet.at("data")), from_box)),
          User{.box = from_box, .ip = e.ip, .port = e.port});
    else
      publish<events::detail::UpdateReceived>(
          packet.at("data"),
          User{.box = from_box, .ip = e.ip, .port = e.port});
  }

} // namespace tejoy::detail::modules
