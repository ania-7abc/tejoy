// update_sort_module.hpp
#pragma once

#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/packet_events.hpp>

#include <boost/circular_buffer.hpp>

namespace tejoy::detail::modules
{

  class UpdateSortModule : public tejoy::detail::modules::Module
  {
  public:
    UpdateSortModule(event_system::EventBus &bus);

    void on_start() override;
    void on_stop() override;

  private:
    void on_update_received(const tejoy::events::detail::UpdateReceived &e);

    boost::circular_buffer<uint32_t> last_ids_;
  };

} // namespace tejoy::detail::modules
