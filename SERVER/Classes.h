#include "Libs.h"

class Product {
	string code;
	string cost;
	string group;
	string name;
	string dealer;
	string amount;

public:
	Product(){};
	Product(string _group, string _code, string _name, string _cost, string _dealer, string _amount) : group(_group), code(_code), name(_name), cost(_cost), dealer(_dealer), amount(_amount) {};

	friend ofstream& operator<<(ofstream& stream, Product& obj) {
		stream << obj.group << " " << obj.code << " " << obj.name << " " << obj.cost << " " << obj.dealer << " " << obj.amount << "\n";
		return stream;
	}
	friend ifstream& operator>>(ifstream& stream, Product& obj) {
		stream >> obj.group >> obj.code >> obj.name >> obj.cost >> obj.dealer >> obj.amount;
		return stream;
	}
	void getFields_to_User(char* _group, char* _name, char* _cost) {
		strcpy(_group, group.c_str());
		strcpy(_name, name.c_str());
		strcpy(_cost, cost.c_str());
	}
	void getFields_to_Admin(char* _group, char* _name, char* _cost, char* _code, char* _dealer) {
		strcpy(_group, group.c_str());
		strcpy(_code, code.c_str());
		strcpy(_dealer, dealer.c_str());
		strcpy(_name, name.c_str());
		strcpy(_cost, cost.c_str());
	}
	void getFields_to_Warehouse(char* _group, char* _name, char* _cost, char* _code, char* _dealer, char* _amount) {
		strcpy(_group, group.c_str());
		strcpy(_code, code.c_str());
		strcpy(_dealer, dealer.c_str());
		strcpy(_name, name.c_str());
		strcpy(_cost, cost.c_str());
		strcpy(_amount, amount.c_str());
	}
	string GetGroup() { return group; }
};