#pragma warning(disable:4996) 

//server app

#include <cstdio>
#include <cstdlib>

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <thread>
#include <Windows.h>

#define MAX_CLIENT 10

void receiveSocket(SOCKET s) {
	printf("OK\n");
	SOCKET local = SOCKET_ERROR;
//	local = s;
	int bytesRecv = 5;
	char nickname[32] = "";
	while (bytesRecv != -1) {
		int bytesSent;
		char sendbuf[32] = "OK!";
		char recvbuf[32] = "";
		bytesRecv = recv(s, nickname, 32, 0);
		printf("%s zostal polacznony\n", nickname);
		break;
	}
	while (bytesRecv!=-1) {
		int bytesSent;
		char sendbuf[32] = "OK!";
		char recvbuf[32] = "";
		bytesRecv = recv(s, recvbuf, 32, 0);
		printf("%s: %s\n", nickname, recvbuf);
	}
	closesocket(s);
	s = SOCKET_ERROR;
	if (s == SOCKET_ERROR) {
		printf("closed\n");
	}
}

int main()
{
	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
		printf("Initialization error.\n");

	SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mainSocket == INVALID_SOCKET)
	{
		printf("Error creating socket: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(27015);

	if (bind(mainSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		printf("bind() failed.\n");
		closesocket(mainSocket);
		return 1;
	}

	if (listen(mainSocket, 1) == SOCKET_ERROR)
		printf("Error listening on socket.\n");
	
	int currentClient = 0;
	printf("Waiting for a client to connect...\n");

	SOCKET acceptSocket[MAX_CLIENT];
	for (int i = 0; i < 10; i++) {
		acceptSocket[i] = SOCKET_ERROR;
	}
	int i = 0;
	std::thread *receive1[10];
	for (int i = 0; i < MAX_CLIENT; i++) {
		for (int j = 0; j < MAX_CLIENT; j++) {
			if (acceptSocket[j] == SOCKET_ERROR) {
				i = j;
				break;
			}
		}
		while (acceptSocket[i] == SOCKET_ERROR)
		{
			acceptSocket[i] = accept(mainSocket, NULL, NULL);
		}
		
		printf("Client %d connected.\n", i);
		receive1[i] = new std::thread(receiveSocket, acceptSocket[i]);
	}

	system("PAUSE");
	return 0;
}