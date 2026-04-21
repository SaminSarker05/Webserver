#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////

static const uint16_t PORT = 9090;

void check(int expr, const char* msg) {
        if (expr < 0) {
                std::cerr << msg << std::endl;
                exit(1);
        }
}

int main() {
        // create a TCP socket accepting IPv4 addresses
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        check(socket_fd, "Failed to create socket");

        // define where to connect
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

        check(connect(socket_fd, (struct sockaddr *) &addr, sizeof(addr)), 
                "Failed to connect to server");

        std::string buffer;
        std::cout << "Enter a message for the server to read: ";
        std::getline(std::cin, buffer);
        send(socket_fd, buffer.c_str(), buffer.length(), 0);

        close(socket_fd);
}

