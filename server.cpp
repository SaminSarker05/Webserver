#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
        // create a TCP socket accepting IPv4 addresses
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        addr.sin_addr.s_addr = INADDR_ANY;  // accept connections from all interfaces

        bind(server_fd, (sockaddr*) &addr, sizeof(addr));
        listen(server_fd, SOMAXCONN);
        std::cout << "server listening on port 8080..." << std::endl;

        while (true) {
                int client_fd = accept(server_fd, NULL, NULL);
                if (client_fd < 0)
                        continue;
                std::cout << client_fd << std::endl;
        }

        close(server_fd);
}

