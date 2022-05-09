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

int numcl = 0;
ifstream Afile;
ifstream Ufile;

DWORD WINAPI ThreadFunc(LPVOID client_socket)
{
	int num = numcl;
	SOCKET s2 = ((SOCKET*)client_socket)[0];
	char buf[100];
	string _log;
	while (recv(s2, buf, sizeof(buf), 0))
	{
		if (!strcmp(buf, "0_1")){
			bool exist = false;
			char lbuf[100], pbuf[100];
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
		else if (!strcmp(buf, "0_0")) cout << "client N-" << numcl << " unauthorised\n";
		
	}
	
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
	// цикл извлечения запросов на подключение из очереди
	while ((client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size))) {
		numcl++;
		print();
		// Вызов нового потока для обслуживания клиента
		DWORD thID;// thID идентификатор типа DWORD
		CreateThread(NULL, NULL, ThreadFunc, &client_socket, NULL, &thID);
	}
}