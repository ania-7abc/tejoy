// base64.cpp

#include <base64/base64.hpp>

#include <stdexcept>

#include <sodium.h>

auto Base64::encode(const std::vector<uint8_t> &data) -> std::string
{
    size_t encoded_len = sodium_base64_encoded_len(data.size(), sodium_base64_VARIANT_ORIGINAL);

    std::string base64;
    base64.resize(encoded_len - 1); // resize requires size without \0

    if (sodium_bin2base64(base64.data(),                 // Output buffer
                          encoded_len,                   // Size of output buffer with \0
                          data.data(),                   // Input buffer
                          data.size(),                   // Size of input buffer
                          sodium_base64_VARIANT_ORIGINAL // Encoding variant
                          ) == nullptr)
    {
        throw std::runtime_error("Failed to convert to Base64");
    }
    base64.resize(base64.length());
    return base64;
}
auto Base64::encode(const std::string &text) -> std::string
{
    return encode(std::vector<uint8_t>(text.begin(), text.end()));
}

auto Base64::decode(const std::string &base64) -> std::vector<uint8_t>
{
    std::vector<uint8_t> data;
    data.resize(base64.length());
    size_t text_len = 0;
    if (sodium_base642bin(data.data(),                   // Output buffer
                          data.size(),                   // Size of output buffer
                          base64.data(),                 // Input buffer
                          base64.size(),                 // Size of input buffer
                          nullptr,                       // Characters to ignore
                          &text_len,                     // Length of output data
                          nullptr,                       // Pointer to stop position
                          sodium_base64_VARIANT_ORIGINAL // Encoding variant
                          ) != 0)
    {
        throw std::runtime_error("Failed to convert from Base64");
    }

    data.resize(text_len);
    return data;
}
auto Base64::decode_to_string(const std::string &base64) -> std::string
{
    auto data = decode(base64);
    return {data.begin(), data.end()};
}
