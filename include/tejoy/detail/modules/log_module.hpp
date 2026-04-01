// log_module.hpp
#pragma once
#include <event_system/event.hpp>
#include <tejoy/detail/modules/module.hpp>

namespace tejoy::detail::modules
{

class LogModule : public tejoy::detail::modules::Module
{
  public:
    using tejoy::detail::modules::Module::Module;
    void on_start() override;
    void on_stop() override;
    auto priority() const -> int override
    {
        return 1000; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    }

  private:
    void on_any_event(const event_system::AnyEvent &event);

    std::vector<std::string> filter_from_;
    bool reply_event_;
    bool reply_console_;
    std::string reply_file_;
    bool short_names_;
};

} // namespace tejoy::detail::modules
