// module_manager.hpp
#pragma once

#include <tejoy/detail/modules/module.hpp>

#include <memory>
#include <typeindex>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include <storage/storage.hpp>

namespace tejoy::detail::modules
{

class ModuleManager
{
  public:
    explicit ModuleManager(event_system::EventBus &bus, Storage &storage);

    template <typename T, typename... Args>
    auto create_module(const nlohmann::json_pointer<std::string> &path_in_config, Args &&...args) -> T &
    {
        static_assert(std::is_base_of_v<Module, T>, "T must be derived from Module");
        if (!storage_.data().contains(path_in_config))
        {
            storage_.data()[path_in_config] = {};
        }
        auto module = std::make_shared<T>(bus_, storage_.data().at(path_in_config), std::forward<Args>(args)...);
        if (modules_.find(std::type_index(typeid(T))) != modules_.end())
        {
            throw std::runtime_error("Module of this type already exists");
        }
        modules_[std::type_index(typeid(T))] = module;
        return *module;
    }

    template <typename T> auto get_module() -> T &
    {
        auto module = modules_.find(std::type_index(typeid(T)));
        if (module == modules_.end())
        {
            throw std::runtime_error("Module not found");
        }
        return *std::dynamic_pointer_cast<T>(module->second);
    }

    template <typename T> void restart_module()
    {
        auto &mod = get_module<T>();
        mod.on_stop();
        mod.on_start();
    }

    template <typename T, typename... Args> void recreate_module(Args &&...args)
    {
        auto module = modules_.find(std::type_index(typeid(T)));
        if (module != modules_.end())
        {
            module->second->on_stop();
            modules_.erase(module);
        }
        create_module<T>(std::forward<Args>(args)...);
    }

    void start_all();
    void stop_all();

  private:
    Storage &storage_;            // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    event_system::EventBus &bus_; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    std::unordered_map<std::type_index, std::shared_ptr<tejoy::detail::modules::Module>> modules_;
};

} // namespace tejoy::detail::modules
