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
int counter = 0;
float TotCost = 0;

void Replace(char* buf, char replace, char to) {
	for (int i = 0; i < 60; i++) {
		if (buf[i] == replace) buf[i] = to;
	}
}

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
	}
}
void Unreg() {
	user.setReg(false, false);
	unreg = true;
}
void NewAcc(SOCKET soc){

}

void SendAdd(SOCKET soc) {
	Menu menu;
	char buf[100];
	menu.SetHeader("Группа товара");

	menu.CreateMenu(3, "Бакалея", "Консервация", "Масла,уксусы");

	bool running = true;
	while (running) {
		menu.ShowMenu();
		menu.Navigation(&running);

		if (menu.currentID == "1") {
			system("cls");
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
			system("cls");
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
			system("cls");
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

void AdminSendShow(SOCKET soc) {
	system("cls");
	cout << "+--------+---------------+--------------------------------------------------+-----+---------------+\n";
	cout << "|   Код  |     Группа    |                     Название                     | Цена|    Постащик   |\n";
	cout << "+--------+---------------+--------------------------------------------------+-----+---------------+\n";
	char group[100], name[100], cost[100], code[100], dealer[100];
	while (true) {
		recv(soc, group, sizeof(group), 0);
		if (strcmp(group, "0")) {
			recv(soc, name, sizeof(name), 0);
			Replace(name, '_', ' ');
			recv(soc, cost, sizeof(cost), 0);
			recv(soc, code, sizeof(code), 0);
			recv(soc, dealer, sizeof(dealer), 0);
			printf("|%8s|%15s|%50s|%5s|%15s|\n", code, group, name, cost, dealer);
			cout << "+--------+---------------+--------------------------------------------------+-----+---------------+\n";
		}
		else break;
	}
}
void UserSendShow(SOCKET soc) {
	system("cls");
	cout << "+----+---------------+--------------------------------------------------+-----+----------------+\n";
	cout << "|  № |     Группа    |                     Название                     | Цена|     Кол-во     +\n";
	cout << "+----+---------------+--------------------------------------------------+-----+----------------+\n";
	char group[100], name[100], cost[100], state[100];
	counter = 0;
	while (true) {
		recv(soc, group, sizeof(group), 0);
		if (strcmp(group, "0")) {
			recv(soc, name, sizeof(name), 0);
			Replace(name, '_', ' ');
			recv(soc, cost, sizeof(cost), 0);
			recv(soc, state, sizeof(state), 0);
			printf("|%4d|%15s|%50s|%5s|%16s|\n", counter + 1, group, name, cost, state);
			cout << "+----+---------------+--------------------------------------------------+-----+----------------+\n";
			counter++;
		}
		else break;
	}
}
void AdminSendShowWarehouse(SOCKET soc) {
	Menu getCity;

	getCity.CreateMenu(6, "Минск", "Витебск", "Брест", "Гродно", "Гомель", "Могилёв");

	bool running = true;
	while (running) {
		getCity.ShowMenu();
		getCity.Navigation(&running);

		if (getCity.currentID == "1") {
			send(soc, "Минск", sizeof("Минск"), 0);
			running = false;
		}
		else if (getCity.currentID == "2") {
			send(soc, "Витебск", sizeof("Витебск"), 0);
			running = false;
		}
		else if (getCity.currentID == "3") {
			send(soc, "Брест", sizeof("Брест"), 0);
			running = false;
		}
		else if (getCity.currentID == "4") {
			send(soc, "Гродно", sizeof("Гродно"), 0);
			running = false;
		}
		else if (getCity.currentID == "5") {
			send(soc, "Гомель", sizeof("Гомель"), 0);
			running = false;
		}
		else if (getCity.currentID == "6") {
			send(soc, "Могилёв", sizeof("Могилёв"), 0);
			running = false;
		}
	}


	system("cls");
	cout << "+----+--------+---------------+--------------------------------------------------+-----+---------------+------+\n";
	cout << "|  № |   Код  |     Группа    |                     Название                     | Цена|    Постащик   |Кол-во|\n";
	cout << "+----+--------+---------------+--------------------------------------------------+-----+---------------+------+\n";
	char group[100], name[100], cost[100], code[100], dealer[100], amount[100];
	counter = 0;
	while (true) {
		recv(soc, group, sizeof(group), 0);
		if (strcmp(group, "0")) {
			recv(soc, name, sizeof(name), 0);
			Replace(name, '_', ' ');
			recv(soc, cost, sizeof(cost), 0);
			recv(soc, code, sizeof(code), 0);
			recv(soc, dealer, sizeof(dealer), 0);
			recv(soc, amount, sizeof(amount), 0);
			printf("|%4d|%8s|%15s|%50s|%5s|%15s|%6s|\n", counter + 1, code, group, name, cost, dealer, amount);
			cout << "+----+--------+---------------+--------------------------------------------------+-----+---------------+------+\n";
			counter++;
		}
		else break;
	}
}

void WarehouseOrder(SOCKET soc) {
	cout << "\n\nВыберите номер позиции: ";
	int choise = 0, amount_ = 0;
	char buf[100], amount[100];
	cin >> choise;
	if (choise < 0 || choise > counter) {
		system("cls");
		cout << "Такого номера нет!";
	}
	else {
		choise--;
		cout << "Введите кол-во: ";
		cin >> amount_;
		_itoa_s(choise, buf, sizeof(buf), 10);
		_itoa_s(amount_, amount, sizeof(amount), 10);
	}
	send(soc, buf, sizeof(buf), 0);
	send(soc, amount, sizeof(amount), 0);
}

void SendSearch(SOCKET soc) {
	system("cls");
	bool exist = false;
	char str[100];
	cout << "Поиск: ";
	cin >> str;
	send(soc, str, sizeof(str), 0);
	system("cls");
	cout << "+---------------+--------------------------------------------------+-----+----------------+\n";
	cout << "|     Группа    |                     Название                     | Цена|     Наличие    +\n";
	cout << "+---------------+--------------------------------------------------+-----+----------------+\n";
	char group[100], name[100], cost[100], state[100];
	while (true) {

		recv(soc, group, sizeof(group), 0);

		if (group[0] != '\0') {
			exist = true;
			recv(soc, name, sizeof(name), 0);
			Replace(name, '_', ' ');
			recv(soc, cost, sizeof(cost), 0);
			recv(soc, state, sizeof(state), 0);
			printf("|%15s|%50s|%5s|%16s|\n", group, name, cost, state);
			cout << "+---------------+--------------------------------------------------+-----+----------------+\n";
			*group = '\0';
		}
		else break;
	}
	if (exist == false) {
		printf("|                                Совпадений не найдено                                    |\n");
		printf("+-----------------------------------------------------------------------------------------+\n");
	}
}

void SendCart(SOCKET soc) {
	int choise, amount;
	char _choise[100], _amount[100];
	cout << "\n\nВыберите товар: ";
	cin >> choise;
	if (choise < 0 || choise > counter) {
		system("cls");
		cout << "Такого номера нет!";
	}
	else {
		choise--;
		cout << "Введите кол-во: ";
		cin >> amount;
		_itoa_s(choise, _choise, sizeof(_choise), 10);
		_itoa_s(amount, _amount, sizeof(_amount), 10);
	}
	send(soc, _choise, sizeof(_choise), 0);
	send(soc, _amount, sizeof(_amount), 0);
}
void ShowCart(SOCKET soc) {
	system("cls");
	cout << "+----+---------------+--------------------------------------------------+-----+----------------+\n";
	cout << "|  № |     Группа    |                     Название                     | Цена|     Кол-во     +\n";
	cout << "+----+---------------+--------------------------------------------------+-----+----------------+\n";
	char group[100], name[100], cost[100], state[100];
	counter = 0;
	while (true) {
		recv(soc, group, sizeof(group), 0);
		if (group[0] != '\0') {
			recv(soc, name, sizeof(name), 0);
			Replace(name, '_', ' ');
			recv(soc, cost, sizeof(cost), 0);
			recv(soc, state, sizeof(state), 0);
			TotCost += (float)strtod(cost, NULL) * atoi(state);
			printf("|%4d|%15s|%50s|%5s|%16s|\n", counter + 1, group, name, cost, state);
			cout << "+----+---------------+--------------------------------------------------+-----+----------------+\n";
			counter++;
		}
		else break;
	}
}
void DeleteCart(SOCKET soc) {
	int choise;
	char _choise[100];
	cout << "\n\nВыберите товар: ";
	cin >> choise;
	if (choise < 0 || choise > counter) {
		system("cls");
		cout << "Такого номера нет!";
	}
	else {
		choise--;
		_itoa_s(choise, _choise, sizeof(_choise), 10);
	}
	send(soc, _choise, sizeof(_choise), 0);
}
void PayCart(SOCKET soc) {
	printf("%72c|  К оплате  |%9.2f|\n", ' ', TotCost);
	cout << "                                                                        +------------+---------+\n";
	TotCost = 0;
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

	RegMenu.CreateMenu(2, "Вход", "Регистрация");
	UserMenu.CreateMenu(4, "Показать товары", "Поиск", "Корзина", "Выйти из уч.з.");
	{
		UserMenu.sub[2].CreateMenu(4, "Посмотреть", "Добавить", "Удалить", "Оплатить");
		UserMenu.function[3] = Unreg;
	}
	AdminMenu.CreateMenu(4, "Добавить наим-ние", "Показать наим-ния", "Склад", "Выйти из уч.з.");
	{
		AdminMenu.sub[2].CreateMenu(2, "Показать товары", "Заказать товар");
		AdminMenu.function[3] = Unreg;
	}
	

	bool running = true;
	while (running) {
		if (!user.GetStatus() && user.GetSmth()) MenuPtr = &UserMenu;
		else if (!user.GetStatus() && !user.GetSmth()) MenuPtr = &RegMenu;
		else if (user.GetStatus() && user.GetSmth()) MenuPtr = &AdminMenu;

		MenuPtr->currentID = "0";
		MenuPtr->ShowMenu();
		MenuPtr->Navigation(&running);

		if (!user.GetStatus() && !user.GetSmth()) {					//Unreg user or admin
			if (MenuPtr->currentID == "1") {
				SendRequest(s, "0_" + MenuPtr->currentID);
				SendReg(s);
				_getch();
			}
			if (MenuPtr->currentID == "2") {
				SendRequest(s, "0_" + MenuPtr->currentID);
				SendReg(s);
				_getch();
			}
		}
		else if (user.GetStatus() && user.GetSmth()) {             //Admin 
			if (MenuPtr->currentID == "1") {
				SendRequest(s, "1_" + MenuPtr->currentID);
				SendAdd(s);
				_getch();
			}
			else if (MenuPtr->currentID == "2") {
				SendRequest(s, "1_" + MenuPtr->currentID);
				AdminSendShow(s);
				_getch();
			}
			else if (MenuPtr->currentID == "31") {
				SendRequest(s, "1_" + MenuPtr->currentID);
				AdminSendShowWarehouse(s);
				_getch();
			}
			else if (MenuPtr->currentID == "32") {
				SendRequest(s, "1_" + MenuPtr->currentID);
				AdminSendShowWarehouse(s);
				WarehouseOrder(s);
				_getch();
			}
		}
		else if (!user.GetStatus() && user.GetSmth()) {             //User 
			if (MenuPtr->currentID == "1") {
				SendRequest(s, "2_" + MenuPtr->currentID);
				UserSendShow(s);
				_getch();
			}
			else if (MenuPtr->currentID == "2") {
				SendRequest(s, "2_" + MenuPtr->currentID);
				SendSearch(s);
				_getch();
			}
			else if (MenuPtr->currentID == "31") {
				SendRequest(s, "2_" + MenuPtr->currentID);
				ShowCart(s);
				_getch();
			}
			else if (MenuPtr->currentID == "32") {
				SendRequest(s, "2_" + MenuPtr->currentID);
				UserSendShow(s);
				SendCart(s);
			}
			else if (MenuPtr->currentID == "33") {
				SendRequest(s, "2_" + MenuPtr->currentID);
				ShowCart(s);
				DeleteCart(s);
			}
			else if (MenuPtr->currentID == "34") {
				SendRequest(s, "2_" + MenuPtr->currentID);
				ShowCart(s);
				PayCart(s);
				_getch();
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