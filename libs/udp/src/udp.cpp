// udp.cpp
#include <udp.hpp>

#include <boost/asio.hpp>
#include <cstdint>
#include <functional>
#include <thread>
#include <string>
#include <array>

UDP::UDP(uint16_t port)
    : io_(), socket_(io_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
      running_(true)
{

    thread_ = std::thread([this]()
                         { io_.run(); });
}

UDP::~UDP()
{
    running_ = false;
    io_.stop();
    if (thread_.joinable())
        thread_.join();
}

void UDP::start(Callback callback)
{
    this->callback_ = callback;
    receive();
}

void UDP::send(const std::string &msg, const std::string &ip, uint16_t port)
{
    auto ep = boost::asio::ip::udp::endpoint(
        boost::asio::ip::make_address(ip), port);

    socket_.async_send_to(boost::asio::buffer(msg), ep,
                         [](auto, auto) {});
}

void UDP::receive()
{
    if (!callback_)
        return;

    socket_.async_receive_from(boost::asio::buffer(buffer_), sender_,
                              [this](auto ec, size_t bytes)
                              {
                                  if (!ec && bytes > 0 && callback_)
                                  {
                                      callback_(std::string(buffer_.data(), bytes),
                                               sender_.address().to_string(),
                                               sender_.port());
                                  }
                                  if (running_)
                                      receive();
                              });
}
