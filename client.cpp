#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
        // create a TCP socket accepting IPv4 addresses
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

        // define where to connect
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.01", &addr.sin_addr);

        connect(socket_fd, (struct sockaddr *) &addr, sizeof(addr));

        close(socket_fd);
}

