// module_manager.hpp
#pragma once
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <tejoy/detail/modules/module.hpp>
#include <vector>

namespace tejoy::detail::modules
{

class ModuleManager
{
  public:
    explicit ModuleManager(std::shared_ptr<event_system::EventBus> bus, std::shared_ptr<nlohmann::json> data);

    ~ModuleManager();
	ModuleManager(const ModuleManager &) = default;
	ModuleManager(ModuleManager &&) = default;
	auto operator=(const ModuleManager &) -> ModuleManager & = default;
	auto operator=(ModuleManager &&) -> ModuleManager & = default;

    template <typename T, typename... Args>
    void create_module(const nlohmann::json_pointer<std::string> &path_in_config, Args &&...args)
    {
        data_->emplace(path_in_config, nlohmann::json());
        auto factory = [this, path_in_config,
                        ... args = std::forward<Args>(args)]() mutable -> std::shared_ptr<Module> {
            return std::make_shared<T>(bus_, data_->at(path_in_config), std::move(args)...);
        };

        entries_.emplace_back(T::priority(), std::move(factory));
    }

    void start_all();

  private:
    struct ModuleEntry
    {
        int priority{};
        std::function<std::shared_ptr<Module>()> factory;
    };

    std::shared_ptr<event_system::EventBus> bus_;
    std::shared_ptr<nlohmann::json> data_;
    std::vector<ModuleEntry> entries_;
    std::vector<std::shared_ptr<Module>> modules_;
};

} // namespace tejoy::detail::modules