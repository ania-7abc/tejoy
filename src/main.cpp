// main.cpp

#include "config.hpp"

#include <atomic>
#include <csignal>
#include <future>
#include <iostream>

#include <tejoy/events/data_requests.hpp>
#include <tejoy/events/detail/updates.hpp>
#include <tejoy/events/discovery.hpp>
#include <tejoy/events/message.hpp>
#include <tejoy/node.hpp>

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static std::atomic<bool> need_stop = false;
extern "C" void handle_signal(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "\rShutting down..." << '\n';
        need_stop = true;
    }
}

auto main() -> int
{
    std::signal(SIGINT, handle_signal);
    std::cout << "Press Ctrl+C to exit" << '\n';

    tejoy::Node node("data");

    auto sub1 = node.get_event_bus().make_subscriber<tejoy::events::detail::UpdateReceived>([](auto &event) {
        if (event.update.at("type").template get<std::string>() == "message")
        {
            std::cout << "\"" << event.update.at("data").at("text").template get<std::string>() << "\" from "
                      << event.sender.ip << ":" << event.sender.port << std::endl;
        }
    });
    auto sub2 = node.get_event_bus().make_subscriber<tejoy::events::DiscoveredNewNode>(
        [](auto &event) { std::cout << "Found " << event.node.ip << std::endl; });

    std::promise<tejoy::User> promise;
    std::future<tejoy::User> fut = promise.get_future();
    node.get_event_bus().publish<tejoy::events::RequestI>(nullptr, promise);
    tejoy::User recipient = fut.get();

    node.get_event_bus().publish<tejoy::events::SendMessageRequest>(nullptr, "Hello, World!", recipient);
    node.get_event_bus().publish<tejoy::events::SendMessageRequest>(nullptr, "Hello, World!", recipient);

    while (!need_stop)
    {
        // Work
    }

    return 0;
}
