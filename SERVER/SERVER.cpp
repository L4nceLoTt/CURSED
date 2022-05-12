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

vector<Product> bak, kons, masl;

void Init() {
	vector<Product>::iterator ptr = bak.begin();
	ifstream Bak, Kons, Masl;
	Product tmp;

	Bak.open("Бакалея.txt", ios::in);
	while (Bak) {
		Bak >> tmp;
		bak.push_back(tmp);
	}
	bak.erase(bak.end() - 1);
	Bak.close();

	ptr = kons.begin();
	Kons.open("Консервация.txt", ios::in);
	while (Kons) {
		Kons >> tmp;
		kons.push_back(tmp);
	}
	kons.erase(kons.end() - 1);
	Kons.close();

	ptr = masl.begin();
	Masl.open("Маслауксусы.txt", ios::in);
	while (Masl) {
		Masl >> tmp;
		masl.push_back(tmp);
	}
	masl.erase(masl.end() - 1);
	Masl.close();
}
void Save() {
	vector<Product>::iterator ptr = bak.begin();
	ofstream Bak, Kons, Masl;

	Bak.open("Бакалея.txt", ios::out);
	for (; ptr != bak.end(); ptr++) {
		Bak << *ptr;
	}
	Bak.close();

	ptr = kons.begin();
	Kons.open("Консервация.txt", ios::out);
	for (; ptr != kons.end(); ptr++) {
		Kons << *ptr;
	}
	Kons.close();

	ptr = masl.begin();
	Masl.open("Маслауксусы.txt", ios::out);
	for (; ptr != masl.end(); ptr++) {
		Masl << *ptr;
	}
	Masl.close();
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
			if (!strcmp(buf, "1_1")) {
				*buf = '\0';
				string group, name, dealer;
				string code;
				string cost;
				recv(s2, buf, 100, 0); group = buf; *buf = '\0';
				recv(s2, buf, 100, 0); group = buf; *buf = '\0';
				recv(s2, buf, 100, 0); code = buf; *buf = '\0';
				recv(s2, buf, 100, 0); name = buf; *buf = '\0';
				recv(s2, buf, 100, 0); cost = buf; *buf = '\0';
				recv(s2, buf, 100, 0); dealer = buf; *buf = '\0';


				Product prod(group, code, name, cost, dealer);

				if (group == "Бакалея") bak.push_back(prod);
				else if (group == "Консервация") kons.push_back(prod);
				else if (group == "Масла,уксусы") masl.push_back(prod);
			}
			else if (!strcmp(buf, "1_2")) {
				vector<Product>::iterator ptr = bak.begin();

				for (; ptr != bak.end(); ptr++) {
					char group[100], name[100], cost[100], dealer[100], code[100];
					ptr->getFields_to_Admin(group, name, cost, code, dealer);
					send(s2, group, sizeof(group), 0);
					send(s2, name, sizeof(name), 0);
					send(s2, cost, sizeof(cost), 0);
					send(s2, code, sizeof(code), 0);
					send(s2, dealer, sizeof(dealer), 0);
				}

				ptr = kons.begin();

				for (; ptr != kons.end(); ptr++) {
					char group[100], name[100], cost[100], dealer[100], code[100];
					ptr->getFields_to_Admin(group, name, cost, code, dealer);
					send(s2, group, sizeof(group), 0);
					send(s2, name, sizeof(name), 0);
					send(s2, cost, sizeof(cost), 0);
					send(s2, code, sizeof(code), 0);
					send(s2, dealer, sizeof(dealer), 0);
				}

				ptr = masl.begin();

				for (; ptr != masl.end(); ptr++) {
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
		}
		else if (buf[0] == '2'){
			if (!strcmp(buf, "2_1")){
				vector<Product>::iterator ptr = bak.begin();

				for (; ptr != bak.end(); ptr++) {
					char group[100], name[100], cost[100];
					ptr->getFields_to_User(group, name, cost);
					send(s2, group, sizeof(group), 0);
					send(s2, name, sizeof(name), 0);
					send(s2, cost, sizeof(cost), 0);
				}

				ptr = kons.begin();

				for (; ptr != kons.end(); ptr++) {
					char group[100], name[100], cost[100], dealer[100], code[100];
					ptr->getFields_to_User(group, name, cost);
					send(s2, group, sizeof(group), 0);
					send(s2, name, sizeof(name), 0);
					send(s2, cost, sizeof(cost), 0);
				}

				ptr = masl.begin();

				for (; ptr != masl.end(); ptr++) {
					char group[100], name[100], cost[100], dealer[100], code[100];
					ptr->getFields_to_User(group, name, cost);
					send(s2, group, sizeof(group), 0);
					send(s2, name, sizeof(name), 0);
					send(s2, cost, sizeof(cost), 0);
				}
				send(s2, "0", sizeof("0"), 0);
			}
			else if (!strcmp(buf, "2_2")) {
				char str[100];
				recv(s2, str, sizeof(str), 0);
				recv(s2, str, sizeof(str), 0);

				vector<Product>::iterator ptr = bak.begin();

				for (; ptr != bak.end(); ptr++) {
					char group[100], name[100], cost[100];
					ptr->getFields_to_User(group, name, cost);
					if (strstr(group, str) || strstr(name, str)) {
						send(s2, group, sizeof(group), 0);
						send(s2, name, sizeof(name), 0);
						send(s2, cost, sizeof(cost), 0);
					}
				}

				ptr = kons.begin();

				for (; ptr != kons.end(); ptr++) {
					char group[100], name[100], cost[100];
					ptr->getFields_to_User(group, name, cost);
					if (strstr(group, str) || strstr(name, str)) {
						send(s2, group, sizeof(group), 0);
						send(s2, name, sizeof(name), 0);
						send(s2, cost, sizeof(cost), 0);
					}
				}

				ptr = masl.begin();

				for (; ptr != masl.end(); ptr++) {
					char group[100], name[100], cost[100], dealer[100], code[100];
					ptr->getFields_to_User(group, name, cost);
					if (strstr(group, str) || strstr(name, str)) {
						send(s2, group, sizeof(group), 0);
						send(s2, name, sizeof(name), 0);
						send(s2, cost, sizeof(cost), 0);
					}
				}
				send(s2, "0", sizeof("0"), 0);
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