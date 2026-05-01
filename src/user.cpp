// user.cpp
#include <base64/base64.hpp>
#include <tejoy/user.hpp>

namespace tejoy
{

auto operator==(const User &lhs, const User &rhs) -> bool
{
    return lhs.box.get_public_key() == rhs.box.get_public_key();
}

void to_json(nlohmann::json &json, const User &user)
{
    json = nlohmann::json{};
    json["ip"] = user.ip;
    json["port"] = user.port;
    if (user.box.has_secret_key())
        json["key"] = Base64::encode(user.box.get_secret_key());
    if (user.box.has_public_key())
        json["id"] = Base64::encode(user.box.get_public_key());
}

void from_json(const nlohmann::json &json, User &user)
{
    json.at("ip").get_to(user.ip);
    json.at("port").get_to(user.port);
    if (json.contains("id") && json.contains("key"))
        user.box = SecretBox(Base64::decode(json["key"]), Base64::decode(json["id"]));
    else if (json.contains("id"))
        user.box = SecretBox(Base64::decode(json["id"]));
    else if (json.contains("key"))
        throw std::runtime_error("SecretBox must not contain only the private key");
    else
        user.box = SecretBox(false);
}

} // namespace tejoy

auto std::hash<tejoy::User>::operator()(const tejoy::User &user) const -> std::size_t
{
    return boost::hash_range(user.box.get_public_key().begin(), user.box.get_public_key().end());
}
