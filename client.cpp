#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

int main() {
        // create a TCP socket accepting IPv4 addresses
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

        // define where to connect
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

        connect(socket_fd, (struct sockaddr *) &addr, sizeof(addr));

        std::string buffer;
        std::cout << "Enter a message to send to the server: ";
        std::getline(std::cin, buffer);
        send(socket_fd, buffer.c_str(), buffer.length(), 0);

        close(socket_fd);
}

