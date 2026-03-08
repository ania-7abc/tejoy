// udp.hpp
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <string>
#include <thread>

#include <boost/asio.hpp>

class UDP
{
  public:
    using Callback =
        std::function<void(const std::string &message, const std::string &sender_ip, uint16_t sender_port)>;
    explicit UDP(uint16_t port);
    ~UDP();
    UDP(const UDP &) = delete;
    auto operator=(const UDP &) -> UDP & = delete;
    UDP(UDP &&) = delete;
    auto operator=(UDP &&) -> UDP & = delete;

    void set_callback(Callback callback);
    void stop();

    void send(const std::string &msg, const std::string &recipient_ip, uint16_t recipient_port);

    void join_multicast_group(const std::string &multicast_ip);
    void leave_multicast_group(const std::string &multicast_ip);
    void set_multicast_interface(const std::string &local_ip);

  private:
    void receive();

    boost::asio::io_context io_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
    std::thread thread_;

    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint sender_;
    // 65536 bytes max. NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    std::array<char, 1 << 16> buffer_{};
    Callback callback_;
    bool running_{true};
};
