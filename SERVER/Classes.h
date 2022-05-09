#include "Libs.h"

class Product {
	string code;
	string cost;
	string group;
	string name;
	string dealer;

public:
	Product(){};
	Product(string _group, string _code, string _name, string _cost, string _dealer) : group(_group), code(_code), name(_name), cost(_cost), dealer(_dealer) {};

	friend ofstream& operator<<(ofstream& stream, Product& obj) {
		stream << obj.group << " " << obj.code << " " << obj.name << " " << obj.cost << " " << obj.dealer << "\n";
		return stream;
	}
	friend ifstream& operator>>(ifstream& stream, Product& obj) {
		stream >> obj.group >> obj.code >> obj.name >> obj.cost >> obj.dealer;
		return stream;
	}
	void getFields(char* _group, char* _name, char* _cost) {
		strcpy(_group, group.c_str());
		strcpy(_name, name.c_str());
		strcpy(_cost, cost.c_str());
	}
};