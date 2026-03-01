// main.cpp
#include <iostream>
#include <atomic>
#include <csignal>
#include <queue>
#include <unistd.h>

#include <boost/asio.hpp>

#include <tejoy/node.hpp>
#include <tejoy/events/detail/packet_events.hpp>
#include <event_system/subscriber.hpp>
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

class c : public event_system::Subscriber
{
public:
  c(event_system::EventBus &bus) : Subscriber(bus)
  {
  }
  void on_start()
  {
    subscribe<PacketReceivedEvent>([this](const PacketReceivedEvent &e)
                                   { std::cout << e.message << " from " << e.ip << ":" << e.port << std::endl; });
    publish<NeedSendPacketEvent>("message", "127.0.0.1", PORT);
  }
};

int main()
{
  std::signal(SIGINT, handle_signal);
  std::cout << "Press Ctrl+C to exit" << std::endl;

  tejoy::Node node("data", PORT);

  sleep(1);

  auto cl = std::make_shared<c>(node.get_event_bus());
  cl->on_start();

  while (!need_stop)
  {
    // Work
  }

  return 0;
}
