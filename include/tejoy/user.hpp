// user.hpp
#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>
#include <secret_box/secret_box.hpp>
#include <string>

namespace tejoy
{

struct User
{
    SecretBox box = SecretBox(false);
    std::string ip;
    uint16_t port{};
};

void to_json(nlohmann::json &json, const User &user);
void from_json(const nlohmann::json &json, User &user);

} // namespace tejoy
