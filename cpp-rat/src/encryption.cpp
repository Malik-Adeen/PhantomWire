#include "encryption.hpp"

std::string xor_encrypt(const std::string &data, const std::string &key) {
  std::string encrypted = data;
  for (size_t i = 0; i < data.size(); ++i) {
    encrypted[i] ^= key[i % key.size()];
  }
  return encrypted;
}

std::string xor_decrypt(const std::string &data, const std::string &key) {
  // XOR decryption is the same as encryption
  return xor_encrypt(data, key);
}
