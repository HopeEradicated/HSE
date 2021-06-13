#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <wchar.h>

using namespace std;
#define SIZE  8
#define SIZED 20

char gameField[SIZE][SIZE] = { 0 };
char movesField[SIZE][SIZE] = { 0 };
char dangerFieldB[SIZE][SIZE] = { 0 };
char dangerFieldW[SIZE][SIZE] = { 0 };


char curFigure = ' ';
COORD posOfCurFigure;
COORD whiteKingPos;
COORD blackKingPos;
bool turnOfWhite = true;
bool turnOfBlack = false;
bool isPeshkaCanMove = true;
int n = 0;
int m = 0;
int waitingForStart = 0;
enum ConsoleColor {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

enum Teams {
	whiteTeam = 0,
    blackTeam = 1
};

void drawCursor() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (Green << 4) | White);
	cout << gameField[n][m] ;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (Black << 4) | White);
	
}

void setCursorPosition(COORD position) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}


void drawGameField() {
	COORD start;
	start.X = 0;
	start.Y = 0;
	setCursorPosition(start);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
				if ((i + j) % 2 == 0) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (Black << 4) | White);
				} else {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (White << 4) | Black);
				
				}
              
				if (movesField[i][j] == '-') {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (LightBlue << 4) | White);
					if (i != n || j != m)
						cout << gameField[i][j];
					else
						drawCursor();
				} else {
					if (i != n || j != m)
						cout << gameField[i][j];
					else
						drawCursor();
				}
				if (gameField[i][j] == '\0') {
					cout << "q";
				}
				cout << " ";
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (Black << 4) | White);
		cout << i+1 <<"\n";
	}
	char letters[SIZE] = { 'A','B','C','D','E','F','G','H' };
	for (int i = 0; i < SIZE; i++) {
		cout << letters[i] << " ";
	}
	cout << '\n';
}

void fillGameField(char mat[8][8]) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if ((i == 0 || i == 7) && (j == 0 || j == 7)) {
				if (i == 7) {
					gameField[i][j] = '8';
				}
				else {
					gameField[i][j] = 82;
				}
		    }

			if ((i == 0 || i == 7) && (j == 1 || j == 6)) {
				if (i == 7) {
					gameField[i][j] = '7';
				}
				else {
					gameField[i][j] = 'H';
				}
			}

			if ((i == 0 || i == 7) && (j == 2 || j == 5)) {
				if (i == 7) {
					gameField[i][j] = 24;
				}
				else {
					gameField[i][j] = 'E';
				}
			}

			if ((i == 0 || i == 7) && (j == 4)) {
				if (i == 7) {
					gameField[i][j] = 1;
				}
				else {
					gameField[i][j] = 'K';
				}
			}

			if ((i == 0 || i == 7) && (j == 3)) {
				if (i == 7) {
					gameField[i][j] = 87;
				} else {
					gameField[i][j] = 88;
				}
			}

			if (i == 1 || i == 6) {
				if (i == 6) {
					gameField[i][j] = 30;
				}
				else {
					gameField[i][j] = 'P';
				}
			}
		}
	}
}

int determineTeam(char elem) { 
	if (elem == ' ') {
		return 3;
	}
	char blackTeam[6] = { 'R','H','E','X','K','P'};
	char whiteTeam[6] = { 1,30,87,24,'7','8' };
	int counter = 0;
	for (int i = 0; i < 6; i++) {
		if (blackTeam[i] == elem) {
			counter++;
		}
	}
	if (counter == 1) {
		return 1;
	} else {
		return 0;
	}
}

bool isDifTeam(char fElem, char sElem) {
	if (fElem == ' ' || sElem == ' ') {
		return true;
	}

	char blackTeam[6] = { 'R','H','E','X','K','P' };
	char whiteTeam[6] = { 1,30,87,24,'7','8' };
	int counter = 0;
	for (int i = 0; i < 6; i++) {
		if (blackTeam[i] == fElem) {
			counter++;
		}
		if (blackTeam[i] == sElem) {
			counter++;
		}
	}
	
	if (counter == 2 || counter == 0) {
		return false;
	}
	return true;
}

void markPossibleMoves(char mat[8][8], char output[8][8], COORD position, char element, char symbol) {
	COORD temp = position;
	if (element == 30 || element == 'P') {

		if (temp.X == 1 || temp.X == 6) {
			if (temp.X == 1 && element == 'P') {
				if (isPeshkaCanMove == true) {
					if (gameField[temp.X + 1][temp.Y] == ' ') {
						output[temp.X + 1][temp.Y] = symbol;

						if (gameField[temp.X + 2][temp.Y] == ' ') {
							output[temp.X + 2][temp.Y] = symbol;
						}
					}
				}
				if (isDifTeam(gameField[temp.X + 1][temp.Y + 1], gameField[temp.X][temp.Y]) == true && gameField[temp.X + 1][temp.Y + 1] != ' ' && temp.X + 1 < SIZE && temp.Y + 1 < SIZE) {
					output[temp.X + 1][temp.Y + 1] = symbol;
				}
				if (isDifTeam(gameField[temp.X + 1][temp.Y - 1], gameField[temp.X][temp.Y]) == true && gameField[temp.X + 1][temp.Y - 1] != ' ' && temp.X + 1 < SIZE && temp.Y - 1 >= 0) {
					output[temp.X + 1][temp.Y - 1] = symbol;
				}
			}

			if (temp.X == 6 && element == 30) {
				if (isPeshkaCanMove == true) {
					if (gameField[temp.X - 1][temp.Y] == ' ') {
						output[temp.X - 1][temp.Y] = symbol;

						if (gameField[temp.X - 2][temp.Y] == ' ') {
							output[temp.X - 2][temp.Y] = symbol;
						}
					}
				}
				if (isDifTeam(gameField[temp.X - 1][temp.Y + 1], gameField[temp.X][temp.Y]) == true && gameField[temp.X - 1][temp.Y + 1] != ' ' && temp.X - 1 >= 0 && temp.Y + 1 < SIZE) {
					output[temp.X - 1][temp.Y + 1] = symbol;
				}
				if (isDifTeam(gameField[temp.X - 1][temp.Y - 1], gameField[temp.X][temp.Y]) == true && gameField[temp.X - 1][temp.Y - 1] != ' ' && temp.X - 1 >= 0 && temp.Y - 1 >= 0) {
					output[temp.X - 1][temp.Y - 1] = symbol;
				}
			}
		}
		else {
			if (element == 30) {
				if (isPeshkaCanMove == true) {
					if (gameField[temp.X - 1][temp.Y] == ' ') {
						output[temp.X - 1][temp.Y] = symbol;
					}
				}
				if (isDifTeam(gameField[temp.X - 1][temp.Y + 1], gameField[temp.X][temp.Y]) == true && gameField[temp.X - 1][temp.Y + 1] != ' ' && temp.X - 1 >= 0 && temp.Y + 1 < SIZE) {
					output[temp.X - 1][temp.Y + 1] = symbol;
				}
				if (isDifTeam(gameField[temp.X - 1][temp.Y - 1], gameField[temp.X][temp.Y]) == true && gameField[temp.X - 1][temp.Y - 1] != ' ' && temp.X - 1 >= 0 && temp.Y - 1 >= 0) {
					output[temp.X - 1][temp.Y - 1] = symbol;
				}
			}

			if (element == 'P') {
				if (isPeshkaCanMove == true) {
					if (gameField[temp.X + 1][temp.Y] == ' ') {
						output[temp.X + 1][temp.Y] = symbol;
					}
				}
				if (isDifTeam(gameField[temp.X + 1][temp.Y + 1], gameField[temp.X][temp.Y]) == true && gameField[temp.X + 1][temp.Y + 1] != ' ' && temp.X + 1 < SIZE && temp.Y + 1 < SIZE) {
					output[temp.X + 1][temp.Y + 1] = symbol;
				}
				if (isDifTeam(gameField[temp.X + 1][temp.Y - 1], gameField[temp.X][temp.Y]) == true && gameField[temp.X + 1][temp.Y - 1] != ' ' && temp.X + 1 < SIZE && temp.Y - 1 >= 0) {
					output[temp.X + 1][temp.Y - 1] = symbol;
				}
			}
		}

	}

	if (element == 24 || element == 'E') {
		COORD save = temp;
		temp.X--;
		temp.Y++;
		while (gameField[temp.X][temp.Y] == ' ' && temp.X >= 0 && temp.Y < SIZE) {
			output[temp.X][temp.Y] = symbol;
			temp.X--;
			temp.Y++;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X >= 0 && temp.Y < SIZE) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.X--;
		temp.Y--;
		while (gameField[temp.X][temp.Y] == ' ' && temp.X >= 0 && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
			temp.X--;
			temp.Y--;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X >= 0 && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.X++;
		temp.Y--;
		while (gameField[temp.X][temp.Y] == ' ' && temp.X < SIZE && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
			temp.X++;
			temp.Y--;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X < SIZE && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.X++;
		temp.Y++;
		while (gameField[temp.X][temp.Y] == ' ' && temp.X < SIZE && temp.Y < SIZE) {
			output[temp.X][temp.Y] = symbol;
			temp.X++;
			temp.Y++;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X < SIZE && temp.Y < SIZE) {
			output[temp.X][temp.Y] = symbol;
		}
	}
	if (element == '8' || element == 82) {
		COORD save = temp;
		temp.X--;
		while (gameField[temp.X][temp.Y] == ' ') {
			output[temp.X][temp.Y] = symbol;
			temp.X--;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X >= 0) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.X++;
		while (gameField[temp.X][temp.Y] == ' ') {
			output[temp.X][temp.Y] = symbol;
			temp.X++;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X < SIZE) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.Y--;
		while (gameField[temp.X][temp.Y] == ' ' && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
			temp.Y--;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.Y++;
		while (gameField[temp.X][temp.Y] == ' ' && temp.Y <= 7) {
			output[temp.X][temp.Y] = symbol;
			temp.Y++;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.Y < SIZE) {
			output[temp.X][temp.Y] = symbol;
		}
	}
	if (element == '7' || element == 'H') {
		if (isDifTeam(gameField[temp.X - 2][temp.Y - 1], gameField[temp.X][temp.Y]) == true && temp.X - 2 >= 0 && temp.Y - 1 >= 0) {
			output[temp.X - 2][temp.Y - 1] = symbol;
		}
		if (isDifTeam(gameField[temp.X - 2][temp.Y + 1], gameField[temp.X][temp.Y]) == true && temp.X - 2 >= 0 && temp.Y + 1 < SIZE) {
			output[temp.X - 2][temp.Y + 1] = symbol;
		}
		if (isDifTeam(gameField[temp.X + 2][temp.Y + 1], gameField[temp.X][temp.Y]) == true && temp.X + 2 < SIZE && temp.Y + 1 < SIZE) {
			output[temp.X + 2][temp.Y + 1] = symbol;
		}
		if (isDifTeam(gameField[temp.X + 2][temp.Y - 1], gameField[temp.X][temp.Y]) == true && temp.X + 2 < SIZE && temp.Y - 1 >= 0) {
			output[temp.X + 2][temp.Y - 1] = symbol;
		}
		if (isDifTeam(gameField[temp.X - 1][temp.Y + 2], gameField[temp.X][temp.Y]) == true && temp.X - 1 >= 0 && temp.Y + 2 < SIZE) {
			output[temp.X - 1][temp.Y + 2] = symbol;
		}
		if (isDifTeam(gameField[temp.X + 1][temp.Y + 2], gameField[temp.X][temp.Y]) == true && temp.X + 1 < SIZE && temp.Y + 2 < SIZE) {
			output[temp.X + 1][temp.Y + 2] = symbol;
		}
		if (isDifTeam(gameField[temp.X - 1][temp.Y - 2], gameField[temp.X][temp.Y]) == true && temp.X - 1 >= 0 && temp.Y - 2 >= 0) {
			output[temp.X - 1][temp.Y - 2] = symbol;
		}
		if (isDifTeam(gameField[temp.X + 1][temp.Y - 2], gameField[temp.X][temp.Y]) == true && temp.X + 1 < SIZE && temp.Y - 2 >= 0) {
			output[temp.X + 1][temp.Y - 2] = symbol;
		}
	}
	if (element == 1 || element == 'K') {
		if (isDifTeam(gameField[temp.X - 1][temp.Y - 1], gameField[temp.X][temp.Y]) == true && temp.X - 1 >= 0 && temp.Y - 1 >= 0) {
			output[temp.X - 1][temp.Y - 1] = symbol;
		}
		if (isDifTeam(gameField[temp.X - 1][temp.Y + 1], gameField[temp.X][temp.Y]) == true && temp.X - 1 >= 0 && temp.Y + 1 < SIZE) {
			output[temp.X - 1][temp.Y + 1] = symbol;
		}
		if (isDifTeam(gameField[temp.X + 1][temp.Y + 1], gameField[temp.X][temp.Y]) == true && temp.X + 1 < SIZE && temp.Y + 1 < SIZE) {
			output[temp.X + 1][temp.Y + 1] = symbol;
		}
		if (isDifTeam(gameField[temp.X + 1][temp.Y - 1], gameField[temp.X][temp.Y]) == true && temp.X + 1 < SIZE && temp.Y - 1 >= 0) {
			output[temp.X + 1][temp.Y - 1] = symbol;
		}
		if (isDifTeam(gameField[temp.X - 1][temp.Y], gameField[temp.X][temp.Y]) == true && temp.X - 1 >= 0) {
			output[temp.X - 1][temp.Y] = symbol;
		}
		if (isDifTeam(gameField[temp.X + 1][temp.Y], gameField[temp.X][temp.Y]) == true && temp.X + 1 < SIZE) {
			output[temp.X + 1][temp.Y] = symbol;
		}
		if (isDifTeam(gameField[temp.X][temp.Y - 1], gameField[temp.X][temp.Y]) == true && temp.Y - 1 >= 0) {
			output[temp.X][temp.Y - 1] = symbol;
		}
		if (isDifTeam(gameField[temp.X][temp.Y + 1], gameField[temp.X][temp.Y]) == true && temp.Y + 1 < SIZE) {
			output[temp.X][temp.Y + 1] = symbol;
		}
	}
	if (element == 'X' || element == 'W') {
		COORD save = temp;
		temp.X--;
		temp.Y++;
		while (gameField[temp.X][temp.Y] == ' ' && temp.X >= 0 && temp.Y < SIZE) {
			output[temp.X][temp.Y] = symbol;
			temp.X--;
			temp.Y++;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X >= 0 && temp.Y < SIZE) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.X--;
		temp.Y--;
		while (gameField[temp.X][temp.Y] == ' ' && temp.X >= 0 && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
			temp.X--;
			temp.Y--;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X >= 0 && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.X++;
		temp.Y--;
		while (gameField[temp.X][temp.Y] == ' ' && temp.X < SIZE && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
			temp.X++;
			temp.Y--;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X < SIZE && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.X++;
		temp.Y++;
		while (gameField[temp.X][temp.Y] == ' ' && temp.X < SIZE && temp.Y < SIZE) {
			output[temp.X][temp.Y] = symbol;
			temp.X++;
			temp.Y++;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X < SIZE && temp.Y < SIZE) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.X--;
		while (gameField[temp.X][temp.Y] == ' ') {
			output[temp.X][temp.Y] = symbol;
			temp.X--;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X >= 0) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.X++;
		while (gameField[temp.X][temp.Y] == ' ') {
			output[temp.X][temp.Y] = symbol;
			temp.X++;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.X < SIZE) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.Y--;
		while (gameField[temp.X][temp.Y] == ' ' && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
			temp.Y--;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.Y >= 0) {
			output[temp.X][temp.Y] = symbol;
		}
		temp = save;
		temp.Y++;
		while (gameField[temp.X][temp.Y] == ' ' && temp.Y <= 7) {
			output[temp.X][temp.Y] = symbol;
			temp.Y++;
		}
		if (isDifTeam(gameField[temp.X][temp.Y], gameField[save.X][save.Y]) == true && temp.Y < SIZE) {
			output[temp.X][temp.Y] = symbol;
		}
	}

}

void clearGameField(char field[8][8]) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (field[i][j] == '*' || field[i][j] == '-') {
				field[i][j] = ' ';
			}
		}
	}
}

bool isNoPosMoves() {
	int counter = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (movesField[i][j] == '-') {
				counter++;
			}
		}
	}
	if (counter == 0) {
		return true;
	}
	return false;
}

void playerCommands() {
	char command[SIZED] = { 0 };
	cin >> command;
	if (strcmp(command, "!help") == 0) {
		cout << "no help\n";
	} else if (strcmp(command, "!play") == 0) {
		waitingForStart++;
	} else {
		cout << "Unknown command!\n";
	}
}

bool whiteCheckMate() {
	
		clearGameField(dangerFieldW);
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {

				if (determineTeam(gameField[i][j]) == 1) {
					COORD tempPos;
					tempPos.X = i;
					tempPos.Y = j;
					isPeshkaCanMove = false;
					markPossibleMoves(gameField, dangerFieldW, tempPos, gameField[i][j], '-');
					isPeshkaCanMove = true;
					dangerFieldW[i][j] = '-';
				}
			}
		}
	int counter = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (determineTeam(gameField[i][j]) == 0) {
				if (gameField[i][j] != ' ' && gameField[i][j] != 1) {
					dangerFieldW[i][j] = '*';
				}
			}
		}
	}
	if (dangerFieldW[whiteKingPos.X][whiteKingPos.Y] == '-') {
		return true;
	} 
		if (dangerFieldW[whiteKingPos.X - 1][whiteKingPos.Y - 1] == '-'  || whiteKingPos.X - 1 < 0 || whiteKingPos.Y - 1 < 0) {
			counter++;
		}
		if (dangerFieldW[whiteKingPos.X - 1][whiteKingPos.Y + 1] == '-' || whiteKingPos.X - 1 < 0 || whiteKingPos.Y + 1 >= SIZE) {
			counter++;
		}
		if (dangerFieldW[whiteKingPos.X + 1][whiteKingPos.Y + 1] == '-' || whiteKingPos.X + 1 >= SIZE || whiteKingPos.Y + 1 >= SIZE) {
			counter++;
		}
		if (dangerFieldW[whiteKingPos.X + 1][whiteKingPos.Y - 1] == '-' || whiteKingPos.X + 1 >= SIZE || whiteKingPos.Y - 1 < 0) {
			counter++;
		}
		if (dangerFieldW[whiteKingPos.X - 1][whiteKingPos.Y] == '-' || whiteKingPos.X - 1 < 0) {
			counter++;
			
		}
		if (dangerFieldW[whiteKingPos.X + 1][whiteKingPos.Y] == '-'  || whiteKingPos.X + 1 >= SIZE) {
			counter++;
		}
		if (dangerFieldW[whiteKingPos.X][whiteKingPos.Y - 1] == '-'  || whiteKingPos.Y - 1 < 0) {
			counter++;
		}
		if (dangerFieldW[whiteKingPos.X][whiteKingPos.Y + 1] == '-'  || whiteKingPos.Y + 1 >= SIZE) {
			counter++;
		}
	
		if (counter == 8) {
			return true;
		
		}
		return false;
}

bool blackCheckMate() {
		clearGameField(dangerFieldB);
		
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {

				if (determineTeam(gameField[i][j]) == 0) {
					COORD tempPos;
					tempPos.X = i;
					tempPos.Y = j;
					isPeshkaCanMove = false;
					markPossibleMoves(gameField, dangerFieldB, tempPos, gameField[i][j], '-');
					isPeshkaCanMove = true;
					dangerFieldB[i][j] = '-';
				} 
			}
		}
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (determineTeam(gameField[i][j]) == 1) {
				if (gameField[i][j] != ' ' && gameField[i][j] != 'K') {
					dangerFieldB[i][j] = '*';
				}
			}
		}
	}
	int counter = 0;
	if (dangerFieldB[blackKingPos.X][blackKingPos.Y] == '-') {
		return true;
	}
	if (dangerFieldB[blackKingPos.X - 1][blackKingPos.Y - 1] == '-' || blackKingPos.X - 1 < 0 || blackKingPos.Y - 1 < 0) {
		counter++;
	}
	if (dangerFieldB[blackKingPos.X - 1][blackKingPos.Y + 1] == '-' || blackKingPos.X - 1 < 0 || blackKingPos.Y + 1 >= SIZE) {
		counter++;
	}
	if (dangerFieldB[blackKingPos.X + 1][blackKingPos.Y + 1] == '-' || blackKingPos.X + 1 >= SIZE || blackKingPos.Y + 1 >= SIZE) {
		counter++;
	}
	if (dangerFieldB[blackKingPos.X + 1][blackKingPos.Y - 1] == '-' || blackKingPos.X + 1 >= SIZE || blackKingPos.Y - 1 < 0) {
		counter++;
	}
	if (dangerFieldB[blackKingPos.X - 1][blackKingPos.Y] == '-' || blackKingPos.X - 1 < 0) {
		counter++;
	}
	if (dangerFieldB[blackKingPos.X + 1][blackKingPos.Y] == '-' || blackKingPos.X + 1 >= SIZE) {
		counter++;
	}
	if (dangerFieldB[blackKingPos.X][blackKingPos.Y - 1] == '-' || blackKingPos.Y - 1 < 0) {
		counter++;
	}
	if (dangerFieldB[blackKingPos.X][blackKingPos.Y + 1] == '-' || blackKingPos.Y + 1 >= SIZE) {
		counter++;
	}
	
	if (counter == 8) {
		return true;
	}
	return false;

}

bool isAnyPossibilitiesW() {
	char temp[SIZE][SIZE] = { 0 };
	char tempElem = ' ';
	bool kingCanBeSaved = false;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {

			if (determineTeam(gameField[i][j]) == 0) {
					COORD tempPos;
					tempPos.X = i;
					tempPos.Y = j;
					markPossibleMoves(gameField, temp, tempPos, gameField[i][j], '*');
					
					for (int ia = 0; ia < SIZE; ia++) {
						for (int ja = 0; ja < SIZE; ja++) {
							if (temp[ia][ja] == '*') {
								tempElem = gameField[ia][ja];
								gameField[ia][ja] = gameField[i][j];
								gameField[i][j] = ' ';
								if (!whiteCheckMate()) {
									gameField[i][j] = gameField[ia][ja];
									gameField[ia][ja] = tempElem;
									kingCanBeSaved = true;
									break;
								} 
								gameField[i][j] = gameField[ia][ja];
								gameField[ia][ja] = tempElem;
							}
						}
					}
					clearGameField(temp);
			}
			if (kingCanBeSaved == true) {
				return true;
			}
		}
	}
	return false;
}

bool isAnyPossibilitiesB() {
	char temp[SIZE][SIZE] = { 0 };
	char tempElem = ' ';
	bool kingCanBeSaved = false;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {

			if (determineTeam(gameField[i][j]) == 1) {
				COORD tempPos;
				tempPos.X = i;
				tempPos.Y = j;
				markPossibleMoves(gameField, dangerFieldB, tempPos, gameField[i][j], '*');
			
				for (int ia = 0; ia < SIZE; ia++) {
					for (int ja = 0; ja < SIZE; ja++) {
						if (temp[ia][ja] == '*') {
							tempElem = gameField[ia][ja];
							gameField[ia][ja] = gameField[i][j];
							gameField[i][j] = ' ';
							if (!blackCheckMate()) {
								gameField[i][j] = gameField[ia][ja];
								gameField[ia][ja] = tempElem;
								kingCanBeSaved = true;
								break;
							}
							gameField[i][j] = gameField[ia][ja];
							gameField[ia][ja] = tempElem;
						}
					}
				}
				clearGameField(temp);
			}
			if (kingCanBeSaved == true) {
				return true;
			}
		}
	}
	return false;
}

void canWeEatDangerW() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (determineTeam(gameField[i][j]) == 1) {
				if (dangerFieldB[i][j] == '*') {
					COORD tempPos;
					tempPos.X = i;
					tempPos.Y = j;
					markPossibleMoves(gameField, dangerFieldB, tempPos, gameField[i][j], '*');
					dangerFieldW[i][j] = '*';
				}
			}
		}
	}
}

void canWeEatDangerB() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {

			if (determineTeam(gameField[i][j]) == 0) {
				if (dangerFieldB[i][j] == '*') {
					COORD tempPos;
					tempPos.X = i;
					tempPos.Y = j;
					markPossibleMoves(gameField, dangerFieldW, tempPos, gameField[i][j], '*');
					dangerFieldW[i][j] = '*';
				}
			}
		}
	}
}

void playerInput() {
	switch (_getch()) {
	   case('w'):
	   case('W'):
		   system("cls");
		   if (n - 1 >= 0) {
			   n--;
		   }
		   break;
	   case('a'):
	   case('A'):
		   system("cls");
		   if (m - 1 >= 0) {
			   m--;
		   }
		   break;
	   case('s'):
	   case('S'):
		   system("cls");
		   if (n + 1 < SIZE) {
			   n++;
		   }
		   break;
	   case('d'):
	   case('D'):
		   system("cls");
		   if (m + 1 < SIZE) {
			   m++;
		   }
		   break;
	   case('c'):
	   case('C'):
		   if (gameField[n][m] == ' ') {
			   cout << "Cell is empty\n";
		   } else if (curFigure == ' ') {
			   if (determineTeam(gameField[n][m]) == 0 && turnOfWhite == true) {
				   swap(turnOfBlack, turnOfWhite);

				   posOfCurFigure.X = n;
				   posOfCurFigure.Y = m;
				   curFigure = gameField[n][m];
				   markPossibleMoves(gameField,movesField,posOfCurFigure,curFigure,'-');

				   if (isNoPosMoves() == true) {
					   cout << "No possible moves, chose another figure";
					   swap(turnOfBlack, turnOfWhite);
					   curFigure = ' ';
				   }
				  
			   }
			   else if (determineTeam(gameField[n][m]) == 1 && turnOfBlack == true) {
				   swap(turnOfBlack, turnOfWhite);

				   posOfCurFigure.X = n;
				   posOfCurFigure.Y = m;
				   curFigure = gameField[n][m];
				   markPossibleMoves(gameField, movesField,posOfCurFigure,curFigure,'-');
				   if (isNoPosMoves() == true) {
					   cout << "No possible moves, chose another figure";
					   swap(turnOfBlack, turnOfWhite);
					   curFigure = ' ';
				   }
				  
			   }
			   else if (determineTeam(gameField[n][m]) == 1 && turnOfBlack == false || determineTeam(gameField[n][m]) == 0 && turnOfWhite == false) {
				   cout << "Not your turn or figure";
			   }
		   } else {
			   cout << "You already choose some figure!";
		   }
		   break;
	   case('p'):
	   case('P'):
		   if (movesField[n][m] == '-') {
			   if (gameField[n][m] == 'K' || gameField[n][m] == 1 ) {
				   cout << "You can not eat the king!";
			   } else {
				   gameField[n][m] = gameField[posOfCurFigure.X][posOfCurFigure.Y];
				   gameField[posOfCurFigure.X][posOfCurFigure.Y] = ' ';
			   }
			
			   clearGameField(movesField);
			   curFigure = ' ';
			   if (gameField[n][m] == 1) {
				   whiteKingPos.X = n;
				   whiteKingPos.Y = m;
			   }
			   if (gameField[n][m] == 'K') {
				   blackKingPos.X = n;
				   blackKingPos.Y = m;
			   }
		   } else {
			   cout << "You can not move figure there\n";
		   }
		   break;
	   case('F'):
	   case('f'):
		   n = 0;
		   m = 0;
		   break;
	   case('q'):
	   case('Q'):
		   for (int i = 0; i < SIZE; i++) {
			   for (int j = 0; j < SIZE; j++) {
				   if (gameField[i][j] == '\0') {
					   gameField[i][j] = ' ';
				   }
			   }
		   }
		   break;
	   defult:
		   cout << "Unknown input";
		   break;
	}
}

void saveKing() {
	switch (_getch()) {
	case('w'):
	case('W'):
		system("cls");
		if (n - 1 >= 0) {
			n--;
		}
		break;
	case('a'):
	case('A'):
		system("cls");
		if (m - 1 >= 0) {
			m--;
		}
		break;
	case('s'):
	case('S'):
		system("cls");
		if (n + 1 < SIZE) {
			n++;
		}
		break;
	case('d'):
	case('D'):
		system("cls");
		if (m + 1 < SIZE) {
			m++;
		}
		break;
	case('c'):
	case('C'):
		if (gameField[n][m] == ' ') {
			cout << "Cell is empty\n";
		}
		else if (curFigure == ' ') {
			if (determineTeam(gameField[n][m]) == 0 && turnOfWhite == true) {
				posOfCurFigure.X = n;
				posOfCurFigure.Y = m;
				curFigure = gameField[n][m];
				markPossibleMoves(gameField, movesField, posOfCurFigure, curFigure,'-');

				if (isNoPosMoves() == true) {
					cout << "No possible moves, chose another figure!";
					curFigure = ' ';
				}

			}
			else if (determineTeam(gameField[n][m]) == 1 && turnOfBlack == true) {
				posOfCurFigure.X = n;
				posOfCurFigure.Y = m;
				curFigure = gameField[n][m];
				markPossibleMoves(gameField, movesField, posOfCurFigure, curFigure,'-');
				if (isNoPosMoves() == true) {
					cout << "No possible moves, chose another figure";
					curFigure = ' ';
				}

			}
			else if (determineTeam(gameField[n][m]) == 1 && turnOfBlack == false || determineTeam(gameField[n][m]) == 0 && turnOfWhite == false) {
				cout << "Not your turn or figure";
			}
		}
		else {
			cout << "You already choose some figure";
		}
		break;
	case('p'):
	case('P'):
		if (movesField[n][m] == '-') {
			COORD oldKingpos;
			if (blackCheckMate()) {
				oldKingpos = blackKingPos;
			} else {
				oldKingpos = whiteKingPos;
			}
			char save = gameField[n][m];
			gameField[n][m] = gameField[posOfCurFigure.X][posOfCurFigure.Y];
			gameField[posOfCurFigure.X][posOfCurFigure.Y] = ' ';
			if (gameField[n][m] == 1) {
				whiteKingPos.X = n;
				whiteKingPos.Y = m;
			}
			if (gameField[n][m] == 'K') {
				blackKingPos.X = n;
				blackKingPos.Y = m;
			}
			if (blackCheckMate() || whiteCheckMate()) {
				gameField[posOfCurFigure.X][posOfCurFigure.Y] = gameField[n][m];
				gameField[n][m] = save;
				if (blackCheckMate()) {
					blackKingPos = oldKingpos;
				} else {
					whiteKingPos = oldKingpos;
				}
				cout << "Do another move!";
			
			} else {
				swap(turnOfBlack, turnOfWhite);
			}
			clearGameField(movesField);
			curFigure = ' ';
			if (gameField[n][m] == 1) {
				whiteKingPos.X = n;
				whiteKingPos.Y = m;
			}
			if (gameField[n][m] == 'K') {
				blackKingPos.X = n;
				blackKingPos.Y = m;
			}
			
		}
		else {
			cout << "You can not move figure there\n";
		}
		break;
	
	defult:
		cout << "Unknown input";
		break;
	}
	
}

int main() {
	whiteKingPos.X = 7;
	whiteKingPos.Y = 4;
	blackKingPos.X = 0;
	blackKingPos.Y = 4;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			gameField[i][j] = ' ';
			dangerFieldB[i][j] = ' ';
			dangerFieldW[i][j] = ' ';
		}
	}
	
	fillGameField(gameField);

	while (waitingForStart != 1) {
		playerCommands();
	}	
	system("cls");

	bool endOfGameW = false;
	bool endOfGameB = false;
	while (endOfGameW !=true && endOfGameB != true) {
		drawGameField();
		cout << "\n";
        for (int i = 0; i < SIZE; i++) {
	     for (int j = 0; j < SIZE; j++) {
		  cout << dangerFieldW[i][j];
	     }
	      cout << "\n";
        }
		playerInput();
		if (blackCheckMate()) {
			if (!turnOfBlack) {
				swap(turnOfBlack, turnOfWhite);
			}
				
			if (!isAnyPossibilitiesB()) {
				endOfGameB = true;
			} else {
				drawGameField();
				cout << "Black king is in danger\n";
				saveKing();
			}
		}
		if (whiteCheckMate()) {
			if (!turnOfWhite) {
				swap(turnOfBlack, turnOfWhite);
			}
		
			if (!isAnyPossibilitiesW()) {
				endOfGameW = true;
			} else {
				drawGameField();
				cout << "White king is in danger\n";
				saveKing();
			}
		}
	}

	system("cls");
	if (endOfGameB == true) {
		cout << "White team wins!";
	} else {
		cout << "Black team wins!";
	}
	return 322;
}
// Сделать другое придупреждение, если игрок пытается выбрать ещё одну фигуру, если какая-то фигура уже выбрана - !
// Если фигура заблокирована добавить предупреждение и взможность выбрать другую фигуру - !
// Пометка фигуры, которую может съесть игрок - !
// Выделить в отдельную функцию ход повторяющийся для белых и чёрных
// Добавить условие окончания игры -> исправить условия окончания игры - !
// Пофиксить канибализм среди чёрных пешек - !
// Добавить, что короля съесть нельзя-!
// Добавить рокировку
// Исправить баг дерьма!!
// Король не ест наискосок
/*cout << "\n";
for (int i = 0; i < SIZE; i++) {
	for (int j = 0; j < SIZE; j++) {
		cout << dangerFieldB[i][j];
	}
	cout << "\n";
}*/