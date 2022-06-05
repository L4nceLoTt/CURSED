#include "Libs.h"

class Product {
	string code;
	string cost;
	string name;
	string group;
	string dealer;
	string amount;

public:
	Product() {};
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
	string GetName() { return name; }
	void AddAmount(const char* adding) {
		char _amount[100];
		int _adding = atoi(adding);
		int _amount_ = atoi(amount.c_str());
		_itoa_s(_adding + _amount_, _amount, sizeof(_amount), 10);
		amount = _amount;
	}
	void SetAmount(const char* _amount) {
		amount = _amount;
	}
	bool isExist() {
		if (amount == "0") return false;
		else return true;
	}
	string GetAmount() { return amount; }
};

//class Oil : public Product{
//	string group;
//
//public:
//	void SetGroup(string _group) { group = _group; }
//	string GetGroup() { return group; }
//};
//class Conserv : public Product{
//	string group;
//
//public:
//	void SetGroup(string _group) { group = _group; }
//	string GetGroup() { return group; }
//};
//class Grocery : public Product{
//	string group;
//
//public:
//	void SetGroup(string _group) { group = _group; }
//	string GetGroup() { return group; }
//};