// MIT License
// 
// Copyright (c) 2026 Anya Baykina Dmitrievna
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

// base64.cpp
#include <base64.hpp>

#include <vector>
#include <string>
#include <cstdint>
#include <sodium.h>
#include <stdexcept>

std::string Base64::encode(const std::vector<uint8_t> &data)
{
    size_t encoded_len = sodium_base64_encoded_len(
        data.size(),
        sodium_base64_VARIANT_ORIGINAL);

    std::string base64;
    base64.resize(encoded_len - 1); // resize requires size without \0

    if (sodium_bin2base64(
            base64.data(),                 // Output buffer
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
std::string Base64::encode(const std::string &text) { return encode(std::vector<uint8_t>(text.begin(), text.end())); }

std::vector<uint8_t> Base64::decode(const std::string &base64)
{
    std::vector<uint8_t> data;
    data.resize(base64.length());
    size_t text_len = 0;
    if (sodium_base642bin(
            reinterpret_cast<uint8_t *>(data.data()), // Output buffer
            data.size(),                              // Size of output buffer
            base64.data(),                            // Input buffer
            base64.size(),                            // Size of input buffer
            nullptr,                                  // Characters to ignore
            &text_len,                                // Length of output data
            nullptr,                                  // Pointer to stop position
            sodium_base64_VARIANT_ORIGINAL            // Encoding variant
            ) != 0)
    {
        throw std::runtime_error("Failed to convert from Base64");
    }

    data.resize(text_len);
    return data;
}
std::string Base64::decode_to_string(const std::string &base64)
{
    auto data = decode(base64);
    return std::string(data.begin(), data.end());
}
