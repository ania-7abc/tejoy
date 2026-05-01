// module_manager.cpp
#include <tejoy/detail/modules/module_manager.hpp>

namespace tejoy::detail::modules
{

ModuleManager::ModuleManager(std::shared_ptr<event_system::EventBus> bus, std::shared_ptr<nlohmann::json> data)
    : bus_(std::move(bus)), data_(std::move(data))
{
}

ModuleManager::~ModuleManager()
{
    modules_.clear();
}

void ModuleManager::start_all()
{
    std::ranges::sort(entries_, [](const ModuleEntry &module_a, const ModuleEntry &module_b) -> bool {
        return module_a.priority > module_b.priority;
    });

    for (const auto &[priority, factory] : entries_)
    {
        auto module_ptr = factory();
        module_ptr->run_subscribes();
        modules_.push_back(std::move(module_ptr));
    }

    entries_.clear();
}

} // namespace tejoy::detail::modules
