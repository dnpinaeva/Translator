#pragma once

#include <string>
#include <vector>

enum class TypePoliz {plus_, operation_, move_, adress_, separator_};

using std::string;
using std::vector;

struct StructPoliz {
	TypePoliz type;
	string name; // ID name or operation
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
	void Clear_to_Position(int from) {
		while (data_.size() > from) data_.pop_back();
	}
	int Get_Position() { return (int)data_.size() - 1; }
	void Write_Position(int position, StructPoliz to);
private:
	vector<StructPoliz> data_;
};
