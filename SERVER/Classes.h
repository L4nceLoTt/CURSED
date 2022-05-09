#include "Libs.h"

class Product {
	int code;
	float cost;
	string group;
	string name;
	string dealer;

public:
	Product(string _group) : group(_group) {};
	Product(string _group, int _code, string _name, int _cost, string _dealer) : group(_group), code(_code), name(_name), cost(_cost), dealer(_dealer) {};

	friend ofstream& operator<<(ofstream& stream, Product& obj) {
		stream << obj.group << " " << obj.code << " " << obj.name << " " << obj.cost << " " << obj.dealer << "\n";
		return stream;
	}
	friend ifstream& operator>>(ifstream& stream, Product& obj) {
		stream >> obj.group >> obj.code >> obj.name >> obj.cost >> obj.dealer;
		return stream;
	}
};