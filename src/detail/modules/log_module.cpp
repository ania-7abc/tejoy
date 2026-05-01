// log_module.cpp
#include <boost/core/demangle.hpp>
#include <nlohmann/json.hpp>
#include <simple_io/simple_io.hpp>
#include <tejoy/detail/modules/log_module.hpp>
#include <tejoy/events/log.hpp>

#include <iostream>

namespace tejoy::detail::modules
{

LogModule::LogModule(event_system::EventBus &bus, nlohmann::json &config) : Module(bus, config)
{
    config_.emplace("from", nlohmann::json::array()).first.value().get_to(filter_from_);
    config_.emplace("short_names", false).first.value().get_to(short_names_);
    config_["reply"].emplace("event", false).first.value().get_to(reply_event_);
    config_["reply"].emplace("console", false).first.value().get_to(reply_console_);
    config_["reply"].emplace("file", "").first.value().get_to(reply_file_);

    if (!reply_file_.empty())
    {
        if (SimpleIO::exists(reply_file_))
            SimpleIO::remove(reply_file_);
        SimpleIO::write(reply_file_, "");
    }
}

void LogModule::run_subscribes()
{
    subscribe<event_system::AnyEvent>([this](auto &event) { on_any_event(event); });
}

void LogModule::on_any_event(const event_system::AnyEvent &event) const
{
    if (event.original_type == typeid(events::LogEvent))
        return;

    std::string type = boost::core::demangle(event.original_type.name());
    std::string from = boost::core::demangle(event.sender().name());

    bool log = true;

    if (!filter_from_.empty())
    {
        log = false;
        for (const auto &element : filter_from_)
            if (element == from)
                log = true;
    }

    if (!log)
        return;

    if (short_names_)
    {
        size_t pos = type.rfind("::");
        if (pos != std::string::npos)
            type.assign(type.substr(pos + 2));
        pos = from.rfind("::");
        if (pos != std::string::npos)
            from.assign(from.substr(pos + 2));
    }

    if (reply_event_)
        publish<events::LogEvent>(type, from);
    const std::string log_str = "Received event \"" + type + "\" from \"" + from + "\"";
    if (reply_console_)
        std::cout << log_str << '\n';
    if (!reply_file_.empty())
        SimpleIO::append(reply_file_, log_str + "\n");
}

} // namespace tejoy::detail::modules
