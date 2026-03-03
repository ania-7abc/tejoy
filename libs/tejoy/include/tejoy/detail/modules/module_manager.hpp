// module_manager.hpp
#pragma once

#include <tejoy/detail/modules/module.hpp>

#include <memory>
#include <typeindex>
#include <unordered_map>

namespace tejoy::detail::modules
{

  class ModuleManager
  {
  public:
    explicit ModuleManager(event_system::EventBus &bus);

    template <typename T, typename... Args>
    T &create_module(Args &&...args)
    {
      static_assert(std::is_base_of_v<Module, T>, "T must be derived from Module");
      auto module = std::make_shared<T>(bus_, std::forward<Args>(args)...);
      auto it = modules_.find(std::type_index(typeid(T)));
      if (it != modules_.end())
        throw std::runtime_error("Module of this type already exists");
      modules_[std::type_index(typeid(T))] = module;
      return *module;
    }

    template <typename T>
    T &get_module()
    {
      auto it = modules_.find(std::type_index(typeid(T)));
      if (it == modules_.end())
        throw std::runtime_error("Module not found");
      return *std::dynamic_pointer_cast<T>(it->second);
    }

    template <typename T>
    void restart_module()
    {
      auto &mod = get_module<T>();
      mod.on_stop();
      mod.on_start();
    }

    template <typename T, typename... Args>
    void recreate_module(Args &&...args)
    {
      auto it = modules_.find(std::type_index(typeid(T)));
      if (it != modules_.end())
      {
        it->second->on_stop();
        modules_.erase(it);
      }
      create_module<T>(std::forward<Args>(args)...);
    }

    void start_all();
    void stop_all();

  private:
    event_system::EventBus &bus_;
    std::unordered_map<std::type_index, std::shared_ptr<tejoy::detail::modules::Module>> modules_;
  };

} // namespace tejoy::detail::modules
