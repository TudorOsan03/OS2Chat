#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "chat_log.cpp"

std::mutex clientMutex;
std::condition_variable clientCondition;

void handleClient(int clientSocket) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    getpeername(clientSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);

    std::string clientInfo = std::string(inet_ntoa(clientAddr.sin_addr)) + ":" + std::to_string(ntohs(clientAddr.sin_port));

    char buffer[1024] = {0};
    while (true) {
        int bytesRead = read(clientSocket, buffer, 1024);
        if (bytesRead <= 0) {
            std::cerr << "Client disconnected or error occurred" << std::endl;
            close(clientSocket);
            return;
        }

        std::string message(buffer, bytesRead);
        logMessage(clientInfo, message);

        std::string chatLog = getChatLog();
        send(clientSocket, chatLog.c_str(), chatLog.size(), 0);
    }
}

int main() {
    // Socket !!!
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    // Socket used
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }
    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Error listening for connections" << std::endl;
        return 1;
    }
    std::cout << "Server started. Listening for connections..." << std::endl;

    while (true) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }
        std::thread t(handleClient, clientSocket);
        t.detach();
    }

    return 0;
}
