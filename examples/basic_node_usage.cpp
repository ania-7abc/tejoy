#include <atomic>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <nlohmann/json.hpp>
#include <tejoy/node.hpp>
#include <tejoy/events/detail/network.hpp>

std::atomic<bool> stop_flag;

int main()
{
    // Load config (1)
    nlohmann::json config;
    if (std::filesystem::exists("config.json"))
    {
        std::ifstream ifs("config.json");
        config = nlohmann::json::parse(ifs);
    }

    boost::asio::io_context io; // Create io_context

    // Set SIGINT/SIGTERM handler
    boost::asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([&io](boost::system::error_code, int)
    {
        std::cout << "Stoping...\n";
        stop_flag.store(true);
        io.stop();
    }); // (2)!

    tejoy::Node node(io.get_executor(), config); // Creating a Node

    auto& bus = node.get_bus();

    bus.subscribe<tejoy::events::detail::DatagramReceived>([](auto& event)
    {
        std::cout << "Received datagram from "
            << event.sender.address().to_string() << ':' << event.sender.port()
            << ": " << event.data << '\n';
    });

    auto endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 5768);
    for (int i = 0; i < 10; i++)
        bus.emit(tejoy::events::detail::requests::SendDatagram{
            endpoint,
            "Datagram number " + std::to_string(i)
        });

    while (!stop_flag.load())
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // (3)!

    // Save config (4)
    std::ofstream ofs("config.json");
    ofs << config.dump(4);
}
