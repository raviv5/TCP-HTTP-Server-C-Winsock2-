#ifndef main_H
#define main_H
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <string.h>
#include <time.h>

#include "Response.h"
#include "Request.h"
#define SIZE 4096

struct SocketState
{
	SOCKET id;			// Socket handle
	int	recv;			// Receiving?
	int	send;			// Sending?
	char buffer[SIZE];  //4096 bytes
	int len;
	time_t activeLastIn;
};

const int TIME_PORT = 27015;
const int MAX_SOCKETS = 60;
const int EMPTY = 0;
const int LISTEN = 1;
const int RECEIVE = 2;
const int IDLE = 3;
const int SEND = 4;

bool addSocket(SOCKET id, int what);
void removeSocket(int index);
void acceptConnection(int index);
void receiveMessage(int index);
void sendMessage(int index);
void run();

struct SocketState sockets[MAX_SOCKETS] = { 0 }; //globel
int socketsCount = 0;
#endif // !main_H

