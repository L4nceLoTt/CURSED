#include "Libs.h"

void SetColor(int text, int bg) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

class Menu {
	int bg = 2;
	int countParam;
	int paragraph;
	int border;

	typedef void (*voidFunctionPtr)();

	Menu* main = this, * parent = NULL;
	string* arr;
	string header;
	string id;

public:
	string currentID = "0";
	Menu* sub;
	voidFunctionPtr* function = NULL;

	Menu() { countParam = 0; paragraph = 1; arr = NULL; border = 0; sub = NULL; id = "0"; }

 	string MakeID(Menu* ptr) {
		if (ptr->parent != NULL) {
			return MakeID(ptr->parent) + ptr->id;
		}
		return "\0";
	}

	void CreateMenu(int num, const char* name, ...) {
		if (main->parent == NULL) main->header = "Главное меню";

		main->border = num;
		va_list args;
		va_start(args, num);

		main->function = new voidFunctionPtr[num];
		for (int i = 0; i < num; i++) {
			main->function[i] = NULL;
		}
		main->arr = new string[num];
		main->sub = new Menu[num];
		for (int i = 0; i < num; i++) {
			main->sub[i].parent = main;
			main->sub[i].id = to_string(i + 1);
		}
		int i = 0;
		while (num != 0) {
			main->arr[i] = va_arg(args, char*);
			main->countParam++;
			i++;
			--num;
		}
		va_end(args);
	}

	void ShowMenu() {
		system("cls");
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		string menu;
		if (main->parent != NULL) main->header = main->parent->arr[main->parent->paragraph - 1];
		menu += "\n\n\n\n\n\n\n\n\t\t\t\t\t\t+--------------------+\n\t\t\t\t\t\t|";

		for (int j = 0; j < (20 - main->header.length()) / 2; j++) menu += " ";
		menu += main->header;
		for (int j = 0; j < 20 - (main->header.length() + (20 - main->header.length()) / 2); j++) menu += " ";
		menu += "|\n\t\t\t\t\t\t+--------------------+\n";

		if (main->arr != NULL) {
			for (int i = 0; i < main->countParam; i++) {
				menu += "\t\t\t\t\t\t|";
				if (i == (main->paragraph - 1)) {
					cout << menu;
					SetColor(0, bg);
					cout << main->arr[i];
					if (main->arr[i].length() < 20) {
						for (int j = 0; j < 20 - main->arr[i].length(); j++) cout << " ";
					}
					SetColor(7, 0);
					menu.clear();
				}
				else {
					menu += main->arr[i];
					if (main->arr[i].length() < 20) {
						for (int j = 0; j < 20 - main->arr[i].length(); j++) menu += " ";
					}
				}
				menu += "|\n\t\t\t\t\t\t+--------------------+\n";
			}
			cout << menu;
		}
		else main = main->parent;

	}

	void Navigation(bool* running) {
		currentID = "0";
		char keycap = _getch();

		if (keycap == 80) main->paragraph++;
		else if (keycap == 72) main->paragraph--;
		else if (keycap == 13) {
			if (main->function[main->paragraph - 1] == NULL && main->sub[main->paragraph - 1].arr != NULL) main = &main->sub[main->paragraph - 1];
			else if (main->function[main->paragraph - 1] == NULL && main->sub[main->paragraph - 1].arr == NULL) currentID = MakeID(&main->sub[main->paragraph- 1]);
			else if (main->function[main->paragraph - 1] != NULL) {
				system("cls");
				main->function[main->paragraph - 1]();
				_getch();
			}
		}
		else if (keycap == 27) {
			if (main->parent != NULL) {
				main->paragraph = 1;
				main = main->parent;
			}
			else if (main->parent == NULL) *running = false;
		}
		else if (keycap == 83) {
			vector<string> arr = { "Синий", "Зелёный", "Голубой", "Красный", "Розовый", "Жёлтый" };
			int paragraph = 0;
			char keycap = NULL;
			while (keycap != 27) {
				system("cls");
				SetConsoleCP(1251);
				SetConsoleOutputCP(1251);

				string menu;
				string header = "НАСТРОЙКА";
				menu += "\n\n\n\n\n\n\n\n\t\t\t\t\t\t+--------------------+\n\t\t\t\t\t\t|";

				for (int j = 0; j < (20 - header.length()) / 2; j++) menu += " ";
				menu += header;
				for (int j = 0; j < 20 - (header.length() + (20 - header.length()) / 2); j++) menu += " ";
				menu += "|\n\t\t\t\t\t\t+--------------------+\n";

				for (int i = 0; i < 6; i++) {
					menu += "\t\t\t\t\t\t|";
					if (i == paragraph) {
						cout << menu;
						SetColor(0, i + 1);
						cout << arr[i];
						if (arr[i].length() < 20) {
							for (int j = 0; j < 20 - arr[i].length(); j++) cout << " ";
						}
						SetColor(7, 0);
						menu.clear();
					}
					else {
						menu += arr[i];
						if (arr[i].length() < 20) {
							for (int j = 0; j < 20 - arr[i].length(); j++) menu += " ";
						}
					}
					menu += "|\n\t\t\t\t\t\t+--------------------+\n";
				}
				cout << menu;

				keycap = _getch();

				if (keycap == 80) paragraph++;
				else if (keycap == 72) paragraph--;
				else if (keycap == 13) {
					bg = paragraph + 1;
					keycap = 27;
				}

				if (paragraph > 5) paragraph = 0;
				else if (paragraph < 0) paragraph = 5;
			}
		}

		if (main->paragraph > main->border) main->paragraph = 1;
		else if (main->paragraph < 1) main->paragraph = main->border;
	}

	string ShowID() { return MakeID(main); }
};