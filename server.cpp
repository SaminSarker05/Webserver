#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

static const int BACKLOG = 10;

void check(int expr, const char* msg) {
        if (expr < 0) {
                std::cerr << msg << std::endl;
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
        addr.sin_port = htons(8080);
        addr.sin_addr.s_addr = INADDR_ANY;

        // bind socket to all interfaces on port 8080
        check(bind(server_fd, (sockaddr*) &addr, sizeof(addr)),  
                "Failed to bind socket");
        listen(server_fd, BACKLOG);
        check(listen(server_fd, BACKLOG), "Failed to listen on socket");
        std::cout << "server listening on port 8080..." << std::endl;

        while (true) {
                // wait for and accept incoming client connections
                sockaddr_in client_addr{};
                socklen_t client_len = sizeof(client_addr);

                std::cout << "debug3" << std::endl;
                int client_fd = accept(server_fd, (sockaddr *) &client_addr, 
                        &client_len);
                std::cout << "debug2" << std::endl;
                if (client_fd < 0)
                        continue;

                // store client ip address
                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, 
                        &client_addr.sin_addr, client_ip, sizeof(client_ip));
                int client_port = ntohs(client_addr.sin_port);
                std::cout << "debug1" << std::endl;
                std::cout << "[client] accepeted " << 
                        client_ip << ":" << client_port << std::endl;

                handle_connection(client_fd);
                // std::thread(handle_connection, client_fd).detach();
        }

        close(server_fd);
        return 0;
}

