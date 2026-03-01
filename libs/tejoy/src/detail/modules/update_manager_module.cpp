// MIT License
//
// Copyright (c) 2026 Anya Baykina Dmitrievna
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// update_manager_module.cpp
#include <tejoy/detail/modules/update_manager_module.hpp>
#include <tejoy/events/data_request.hpp>
#include <future>
#include <base64.hpp>
#include <secret_box.hpp>

namespace tejoy::detail::modules
{
    UpdateManagerModule::UpdateManagerModule(event_system::EventBus &bus, Storage &storage) : Module(bus), storage_(storage) {}

    void UpdateManagerModule::on_start()
    {
        if (!storage_.data.contains("/i"_json_pointer))
        {
            std::promise<uint16_t> promise_port;
            std::future<uint16_t> fut_port = promise_port.get_future();
            publish<tejoy::events::RequestPort>(promise_port);

            std::promise<std::string> promise_ip;
            std::future<std::string> fut_ip = promise_ip.get_future();
            publish<tejoy::events::RequestIp>(promise_ip);

            i_ = User{.box = SecretBox(), .ip = fut_ip.get(), .port = fut_port.get()};
            storage_.data["/i"_json_pointer] = i_;
        }
        else
            storage_.data.at("/i"_json_pointer).get_to(i_);

        subscribe<tejoy::events::detail::SendUpdateRequest>([this](auto &e)
                                                            { onSendUpdateRequest(e); });
        subscribe<tejoy::events::detail::PacketReceived>([this](auto &e)
                                                         { onPacketReceived(e); });
        subscribe<tejoy::events::RequestI>([this](auto &e)
                                           { e.promise.set_value(i_); });
    }

    void UpdateManagerModule::on_stop()
    {
    }

    void UpdateManagerModule::onSendUpdateRequest(const tejoy::events::detail::SendUpdateRequest &e)
    {
        nlohmann::json packet({{"data", Base64::encode(i_.box.encrypt(e.update.dump(), e.to.box))},
                               {"id", Base64::encode(i_.box.get_public_key())}});
        publish<tejoy::events::detail::SendPacketRequest>(packet.dump(), e.to.ip, e.to.port);
    }

    void UpdateManagerModule::onPacketReceived(const tejoy::events::detail::PacketReceived &e)
    {
        nlohmann::json packet = nlohmann::json::parse(e.message.begin(), e.message.end());
        SecretBox from_box(Base64::decode(packet.at("id")));
        publish<tejoy::events::detail::UpdateReceived>(
            nlohmann::json::parse(i_.box.decrypt(Base64::decode(packet.at("data")), from_box)),
            User{.box = from_box, .ip = e.ip, .port = e.port});
    }

} // namespace tejoy::detail::modules
