// base64.hpp
#ifndef BASE_64_HPP
#define BASE_64_HPP

#include <string>
#include <vector>
#include <cstdint>

class Base64
{
public:
    static std::string encode(const std::vector<uint8_t> &data);
    static std::string encode(const std::string &text);
    static std::vector<uint8_t> decode(const std::string &base64);
    static std::string decode_to_string(const std::string &base64);
};

#endif // BASE_64_HPP
