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
    SecretBox box = SecretBox(false);
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
    if (u.box.has_public_key())
      j["id"] = Base64::encode(u.box.get_public_key());
  }

  inline void from_json(const nlohmann::json &j, User &u)
  {
    j.at("ip").get_to(u.ip);
    j.at("port").get_to(u.port);
    if (j.contains("id") && j.contains("key"))
      u.box = SecretBox(Base64::decode(j["key"]), Base64::decode(j["id"]));
    else if (j.contains("id"))
      u.box = SecretBox(Base64::decode(j["id"]));
    else if (j.contains("key"))
      throw std::runtime_error("SecretBox must not contain only the private key");
    else
      u.box = SecretBox(false);
  }

}
