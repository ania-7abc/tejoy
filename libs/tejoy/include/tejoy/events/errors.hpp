// errors.hpp
#pragma once

#include <string>

#include <event_system/event.hpp>

namespace tejoy::events
{

  struct UpdateSendError : event_system::Event
  {
    UpdateSendError(uint32_t pkg_id, std::string reason) : pkg_id(pkg_id), reason(std::move(reason)) {}
    uint32_t pkg_id;
    std::string reason;
  };

  struct InvalidUpdateError : event_system::Event
  {
    InvalidUpdateError(uint32_t pkg_id, std::string type, std::string reason)
        : pkg_id(pkg_id), type(std::move(type)), reason(std::move(reason)) {}
    uint32_t pkg_id;
    std::string type;
    std::string reason;
  };

} // namespace tejoy::events
