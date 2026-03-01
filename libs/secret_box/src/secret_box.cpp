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

// secret_box.cpp
#include <secret_box.hpp>

#include <stdexcept>
#include <vector>
#include <sodium.h>

void SecretBox::init_sodium()
{
    if (sodium_init() < 0)
        throw std::runtime_error("Sodium is not initialized correctly");
}

SecretBox::SecretBox() : secret_key_(crypto_box_SECRETKEYBYTES), public_key_(crypto_box_PUBLICKEYBYTES), has_secret_(true)
{
    init_sodium();
    if (crypto_box_keypair(public_key_.data(), secret_key_.data()))
        throw std::runtime_error("Failed to generate keys");
}
SecretBox::SecretBox(const std::vector<uint8_t> &public_key) : public_key_(public_key), has_secret_(false)
{
    init_sodium();
    if (public_key_.size() != crypto_box_PUBLICKEYBYTES)
        throw std::runtime_error("Invalid public key size");
}
SecretBox::SecretBox(const std::vector<uint8_t> &secret_key, const std::vector<uint8_t> &public_key) : public_key_(public_key), secret_key_(secret_key), has_secret_(true)
{
    init_sodium();
    if (public_key_.size() != crypto_box_PUBLICKEYBYTES)
        throw std::runtime_error("Invalid public key size");
    if (public_key_.size() != crypto_box_SECRETKEYBYTES)
        throw std::runtime_error("Invalid secret key size");
}

const std::vector<uint8_t> &SecretBox::get_public_key() const { return public_key_; }
const std::vector<uint8_t> &SecretBox::get_secret_key() const { return secret_key_; }
bool SecretBox::has_secret_key() const { return has_secret_; }

std::vector<uint8_t> SecretBox::encrypt(const std::string &text, const std::vector<uint8_t> &recipient_public_key) const { return encrypt(text, SecretBox(recipient_public_key)); }
std::vector<uint8_t> SecretBox::encrypt(const std::string &text, const SecretBox &recipient) const
{
    if (!has_secret_)
        throw std::runtime_error("Encryption unavailable without secret key");

    std::vector<uint8_t> result;
    result.resize(crypto_box_NONCEBYTES + (text.length() + crypto_box_MACBYTES));

    randombytes_buf(result.data(), crypto_box_NONCEBYTES);

    if (crypto_box_easy(
            result.data() + crypto_box_NONCEBYTES,          // output buffer
            reinterpret_cast<const uint8_t *>(text.data()), // text
            text.length(),                                  // data length
            result.data(),                                  // nonce
            recipient.public_key_.data(),                   // public key B
            secret_key_.data()                              // secret key A
            ) != 0)
    {
        throw std::runtime_error("Failed to encrypt data");
    }

    return result;
}

std::string SecretBox::decrypt(const std::vector<uint8_t> &data, const std::vector<uint8_t> &sender_public_key) const { return decrypt(data, SecretBox(sender_public_key)); }
std::string SecretBox::decrypt(const std::vector<uint8_t> &data, const SecretBox &sender) const
{
    if (!has_secret_)
        throw std::runtime_error("Decryption unavailable without secret key");

    int text_len = (data.size() - crypto_box_NONCEBYTES) - crypto_box_MACBYTES;
    if (text_len < 0)
        throw std::runtime_error("Invalid encrypted data");

    std::string text;
    text.resize(text_len);

    if (crypto_box_open_easy(
            reinterpret_cast<uint8_t *>(text.data()), // output buffer
            data.data() + crypto_box_NONCEBYTES,      // ciphertext
            data.size() - crypto_box_NONCEBYTES,      // ciphertext length
            data.data(),                              // nonce
            sender.public_key_.data(),                // public key A
            secret_key_.data()                        // secret key B
            ) != 0)
    {
        throw std::runtime_error("Failed to decrypt data");
    }

    return text;
}
