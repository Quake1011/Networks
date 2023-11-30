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
            // �������� �������� ������� (���������� ������� �� ���������� ���������)
            // ��������, ����� ��������� � ���� ������
            std::string studentInfo = student.fullName + " " + student.groupNumber;
            send(clientSocket, studentInfo.c_str(), studentInfo.size(), 0);
        }
    }
}

int main() {
    // ������������� Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // �������� ������
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(12345); // ���� ��� �������������

    // �������� ������
    bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));

    // ������������� ������
    listen(serverSocket, SOMAXCONN);

    std::vector<Student> students = {
        {"������ ���� ��������", "������1", 1500, {4, 5, 4}},
        {"������ ���� ��������", "������2", 1200, {5, 4, 5}},
        // �������� ��������� ���������...
    };

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

        // �������� ��������� �������
        sendStudentsWithoutGradeThree(clientSocket, students);

        closesocket(clientSocket);
    }

    // ���������� Winsock
    WSACleanup();

    return 0;
}