#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

// Функция для определения, является ли символ гласной буквой
bool isVowel(char c) {
    c = tolower(c);  // Приводим к нижнему регистру для учета всех случаев
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

// Функция для обработки строки и подсчета гласных и согласных букв
void countVowelsAndConsonants(const std::string& input, int& vowels, int& consonants) {
    vowels = 0;
    consonants = 0;

    for (char c : input) {
        if (isalpha(c)) {
            if (isVowel(c)) {
                ++vowels;
            }
            else {
                ++consonants;
            }
        }
    }
}

char* daytime(const std::string& input) {
    // Подсчет гласных и согласных
    int vowels = 0, consonants = 0;
    countVowelsAndConsonants(input, vowels, consonants);

    // Сформируем строку с результатами
    std::string result = "Vowels: " + std::to_string(vowels) + ", Consonants: " + std::to_string(consonants);

    // Копируем строку из буфера в динамическую память
    char* resultString = _strdup(result.c_str());
    if (resultString == nullptr) {
        std::cerr << "Error duplicating time string" << std::endl;
    }

    return resultString;
}

DWORD WINAPI udp_thread(LPVOID lpParameter) {
    SOCKET sock = reinterpret_cast<SOCKET>(lpParameter);
    sockaddr_in from;
    int len = sizeof(from);
    char buf[81];

    while (true) {
        memset(buf, 0, 81);
        int bytesReceived = recvfrom(sock, buf, 80, 0, reinterpret_cast<SOCKADDR*>(&from), &len);

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Error receiving data" << std::endl;
            break;
        }

        std::cout << "udp incoming: " << buf << std::endl;

        char* result = daytime(buf);

        if (sendto(sock, result, static_cast<int>(strlen(result)), 0, reinterpret_cast<SOCKADDR*>(&from), len) == SOCKET_ERROR) {
            std::cerr << "Error sending data" << std::endl;
            break;
        }

        std::cout << "answer udp" << std::endl;

        // Освобождаем память, выделенную под результат
        free(result);
    }

    return 1;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return 1;
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1212);
    addr.sin_addr.s_addr = INADDR_ANY;

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        return 1;
    }

    if (bind(sock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    const int threads = 1;  // В UDP используем только один поток
    HANDLE tid[threads];

    // Создание потока
    tid[0] = CreateThread(NULL, 0, udp_thread, reinterpret_cast<LPVOID>(sock), 0, NULL);

    // Ожидание завершения потока
    WaitForSingleObject(tid[0], INFINITE);

    // Закрытие сокета
    closesocket(sock);
    WSACleanup();

    return 0;
}