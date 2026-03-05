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

void UDP::set_callback(Callback callback)
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

void UDP::join_multicast_group(const std::string &multicast_ip)
{
  auto address = boost::asio::ip::make_address(multicast_ip);
  socket_.set_option(boost::asio::ip::multicast::join_group(address));
}

void UDP::leave_multicast_group(const std::string &multicast_ip)
{
  auto address = boost::asio::ip::make_address(multicast_ip);
  socket_.set_option(boost::asio::ip::multicast::leave_group(address));
}

void UDP::set_multicast_interface(const std::string &local_ip)
{
  auto address = boost::asio::ip::make_address(local_ip);
  socket_.set_option(boost::asio::ip::multicast::outbound_interface(address.to_v4()));
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
