// secret_box.hpp
#pragma once
#include <sodium.h>
#include <stdexcept>
#include <vector>

class SecretBox
{
  private:
    std::vector<uint8_t> secret_key_, public_key_;
    bool has_secret_, has_public_;

    static void init_sodium();

  public:
    explicit SecretBox(bool gen_keys = true);
    explicit SecretBox(const std::vector<uint8_t> &public_key);
    SecretBox(const std::vector<uint8_t> &secret_key, const std::vector<uint8_t> &public_key);

    [[nodiscard]] auto get_public_key() const -> const std::vector<uint8_t> &;
    [[nodiscard]] auto get_secret_key() const -> const std::vector<uint8_t> &;
    [[nodiscard]] auto has_secret_key() const -> bool;
    [[nodiscard]] auto has_public_key() const -> bool;

    [[nodiscard]] auto encrypt(const std::string &text,
                               const std::vector<uint8_t> &recipient_public_key) const -> std::vector<uint8_t>;
    [[nodiscard]] auto encrypt(const std::string &text, const SecretBox &recipient) const -> std::vector<uint8_t>;

    [[nodiscard]] auto decrypt(const std::vector<uint8_t> &data,
                               const std::vector<uint8_t> &sender_public_key) const -> std::string;
    [[nodiscard]] auto decrypt(const std::vector<uint8_t> &data, const SecretBox &sender) const -> std::string;
};
