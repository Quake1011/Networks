#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    char buf[100];
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

    if (InetPton(AF_INET, L"127.0.0.1", &add.sin_addr) != 1) {
        std::cerr << "Failed to convert IP address" << std::endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    int t = sizeof(add);
    std::cout << "Enter the string, please" << std::endl;
    std::cin.getline(buf, sizeof(buf));

    sendto(s, buf, strlen(buf), 0, (struct sockaddr*)&add, t);

    int letterCount[256] = { 0 };
    int rv = recvfrom(s, reinterpret_cast<char*>(letterCount), sizeof(letterCount), 0, (struct sockaddr*)&add, &t);

    if (rv == SOCKET_ERROR) {
        std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
    }
    else {
        for (int i = 0; i < 256; ++i) {
            if (letterCount[i] > 0) {
                std::cout << "Character '" << static_cast<char>(i) << "': " << letterCount[i] << " occurrences" << std::endl;
            }
        }
    }

    closesocket(s);
    WSACleanup();

    system("pause");
    return 0;
}
