// log_module.hpp
#pragma once
#include <event_system/event.hpp>
#include <tejoy/detail/modules/module.hpp>

namespace tejoy::detail::modules
{

class LogModule : public Module
{
  public:
    explicit LogModule(std::shared_ptr<event_system::EventBus> bus, nlohmann::json &config);
    void run_subscribes() override;
    static auto priority() -> int
    {
        return 1000; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    }

  private:
    void on_any_event(const event_system::AnyEvent &event) const;

    std::vector<std::string> filter_from_;
    bool reply_event_{};
    bool reply_console_{};
    std::string reply_file_;
    bool short_names_{};
};

} // namespace tejoy::detail::modules
