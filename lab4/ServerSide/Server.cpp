#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <tchar.h>

using namespace std;

int factorial(int num) {
    return (num == 0 || num == 1) ? 1 : num * factorial(num - 1);
}

int main() {
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);

    if (WSAStartup(wVersionRequested, &wsaData) != 0) {
        cerr << "Failed to initialize WinSock." << endl;
        return 1;
    }

    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(1280);

    if (InetPton(AF_INET, _T("127.0.0.1"), &peer.sin_addr) != 1) {
        cerr << "Invalid address: 127.0.0.1" << endl;
        WSACleanup();
        return 1;
    }

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(s, (struct sockaddr*)&peer, sizeof(peer)) == SOCKET_ERROR) {
        cerr << "Failed to bind the socket." << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    listen(s, 5);

    cout << "Server is listening for incoming connections..." << endl;

    while (true) {
        sockaddr_in remote_addr;
        int size = sizeof(remote_addr);
        SOCKET s2 = accept(s, (struct sockaddr*)&remote_addr, &size);

        int m, n;
        recv(s2, (char*)&m, sizeof(m), 0);
        recv(s2, (char*)&n, sizeof(n), 0);

        int result = factorial(m) + factorial(n);

        send(s2, (char*)&result, sizeof(result), 0);

        closesocket(s2);
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
