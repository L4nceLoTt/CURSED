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

void SendRequest(SOCKET soc, string id){
	char buf[100];
	strcpy(buf, id.c_str());
	send(soc, buf, sizeof(buf), 0);
	*buf = '\0';
	if (recv(soc, buf, sizeof(buf), 0) != 0)
	{
		system("cls");
		cout << buf;
		_getch();
	}
}

char UserAuth(SOCKET soc, string id)
{
	char buf[100];
	char lbuf[100];
	char pbuf[100];
	char boolbuf;

	strcpy(buf, id.c_str());
	
	system("cls");
	cout << "Логин: ";
	cin >> lbuf;
	cout << "Пароль: ";
	cin >> pbuf;

	send(soc, buf, sizeof(lbuf), 0);
	send(soc, lbuf, sizeof(lbuf), 0);
	send(soc, pbuf, sizeof(lbuf), 0);

	*buf = '\0';
	*lbuf = '\0';
	*pbuf = '\0';
	if (recv(soc, buf, sizeof(buf), 0) != 0)
	{
		boolbuf = buf[0];
		//recv(soc, boolbuf, sizeof(boolbuf), 0);
		system("cls");
		cout << buf;
		_getch();
	}
	return boolbuf;
}

char AdminAuth(SOCKET soc, string id)
{
	char buf[100];
	char lbuf[100];
	char pbuf[100];
	char boolbuf;

	strcpy(buf, id.c_str());

	system("cls");
	cout << "Логин: ";
	cin >> lbuf;
	cout << "Пароль: ";
	cin >> pbuf;

	send(soc, buf, sizeof(lbuf), 0);
	send(soc, lbuf, sizeof(lbuf), 0);
	send(soc, pbuf, sizeof(lbuf), 0);

	*buf = '\0';
	*lbuf = '\0';
	*pbuf = '\0';
	if (recv(soc, buf, sizeof(buf), 0) != 0)
	{
		boolbuf = buf[0];
		system("cls");
		cout << buf;
		_getch();
	}
	return boolbuf;
}

void main() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	char boolbuf;
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


	menu.CreateMenu(2, "Пользователь", "Админ");
	{
		//menu.sub[0].CreateMenu(5, "Просмотреть", "Изменить", "Удалить", "Отфильтровать", "Сортировать");
		//{
		//	menu.sub[0].sub[3].CreateMenu(2, "какой-то", "фильтр");
		//	menu.sub[0].sub[4].CreateMenu(2, "ну", "сортировка");
		//}
		//menu.sub[0].CreateMenu(5, "Просмотреть", "Изменить", "Удалить", "Отфильтровать", "Сортировать");
		//{
		//	menu.sub[0].sub[3].CreateMenu(2, "какой-то", "фильтр");
		//	menu.sub[0].sub[4].CreateMenu(2, "ну", "сортировка");
		//}
	}
	

	bool running = true;
	while (running) {
		menu.ShowMenu();
		menu.Navigation(&running);
		if (menu.currentID == "1") {
			boolbuf = UserAuth(s, menu.currentID);
		}
		if (menu.currentID == "2") {
			AdminAuth(s, menu.currentID);
		}
		if (menu.currentID == "11") {
			SendRequest(s, menu.currentID);
		}
		if (menu.currentID == "12") {
			SendRequest(s, menu.currentID);
		}
		if (menu.currentID == "13") {
			SendRequest(s, menu.currentID);
		}
		if (menu.currentID == "141") {
			SendRequest(s, menu.currentID);
		}
		if (menu.currentID == "142") {
			SendRequest(s, menu.currentID);
		}
		if (menu.currentID == "151") {
			SendRequest(s, menu.currentID);
		}
		if (menu.currentID == "152") {
			SendRequest(s, menu.currentID);
		}
	}

	closesocket(s);
	WSACleanup();
}