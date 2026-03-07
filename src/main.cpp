// main.cpp

#include "config.hpp"

#include <atomic>
#include <csignal>
#include <future>
#include <iostream>

#include <tejoy/events/data_requests.hpp>
#include <tejoy/events/message.hpp>
#include <tejoy/events/updates.hpp>
#include <tejoy/events/discovery.hpp>
#include <tejoy/node.hpp>

static std::atomic<bool> need_stop = false;
extern "C" void handle_signal(int signal)
{
  if (signal == SIGINT)
  {
    std::cout << "\rShutting down..." << std::endl;
    need_stop = true;
  }
}

int main()
{
  std::signal(SIGINT, handle_signal);
  std::cout << "Press Ctrl+C to exit" << std::endl;

  tejoy::Node node("data", PORT);

  auto sub1 = node.get_event_bus().make_subscriber<tejoy::events::MessageUpdateReceived>(
      [](auto &e)
      {
        std::cout << "\"" << e.text << "\" from " << e.from.ip << ":" << e.from.port << std::endl;
      });
  auto sub2 = node.get_event_bus().make_subscriber<tejoy::events::DiscoveredNewNode>(
      [](auto &e)
      {
        std::cout << "Found " << e.node.ip << std::endl;
      });

  std::promise<tejoy::User> promise;
  std::future<tejoy::User> fut = promise.get_future();
  node.get_event_bus().publish<tejoy::events::RequestI>(nullptr, promise);
  tejoy::User i = fut.get();
  
  node.get_event_bus().publish<tejoy::events::SendMessageRequest>(nullptr, "Hello, World!", i);
  node.get_event_bus().publish<tejoy::events::SendMessageRequest>(nullptr, "Hello, World!", i);

  while (!need_stop)
  {
    // Work
  }

  return 0;
}
