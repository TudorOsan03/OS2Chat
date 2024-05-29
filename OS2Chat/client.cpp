#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void startClient(const std::string& serverAddress, int port) {
    //socket!!!
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(clientSocket);
        return;
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        close(clientSocket);
        return;
    }

    std::string message;
    while (true) {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        send(clientSocket, message.c_str(), message.size(), 0);

        char buffer[1024] = {0};
        int bytesRead = read(clientSocket, buffer, 1024);
        if (bytesRead <= 0) {
            std::cerr << "Server disconnected or error occurred" << std::endl;
            break;
        }

        std::cout << "Chat log:\n" << std::string(buffer, bytesRead) << std::endl;
    }

    close(clientSocket);
}

int main() {
    std::string serverAddress = "127.0.0.1";
    int port = 8080;

    startClient(serverAddress, port);

    return 0;
}
