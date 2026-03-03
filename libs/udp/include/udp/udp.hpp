// udp.hpp
#pragma once
#include <cstdint>
#include <functional>
#include <thread>
#include <string>
#include <array>

#include <boost/asio.hpp>

class UDP
{
public:
  using Callback = std::function<void(const std::string &message, const std::string &ip, uint16_t port)>;
  UDP(uint16_t port);
  ~UDP();
  void start(Callback callback);
  void stop();
  void send(const std::string &msg, const std::string &ip, uint16_t port);

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
