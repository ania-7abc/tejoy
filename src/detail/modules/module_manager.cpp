// module_manager.cpp

#include <tejoy/detail/modules/module_manager.hpp>

namespace tejoy::detail::modules
{

ModuleManager::ModuleManager(event_system::EventBus &bus, nlohmann::json &data) : bus_(bus), data_(data)
{
}

void ModuleManager::start_all()
{
    std::vector<std::shared_ptr<Module>> modules;
    modules.reserve(modules_.size());
    for (const auto &[_, mod] : modules_)
    {
        modules.push_back(mod);
    }
    std::sort(modules.begin(), modules.end(),
              [](const auto &module1, const auto &module2) { return module1->priority() > module2->priority(); });
    for (const auto &mod : modules)
    {
        mod->on_start();
    }
}

void ModuleManager::stop_all()
{
    std::vector<std::shared_ptr<Module>> modules;
    modules.reserve(modules_.size());
    for (const auto &[_, mod] : modules_)
    {
        modules.push_back(mod);
    }
    std::sort(modules.begin(), modules.end(),
              [](const auto &module1, const auto &module2) { return module1->priority() < module2->priority(); });
    for (const auto &mod : modules)
    {
        mod->on_stop();
    }
}

} // namespace tejoy::detail::modules
