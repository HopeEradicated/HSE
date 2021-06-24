#define SZE 1024
#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char nick[SZE] = "ABO";
char password[SZE] = "1234";
char ip[SZE] = "127.0.0.1";
char gameField[7][7] = { 0 };
char basegameField[7][7] = { 0 };
char gameId[SZE] = { 0 };
int Pos1x = 0, Pos1y = 0, Pos2x = 0, Pos2y = 0, Side;
SOCKET client;

char* GetFirstWord(char str[]) {
	int tmp_i = 0;
	char firstWord[SZE] = { 0 };
	while (str[tmp_i] == ' ' && tmp_i < strlen(str)) {
		tmp_i++;
	}
	if (str[tmp_i] != ' ') {
		int tmp_j = 0;
		while (str[tmp_i] != ' ' && str[tmp_i] != '\n' && str[tmp_i] != '\0' && str[tmp_i] != '\r') {
			firstWord[tmp_j] = str[tmp_i];
			tmp_i++;
			tmp_j++;
		}
	}
	return firstWord;
}

int VarInit(char str[]) {
	if (strcmp(GetFirstWord(str), "/game") == 0)
		return 1;
	if (strcmp(GetFirstWord(str), "/move") == 0)
		return 2;
	if (strcmp(GetFirstWord(str), "/elose") == 0)
		return 3;
	return 0;
}

void SendData2Server()
{
	/////// SERVER INIT /////////////////////////////////////////////////////////////////////////////////////////////////////////
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (client == INVALID_SOCKET)
	{
		printf("Error create socket\n");
		return;
	}
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(5510); //the same as in server
	server.sin_addr.S_un.S_addr = inet_addr(ip); //special look-up address
	if (connect(client, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Can't connect to server\n");
		closesocket(client);
		return;
	}
	/////// LOGIN ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	char message[1024];
	sprintf(message, "/login %s %s",nick,password);
	int ret = send(client, message, strlen(message), 0);
	if (ret == SOCKET_ERROR)
	{
		printf("Can't login\n");
		closesocket(client);
		return;
	}
	ret = SOCKET_ERROR;
	while (ret == SOCKET_ERROR)
	{
		ret = recv(client, message, 1024, 0);
		if (ret == 0 || ret == WSAECONNRESET)
		{
			printf("Connection closed\n");
			return;
		}
		printf("%s\n", message);
	}

	sprintf(message, "/off");
	ret = send(client, message, strlen(message), 0);
	if (ret == SOCKET_ERROR)
	{
		printf("Can't login\n");
		closesocket(client);
		return;
	}

	/////// MAINPART ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// /game gameid B/W W-0 B-1
	// /move Pos1x Pos1y Pos2x Pos2y
	// /Elose


	// /login nick password
	// /move gameid Pos1x Pos1y Pos2x Pos2y
	// /lose gameid

	// /game nick/number
	// /list
	// /off
	while (1)
	{
		ret = recv(client, message, 1024, 0);
		if (ret > 0)
		{
			char str[] = { 0 };
			strcpy(str, message);
			char* pch = strtok(str, " ");
			switch (VarInit(message))
			{
			case 1:
			{
				pch = strtok(NULL, " ");
				strcpy(gameId, pch);
				pch = strtok(NULL, " ");
				Side = atoi(pch);
				/////// GAME /////////////////////////////////////////////////////////////////////////////////////////////////////////

				

				break;
			}
			case 2:
			{
				pch = strtok(NULL, " ");
				Pos1x = atoi(pch);
				pch = strtok(NULL, " ");
				Pos1y = atoi(pch);
				pch = strtok(NULL, " ");
				Pos2x = atoi(pch);
				pch = strtok(NULL, " ");
				Pos2y = atoi(pch);
				basegameField[Pos2x][Pos2y] = basegameField[Pos1x][Pos1y];
				basegameField[Pos1x][Pos1y] = ' ';
				break;
			}
			case 3:
			{
				printf("You win");
				break;
			}
			default:
				printf("%s",message);
				break;
			}
			ret = SOCKET_ERROR;
		}
	}
	closesocket(client);
}

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0)
	{
		printf("Can't connect to socket lib");
		return 1;
	}
	srand(time(0));
	SendData2Server();
	printf("Session is closed\n");
	Sleep(1000);
	return 0;
}

