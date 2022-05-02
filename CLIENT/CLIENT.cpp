#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <iostream>
#include <cstring>
#include <string>
#include <string.h>
#include <Windows.h>
#include "Menu.h"
#include "Libs.h"

#pragma warning(disable : 4996)

using namespace std;


void main() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) { return; }
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	// указание адреса и порта сервера
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(1280);
	dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(s, (sockaddr*)&dest_addr, sizeof(dest_addr));
	
	Menu menu;

	menu.CreateMenu(3, "Соси", "Залупу", "Ленина");
	{

	}

	bool running = true;
	while (running) {
		menu.ShowMenu();
		menu.Navigation(&running);
		if (menu.currentID == "1") {
			char* buf = _strdup(menu.currentID.c_str());
			send(s, buf, 100, 0);
			*buf = '\0';

			if (recv(s, buf, sizeof(buf), 0) != 0)
			{
				system("cls");
				cout << buf;
				_getch();
			}
		}
	}

	closesocket(s);
	WSACleanup();
}