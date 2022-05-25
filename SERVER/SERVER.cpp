#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <iostream>
#include <cstring>
#include <string>
#include <string.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "Classes.h"
#include "Libs.h"

#pragma warning(disable : 4996)

using namespace std;

int numcl = 0;
ifstream Afile;
ifstream Ufile;

vector<Product> arr;
vector<Product> cart;

map<string, vector<Product>> warehouses;
vector<string> cities = { "Минск", "Витебск", "Брест", "Гродно", "Гомель", "Могилёв" };

char city[100];

void Init() {
	vector<Product>::iterator ptr = arr.begin();
	ifstream Bak, Kons, Masl;
	Product tmp;

	Bak.open("Бакалея.txt", ios::in);
	while (Bak) {
		Bak >> tmp;
		arr.push_back(tmp);
	}
	if (Bak.eof()) arr.erase(arr.end() - 1);
	Bak.close();

	Kons.open("Консервация.txt", ios::in);
	while (Kons) {
		Kons >> tmp;
		arr.push_back(tmp);
	}
	if (Kons.eof()) arr.erase(arr.end() - 1);
	Kons.close();

	Masl.open("Маслауксусы.txt", ios::in);
	while (Masl) {
		Masl >> tmp;
		arr.push_back(tmp);
	}
	if (Masl.eof()) arr.erase(arr.end() - 1);
	Masl.close();

	for (vector<string>::iterator _ptr = cities.begin(); _ptr != cities.end(); _ptr++) {
		vector<Product> _tmp_;
		Product _tmp;

		ifstream f;
		string filename = *_ptr;
		filename += ".txt";
		f.open(filename.c_str(), ios::in);

		while (f) {
			f >> _tmp;
			_tmp_.push_back(_tmp);
		}
		if (!_tmp_.empty()) _tmp_.erase(_tmp_.end() - 1);

		warehouses.insert(pair<string, vector<Product>>(*_ptr, _tmp_));

		f.close();
	}
}
void Save() {
	vector<Product>::iterator ptr = arr.begin();
	ofstream Bak, Kons, Masl;

	Bak.open("Бакалея.txt", ios::out);
	Kons.open("Консервация.txt", ios::out);
	Masl.open("Маслауксусы.txt", ios::out);
	for (; ptr != arr.end(); ptr++) {
		if (ptr->GetGroup() == "Бакалея") Bak << *ptr;
		else if (ptr->GetGroup() == "Консервация") Kons << *ptr;
		else if (ptr->GetGroup() == "Масла,уксусы") Masl << *ptr;
	}
	Bak.close();
	Kons.close();
	Masl.close();

	for (vector<string>::iterator ptr = cities.begin(); ptr != cities.end(); ptr++) {
		map<string, vector<Product>>::iterator point = warehouses.find(*ptr);
		string filename = *ptr;
		filename += ".txt";

		ofstream f;
		f.open(filename.c_str(), ios::out);
		for (vector<Product>::iterator i = point->second.begin(); i != point->second.end(); i++) {
			f << *i;
		}
		f.close();
	}
}

void AddProd(SOCKET s2, char* buf) {
	*buf = '\0';
	string group, name, dealer, code, cost, amount;
	recv(s2, buf, 100, 0); group = buf; *buf = '\0';
	recv(s2, buf, 100, 0); group = buf; *buf = '\0';
	recv(s2, buf, 100, 0); code = buf; *buf = '\0';
	recv(s2, buf, 100, 0); name = buf; *buf = '\0';
	recv(s2, buf, 100, 0); cost = buf; *buf = '\0';
	recv(s2, buf, 100, 0); dealer = buf; *buf = '\0';

	arr.push_back(Product(group, code, name, cost, dealer, "0"));

	for (vector<string>::iterator i = cities.begin(); i != cities.end(); i++) {
		map<string, vector<Product>>::iterator ptr = warehouses.find(*i);
		ptr->second.push_back(arr[arr.size() - 1]);
	}
}

void ShowProdAdmin(SOCKET s2, char* buf) {
	vector<Product>::iterator ptr = arr.begin();

	for (; ptr != arr.end(); ptr++) {
		char group[100], name[100], cost[100], dealer[100], code[100];
		ptr->getFields_to_Admin(group, name, cost, code, dealer);
		send(s2, group, sizeof(group), 0);
		send(s2, name, sizeof(name), 0);
		send(s2, cost, sizeof(cost), 0);
		send(s2, code, sizeof(code), 0);
		send(s2, dealer, sizeof(dealer), 0);
	}
	send(s2, "0", sizeof("0"), 0);
}
void ShowProdUser(SOCKET s2, char* buf) {
	vector<Product>::iterator ptr = arr.begin();

	for (; ptr != arr.end(); ptr++) {
		char group[100], name[100], cost[100], state[100];
		ptr->getFields_to_User(group, name, cost);
		send(s2, group, sizeof(group), 0);
		send(s2, name, sizeof(name), 0);
		send(s2, cost, sizeof(cost), 0);
		if (ptr->isExist()) {
			strcpy(state, "Есть в наличии");
		}
		else if (!ptr->isExist()) {
			strcpy(state, "Нет в наличии");
		}
		send(s2, state, sizeof(state), 0);
	}
	send(s2, "0", sizeof("0"), 0);
}
void ShowProdWarehouse(SOCKET s2, char* buf) {
	vector<Product>::iterator ptr = arr.begin();

	recv(s2, city, 100, 0);
	recv(s2, city, 100, 0);

	map<string, vector<Product>>::iterator i = warehouses.find(city);

	for (vector<Product>::iterator ptr = i->second.begin(); ptr != i->second.end(); ptr++) {
		char group[100], name[100], cost[100], dealer[100], code[100], amount[100];
		ptr->getFields_to_Warehouse(group, name, cost, code, dealer, amount);
		send(s2, group, sizeof(group), 0);
		send(s2, name, sizeof(name), 0);
		send(s2, cost, sizeof(cost), 0);
		send(s2, code, sizeof(code), 0);
		send(s2, dealer, sizeof(dealer), 0);
		send(s2, amount, sizeof(amount), 0);
	}
	send(s2, "0", sizeof("0"), 0);
}

void SearchProd(SOCKET s2, char* buf) {
	char str[100];
	recv(s2, str, sizeof(str), 0);
	recv(s2, str, sizeof(str), 0);

	vector<Product>::iterator ptr = arr.begin();

	for (; ptr != arr.end(); ptr++) {
		char group[100], name[100], cost[100], state[100];
		ptr->getFields_to_User(group, name, cost);
		if (strstr(group, str) || strstr(name, str) || strstr(cost, str)) {
			send(s2, group, sizeof(group), 0);
			send(s2, name, sizeof(name), 0);
			send(s2, cost, sizeof(cost), 0);
			if (ptr->isExist()) {
				strcpy(state, "Есть в наличии");
			}
			else if (!ptr->isExist()) {
				strcpy(state, "Нет в наличии");
			}
			send(s2, state, sizeof(state), 0);
		}
	}
	send(s2, "\0", sizeof("\0"), 0);
}

void AdminOrder(SOCKET s2){
	map<string, vector<Product>>::iterator i = warehouses.find(city);

	char buf[100], amount[100];
	recv(s2, buf, sizeof(buf), 0);
	recv(s2, amount, sizeof(amount), 0);
	int ind = atoi(buf);
	if (!(ind < 0 || ind > i->second.size())) i->second[ind].AddAmount(amount);
}

void AddToCart(SOCKET s2) {
	char buf[100], amount[100];
	recv(s2, buf, sizeof(buf), 0);
	recv(s2, buf, sizeof(buf), 0);
	recv(s2, amount, sizeof(amount), 0);
	int i = atoi(buf);
	if (!(i < 0 || i > arr.size())) {
		cart.push_back(arr[i]);
		cart[cart.size() - 1].SetAmount(amount);
	}
}
void ShowCart(SOCKET s2) {
	vector<Product>::iterator ptr = cart.begin();

	for (; ptr != cart.end(); ptr++) {
		char group[100], name[100], cost[100], state[100];
		ptr->getFields_to_User(group, name, cost);
		send(s2, group, sizeof(group), 0);
		send(s2, name, sizeof(name), 0);
		send(s2, cost, sizeof(cost), 0);
		strcpy(state, ptr->GetAmount().c_str());
		send(s2, state, sizeof(state), 0);
	}
	send(s2, "\0", sizeof("\0"), 0);
}
void DeleteCart(SOCKET s2) {
	char buf[100];
	recv(s2, buf, sizeof(buf), 0);
	recv(s2, buf, sizeof(buf), 0);
	int i = atoi(buf);
	if (!(i < 0 || i > arr.size())) {
		cart.erase(cart.begin() + i);
	}
}

DWORD WINAPI ThreadFunc(LPVOID client_socket)
{
	Init();

	int num = numcl;
	SOCKET s2 = ((SOCKET*)client_socket)[0];
	char buf[50];
	string _log;
	while (recv(s2, buf, sizeof(buf), 0))
	{
		if (buf[0] == '0') 
		{
			if (!strcmp(buf, "0_0")) cout << "client N-" << numcl << " unauthorised\n";
			else if (!strcmp(buf, "0_1")) {
				bool exist = false;
				char lbuf[100], pbuf[100];
				recv(s2, lbuf, 100, 0);
				recv(s2, lbuf, 100, 0);
				recv(s2, pbuf, 100, 0);

				while (!exist) {
					string log, pass;
					Afile.open("admin.txt", ios::in);
					while (Afile) {
						Afile >> log;
						Afile >> pass;

						if (!strcmp(log.c_str(), lbuf) && !strcmp(pass.c_str(), pbuf)) {
							send(s2, "11", sizeof("11"), 0);
							_log = log;
							cout << "client N-" << num << " authorised like " << "\"" << log << "\"\n" << "";
							exist = true;
							Afile.close();
							break;
						}
					}
					Afile.close();

					if (exist == false) {
						Ufile.open("user.txt", ios::in);
						while (Ufile) {
							Ufile >> log;
							Ufile >> pass;

							if (!strcmp(log.c_str(), lbuf) && !strcmp(pass.c_str(), pbuf)) {
								send(s2, "21", sizeof("21"), 0);
								_log = log;
								cout << "client N-" << num << " authorised like " << "\"" << log << "\"\n" << "";
								exist = true;
								Ufile.close();
								break;
							}
						}
						Ufile.close();
					}

					if (exist == false) {
						send(s2, "00", sizeof("00"), 0);
						cout << "client N-" << num << " failed his authorisation\n";
						exist = true;
					}
				}
			}
		}
		else if (buf[0] == '1') 
		{
			if (!strcmp(buf, "1_1")) AddProd(s2, buf);
			else if (!strcmp(buf, "1_2")) ShowProdAdmin(s2, buf);
			else if (!strcmp(buf, "1_31")) ShowProdWarehouse(s2, buf);
			else if (!strcmp(buf, "1_32")) {
				ShowProdWarehouse(s2, buf);
				AdminOrder(s2);
			}
		}
		else if (buf[0] == '2'){
			if (!strcmp(buf, "2_1")) ShowProdUser(s2, buf);
			else if (!strcmp(buf, "2_2")) SearchProd(s2, buf);
			else if (!strcmp(buf, "2_31")) ShowCart(s2);
			else if (!strcmp(buf, "2_32")) {
				ShowProdUser(s2, buf);
				AddToCart(s2);
			}
			else if (!strcmp(buf, "2_33")) {
				ShowCart(s2);
				DeleteCart(s2);
			}
			else if (!strcmp(buf, "2_34")) {
				ShowCart(s2);
			}
		}
		*buf = '\0';
	}
	
	Save();

	closesocket(s2);
	cout << "client N-" << num << " disconnected\n";

	return 0;
}

void print()
{
	if (numcl) cout << "client N-" << numcl << " connected\n";
	else printf("No clients connected\n");
}

void main() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) { return; }

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(1280);
	local_addr.sin_addr.s_addr = 0;
	bind(s, (sockaddr*)&local_addr, sizeof(local_addr));
	int c = listen(s, 5);
	cout << "Server receive ready" << endl;
	cout << endl;
	SOCKET client_socket;
	sockaddr_in client_addr;
	int client_addr_size = sizeof(client_addr);
	while ((client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size))) {
		numcl++;
		print();
		DWORD thID;
		CreateThread(NULL, NULL, ThreadFunc, &client_socket, NULL, &thID);
	}
}