// WsClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in server;
	char *message, server_reply[2000], addr[100], ip[100], msg[1000], cNem[100];
	int recv_size, i;
	unsigned int port;
	struct hostent *he;
	struct in_addr **addr_list;

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	printf("Winsock Initialised.\n");

	while (strcmp(addr, "/quit") != 0){
		printf("\n[/quit] to Exit\n");
		printf("\nCreating New Socket...\n");
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		{
			printf("Could not create socket : %d", WSAGetLastError());
		}
		printf("Socket created.\n");
		printf("http Port: 80\nLocalhost Port: 8888\n");
		
		printf("\nEnter IP Address : ");
		scanf("%s", &addr);
		if (strcmp(addr,"/quit") == 0) break;
		printf("Enter Port       : ");
		scanf("%d", &port);

		if ((he = gethostbyname(addr)) == NULL)
		{
			printf("gethostbyname failed : %d", WSAGetLastError());
			return 1;
		}
		addr_list = (struct in_addr **) he->h_addr_list;

		for (i = 0; addr_list[i] != NULL; i++)
		{
			strcpy(ip, inet_ntoa(*addr_list[i]));
		}
		printf("IP : %s\n", ip);

		server.sin_addr.s_addr = inet_addr(ip);
		server.sin_family = AF_INET;
		server.sin_port = htons(port);

		printf("\nConnecting to Remote Server...\n");
		if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
		{
			puts("Error connecting to server");
			return 1;
		}
		else
		{
			puts("Connected");
		}

		if (strcmp(ip, "127.0.0.1")==0)
		{
			printf("Enter Your Name: \n>");
			scanf("%s",&cNem);
			message = cNem;
			if (send(sock, message, strlen(message), 0) < 0)
			{
				puts("Send failed");
				return 1;
			}
			printf("\nSending message to server...\n");
			printf("Enter Message \n");
			memset(msg, 0, sizeof msg);
			while (strcmp(msg, "/quit") != 0){
				printf("%s>", cNem);
				scanf("%s", &msg);
				message = msg;
				if (send(sock, message, strlen(message), 0) < 0)
				{
					puts("Send failed");
					return 1;
				}
			}
		}
		else
		{
			printf("\nSending message to server...\n");
			message = "GET / HTTP/1.1\r\n\r\n";
			if (send(sock, message, strlen(message), 0) < 0)
			{
				puts("Send failed");
				return 1;
			}
			else
			{
				puts("Data Send");
			}
		}

		if ((recv_size = recv(sock, server_reply, 2000, 0)) == SOCKET_ERROR)
		{
			printf("\nReceive reply from server...\n"); 
			puts("recv failed");
		}
		else
		{
			printf("\nReceive reply from server...\n");
			puts("Reply received\n");
		}

		printf("Printing server reply...\n");
		server_reply[recv_size] = '\0';
		puts(server_reply);
		printf("Terminated\n");

	}
	closesocket(sock);
	WSACleanup();

	return 0;
}

