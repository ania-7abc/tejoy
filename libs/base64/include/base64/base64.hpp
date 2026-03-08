// base64.hpp
#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Base64
{
  public:
    static auto encode(const std::vector<uint8_t> &data) -> std::string;
    static auto encode(const std::string &text) -> std::string;
    static auto decode(const std::string &base64) -> std::vector<uint8_t>;
    static auto decode_to_string(const std::string &base64) -> std::string;
};
