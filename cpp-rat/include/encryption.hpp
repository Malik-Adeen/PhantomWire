#pragma once
#include <string>

// XOR encryption/decryption using a shared key
std::string xor_encrypt(const std::string &data, const std::string &key);
std::string xor_decrypt(const std::string &data, const std::string &key);
