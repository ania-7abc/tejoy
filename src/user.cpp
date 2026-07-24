/**
 * @file user.cpp
 * @copyright Copyright (c) 2026 ania-7abc SPDX-License-Identifier: MIT
 */

#include <tejoy/user.hpp>
#include <sodium.h>

namespace tejoy
{
    static std::string b64_encode(const std::vector<uint8_t>& bin)
    {
        std::string b64;
        b64.resize(sodium_base64_ENCODED_LEN(bin.size(), sodium_base64_VARIANT_URLSAFE));
        if (!sodium_bin2base64(b64.data(), b64.size(),
                               bin.data(), bin.size(),
                               sodium_base64_VARIANT_URLSAFE))
        {
            throw errors::sodium_error("sodium_bin2base64 failed");
        }
        b64.pop_back(); // \0
        return b64;
    }

    static std::vector<uint8_t> b64_decode(const std::string& b64)
    {
        std::vector<uint8_t> bin;
        bin.resize(b64.size());
        size_t bin_len;
        if (sodium_base642bin(bin.data(), bin.size(),
                              b64.data(), b64.size(),
                              nullptr, &bin_len, nullptr,sodium_base64_VARIANT_URLSAFE))
        {
            throw errors::sodium_error("sodium_base642bin failed");
        }
        bin.resize(bin_len);
        return bin;
    }

    UserId::UserId(const std::string& public_key, const std::string& secret_key)
    {
        if (sodium_init() < 0)
            throw errors::sodium_error("sodium_init failed");
        pk_ = b64_decode(public_key);
        sk_ = std::make_optional(b64_decode(secret_key));
        if (pk_.size() != crypto_box_PUBLICKEYBYTES)
            throw std::logic_error("invalid public key length");
        if (sk_->size() != crypto_box_SECRETKEYBYTES)
            throw std::logic_error("invalid secret key length");
    }

    UserId::UserId(const std::string& public_key)
    {
        if (sodium_init() < 0)
            throw errors::sodium_error("sodium_init failed");
        pk_ = b64_decode(public_key);
        sk_ = std::nullopt;
        if (pk_.size() != crypto_box_PUBLICKEYBYTES)
            throw std::logic_error("invalid public key length");
    }

    UserId UserId::generate()
    {
        if (sodium_init() < 0)
            throw errors::sodium_error("sodium_init failed");

        std::vector<uint8_t> pk;
        std::vector<uint8_t> sk;
        pk.resize(crypto_box_PUBLICKEYBYTES);
        sk.resize(crypto_box_SECRETKEYBYTES);

        if (crypto_box_keypair(pk.data(), sk.data()))
            throw errors::sodium_error("crypto_box_keypair failed");

        return {b64_encode(pk), b64_encode(sk)};
    }

    std::string UserId::encrypt(const std::string& text, const UserId& recipient_id) const
    {
        if (!sk_.has_value())
            throw std::logic_error("encrypt requires a secret key");

        auto cypher = std::vector<uint8_t>(crypto_box_NONCEBYTES + crypto_box_MACBYTES + text.size());
        randombytes_buf(cypher.data(), crypto_box_NONCEBYTES);

        if (crypto_box_easy(cypher.data() + crypto_box_NONCEBYTES,
                            reinterpret_cast<const unsigned char*>(text.data()), text.size(),
                            cypher.data(),
                            pk_.data(), recipient_id.sk_->data()))
        {
            throw errors::sodium_error("crypto_box_easy failed");
        }

        return b64_encode(cypher);
    }

    std::string UserId::decrypt(const std::string& cypher, const UserId& sender_id) const
    {
        if (!sk_.has_value())
            throw std::logic_error("decrypt requires a secret key");

        const auto cypher_bin = b64_decode(cypher);

        std::string text;
        text.resize(cypher_bin.size() - crypto_box_NONCEBYTES - crypto_box_MACBYTES);

        if (crypto_box_open_easy(reinterpret_cast<unsigned char*>(text.data()),
                                 cypher_bin.data() + crypto_box_NONCEBYTES, cypher_bin.size() - crypto_box_NONCEBYTES,
                                 cypher_bin.data(),
                                 pk_.data(), sender_id.sk_->data()))
        {
            throw errors::sodium_error("crypto_box_open failed");
        }

        return text;
    }

    std::string UserId::get_public_key() const
    {
        return b64_encode(pk_);
    }

    bool UserId::has_secret_key() const
    {
        return sk_.has_value();
    }

    std::string UserId::get_secret_key() const
    {
        if (!sk_.has_value())
            throw std::logic_error("get_secret_key requires a secret key");
        return b64_encode(*sk_);
    }
}
