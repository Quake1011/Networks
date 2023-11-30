#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        std::cerr << "WSAStartup failed with error: " << err << std::endl;
        return 1;
    }

    SOCKET s;
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == INVALID_SOCKET) {
        std::cerr << "Failed to create socket with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_port = htons(1024);
    add.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr*)&add, sizeof(add)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    char buf[100];
    int t = sizeof(add);

    while (true) {
        int letterCount[256] = { 0 };

        int rv = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&add, &t);
        if (rv == SOCKET_ERROR) {
            std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
            closesocket(s);
            WSACleanup();
            return 1;
        }

        if (rv == 0) {
            break;
        }

        buf[rv] = '\0';

        for (int i = 0; buf[i] != '\0'; ++i) {
            char ch = toupper(buf[i]);
            if (strchr("WINDOWS", ch) != nullptr) {
                letterCount[ch]++;
            }
        }

        sendto(s, reinterpret_cast<char*>(letterCount), sizeof(letterCount), 0, (struct sockaddr*)&add, t);
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
