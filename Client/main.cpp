#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define PACKET_SIZE 1024
#define PORT 4444

#define SERVER_IP "121.141.66.31"
// 121.141.66.31
// 121.141.66.254



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

	// 접속
	std::cout << "유저 이름 입력 : ";
	std::cin >> UserName;

	//std::cout << "유저 IP 입력 : ";
	//std::cin >> IP;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = { 0 };
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	addr.sin_port = PORT;
	addr.sin_family = AF_INET;

	// 지속적 연결시도
	int WSAGetLastError(void);

	// 해결중
	INVALID_SOCKET;

	while (connect(server, (SOCKADDR*)&addr, sizeof addr))
	{
		std::cout << "연결 시도중..." << std::endl;
	}
	// 접속하자마자 메세지(유저이름) 전송, UserName크기 1024
	send(server, UserName, sizeof UserName, 0);

	std::thread(recvData, std::ref(server)).detach();

	// 데이터 주고받기
	std::string Line;
	while (TRUE)
	{
		ZeroMemory(SendData, sizeof SendData);
		std::cout << "메세지 입력 : ";
		std::cin >> SendData;

		// 채팅 전송
		send(server, SendData, sizeof SendData, 0);
	}
}