#include <atomic>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <nlohmann/json.hpp>
#include <tejoy/node.hpp>

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

    while (!stop_flag.load())
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // (3)!

    // Save config (4)
    std::ofstream ofs("config.json");
    ofs << config.dump(4);
}
