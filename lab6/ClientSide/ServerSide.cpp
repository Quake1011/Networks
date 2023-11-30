#include <iostream>
#include <vector>
#include <winsock2.h>

struct Student {
    std::string fullName;
    std::string groupNumber;
    float scholarship;
    std::vector<int> grades;

    Student(const std::string& name, const std::string& group, float scholarship, const std::vector<int>& grades)
        : fullName(name), groupNumber(group), scholarship(scholarship), grades(grades) {}
};

void sendStudentsWithoutGradeThree(SOCKET clientSocket, const std::vector<Student>& students) {
    for (const auto& student : students) {
        if (std::find(student.grades.begin(), student.grades.end(), 3) == student.grades.end()) {
            // Отправка студента клиенту (реализация зависит от требований протокола)
            // Например, можно отправить в виде строки
            std::string studentInfo = student.fullName + " " + student.groupNumber;
            send(clientSocket, studentInfo.c_str(), studentInfo.size(), 0);
        }
    }
}

int main() {
    // Инициализация Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Создание сокета
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(12345); // Порт для прослушивания

    // Привязка сокета
    bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));

    // Прослушивание сокета
    listen(serverSocket, SOMAXCONN);

    std::vector<Student> students = {
        {"Иванов Иван Иванович", "Группа1", 1500, {4, 5, 4}},
        {"Петров Петр Петрович", "Группа2", 1200, {5, 4, 5}},
        // Добавьте остальных студентов...
    };

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

        // Отправка студентов клиенту
        sendStudentsWithoutGradeThree(clientSocket, students);

        closesocket(clientSocket);
    }

    // Завершение Winsock
    WSACleanup();

    return 0;
}