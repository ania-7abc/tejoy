/**
 * @file user.hpp
 * @brief Defines a UserId class and a User structure
 * @copyright Copyright (c) 2026 ania-7abc SPDX-License-Identifier: MIT
 */

#pragma once

#include <boost/asio/ip/udp.hpp>
#include <vector>
#include <optional>
#include <stdexcept>

namespace tejoy
{
    /**
     * @namespace errors
     * @brief Namespace for exceptions thrown by tejoy
     */
    namespace errors
    {
        /**
         * @class sodium_error
         * @brief Error thrown when there is an error in encryption, Base64 encoding, sodium initialization
         */
        class sodium_error : public std::runtime_error
        {
        public:
            /**
             * @brief Constructor sodium_error
             * @param msg Message explaining the reason for the error
             */
            explicit sodium_error(const std::string& msg)
                : std::runtime_error(msg)
            {
            }
        };
    }

    /**
     * @class UserId
     * @brief Класс представляющий ID пользователя tejoy, используется для шифрования
     */
    class UserId
    {
        /// Публичный ключ
        std::vector<uint8_t> pk_;
        /// Секретный ключ
        std::optional<std::vector<uint8_t>> sk_;

    public:
        /**
         * @brief Constructor for creating a UserId capable of encrypting and decrypting data
         * @param public_key Public key. Should live only for the duration of construction
         * @param secret_key Secret key. Should live only for the duration of construction
         * @throw errors::sodium_error If it was not possible to initialize sodium
         * @throw errors::sodium_error If it was not possible to decode a public or private key from Base64
         */
        UserId(const std::string& public_key, const std::string& secret_key);

        /**
         * @brief Constructor for creating a UserId that serves only to identify the user
         * @param public_key Public key. Should live only for the duration of construction
         * @warning An object created by this constructor cannot encrypt or decrypt data.
         *          It is usually used to indicate the recipient and/or sender of an encrypted message
         * @throw errors::sodium_error If it was not possible to initialize sodium
         * @throw errors::sodium_error If it was not possible to decode a public key from Base64
         */
        explicit UserId(const std::string& public_key);

        /**
         * @brief Constructor for generating a new full (can encrypt and decrypt) UserId
         * @return Unique UserId
         * @throw errors::sodium_error If it was not possible to initialize sodium
         * @throw errors::sodium_error If it was not possible to generate keys
         * @throw errors::sodium_error If you couldn't encode the keys in Base64
         */
        static auto generate() -> UserId;

        /**
         * @brief Function to encrypt text for a specific recipient
         * @param text The text to be encrypted
         * @param recipient_id Recipient, except for whom no one can decrypt
         * @return Base64 encoded ciphertext
         * @see decrypt
         * @throw std::logic_error If this class does not have a private key
         * @throw errors::sodium_error If you couldn't encrypt the text
         * @throw errors::sodium_error If the result could not be encoded in Base64
         */
        [[nodiscard]] auto encrypt(const std::string& text, const UserId& recipient_id) const -> std::string;

        /**
         * @brief Function to decrypt text from a specific sender
         * @param cypher Base64 encoded ciphertext
         * @param sender_id The sender who encoded the text
         * @return Decrypted text
        * @see encrypt
         * @throw std::logic_error If this class does not have a private key
         * @throw errors::sodium_error If you couldn't decrypt the text
         * @throw errors::sodium_error If it was not possible to decode the input cipher from Base64
         */
        [[nodiscard]] auto decrypt(const std::string& cypher, const UserId& sender_id) const -> std::string;

        /**
         * @brief Геттре для публичного ключа
         * @return Публичный ключ закодированный в Base64
         */
        [[nodiscard]] auto get_public_key() const -> std::string;
        /**
         * @brief Функция для проверки наличия секретного ключа
         * @return true если этот объект содержит секретный ключ
         */
        [[nodiscard]] auto has_secret_key() const -> bool;
        /**
         * @brief Геттер для приватного ключа
         * @return Приватный ключ закодированный в Base64
         * @throw std::logic_error If this class does not have a private key
         */
        [[nodiscard]] auto get_secret_key() const -> std::string;
    };

    /**
     * @struct User
     * @brief Structure representing user tejoy
     */
    struct User
    {
        /// User's online address
        boost::asio::ip::udp::endpoint endpoint;
        /// User ID
        UserId id;
    };
}
