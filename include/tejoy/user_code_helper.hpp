// user_code_helper.hpp
#pragma once
#include <event_system/subscriber.hpp>
#include <tejoy/events/contacts.hpp>
#include <tejoy/events/errors.hpp>
#include <tejoy/user.hpp>

namespace tejoy
{

class UserCodeHelper : public event_system::Subscriber
{
  public:
    void send_message(const std::string &text, const User &recipient) const;
    auto ping(const User &ping_user) const -> std::future<User>;
    auto add_contact(User &contact) const -> std::future<bool>;

    template <typename EventType> void on(std::function<void(const EventType &event)> handler)
    {
        handlers.push_back(bus().make_subscriber<EventType>([handler](auto &event) { handler(event); }));
    }

    void on_message(const std::function<void(const std::string &text, const User &from)> &handler);
    void on_discovered_node(const std::function<void(const User &node)> &handler);
    void on_update_send_error(std::function<void(const events::UpdateSendError &error)> handler);
    void on_log(const std::function<void(const std::string &event_type, const std::string &from)> &handler);

    [[nodiscard]] auto get_ip() const -> std::future<std::string>;
    [[nodiscard]] auto get_port() const -> std::future<uint16_t>;
    [[nodiscard]] auto get_discovery_ip() const -> std::future<std::string>;
    [[nodiscard]] auto get_i() const -> std::future<User>;
    [[nodiscard]] auto get_contacts() const -> std::future<std::vector<User>>;
    [[nodiscard]] auto get_history(const User &user) const -> std::future<events::History>;

  private:
    friend class Node;
    using Subscriber::Subscriber;

    void start();

    std::vector<std::shared_ptr<Subscriber>> handlers;
};

} // namespace tejoy
