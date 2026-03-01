// main.cpp
#include <iostream>
#include <atomic>
#include <csignal>

#include <tejoy/node.hpp>
#include <tejoy/events/detail/packet_events.hpp>
#include <tejoy/events/data_request.hpp>
#include "config.hpp"
#include <future>

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

  auto sub1 = node.get_event_bus().make_subscriber<tejoy::events::detail::PacketReceived>(
      [](auto &e)
      {
        std::cout << "Packet with text \"" << e.message << "\" from " << e.ip << ":" << e.port << std::endl;
      });
  auto sub2 = node.get_event_bus().make_subscriber<tejoy::events::detail::UpdateReceived>(
      [](auto &e)
      {
        std::cout << "Packet with text \"" << e.update << "\" from " << e.from.ip << ":" << e.from.port << std::endl;
      });
  std::promise<tejoy::User> promise;
  std::future<tejoy::User> fut = promise.get_future();
  node.get_event_bus().publish<tejoy::events::RequestI>(nullptr, promise);
  tejoy::User i = fut.get();
  node.get_event_bus().publish<tejoy::events::detail::SendUpdateRequest>(nullptr, nlohmann::json{{"text", "Hello, World!"}} /* update */, i /* to */);

  while (!need_stop)
  {
    // Work
  }

  return 0;
}
