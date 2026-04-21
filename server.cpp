/*
TODO:
- add heartbeat logic to detect and close dead client connections
- broadcast messages from one client to all other clients
*/
#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

static const int BACKLOG = 10;
static const uint16_t PORT = 8080;

void check(int expr, const char* msg) {
        if (expr < 0) {
                perror(msg);  // print error message from errno
                exit(1);
        }
}
////////////////////////////////////////////////////////////////////////////////

void handle_connection(int fd) {
        std::string message;
        char buffer[1024];  // ~1KB buffer to read data
        int bytes_read;
        while ((bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
                message.append(buffer, bytes_read);
        }
        if (!message.empty() && message[message.length() - 1] == '\n') {
                message[message.length() - 1] = '\0';  // remove newline char
        }
        std::cout << fd << " said: " << message << std::endl;
        close(fd);
}

int main() {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        check(server_fd, "Failed to create socket");

        // initialize address struct
        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr = INADDR_ANY;

        // allow immediate reuse of address after server restarts
        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        // bind socket to all interfaces on port 8080
        check(bind(server_fd, (sockaddr*) &addr, sizeof(addr)),  
                "Failed to bind socket");

        check(listen(server_fd, BACKLOG), "Failed to listen on socket");
        std::cout << "server listening on port " << PORT << "..." << std::endl;

        while (true) {
                // wait for and accept incoming client connections
                sockaddr_in client_addr{};
                socklen_t client_len = sizeof(client_addr);

                int client_fd = accept(server_fd, (sockaddr *) &client_addr, 
                        &client_len);
                if (client_fd < 0)
                        continue;

                // store client ip address
                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, 
                        &client_addr.sin_addr, client_ip, sizeof(client_ip));
                int client_port = ntohs(client_addr.sin_port);
                std::cout << "[client] accepted " << 
                        client_ip << ":" << client_port << std::endl;

                // handle_connection(client_fd);
                std::thread(handle_connection, client_fd).detach();
        }

        close(server_fd);
        return 0;
}

