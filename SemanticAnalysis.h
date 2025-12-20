#pragma once

#include "LexicalAnalyzer.h"
#include "Treap.h"
#include "Poliz.h"

#include <string>
#include <vector>

/*
TID type:
0 - int
1 - float
2 - char
3 - array int
4 - array float
5 - array char
*/

enum class TypeValue{Int, Float, Char, ArrayInt, ArrayFloat, ArrayChar, MapIntInt, MapIntFloat, MapIntChar, 
	MapFloatInt, MapFloatFloat, MapFloatChar, MapCharInt, MapCharFloat, MapCharChar
};

struct StructValue {
	bool is_initialized;
	TypeValue type_value;
	int value_int;
	float value_float;
	char value_char;
	vector<int> value_array_int;
	vector<float> value_array_float;
	vector<char> value_array_char;
	Treap<int, int> value_int_int;
	Treap<int, float> value_int_float;
	Treap<int, char> value_int_char;
	Treap<float, int> value_float_int;
	Treap<float, float> value_float_float;
	Treap<float, char> value_float_char;
	Treap<char, int> value_char_int;
	Treap<char, float> value_char_float;
	Treap<char, char> value_char_char;
	StructValue() : value_int(0), value_char(0), value_float(0), is_initialized(1) {}
	StructValue(int value_int) : value_int(value_int), value_char(0), value_float(0), is_initialized(1) {}
	StructValue(float value_float) : value_int(0), value_char(0), value_float(value_float), is_initialized(1) {}
	StructValue(char value_char) : value_int(0), value_char(value_char), value_float(0), is_initialized(1) {}
	StructValue(const vector<int>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_array_int = value;
	}
	StructValue(const vector<float>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_array_float = value;
	}
	StructValue(const vector<char>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_array_char = value;
	}
	StructValue(const Treap<int, int>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_int_int = value;
	}
	StructValue(const Treap<int, float>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_int_float = value;
	}
	StructValue(const Treap<int, char>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_int_char = value;
	}
	StructValue(const Treap<float, int>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_float_int = value;
	}
	StructValue(const Treap<float, float>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_float_float = value;
	}
	StructValue(const Treap<float, char>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_float_char = value;
	}
	StructValue(const Treap<char, int>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_char_int = value;
	}
	StructValue(const Treap<char, float>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_char_float = value;
	}
	StructValue(const Treap<char, char>& value) : value_int(0), value_char(0), value_float(0), is_initialized(1) {
		value_char_char = value;
	}
};

struct StructTid {
	std::string name, type, type_map;
	StructValue value;
	StructTid(const std::string& name, const std::string& type, const std::string& type2) : name(name), type(type), type_map(type2) {}
	bool operator==(const StructTid& other) const {
		return name == other.name && type == other.type;
	}
};


/*
0 - int
1 - float
2 - char
3 - map int
4 - map float
5 - map char
6 - void
*/

struct StructTf {
	std::string name, inner_name, type_back;
	Poliz poliz_function;
	StructValue val_return;
	StructTf(const std::string& name, const std::string& inner_name, const std::string& type_back) :
		name(name), inner_name(inner_name), type_back(type_back) {}
};

struct StructStack {
	int type; std::string name;
	StructStack(int type, const std::string& name) :
		type(type), name(name) {}
};

class SemanticAnalysis {
public:
	void Create_TID();
	void Push_ID(const Lexeme& lex, const std::string& type, std::string type2 = "");
	void Push_ID(const string& name, const std::string& type, std::string type2 = "");
	void Push_Value_TID(const std::string& name, const StructValue& val);
	void Delete_TID();
	std::string Check_ID(const Lexeme& name);
	void New_Func(const Lexeme& name, const std::string& inner_name, const std::string& type);
	std::string Check_Call(const Lexeme& name, const std::string& inner_name);
	void Push_Stack(int type, Lexeme lex);
	void Check_Bin(int number_line, Poliz* where);
	void Check_Uno(int number_line, Poliz* where);
	void Check_If(int number_line);
	void Check_Main();
	StructTf Get_Last_Func() {
		return TF.back();
	}
	StructStack Pop_Stack();
	std::vector<StructTf> TF;

private:
	std::vector<std::vector<StructTid>> StackTID;
	std::vector<StructStack> Stack;
	std::string NumberString;
};