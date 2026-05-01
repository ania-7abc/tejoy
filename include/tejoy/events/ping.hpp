// ping.hpp
#pragma once
#include <event_system/event.hpp>
#include <tejoy/user.hpp>

namespace tejoy::events
{

struct RequestPing : event_system::Event
{
    explicit RequestPing(const User &ping_user, std::function<void(User)> on_result,
                         std::function<void(std::exception_ptr)> on_error)
        : ping_user(std::move(ping_user)), on_result(std::move(on_result)), on_error(std::move(on_error))
    {
    }
    User ping_user;
    std::function<void(User)> on_result;
    std::function<void(std::exception_ptr)> on_error{};
};

class PingFailedError : public std::runtime_error
{
  public:
    explicit PingFailedError(const std::string &message) : std::runtime_error(message)
    {
    }
};

} // namespace tejoy::events
