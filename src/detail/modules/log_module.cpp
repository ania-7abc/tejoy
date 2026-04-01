// log_module.cpp
#include <boost/core/demangle.hpp>
#include <nlohmann/json.hpp>
#include <simple_io/simple_io.hpp>
#include <tejoy/detail/modules/log_module.hpp>
#include <tejoy/events/log.hpp>

#include <iostream>

namespace tejoy::detail::modules
{

void LogModule::on_start()
{
    config_.emplace("from", nlohmann::json::array()).first.value().get_to(filter_from_);
    config_.emplace("short_names", false).first.value().get_to(short_names_);
    config_["reply"].emplace("event", false).first.value().get_to(reply_event_);
    config_["reply"].emplace("console", false).first.value().get_to(reply_console_);
    config_["reply"].emplace("file", "").first.value().get_to(reply_file_);

    if (reply_file_ != "")
    {
        if (SimpleIO::exists(reply_file_))
            SimpleIO::remove(reply_file_);
        SimpleIO::write(reply_file_, "");
    }

    subscribe<event_system::AnyEvent>([this](auto &event) { on_any_event(event); });
}

void LogModule::on_stop()
{
}

void LogModule::on_any_event(const event_system::AnyEvent &event)
{
    if (event.original_type == typeid(events::LogEvent))
        return;

    std::string type = boost::core::demangle(event.original_type.name());
    std::string from = boost::core::demangle(event.sender().name());

    bool log = true;

    if (filter_from_.size() != 0)
    {
        log = false;
        for (int i = 0; i < filter_from_.size(); i++)
            if (filter_from_[i] == from)
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

    if (reply_console_)
        std::cout << "Received event \"" << type << "\" from \"" << from << "\"" << std::endl;
    if (reply_event_)
        publish<events::LogEvent>(event.original_type, event.sender());
    if (reply_file_ != "")
        SimpleIO::append(reply_file_, type + " from " + from + "\n");
}

} // namespace tejoy::detail::modules
