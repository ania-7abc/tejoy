// network_module.hpp
#pragma once
#include <udp.hpp>
#include <string>
#include <cstdint>
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/packet_events.hpp>

namespace tejoy::detail::modules
{
    class NetworkModule : public Module
    {
    public:
        NetworkModule(event_system::EventBus &bus, uint16_t port);
        void on_start() override;
        void on_stop() override;
        void onNeedSendPacket(const NeedSendPacketEvent &e);
        void onNetworkMessage(const std::string &message, const std::string &ip, uint16_t port);

    private:
        UDP udp_;
    };
} // namespace tejoy::detail::modules
