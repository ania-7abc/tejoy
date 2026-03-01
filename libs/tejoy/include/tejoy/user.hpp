// MIT License
// 
// Copyright (c) 2026 ania_7 (Anya Baykina Dmitrievna)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// user.hpp
#pragma once
#include <secret_box.hpp>
#include <base64.hpp>
#include <string.h>
#include <nlohmann/json.hpp>
#include <cstdint>

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
        j["box"] = nlohmann::json{};
        j["box"]["key"] = Base64::encode(u.box.get_secret_key());
        j["box"]["id"] = Base64::encode(u.box.get_public_key());
    }

    inline void from_json(const nlohmann::json &j, User &u)
    {
        j.at("ip").get_to(u.ip);
        j.at("port").get_to(u.port);
        u.box = SecretBox(Base64::decode(j.at("box").at("key").get<std::string>()), Base64::decode(j.at("box").at("id").get<std::string>()));
    }
}
