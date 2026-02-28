#include <iostream>
#include <atomic>
#include <csignal>
#include <queue>
#include <unistd.h>

#include <nlohmann/json.hpp>

#include "config.hpp"

// Self-Write libs
#include <secret_box.hpp>
#include <udp.hpp>
#include <base64.hpp>
#include <simple_io.hpp>
#include <storage.hpp>

using namespace nlohmann;

Storage storage(STORAGE_DIR);

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

  storage.load();
  std::cout << "Press Ctrl+C to exit" << std::endl;

  while (!need_stop)
  {
    // Work
  }

  storage.save();
  return 0;
}
