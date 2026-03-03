// secret_box.hpp
#pragma once
#include <stdexcept>
#include <vector>
#include <sodium.h>

class SecretBox
{
private:
  std::vector<uint8_t> secret_key_, public_key_;
  bool has_secret_;

  void init_sodium();

public:
  SecretBox();
  SecretBox(const std::vector<uint8_t> &public_key);
  SecretBox(const std::vector<uint8_t> &secret_key, const std::vector<uint8_t> &public_key);

  const std::vector<uint8_t> &get_public_key() const;
  const std::vector<uint8_t> &get_secret_key() const;
  bool has_secret_key() const;

  std::vector<uint8_t> encrypt(const std::string &text, const std::vector<uint8_t> &recipient_public_key) const;
  std::vector<uint8_t> encrypt(const std::string &text, const SecretBox &recipient) const;

  std::string decrypt(const std::vector<uint8_t> &data, const std::vector<uint8_t> &sender_public_key) const;
  std::string decrypt(const std::vector<uint8_t> &data, const SecretBox &sender) const;
};
