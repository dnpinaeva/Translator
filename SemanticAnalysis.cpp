#pragma once

#include "SemanticAnalysis.h"

void SemanticAnalysis::Create_TID() {
	StackTID.push_back({});
}

void SemanticAnalysis::Push_ID(const Lexeme& lex, const std::string& type, std::string type2) {
	StructTid new_id(lex.value, type, type2);
	NumberString = std::to_string(lex.line);
	for (auto& el : StackTID.back()) {
		if (el.name == new_id.name) {
			throw "Id " + lex.value + " already exists in string " + NumberString;
		}
	}
	StackTID.back().push_back(new_id);

}

void SemanticAnalysis::Delete_TID() {
	StackTID.pop_back();
}

std::string SemanticAnalysis::Check_ID(const Lexeme& lex) {
	NumberString = std::to_string(lex.line);
	for (auto& cur : StackTID) {
		for (auto& el : cur) {
			if (el.name == lex.value) {
				return el.type + " " + el.type_map;
			}
		}
	}
	throw "Id " + lex.value + " doesn't exist in string " + NumberString;
}

void SemanticAnalysis::New_Func(const Lexeme& lex, const std::string& inner_name, const std::string& type) {
	StructTf new_id(lex.value, inner_name, type);
	NumberString = std::to_string(lex.line);
	for (auto& el : TF) {
		if (el.inner_name == inner_name) {
			throw "Redefining a function " + lex.value + " in string " + NumberString;
		}
	}
	TF.push_back(new_id);
}

std::string SemanticAnalysis::Check_Call(const Lexeme& lex, const std::string& inner_name) {
	NumberString = std::to_string(lex.line);
	cout << "check_call: \"" << inner_name << "\"\n";
	for (auto& el : TF) {
		cout << "\"" << el.inner_name << "\"" << "\n";
		if (el.inner_name == inner_name) {
			return el.type_back;
		}
	}
	throw "Function " + lex.value + " with those parameters doesn't exist in string " + NumberString;
}

void SemanticAnalysis::Push_Stack(int type, Lexeme lex) {
	if (lex.type != 0) lex.value = "";
	Stack.push_back({ type, lex.value});
}

void SemanticAnalysis::Check_Main() {
	bool f = 0;
	for (auto& el : TF) {
		if (el.name == "main") {
			if (f) {
				throw std::string("Function main is already exist");
			}
			f = 1;
			if (el.name != el.inner_name) {
				throw std::string("Function main must not have parameters");
			}
			if (el.type_back != "int ") {
				throw std::string("Function main must return int");
			}
		}
	}
}


/*
[] - -1
a++ -2
a-- -3
++a -4
--a -5
+ -6
- -7
* -8
/ -9
! -10
== -11
!= -12
> -13
< -14
>= -15
<= -16
&& -17
|| -18
= -19
find -20
, - -21
- -22
% -23

0 - int
1 - float
2 - char
3 - array int
4 - array float
5 - array char
6 - map int
7 - map float
8 - map char
*/

void SemanticAnalysis::Check_Bin(int number_line) {
	NumberString = std::to_string(number_line);
	if (Stack.size() < 3) throw "Incorrect expression in string " + NumberString;
	auto b = Stack.back(); Stack.pop_back();
	auto t = Stack.back(); Stack.pop_back();
	auto a = Stack.back(); Stack.pop_back();
	if (t.type >= 0 || t.type == -10 || (t.type >= -2 && t.type <= -5) || a.type < 0 || b.type < 0) throw "Incorrect expression in string " + NumberString;
	if (t.type == -1) {
		if (a.type < 3) throw "Cannot dereference a non-array(non-map) in string " + NumberString;
		if (b.type != 0 && b.type != 2 && a.type < 6) throw "Expression in [] is incorrect in string " + NumberString;
		Stack.push_back({ a.type % 3, a.name });
		return;
	}
	else if (t.type == -6) {
		if (a.type >= 3 || b.type >= 3) throw "Cannot use + with arrays and maps in string " + NumberString;
		if (a.type == 1 || b.type == 1) {
			Stack.push_back({ 1, "" });
		}
		else if (a.type == 0 || b.type == 0) {
			Stack.push_back({ 0, "" });
		}
		else {
			Stack.push_back({ 2, "" });
		}
	}
	else if (t.type == -7) {
		if (a.type >= 3 || b.type >= 3) throw "Cannot use - with arrays and maps in string " + NumberString;
		if (a.type == 1 || b.type == 1) {
			Stack.push_back({ 1, "" });
		}
		else if (a.type == 0 || b.type == 0) {
			Stack.push_back({ 0, "" });
		}
		else {
			Stack.push_back({ 2, "" });
		}
	}
	else if (t.type == -8) {
		if (a.type >= 3 || b.type >= 3) throw "Cannot use * with arrays and maps in string " + NumberString;
		if (a.type == 1 || b.type == 1) {
			Stack.push_back({ 1, "" });
		}
		else if (a.type == 0 || b.type == 0) {
			Stack.push_back({ 0, "" });
		}
		else {
			Stack.push_back({ 2, "" });
		}
	}
	else if (t.type == -9) {
		if (a.type >= 3 || b.type >= 3) throw "Cannot use / with arrays and maps in string " + NumberString;
		if (a.type == 1 || b.type == 1) {
			Stack.push_back({ 1, "" });
		}
		else if (a.type == 0 || b.type == 0) {
			Stack.push_back({ 0, "" });
		}
		else {
			Stack.push_back({ 2, "" });
		}
	}
	else if (t.type == -11) {
		if (a.type >= 3 || b.type >= 3) throw "Cannot use == with arrays and maps in string " + NumberString;
		if (a.type == 1 || b.type == 1) {
			Stack.push_back({ 0, "" });
		}
		else if (a.type == 0 || b.type == 0) {
			Stack.push_back({ 0, "" });
		}
		else {
			Stack.push_back({ 0, "" });
		}
	}
	else if (t.type == -12) {
		if (a.type >= 3 || b.type >= 3) throw "Cannot use != with arrays and maps in string " + NumberString;
		if (a.type == 1 || b.type == 1) {
			Stack.push_back({ 0,  "" });
		}
		else if (a.type == 0 || b.type == 0) {
			Stack.push_back({ 0, "" });
		}
		else {
			Stack.push_back({ 0, "" });
		}
	}
	else if (t.type == -13) {
		if (a.type >= 3 || b.type >= 3) throw "Cannot use > with arrays and maps in string " + NumberString;
		if (a.type == 1 || b.type == 1) {
			Stack.push_back({ 0, "" });
		}
		else if (a.type == 0 || b.type == 0) {
			Stack.push_back({ 0, "" });
		}
		else {
			Stack.push_back({ 0, "" });
		}
	}
	else if (t.type == -14) {
		if (a.type >= 3 || b.type >= 3) throw "Cannot use < with arrays and maps in string " + NumberString;
		if (a.type == 1 || b.type == 1) {
			Stack.push_back({ 0, "" });
		}
		else if (a.type == 0 || b.type == 0) {
			Stack.push_back({ 0, "" });
		}
		else {
			Stack.push_back({ 0, "" });
		}
	}
	else if (t.type == -15) {
		if (a.type >= 3 || b.type >= 3) throw "Cannot use >= with arrays and maps in string " + NumberString;
		if (a.type == 1 || b.type == 1) {
			Stack.push_back({ 0, "" });
		}
		else if (a.type == 0 || b.type == 0) {
			Stack.push_back({ 0, "" });
		}
		else {
			Stack.push_back({ 0, "" });
		}
	}
	else if (t.type == -16) {
		if (a.type >= 3 || b.type >= 3) throw "Cannot use <= with arrays and maps in string " + NumberString;
		if (a.type == 1 || b.type == 1) {
			Stack.push_back({ 0, "" });
		}
		else if (a.type == 0 || b.type == 0) {
			Stack.push_back({ 0, "" });
		}
		else {
			Stack.push_back({ 0, "" });
		}
	}
	else if (t.type == -17) {
		if (a.type || b.type) throw "Cannot use && with non-bool arguments in string " + NumberString;
		Stack.push_back({ 0, "" });
	}
	else if (t.type == -18) {
		if (a.type || b.type) throw "Cannot use || with non-bool arguments in string " + NumberString;
		Stack.push_back({ 0, "" });
	}
	else if (t.type == -19) {
		if (a.name == "") throw "r-value cannot be be to the left of = in string " + NumberString;
		if ((a.type >= 3) ^ (b.type >= 3)) throw "Array/map and number are not compatible in string " + NumberString;
		if ((a.type >= 6) ^ (b.type >= 6)) throw "Array and map are not compatible in string " + NumberString;
		b.name = "";
		Stack.push_back(b);
	}
	else if (t.type == -20) {
		if (a.type < 6) throw "Cannot use find with not-map in string " + NumberString;
		if (b.type > 2) throw "Cannot use find from not number in string " + NumberString;
		Stack.push_back({ 0, "" });
	}
	else if (t.type == -21) {
		Stack.push_back(b);
	}
	else {
		if (a.type >= 3 || b.type >= 3 || a.type == 1 || b.type == 1) throw "Cannot use % with float numbers, arrays and maps in string " + NumberString;
		if (a.type == 0 || b.type == 0) {
			Stack.push_back({ 0, "" });
		}
		else {
			Stack.push_back({ 2, "" });
		}
	}
}

void SemanticAnalysis::Check_Uno(int number_line) {
	NumberString = std::to_string(number_line);
	if (Stack.size() < 2) throw "Incorrect expression in string " + NumberString;
	auto a = Stack.back(); Stack.pop_back();
	auto t = Stack.back(); Stack.pop_back();
	if (!(t.type >= 0 || t.type == -10 || (t.type >= -2 && t.type <= -5) || a.type < 0)) throw "Incorrect expression in string " + NumberString;
	if (t.type == -10) {
		if (a.type != 0) throw "Cannot use ! with not bool expression in string " + NumberString;
		Stack.push_back(a);
	}
	else if (t.type == -2) {
		if (a.type >= 3) throw "Cannot use a++ with arrays and maps in string " + NumberString;
		a.name = "";
		Stack.push_back(a);
	} else if (t.type == -3) {
		if (a.type >= 3) throw "Cannot use a-- with arrays and maps in string " + NumberString;
		a.name = "";
		Stack.push_back(a);
	}
	else if (t.type == -4) {
		if (a.type >= 3) throw "Cannot use ++a with arrays and maps in string " + NumberString;
		if (a.name == "") throw "Cannot use ++a with r-value " + NumberString;
		Stack.push_back(a);
	}
	else if (t.type == -5) {
		if (a.type >= 3) throw "Cannot use --a with arrays and maps in string " + NumberString;
		if (a.name == "") throw "Cannot use --a with r-value " + NumberString;
		Stack.push_back(a);
	}
	else if (t.type == -22) {
		if (a.type >= 3) throw "Cannot use -a with arrays and maps in string " + NumberString;
		Stack.push_back(a);
	}
}

StructStack SemanticAnalysis::Pop_Stack() {
	StructStack tmp = Stack.back();
	Stack.pop_back();
	return tmp;
}

void SemanticAnalysis::Check_If(int number_line) {
	NumberString = std::to_string(number_line);
	if (Stack.back().type != 0) throw "Not-bool expression cannot be in if() in string " + NumberString;
	Stack.pop_back();
}