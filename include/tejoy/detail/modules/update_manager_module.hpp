// update_manager_module.hpp
#pragma once
#include <boost/circular_buffer.hpp>
#include <set>
#include <storage/storage.hpp>
#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/packets.hpp>
#include <tejoy/events/detail/updates.hpp>
#include <tejoy/user.hpp>

namespace tejoy::detail::modules
{

class UpdateManagerModule : public tejoy::detail::modules::Module
{
  public:
    using tejoy::detail::modules::Module::Module;
    void on_start() override;
    void on_stop() override;

  private:
    void on_send_update_request(const tejoy::events::detail::SendUpdateRequest &event) const;
    void on_packet_received(const tejoy::events::detail::PacketReceived &event);

    auto is_duplicate(const std::vector<uint8_t> &recipient_id, uint32_t pkg_id) -> bool;

    User i_;
    using Key = std::pair<std::vector<uint8_t>, uint32_t>;
    boost::circular_buffer<std::set<Key>::iterator> last_id_iterators_;
    std::set<Key> last_ids_;

    bool always_no_encrypt_{};
};

} // namespace tejoy::detail::modules
