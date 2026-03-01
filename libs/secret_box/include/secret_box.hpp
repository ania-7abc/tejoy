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
