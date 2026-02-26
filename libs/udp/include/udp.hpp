// udp.hpp
#ifndef UDP_HPP
#define UDP_HPP

#include <boost/asio.hpp>
#include <cstdint>
#include <functional>
#include <thread>
#include <string>
#include <array>

class UDP
{
public:
    using Callback = std::function<void(const std::string &message, const std::string &ip, uint16_t port)>;
    UDP(uint16_t port);
    ~UDP();
    void start(Callback callback);
    void send(const std::string &msg, const std::string &ip, uint16_t port);

private:
    void receive();

    boost::asio::io_context io_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint sender_;
    std::array<char, 1 << 16> buffer_;
    std::thread thread_;
    Callback callback_;
    bool running_;
};

#endif // UDP_HPP
