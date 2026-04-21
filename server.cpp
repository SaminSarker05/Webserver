#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

void handle_connection(int client_fd) {
        std::string message;
        char buffer[1024];  // ~1KB buffer to read data
        int bytes_read;
        while ((bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
                buffer[bytes_read] = '\0';
                message.append(buffer, bytes_read);
        }
        if (message[message.length() - 1] == '\n') {
                message[message.length() - 1] = '\0';  // remove newline char
        }
        std::cout << client_fd << " said: " << message << std::endl;
        close(client_fd);
}

int main() {
        // create a TCP socket accepting IPv4 addresses
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);

        // initialize address struct
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        addr.sin_addr.s_addr = INADDR_ANY;

        // bind socket to all interfaces on port 8080
        bind(server_fd, (sockaddr*) &addr, sizeof(addr));
        listen(server_fd, SOMAXCONN);
        std::cout << "server listening on port 8080..." << std::endl;

        while (true) {
                // wait for and accept incoming connections
                int client_fd = accept(server_fd, NULL, NULL);
                if (client_fd < 0)
                        continue;

                handle_connection(client_fd);
        }

        close(server_fd);
        return 0;
}

