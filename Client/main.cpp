#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define PACKET_SIZE 1024
#define PORT 4444

#include <iostream>
#include <winsock2.h>
#include <Windows.h>
#include <thread>
#include <string>
#include <cstdlib>

void recvData(SOCKET& _Socket)
{
	char buffer[PACKET_SIZE];
	while (TRUE) {
		ZeroMemory(buffer, sizeof buffer);
		recv(_Socket, buffer, sizeof buffer, 0);
		std::cout << "Server : " << buffer << std::endl;
	}
}

int main()
{
	char IP[PACKET_SIZE] = { 0 };
	char UserName[PACKET_SIZE] = { 0 };
	char SendData[PACKET_SIZE] = { 0 };

	// ����
	std::cout << "���� �̸� �Է� : ";
	std::cin >> UserName;

	//std::cout << "���� IP �Է� : ";
	//std::cin >> IP;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = { 0 };
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = PORT;
	addr.sin_family = AF_INET;

	// ������ ����õ�
	int WSAGetLastError(void);

	// �ذ���
	INVALID_SOCKET;

	while (connect(server, (SOCKADDR*)&addr, sizeof addr))
	{
		std::cout << "���� �õ���..." << std::endl;
	}
	// �������ڸ��� �޼���(�����̸�) ����, UserNameũ�� 1024
	send(server, UserName, sizeof UserName, 0);

	std::thread(recvData, std::ref(server)).detach();

	// ������ �ְ�ޱ�
	std::string Line;
	while (TRUE)
	{
		ZeroMemory(SendData, sizeof SendData);
		std::cout << "�޼��� �Է� : ";
		std::cin >> SendData;

		// ä�� ����
		send(server, SendData, sizeof SendData, 0);
	}
}