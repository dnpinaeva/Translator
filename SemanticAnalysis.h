#pragma once

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
	std::string name; int type;
	StructTid(const std::string& name, int type) : name(name), type(type) {}
	bool operator==(const StructTid& other) const {
		return name == other.name && type == other.type;
	}
};

struct StructTf {
	std::string name, inner_name; int type_back;
	StructTf(const std::string& name, const std::string& inner_name, int type_back) :
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
	void Push_ID(const std::string& name, int type);
	void Delete_TID();
	int Check_ID(const std::string& name);
	void New_Func(const std::string& name, const std::string& inner_name, int type);
	int Check_Call(const std::string& name, const std::string& inner_name);
	void Push_Stack(int type, const std::string& name);
	void Check_Bin();
	void Check_Uno();
	void Check_If();
	StructStack Pop_Stack();

private:
	std::vector<std::vector<StructTid>> StackTID;
	std::vector<StructTf> TF;
	std::vector<StructStack> Stack;
	std::string NumberString;
};