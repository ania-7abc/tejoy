// user_code_helper.cpp
#include <tejoy/events/detail/updates.hpp>
#include <tejoy/events/message.hpp>
#include <tejoy/user_code_helper.hpp>

namespace tejoy
{

void UserCodeHelper::start()
{
    subscribe<events::SendMessageRequest>([this](auto &event) {
        publish<events::detail::SendConfiguredUpdateRequest>(nlohmann::json({{"text", event.text}}), "message",
                                                             event.recipient);
    });

    subscribe<events::detail::UpdateReceived>([this](auto &event) {
        if (event.update.at("type").template get<std::string>() == "message")
        {
            publish<events::MessageReceived>(event.update.at("data").at("text").template get<std::string>(),
                                             event.sender);
        }
    });
}

} // namespace tejoy
