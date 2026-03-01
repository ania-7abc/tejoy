// main.cpp
#include <iostream>
#include <atomic>
#include <csignal>

#include <tejoy/node.hpp>
#include <tejoy/events/detail/packet_events.hpp>
#include "config.hpp"

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

  auto sub = node.get_event_bus().make_subscriber<tejoy::events::detail::PacketReceived>(
      [](auto &e)
      {
        std::cout << "Packet with text \"" << e.message << "\" from " << e.ip << ":" << e.port << std::endl;
      });
  node.get_event_bus().publish<tejoy::events::detail::SendPacketRequest>(sub.get(), "Hello, World!", "127.0.0.1", PORT);

  while (!need_stop)
  {
    // Work
  }

  return 0;
}
