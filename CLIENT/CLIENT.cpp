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
bool unreg = false;

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

void SendRequest(SOCKET soc, string id) {
	char buf[100];
	strcpy(buf, id.c_str());
	send(soc, buf, sizeof(buf), 0);
}

void SendReg(SOCKET soc) {
	string log, pass;

	system("cls");

	cout << "Введите логин: ";
	cin >> log;
	cout << "Введите пароль: ";
	cin >> pass;

	char lbuf[100];
	char pbuf[100];
	char tfbuf[100];

	strcpy(lbuf, log.c_str());
	strcpy(pbuf, pass.c_str());

	send(soc, lbuf, sizeof(lbuf), 0);
	send(soc, pbuf, sizeof(pbuf), 0);

	recv(soc, tfbuf, sizeof(tfbuf), 0);

	if (!strcmp(tfbuf, "11")) user.setReg(true, true);
	else if (!strcmp(tfbuf, "21")) user.setReg(false, true);
	else {
		system("cls");
		cout << "Ошибка авторизации";
		_getch();
	}
}
void Unreg() {
	user.setReg(false, false);
	unreg = true;
}

void SendAdd(SOCKET soc) {
	Menu menu;
	char buf[100];
	menu.SetHeader("Группа товара");

	menu.CreateMenu(3, "Бакалея", "Консервация", "Масла,уксусы")

	bool running = true;
	while (running) {
		menu.ShowMenu();
		menu.Navigation(&running);
		system("cls");
		if (menu.currentID == "1") {
			send(soc, "Бакалея", sizeof("Бакалея"), 0);
			cout << "Введите код товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';
			cout << "Введите название товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';
			cout << "Введите стоимость товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';
			cout << "Введите поставщика товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';

			running = false;
		}
		else if (menu.currentID == "2") {
			send(soc, "Консервация", sizeof("Консервация"), 0);
			cout << "Введите код товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';
			cout << "Введите название товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';
			cout << "Введите стоимость товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';
			cout << "Введите поставщика товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';

			running = false;
		}
		else if (menu.currentID == "3") {
			send(soc, "Масла,уксусы", sizeof("Масла,уксусы"), 0);
			cout << "Введите код товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';
			cout << "Введите название товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';
			cout << "Введите стоимость товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';
			cout << "Введите поставщика товара: ";
			cin >> buf;
			send(soc, buf, 100, 0); *buf = '\0';

			running = false;
		}
	}
}


void main() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) { return; }
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(1280);
	dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(s, (sockaddr*)&dest_addr, sizeof(dest_addr));

	RegMenu.CreateMenu(1, "Войти"); 
	UserMenu.CreateMenu(2, "Пользователь", "Выйти из уч.з.");
	{
		UserMenu.function[1] = Unreg;
	}
	AdminMenu.CreateMenu(2, "Добавить товар", "Выйти из уч.з.");
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

		if (!user.GetStatus() && !user.GetSmth()) {
			if (MenuPtr->currentID == "1") {
				SendRequest(s, "0_" + MenuPtr->currentID);
				SendReg(s);
			}
		}
		else if (user.GetStatus() && user.GetSmth()) {
			if (MenuPtr->currentID == "1") {
				SendRequest(s, "1_" + MenuPtr->currentID);

			}
		}
		if (unreg == true) {
			SendRequest(s, "0_0");
			unreg = false;
		}
	}

	closesocket(s);
	WSACleanup();
}