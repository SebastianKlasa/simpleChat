#pragma warning(disable:4996) 

#include <cstdio>
#include <cstdlib>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <thread>

#define T_LENGTH 1024

void receiveSocket(SOCKET s) {
	//	int bytesSent;

	int bytesRecv = 1;
	char *recvbuf = new char[T_LENGTH];
	while (bytesRecv != -1) {
		bytesRecv = recv(s, recvbuf, T_LENGTH, 0);
		//printf("Bytes received: %ld\n", bytesRecv);
		if (bytesRecv != -1) {
			recvbuf[bytesRecv] = '\0';
			printf("%s", recvbuf);
			printf("\n");
		}
		delete[]recvbuf;
		recvbuf = new char[T_LENGTH];
		//	bytesSent = send(s, sendbuf, strlen(sendbuf), 0);
		//	printf("Bytes sent: %ld\n", bytesSent);
	}
	printf("zamykam \n");
	closesocket(s);
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
	PVOID buff[16];
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(27015);

	if (connect(mainSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		printf("Failed to connect.\n");
		WSACleanup();
		return 1;
	}

	int bytesSent;
	int bytesRecv = SOCKET_ERROR;

	printf("Prosze, o podanie nicku: ");
	char sendbuf[T_LENGTH] = "ziomek";
	scanf("%s", sendbuf);
	send(mainSocket, sendbuf, strlen(sendbuf), 0);
	printf("polaczono pomyslnie!\n");

	std::thread *receive = new std::thread(receiveSocket, mainSocket);
	while (true) {
		char sendbuf[T_LENGTH];
		char recvbuf[T_LENGTH] = "";

		scanf("%s", sendbuf);

		bytesSent = send(mainSocket, sendbuf, strlen(sendbuf), 0);
	}


	system("PAUSE");
}


//client app