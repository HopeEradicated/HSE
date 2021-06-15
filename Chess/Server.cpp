#define SZE 1024
#define _CRT_SECURE_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#pragma comment(lib, "pthreadVC2.lib") 
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "MSVCRTD.lib")
#include <winsock.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

pthread_mutex_t mutex;
pthread_mutex_t mutex_file;
bool stat = false;

char nick[100][SZE] = { 0 };
char password[100][SZE] = { 0 };
int gameField[SZE][3] = { 0 };
int status[SZE] = { -1 };
int currentLastGame = 0, numberOfNick = 0, numberOfNickTemp = 0;
SOCKET clients[100];

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
	if (strcmp(GetFirstWord(str), "/login") == 0)
		return 1;
	if (strcmp(GetFirstWord(str), "/move") == 0)
		return 2;
	if (strcmp(GetFirstWord(str), "/lose") == 0)
		return 3;
	if (strcmp(GetFirstWord(str), "/game") == 0)
		return 4;
	if (strcmp(GetFirstWord(str), "/off") == 0)
		return 5;
	if (strcmp(GetFirstWord(str), "/list") == 0)
		return 6;
	if (strcmp(GetFirstWord(str), "/stop") == 0)
		return 7;
	return 0;
}

void* ClientStart(void* param)
{
	SOCKET client = (SOCKET)param;
	char recieve[2 * SZE], transmit[SZE];
	int ret;
	char TempNick[SZE] = { 0 }, TempPassword[SZE] = { 0 };
	int  TempGameId=0, pos = 0;
	char str[SZE] = { 0 };
	while (1)
	{
		ret = recv(client, recieve, 1024, 0);
		if (!ret || ret == SOCKET_ERROR)
		{
			pthread_mutex_lock(&mutex);
			printf("Error getting data\n");
			pthread_mutex_unlock(&mutex);
			return (void*)2;
		}
		else
		{
			recieve[ret] = ' ';
			recieve[ret + 1] = '\0';
			printf("%s\n", recieve);
			strcpy(str, recieve);
			char* pch = strtok(str, " ");
			switch (VarInit(recieve))
			{
			case 1:
			{
				pch = strtok(NULL, " ");
				strcpy(TempNick, pch);
				pch = strtok(NULL, " ");
				strcpy(TempPassword, pch);
				int i = 0, isInList = 0;
				while ((i < numberOfNick)&&(numberOfNick))
				{
					//printf("|%s| |%s||||%s| |%s|\n", nick[i], password[i], TempNick, TempPassword);
					if (strcmp(TempNick, nick[i]) == 0)
					{
						if (status[i] == 1) return (void*)0;
						isInList = 1;
						if (strcmp(TempPassword, password[i]) == 0)
						{
							sprintf_s(transmit, "Access granted");
							clients[i] = client;
							status[i] = 1;
							pos = i;
						}
						else
							sprintf_s(transmit, "Access denied");
					}
					i++;
				}
				if (isInList == 0)
				{
					strcpy(nick[numberOfNick], TempNick);
					strcpy(password[numberOfNick], TempPassword);
					numberOfNick++;
					sprintf_s(transmit, "Succesfuly registred");
					printf("%dxxx", i);
					clients[i] = client;
					status[i] = 1;
					pos = i;
				}
				ret = send(client, transmit, sizeof(transmit), 0);
				if (ret == SOCKET_ERROR)
				{
					pthread_mutex_lock(&mutex);
					printf("Error sending data\n");
					pthread_mutex_unlock(&mutex);
					return (void*)2;
				}
				strcpy(str, " ");
				break;
			}
			// RECV
			// /login nick password
			// /move gameid Pos1x Pos1y Pos2x Pos2y
			// /lose gameid
			// 
			// /game nick
			// /off
			// /list

			// SEND 
			// /game gameid B/W W-0 B-1
			// /move gameid Pos1x Pos1y Pos2x Pos2y
			// /Elose
			case 2:
			{
				pch = strtok(NULL, " ");
				TempGameId = atoi(pch);
				printf("%d %d %d 2 ??", TempGameId, gameField[TempGameId][0], gameField[TempGameId][1]);
				if (clients[gameField[TempGameId][0]] != client)
				{
					ret = send(clients[gameField[TempGameId][0]], recieve, sizeof(recieve), 0);
					if (ret == SOCKET_ERROR)
					{
						int i = 0;
						while ((i < 20) && (ret == SOCKET_ERROR))
						{
							ret = send(clients[gameField[TempGameId][0]], recieve, sizeof(recieve), 0);
							i++;
						}
						pthread_mutex_lock(&mutex);
						printf("Error sending data\n");
						pthread_mutex_unlock(&mutex);
						return (void*)2;
					}
				}
				if (clients[gameField[TempGameId][1]] != client)
				{
					ret = send(clients[gameField[TempGameId][1]], recieve, sizeof(recieve), 0);
					if (ret == SOCKET_ERROR)
					{
						int i = 0;
						while ((i < 20) && (ret == SOCKET_ERROR))
						{
							ret = send(clients[gameField[TempGameId][0]], recieve, sizeof(recieve), 0);
							i++;
						}
						pthread_mutex_lock(&mutex);
						printf("Error sending data\n");
						pthread_mutex_unlock(&mutex);
						return (void*)2;
					}
				}
				break;
			}
			case 3:
			{
				pch = strtok(NULL, " ");
				TempGameId = atoi(pch);
				sprintf_s(transmit, "/elose");
				printf("%d %d %d 3 ??", TempGameId, gameField[TempGameId][0], gameField[TempGameId][1]);
				if (clients[gameField[TempGameId][0]] != client)
				{
					ret = send(clients[gameField[TempGameId][0]], transmit, sizeof(transmit), 0);
					if (ret == SOCKET_ERROR)
					{
						int i = 0;
						while ((i < 20) && (ret == SOCKET_ERROR))
						{
							ret = send(clients[gameField[TempGameId][0]], transmit, sizeof(transmit), 0);
							i++;
						}
						pthread_mutex_lock(&mutex);
						printf("Error sending data\n");
						pthread_mutex_unlock(&mutex);
						return (void*)2;
					}
					gameField[TempGameId][3] = gameField[TempGameId][0];
				}
				if (clients[gameField[TempGameId][1]] != client)
				{
					ret = send(clients[gameField[TempGameId][1]], transmit, sizeof(transmit), 0);
					if (ret == SOCKET_ERROR)
					{
						int i = 0;
						while ((i < 20) && (ret == SOCKET_ERROR))
						{
							ret = send(clients[gameField[TempGameId][0]], transmit, sizeof(transmit), 0);
							i++;
						}
						pthread_mutex_lock(&mutex);
						printf("Error sending data\n");
						pthread_mutex_unlock(&mutex);
						return (void*)2;
					}
					gameField[TempGameId][3] = gameField[TempGameId][1];
				}
				break;
			}
			case 4:
			{
				pch = strtok(NULL, " ");
				strcpy(TempNick, pch);
				for (int i = 0; i < numberOfNick; i++)
					if (nick[i] == TempNick)
					{
						sprintf_s(transmit, "/game %d %d", currentLastGame, 1);
						ret = send(clients[i], transmit, sizeof(transmit), 0);
						if (ret == SOCKET_ERROR)
						{
							pthread_mutex_lock(&mutex);
							printf("Error sending data\n");
							pthread_mutex_unlock(&mutex);
							return (void*)2;
						}
						else
						{
							sprintf_s(transmit, "/game %d %d", currentLastGame, 0);
							ret = send(client, transmit, sizeof(transmit), 0);
							if (ret == SOCKET_ERROR)
							{
								pthread_mutex_lock(&mutex);
								printf("Error sending data\n");
								pthread_mutex_unlock(&mutex);
								return (void*)2;
							}
							else
								currentLastGame++;
						}
					}
				break;
			}
			case 5:
			{
				status[pos] = 0;
				closesocket(client);
				return (void*)0;
				break;
			}
			case 6:
			{
				sprintf_s(transmit, "Already on server\n");
				ret = send(client, transmit, sizeof(transmit), 0);
				if (ret == SOCKET_ERROR)
				{
					pthread_mutex_lock(&mutex);
					printf("Error sending data\n");
					pthread_mutex_unlock(&mutex);
					return (void*)2;
				}
				else
					for (int i = 0; i < numberOfNick; i++)
						if (status[i] == 1)
						{
							sprintf_s(transmit, "%s\n", nick[i]);
							ret = send(client, transmit, sizeof(transmit), 0);
							if (ret == SOCKET_ERROR)
							{
								pthread_mutex_lock(&mutex);
								printf("Error sending data\n");
								pthread_mutex_unlock(&mutex);
								return (void*)2;
							}
						}
				break;
			}
			case 7:
			{
				stat = true;
				return (void*)0;
				break;
			}
			default:
			{
				for (int i = 0; i < numberOfNick; i++)
					if (status[i] == 1)
					{
						sprintf_s(recieve, "%s: %s", nick[pos], recieve);
						ret = send(clients[i], recieve, sizeof(recieve), 0);
						if (ret == SOCKET_ERROR)
						{
							pthread_mutex_lock(&mutex);
							printf("Error sending data\n");
							pthread_mutex_unlock(&mutex);
							return (void*)2;
						}
					}
				break;
			}
			strcpy(recieve, " ");
			}
		}
	}
	return (void*)0;
}

int CreateServer()
{
	SOCKET server, client;
	sockaddr_in localaddr, clientaddr;

	int size;
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (server == INVALID_SOCKET)
	{
		printf("Error create server\n");
		return 1;
	}
	localaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(5510);//port number is for example, must be more than 1024
	if (bind(server, (sockaddr*)&localaddr, sizeof(localaddr)) == SOCKET_ERROR)
	{
		printf("Can't start server\n");
		return 2;
	}
	else
		printf("Server is started\n");
	listen(server, 50);//50 клиентов в очереди могут стоять
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex_file, NULL);
	int i = 0;
	while (i<3)
	{
		size = sizeof(clientaddr);
		client = accept(server, (sockaddr*)&clientaddr, &size);

		if (client == INVALID_SOCKET)
		{
			printf("Error accept client\n");
			continue;
		}
		else
			printf("Client is accepted\n");

		pthread_t mythread;
		int status = pthread_create(&mythread, NULL, ClientStart, (void*)client);
		pthread_detach(mythread);
		i++;
	}
	pthread_mutex_destroy(&mutex_file);
	pthread_mutex_destroy(&mutex);
	printf("Server is stopped\n");

	FILE* nickfl = fopen("nick.txt", "wt");
	FILE* passwordfl = fopen("password.txt", "wt");
	FILE* gamesfl = fopen("games.txt", "wt");
	int io = 0;
	while (io < numberOfNick)
	{
		fprintf(nickfl,"%s\n",nick[io]);
		fprintf(passwordfl,"%s\n",password[io]);
		io++;
	}
	io = 0;
	while (io < currentLastGame)
	{
		fprintf(gamesfl, "%s VS %s. Win %s\n", nick[gameField[io][0]], nick[gameField[io][1]], nick[gameField[io][3]]);
		io++;
	}
	fclose(gamesfl);
	fclose(nickfl);
	fclose(passwordfl);

	closesocket(server);
	return 0;
}

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd) == 0)
		printf("Connected to socket lib\n");
	else
		return 1;

	FILE* nickfl = fopen("nick.txt", "rt");
	FILE* passwordfl = fopen("password.txt", "rt");
	int i = 0;
	while (!feof(nickfl))
	{
		fgets(nick[i], SZE, nickfl);
		fgets(password[i], SZE, passwordfl);
		for (int j = 0; j < strlen(nick[i]); j++)
		{
			if (nick[i][j] == '\n') nick[i][j] = '\0';
		}
		for (int j = 0; j < strlen(password[i]); j++)
		{
			if (password[i][j] == '\n') password[i][j] = '\0';
		}
		numberOfNick++;
		i++;
	}
	numberOfNickTemp = numberOfNick;
	fclose(nickfl);
	fclose(passwordfl);

	gameField[0][0] = 0;
	gameField[0][1] = 1;
	currentLastGame++;
	return CreateServer();
}