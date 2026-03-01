// MIT License
//
// Copyright (c) 2026 Anya Baykina Dmitrievna
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// udp.cpp
#include <udp.hpp>

#include <boost/asio.hpp>
#include <cstdint>
#include <functional>
#include <thread>
#include <exception>
#include <string>
#include <array>
#include <iostream>

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
