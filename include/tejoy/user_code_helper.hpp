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
    void ping(const User &ping_user);

    template <typename EventType> void on(std::function<void(const EventType &event)> handler)
    {
        handlers.push_back(bus().make_subscriber<EventType>([handler](auto &event) { handler(event); }));
    }

    void on_message(std::function<void(const std::string &text, const User &from)> handler);
    void on_discovered_node(std::function<void(const tejoy::User &node)> handler);
    void on_invalid_update_error(std::function<void(const events::InvalidUpdateError &error)> handler);
    void on_update_send_error(std::function<void(const events::UpdateSendError &error)> handler);
    void on_log(std::function<void(const std::string &event_type, const std::string &from)> handler);
    void on_ping_ok(std::function<void(const User &ping_user)> handler);

    std::string get_ip();
    uint16_t get_port();
    std::string get_discovery_ip();
    User get_i();

  private:
    friend class Node;
    using Subscriber::Subscriber;

    void start();

    std::vector<std::shared_ptr<Subscriber>> handlers;
};

} // namespace tejoy
