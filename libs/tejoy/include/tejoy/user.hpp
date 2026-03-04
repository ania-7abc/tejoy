// user.hpp
#pragma once

#include <cstdint>
#include <string>

#include <base64/base64.hpp>
#include <nlohmann/json.hpp>
#include <secret_box/secret_box.hpp>

namespace tejoy
{

  struct User
  {
    SecretBox box;
    std::string ip;
    uint16_t port;
  };

  inline void to_json(nlohmann::json &j, const User &u)
  {
    j = nlohmann::json{};
    j["ip"] = u.ip;
    j["port"] = u.port;
    if (u.box.has_secret_key())
      j["key"] = Base64::encode(u.box.get_secret_key());
    j["id"] = Base64::encode(u.box.get_public_key());
  }

  inline void from_json(const nlohmann::json &j, User &u)
  {
    j.at("ip").get_to(u.ip);
    j.at("port").get_to(u.port);
    auto id = Base64::decode(j.at("key").get<std::string>());
    if (j.contains("key"))
      u.box = SecretBox(id, Base64::decode(j.at("id").get<std::string>()));
    else
      u.box = SecretBox(id);
  }

}
