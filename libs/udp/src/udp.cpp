// udp.cpp
#include <udp/udp.hpp>

#include <exception>

UDP::UDP(uint16_t port)
    : io_(), work_guard_(boost::asio::make_work_guard(io_)), thread_([this]()
                                                                     { io_.run(); }),
      socket_(io_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
      running_(true)
{
}

UDP::~UDP()
{
  stop();
}

void UDP::start(Callback callback)
{
  if (!running_)
    throw std::runtime_error("UDP is not running");
  this->callback_ = callback;
  receive();
}

void UDP::stop()
{
  running_ = false;
  work_guard_.reset();
  io_.stop();
  if (thread_.joinable())
    thread_.join();
}

void UDP::send(const std::string &msg, const std::string &ip, uint16_t port)
{
  auto ep = boost::asio::ip::udp::endpoint(
      boost::asio::ip::make_address(ip), port);

  socket_.async_send_to(boost::asio::buffer(msg), ep,
                        [this](auto ec, size_t) {});
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
