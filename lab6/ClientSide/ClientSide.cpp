#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")

void handleServerResponse(SOCKET serverSocket) {
    char buffer[1024];
    int bytesReceived = recv(serverSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Received from server:\n" << buffer << std::endl;
    }
    else {
        std::cout << "Failed to receive data from server." << std::endl;
    }
}

int main() {
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        return 0;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1280);
    if (InetPton(AF_INET, _T("127.0.0.1"), &serverAddr.sin_addr) <= 0) {
        std::cout << "Invalid address. Error!" << std::endl;
        return 1;
    }

    int connectResult = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (connectResult != 0) {
        std::cout << "Failed to connect to the server." << std::endl;
        return 0;
    }

    handleServerResponse(clientSocket);

    closesocket(clientSocket);
    WSACleanup();
    system("pause");
    return 0;
}
