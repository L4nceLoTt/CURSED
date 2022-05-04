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

#pragma warning(disable : 4996)

using namespace std;

DWORD WINAPI ThreadFunc(LPVOID client_socket)
{
	SOCKET s2 = ((SOCKET*)client_socket)[0];
	char buf[100];
	while (recv(s2, buf, sizeof(buf), 0))
	{
		if (!strcmp(buf, "0_1")){
			bool exist = false;
			char lbuf[100], pbuf[100];
			recv(s2, lbuf, 100, 0);
			recv(s2, pbuf, 100, 0);

			ifstream Afile; Afile.open("admin.txt", ios::in);
			ifstream Ufile; Ufile.open("user.txt", ios::in);

			while (!exist) {
				string log, pass;
				while (Afile) {
					Afile >> log;
					Afile >> pass;

					if (!strcmp(log.c_str(), lbuf) && !strcmp(pass.c_str(), pbuf)) {
						send(s2, "11", sizeof("11"), 0);
						exist = true;
						Afile.close();
						break;
					}
				}

				if (exist == false) {
					while (Ufile) {
						Ufile >> log;
						Ufile >> pass;

						if (!strcmp(log.c_str(), lbuf) && !strcmp(pass.c_str(), pbuf)) {
							send(s2, "21", sizeof("21"), 0);
							exist = true;
							Ufile.close();
							break;
						}
					}
				}

				if (exist == false) {
					send(s2, "00", sizeof("00"), 0);
					exist = true;
				}
			}
		}
		
	}
	
	closesocket(s2);
	return 0;
}

int numcl = 0;

void print()
{
	if (numcl) printf("%d client connected\n", numcl);
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
	// извлекаем сообщение из очереди
	SOCKET client_socket;    // сокет для клиента
	sockaddr_in client_addr; // адрес клиента (заполняется системой)
	int client_addr_size = sizeof(client_addr);
	// цикл извлечения запросов на подключение из очереди
	while ((client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size))) {
		numcl++;
		print();
		// Вызов нового потока для обслуживания клиента
		DWORD thID;// thID идентификатор типа DWORD
		CreateThread(NULL, NULL, ThreadFunc, &client_socket, NULL, &thID);
	}
}