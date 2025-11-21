#pragma once

#include "LexicalAnalyzer.h"

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

struct StructTid {
	std::string name, type, type_map;
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
	void Delete_TID();
	std::string Check_ID(const Lexeme& name);
	void New_Func(const Lexeme& name, const std::string& inner_name, const std::string& type);
	std::string Check_Call(const Lexeme& name, const std::string& inner_name);
	void Push_Stack(int type, Lexeme lex);
	void Check_Bin(int number_line);
	void Check_Uno(int number_line);
	void Check_If(int number_line);
	StructTf Get_Last_Func() {
		return TF.back();
	}
	StructStack Pop_Stack();

private:
	std::vector<std::vector<StructTid>> StackTID;
	std::vector<StructTf> TF;
	std::vector<StructStack> Stack;
	std::string NumberString;
};