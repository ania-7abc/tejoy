// update_sorter_module.hpp
#pragma once

#include <tejoy/detail/modules/module.hpp>
#include <tejoy/events/detail/packet_events.hpp>

#include <boost/circular_buffer.hpp>

namespace tejoy::detail::modules
{

  class UpdateSorterModule : public tejoy::detail::modules::Module
  {
  public:
    UpdateSorterModule(event_system::EventBus &bus, nlohmann::json &config);

    void on_start() override;
    void on_stop() override;

  private:
    void on_update_received(const tejoy::events::detail::UpdateReceived &e);

    boost::circular_buffer<uint32_t> last_ids_;
  };

} // namespace tejoy::detail::modules
