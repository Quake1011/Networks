#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1212);

    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr)) != 1) {
        std::cerr << "Failed to convert IPv4 address" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::string input;
    std::cout << "Enter a string: ";
    std::getline(std::cin, input);

    if (sendto(sock, input.c_str(), static_cast<int>(input.length()), 0, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error sending data" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    char buf[81];
    memset(buf, 0, 81);
    int len = sizeof(serverAddr);
    int bytesReceived = recvfrom(sock, buf, 80, 0, reinterpret_cast<SOCKADDR*>(&serverAddr), &len);

    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Error receiving data" << std::endl;
    } else {
        std::cout << "Server response: " << buf << std::endl;
    }

    closesocket(sock);
    WSACleanup();

    system("pause");
    return 0;
}
