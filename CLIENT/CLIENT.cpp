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

Menu RegMenu, UserMenu, AdminMenu;
Menu* MenuPtr = &RegMenu;

class User {
	bool isAdmin = false;
	bool isSmth = false;

public:
	void setReg(bool status, bool reg) {
		isAdmin = status;
		isSmth = reg;
	}
	bool GetStatus() {
		return isAdmin;
	}
	bool GetSmth() {
		return isSmth;
	}
};

User user;

void Registr() {
	string choose;
	cout << "Admin? ";
	cin >> choose;

	if (choose == "да") user.setReg(true, true);
	else user.setReg(false, true);
}

void Unreg() {
	user.setReg(false, false);
}


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

	RegMenu.CreateMenu(1, "Войти"); 
	{
		RegMenu.function[0] = Registr;
	}
	UserMenu.CreateMenu(2, "Пользователь", "Выйти");
	{
		UserMenu.function[1] = Unreg;
	}
	AdminMenu.CreateMenu(2, "Админ", "Выйти");
	{
		AdminMenu.function[1] = Unreg;
	}
	

	bool running = true;
	while (running) {
		if (!user.GetStatus() && user.GetSmth()) MenuPtr = &UserMenu;
		else if (!user.GetStatus() && !user.GetSmth()) MenuPtr = &RegMenu;
		else if (user.GetStatus() && user.GetSmth()) MenuPtr = &AdminMenu;

		MenuPtr->ShowMenu();
		MenuPtr->Navigation(&running);
	}

	closesocket(s);
	WSACleanup();
}