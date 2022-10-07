#pragma once
#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define PACKET_SIZE 1024
#define PORT 4444


#include <iostream> // c++ stadard input output
#include <winsock2.h> // winsock
#include <windows.h> // ZeroMemory
#include <vector> //vector
#include <utility> //pair
#include <thread> //thread
#include <cstdlib>

class CLIENT
{
public:
	CLIENT() {}

public:
	SOCKET client;
	SOCKADDR_IN clientaddr = { 0 };
	int clientsize = sizeof clientaddr;
	int number = -1;
}; 

typedef std::pair<CLIENT, std::string> pii;
std::vector<pii> Client;

void recvData(SOCKET _Server, int _Num)
{
	// 클라이언트 이름 받아오기
	char buffer[PACKET_SIZE] = { 0 };
	recv(_Server, buffer, sizeof buffer, 0);
	Client[_Num].second = buffer;

	// 클라이언트 데이터 받아오기
	while (TRUE)
	{
		ZeroMemory(buffer, sizeof buffer);
		recv(_Server, buffer, sizeof buffer, 0);

		std::cout << Client[_Num].second << " : " << buffer << std::endl;
	}
}

void ACCEPT(SOCKET& _Server)
{
	int cnt = 0;
	while (TRUE)
	{
		Client.push_back(pii(CLIENT(), ""));
		Client[cnt].first.client = accept(_Server, (SOCKADDR*)&Client[cnt].first.clientaddr, &Client[cnt].first.clientsize);
		Client[cnt].first.number = cnt;
		std::thread(recvData, Client[cnt].first.client, cnt).detach();

		cnt += 1;
	}
}

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = { 0 };

	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = PORT;
	addr.sin_family = AF_INET;

	bind(server, (SOCKADDR*)&addr, sizeof(addr));
	listen(server, SOMAXCONN);

	std::thread(ACCEPT, std::ref(server)).detach();


	char name[PACKET_SIZE],
		message[PACKET_SIZE];

	while (TRUE) {
		ZeroMemory(name, sizeof name);
		ZeroMemory(message, sizeof message);
		std::cin >> name >> message;
		for (int i = 0; i < Client.size(); i++)
			if (!strcmp(Client[i].second.c_str(), name))
				send(Client[i].first.client, message, sizeof message, 0);
	}
}
