#pragma once

#include <string>
#include <vector>

enum class TypePoliz {operator_, operation_, move_, adress_};

using std::string;
using std::vector;

struct StructPoliz {
	TypePoliz type;
	string name;
	int value_int;
	float value_float;
	char value_char;
	string value_string;
	StructPoliz(): value_int(0), value_float(0), value_char(0), name(""), value_string("") {}
};

class Poliz
{
public:
	void Push_Poliz(StructPoliz);
	void Blanc();
	int Get_Position() { return (int)data_.size() - 1; }
	void Write_Position(int position, StructPoliz to);
private:
	vector<StructPoliz> data_;
};
