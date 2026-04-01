// user_code_helper.hpp
#pragma once
#include <event_system/subscriber.hpp>
#include <tejoy/events/errors.hpp>
#include <tejoy/user.hpp>

namespace tejoy
{

class UserCodeHelper : public event_system::Subscriber
{
  public:
    void send_message(const std::string &text, const User &recipient);
    void on_message(std::function<void(const std::string &, const User &)> handler);
    void on_discovered_node(std::function<void(const tejoy::User &)> handler);
    void on_invalid_update_error(std::function<void(const events::InvalidUpdateError &)> handler);
    void on_update_send_error(std::function<void(const events::UpdateSendError &)> handler);
    void on_log(std::function<void(const std::string &event_type, const std::string &from)> handler);

    std::string get_ip();
    uint16_t get_port();
    std::string get_discovery_ip();
    User get_i();

  private:
    friend class Node;
    using Subscriber::Subscriber;

    void start();

    std::function<void(const std::string &, const User &)> on_message_handler = [](auto, auto) {};
    std::function<void(const tejoy::User &)> on_discovered_node_handler = [](auto) {};
    std::function<void(const events::InvalidUpdateError &)> on_invalid_update_error_handler = [](auto) {};
    std::function<void(const events::UpdateSendError &)> on_update_send_error_handler = [](auto) {};
    std::function<void(const std::string &event_type, const std::string &from)> on_log_handler = [](auto, auto) {};
};

} // namespace tejoy
