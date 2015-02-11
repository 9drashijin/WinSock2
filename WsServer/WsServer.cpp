// WsServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsa;
	SOCKET sock, new_socket;
	struct sockaddr_in server , client;
	int c,recv_size;
	char *message, clientRequest[2000], ClientNem[100];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	printf("Winsock Initialised.\n");

	printf("\nCreating New Socket...\n");
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	printf("Binding...\n");
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}
	puts("Bind done");

	//Listen to incoming connections
	listen(sock, 10);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);
	while ((new_socket = accept(sock, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
	{
		puts("A client has joined the server.");
		//Reply to the client

		char *client_ip = inet_ntoa(client.sin_addr);
		int client_port = ntohs(client.sin_port);

		printf("Client IP Addr: %s \n", client_ip);
		printf("Client Port   : %d \n", client_port);
	
		message = "You have been watching\nBe prepared...\n";
		if (send(new_socket, message, strlen(message), 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		memset(ClientNem, 0, sizeof ClientNem);
		if ((recv_size = recv(new_socket, ClientNem, 100, 0)) == SOCKET_ERROR)
		{
			puts("recv failed");
		}
		ClientNem[recv_size] = '\0';
		/*if (strcmp(client_ip, "127.0.0.1") == 0){
			message = "You have been kicked from the server.\n";
			if (send(new_socket, message, strlen(message), 0) < 0)
			{
				puts("Send failed");
			}
			closesocket(new_socket);
			return 1;
		}*/

		memset(clientRequest, 0, sizeof clientRequest);
		printf("Welcome %s!\n", ClientNem);
		while (strcmp(clientRequest, "/quit") != 0){
			if ((recv_size = recv(new_socket, clientRequest, 2000, 0)) == SOCKET_ERROR)
			{
				puts("recv failed");
			}
			clientRequest[recv_size] = '\0';
			
			if (strcmp(clientRequest, "/quit") == 0)
			{ 
				closesocket(new_socket); 
			}
			else
			{
				printf("%s>%s\n", ClientNem, clientRequest);
			}
		}
		printf("%s has disconnected from the server.\n\n", ClientNem);
	}
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
		return 1;
	}

	closesocket(sock);
	WSACleanup();

	return 0;
}

