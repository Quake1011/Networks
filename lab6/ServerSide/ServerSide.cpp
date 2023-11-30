#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <vector>
#include <string>

#pragma comment(lib, "ws2_32.lib")

struct Student {
    std::string name;
    std::string group;
    int scholarship;
    std::vector<int> grades;
};

std::vector<Student> studentsList;

void initStudentsList() {
    studentsList.push_back({ "Akentiev Danila", "1711",    10887,   {2, 3, 3, 5} });
    studentsList.push_back({ "Anishkova Anastasia", "1711", 11129,   {4, 2, 4, 2} });
    studentsList.push_back({ "Borodulin Andrey", "1711",   9592,    {3, 5, 2, 3} });
    studentsList.push_back({ "Ganich Danila", "1711",       9343,    {4, 2, 5, 3} });
    studentsList.push_back({ "Golubeva Evgenia", "1711",   10418,   {3, 4, 5, 2} });
    studentsList.push_back({ "Grin Roman", "1711",        9642,    {4, 3, 5, 2} });
    studentsList.push_back({ "Ekimova Yana", "1711",        8026,    {2, 3, 5, 4} });
    studentsList.push_back({ "Yurchuk Anna", "1711",         10676,   {5, 2, 4, 3} });
    studentsList.push_back({ "Malik Aleksandr", "1711",    8131,    {3, 4, 2, 5} });
    studentsList.push_back({ "Matveeva Alena", "1711",     11918,   {2, 5, 3, 4} });
    studentsList.push_back({ "Panasenko Yuriy", "1711",     11808,   {5, 3, 2, 4} });
    studentsList.push_back({ "Shabanova Valery", "1711",   10483,   {4, 2, 3, 5} });
    studentsList.push_back({ "Scherbakov Alexey", "1711",   11183,   {3, 4, 2, 5} });
}

void handleClientRequest(SOCKET clientSocket) {
    std::string response;
    for (const auto& student : studentsList)
        if (std::find(student.grades.begin(), student.grades.end(), 3) == student.grades.end())
            response += "Student: " + student.name + ", Group: " + student.group + ", Stipendia: " + std::to_string(student.scholarship) + "\n";

    send(clientSocket, response.c_str(), response.size(), 0);

    closesocket(clientSocket);
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

    initStudentsList();

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(1280);
    localAddr.sin_addr.s_addr = 0;
    bind(serverSocket, (sockaddr*)&localAddr, sizeof(localAddr));

    int listenResult = listen(serverSocket, 5);
    if (listenResult != 0) {
        return 0;
    }

    std::cout << "Server is ready to receive requests." << std::endl;

    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    while (true) {
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket != INVALID_SOCKET) {
            std::cout << "Client connected." << std::endl;
            handleClientRequest(clientSocket);
        }
    }

    WSACleanup();
    return 0;
}
