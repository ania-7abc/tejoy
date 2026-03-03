// module_manager.cpp

#include <tejoy/detail/modules/module_manager.hpp>

namespace tejoy::detail::modules
{

  ModuleManager::ModuleManager(event_system::EventBus &bus) : bus_(bus) {}

  void ModuleManager::start_all()
  {
    for (auto &[_, mod] : modules_)
      mod->on_start();
  }

  void ModuleManager::stop_all()
  {
    for (auto &[_, mod] : modules_)
      mod->on_stop();
  }

} // namespace tejoy::detail::modules
