#pragma once

#include "SemanticAnalysis.h"

void SemanticAnalysis::Create_TID() {
	StackTID.push_back({});
}

void SemanticAnalysis::Push_ID(const std::string& name, int type) {
	StructTid new_id(name, type);
	for (auto& el : StackTID.back()) {
		if (el == new_id) {
			throw "Id " + name + " already exists in string " + NumberString;
		}
	}
	StackTID.back().push_back(new_id);

}

void SemanticAnalysis::Delete_TID() {
	StackTID.pop_back();
}

int SemanticAnalysis::Check_ID(const std::string& name) {
	for (auto& el : StackTID.back()) {
		if (el.name == name) {
			return el.type;
		}
	}
	throw "Id " + name + " doesn't exist in string " + NumberString;
}

void SemanticAnalysis::New_Func(const std::string& name, const std::string& inner_name, int type) {
	StructTf new_id(name, inner_name, type);
	for (auto& el : TF) {
		if (el.inner_name == inner_name) {
			throw "Redefining a function " + name + " in string " + NumberString;
		}
	}
	TF.push_back(new_id);
}

int SemanticAnalysis::Check_Call(const std::string& name, const std::string& inner_name) {
	for (auto& el : TF) {
		if (el.inner_name == inner_name) {
			return el.type_back;
		}
	}
	throw "Function " + name + " with those parameters doesn't exist in string " + NumberString;
}

void SemanticAnalysis::Push_Stack(int type, const std::string& name) {
	// Stack.push_back({ type,  , name});
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

void SemanticAnalysis::Check_Bin() {
	if (Stack.size() < 3) throw "Incorrect expression in string " + NumberString;
	auto b = Stack.back(); Stack.pop_back();
	auto t = Stack.back(); Stack.pop_back();
	auto a = Stack.back(); Stack.pop_back();
	if (t.type >= 0 || t.type == -10 || (t.type >= -2 && t.type <= -5) || a.type < 0 || b.type < 0) throw "Incorrect expression in string " + NumberString;
	if (t.type == -1) {
		if (a.type < 3) throw "Cannot dereference a non-array(non-map) in string " + NumberString;
		if (b.type != 0 && b.type != 2) throw "Expression in [] is incorrect in string " + NumberString;
		Stack.push_back({ a.type % 3, "" });
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
	else {
		if (a.name == "") throw "r-value cannot be be to the left of = in string " + NumberString;
		if ((a.type >= 3) ^ (b.type >= 3)) throw "Array and number/identifier are not compatible in string " + NumberString;
		Stack.push_back(b);
	}
}

void SemanticAnalysis::Check_Uno() {
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
	} if (t.type == -3) {
		if (a.type >= 3) throw "Cannot use a-- with arrays and maps in string " + NumberString;
		a.name = "";
		Stack.push_back(a);
	}
	else if (t.type == -4) {
		if (a.type >= 3) throw "Cannot use ++a with arrays and maps in string " + NumberString;
		Stack.push_back(a);
	} if (t.type == -5) {
		if (a.type >= 3) throw "Cannot use --a with arrays and maps in string " + NumberString;
		Stack.push_back(a);
	}
}

StructStack SemanticAnalysis::Pop_Stack() {
	StructStack tmp = Stack.back();
	Stack.pop_back();
	return tmp;
}

void SemanticAnalysis::Check_If() {
	if (Stack.back().type != 0) throw "Not-bool expression cannot be in if() in string " + NumberString;
	Stack.pop_back();
}