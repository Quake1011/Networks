#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdlib.h>
#include <tchar.h>

using namespace std;

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

    connect(s, (struct sockaddr*)&peer, sizeof(peer));

    int m, n;
    cout << "Enter the first number (m): ";
    cin >> m;
    cout << "Enter the second number (n): ";
    cin >> n;

    send(s, (char*)&m, sizeof(m), 0);
    send(s, (char*)&n, sizeof(n), 0);

    int result;
    recv(s, (char*)&result, sizeof(result), 0);

    cout << "Result from server: " << result << endl;

    closesocket(s);
    WSACleanup();
    system("pause");
    return 0;
}
