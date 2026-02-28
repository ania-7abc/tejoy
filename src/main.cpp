// main.cpp
#include <iostream>
#include <atomic>
#include <csignal>
#include <queue>
#include <unistd.h>

#include <boost/asio.hpp>

#include "tejoy/node.hpp"
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

  boost::asio::io_context io_context;
  tejoy::Node node(io_context, "data");

  while (!need_stop)
  {
    // Work
  }

  io_context.stop();

  return 0;
}
