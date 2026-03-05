// udp.hpp
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <thread>
#include <string>

#include <boost/asio.hpp>

class UDP
{
public:
  using Callback = std::function<void(const std::string &message, const std::string &ip, uint16_t port)>;
  UDP(uint16_t port);
  ~UDP();
  void set_callback(Callback callback);
  void stop();

  void send(const std::string &msg, const std::string &ip, uint16_t port);

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
  std::array<char, 1 << 16> buffer_;
  Callback callback_;
  bool running_;
};
