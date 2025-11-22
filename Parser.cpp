#include "Parser.h"

/* When entering the function for the first time
* and when returning to it from other functions, it is assumed
* that the token has already been read
*/


void Parser::start() {
	current_lexeme = lexer.get();
	program();
	if (current_lexeme.type != 6) {
		throw current_lexeme;
	}
}

void Parser::program() {
	semantic.Create_TID();
	while (current_lexeme.type != 6) {
		if (current_lexeme.value == "map") {
			//cout << "bbbbbbbbbbbbb map\n";
			current_lexeme = lexer.get();
			if (current_lexeme.value != "<") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			string type1 = current_lexeme.value;
			type();
			if (current_lexeme.value != ",") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			string type2 = current_lexeme.value;
			type();
			if (current_lexeme.value != ">") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			Lexeme lex = current_lexeme;
			if (current_lexeme.type != 0) {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			//cout << "current " << current_lexeme.value << "\n";
			if (current_lexeme.value == ";") {
				semantic.Push_ID(lex, type1, type2);
				current_lexeme = lexer.get();
			}
			else if (current_lexeme.value == "=") {
				semantic.Push_ID(lex, type1, type2);
				map_notitle();
				if (current_lexeme.value != ";") {
					throw current_lexeme;
				}
				current_lexeme = lexer.get();
			}
			else {
				function_notitle(lex, type1, type2);
			}
		}
		else if (current_lexeme.value == "void") {
			current_lexeme = lexer.get();
			Lexeme lex = current_lexeme;
			if (current_lexeme.type != 0) {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			function_notitle(lex, "void");
		}
		else {
			string type1 = current_lexeme.value;
			type();
			if (current_lexeme.type != 0) {
				throw current_lexeme;
			}
			Lexeme lex = current_lexeme;
			current_lexeme = lexer.get();
			if (current_lexeme.value == ";") {
				semantic.Push_ID(lex, type1);
				current_lexeme = lexer.get();
			}
			else if (current_lexeme.value == "=") {
				semantic.Push_ID(lex, type1);
				var_notitle(lex);
				if (current_lexeme.value != ";") {
					throw current_lexeme;
				}
				current_lexeme = lexer.get();
			}
			else if (current_lexeme.value == "[") {
				semantic.Push_ID(lex, "array " + type1);
				array_notitle();
				if (current_lexeme.value != ";") {
					throw current_lexeme;
				}
				current_lexeme = lexer.get();
			}
			else {
				function_notitle(lex, type1);
			}
		}
	}
}

void Parser::type() {
	if (current_lexeme.value != "int" && current_lexeme.value != "char" && current_lexeme.value != "float") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	return;
}

void Parser::var_notitle(const Lexeme& lex) {
	if (current_lexeme.value == "=") {
		semantic.Push_Stack(0, lex);
		semantic.Push_Stack(-19, current_lexeme);
		current_lexeme = lexer.get();
		expression();
		semantic.Check_Bin(current_lexeme.line);
	}
	else {
		throw current_lexeme;
	}
}

void Parser::array_notitle() {
	if (current_lexeme.value != "[") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	expression();
	StructStack res = semantic.Pop_Stack();
	if (res.type != 0 && res.type != 2) {
		throw "Incorrect expression to specify array size in string " + std::to_string(current_lexeme.line);
	}
	if (current_lexeme.value != "]") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != "=") {
		return;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != "{") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	expression();
	res = semantic.Pop_Stack();
	if (res.type != 0 && res.type != 1 && res.type != 2) {
		throw "Incorrect expression to array element in string " + std::to_string(current_lexeme.line);
	}
	while (current_lexeme.value == ",") {
		current_lexeme = lexer.get();
		expression();
		res = semantic.Pop_Stack();
		if (res.type != 0 && res.type != 1 && res.type != 2) {
			throw "Incorrect expression to array element in string " + std::to_string(current_lexeme.line);
		}
	}
	if (current_lexeme.value != "}") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	return;
}

void Parser::function_notitle(const Lexeme& lex, const string& type1, string type2 = "") {
	if (current_lexeme.value != "(") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	string inner_name = lex.value;
	semantic.Create_TID();
	if (current_lexeme.value != ")") {
		inner_name += parameters();
	}
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	
	semantic.New_Func(lex, inner_name, type1 + " " + type2);
	block(0);
	semantic.Delete_TID();
}

string Parser::parameters() {
	string for_return = " ";
	for_return += parameters_description();
	while (current_lexeme.value == ",") {
		current_lexeme = lexer.get();
		for_return += " ";
		for_return += parameters_description();
	}
	return for_return;
}

string Parser::parameters_description() {
	string for_return = "";
	if (current_lexeme.value == "map") {
		for_return = "map";
		current_lexeme = lexer.get();
		if (current_lexeme.value != "<") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		string type1 = current_lexeme.value;
		type();
		if (current_lexeme.value != ",") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		string type2 = current_lexeme.value;
		type();
		if (current_lexeme.value != ">") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		if (current_lexeme.type != 0) {
			throw current_lexeme;
		}
		semantic.Push_ID(current_lexeme, type1, type2);
		current_lexeme = lexer.get();
		return for_return;
	}
	else {
		for_return += "number";
		string type1 = current_lexeme.value;
		type();
		if (current_lexeme.type != 0) {
			throw current_lexeme;
		}
		Lexeme prev = current_lexeme;
		current_lexeme = lexer.get();
		if (current_lexeme.value != "[") {
			semantic.Push_ID(prev, type1);
			return for_return;
		}
		current_lexeme = lexer.get();
		if (current_lexeme.value != "]") {
			throw current_lexeme;
		}
		for_return += "[]";
		current_lexeme = lexer.get();
		semantic.Push_ID(prev, "array " + type1);
		return for_return;
	}
}

void Parser::map_notitle() {
	//cout << "map_notitle\n";
	if (current_lexeme.value != "=") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != "{") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != "{") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	expression1();
	StructStack res = semantic.Pop_Stack();
	if (res.type != 0 && res.type != 1 && res.type != 2) {
		throw "Incorrect expression to map element in string " + std::to_string(current_lexeme.line);
	}
	if (current_lexeme.value != ",") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	//cout << "aaaaaaaaaaaaa\n";
	expression1();
	res = semantic.Pop_Stack();
	if (res.type != 0 && res.type != 1 && res.type != 2) {
		throw "Incorrect expression to map element in string " + std::to_string(current_lexeme.line);
	}
	if (current_lexeme.value != "}") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	while (current_lexeme.value == ",") {
		current_lexeme = lexer.get();
		if (current_lexeme.value != "{") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		expression1();
		res = semantic.Pop_Stack();
		if (res.type != 0 && res.type != 1 && res.type != 2) {
			throw "Incorrect expression to map element in string " + std::to_string(current_lexeme.line);
		}
		if (current_lexeme.value != ",") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		expression1();
		res = semantic.Pop_Stack();
		if (res.type != 0 && res.type != 1 && res.type != 2) {
			throw "Incorrect expression to map element in string " + std::to_string(current_lexeme.line);
		}
		if (current_lexeme.value != "}") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
	}
	if (current_lexeme.value != "}") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
}

void Parser::block(bool is_tid_needed) {
	if (is_tid_needed) {
		semantic.Create_TID();
	}
	if (current_lexeme.value != "{") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	while (current_lexeme.value != "}") {
		operator_();
	}
	current_lexeme = lexer.get();
	if (is_tid_needed) {
		semantic.Delete_TID();
	}
}

void Parser::operator_(bool is_tid_needed = 0) {
	if (current_lexeme.value == "int" || current_lexeme.value == "float"
		|| current_lexeme.value == "char" || current_lexeme.value == "map") {
		description();
		if (current_lexeme.value != ";") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
	}
	else if (current_lexeme.value == "if") {
		if_();
	}
	else if (current_lexeme.value == "while") {
		while_();
	}
	else if (current_lexeme.value == "for") {
		for_();
	}
	else if (current_lexeme.value == "{") {
		block(is_tid_needed);
	}
	else if (current_lexeme.value == "print") {
		print();
		if (current_lexeme.value != ";") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
	}
	else if (current_lexeme.value == "input") {
		input();
		if (current_lexeme.value != ";") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
	}
	else if (current_lexeme.value == "return") {
		current_lexeme = lexer.get();
		if (current_lexeme.value == ";") {
			current_lexeme = lexer.get();
			return;
		}
		expression();
		StructTf last = semantic.Get_Last_Func();
		StructStack res = semantic.Pop_Stack();
		bool is_ok = 0;
		if (res.type >= 0 && res.type <= 2 && (last.type_back == "int" || last.type_back == "char" || last.type_back == "float")) {
			is_ok = 1;
		}
		if (res.type >= 6 && res.type <= 8 && !(last.type_back == "int" || last.type_back == "char" || last.type_back == "float")) {
			is_ok = 1;
		}
		if (!is_ok) {
			throw "Returning value type is not correct in string " + std::to_string(current_lexeme.line);
		}
		if (current_lexeme.value != ";") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
	}
	else {
		char* prev = lexer.current;
		Lexeme prev_lex = current_lexeme;
		//cout << prev_lex.value << "\n";
		try {
			map_delete();
			if (current_lexeme.value != ";") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
		}
		catch (Lexeme lex) {
			lexer.current = prev;
			current_lexeme = prev_lex;
			//cout << prev_lex.value << " " << current_lexeme.value << "\n";
			expression();
			if (current_lexeme.value != ";") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
		}
		/*expression();
		if (current_lexeme.value != ";") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();*/
	}
}

void Parser::description() {
	if (current_lexeme.value == "map") {
		current_lexeme = lexer.get();
		if (current_lexeme.value != "<") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		string type1 = current_lexeme.value;
		type();
		if (current_lexeme.value != ",") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		string type2 = current_lexeme.value;
		type();
		if (current_lexeme.value != ">") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		Lexeme lex = current_lexeme;
		if (current_lexeme.type != 0) {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		semantic.Push_ID(lex, type1, type2);
		if (current_lexeme.value == ";") {
			return;
		}
		else if (current_lexeme.value == "=") {
			map_notitle();
			/*if (current_lexeme.value != ";") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();*/
		}
		else {
			throw current_lexeme;
		}
	}
	else {
		string type1 = current_lexeme.value;
		type();
		if (current_lexeme.type != 0) {
			throw current_lexeme;
		}
		Lexeme lex = current_lexeme;
		current_lexeme = lexer.get();
		if (current_lexeme.value == ";") {
			//current_lexeme = lexer.get();
			semantic.Push_ID(lex, type1);
			return;
		}
		if (current_lexeme.value == "=") {
			semantic.Push_ID(lex, type1);
			var_notitle(lex);
		}
		else {
			semantic.Push_ID(lex, "array " +  type1);
			array_notitle();
		}
	}
}

void Parser::if_() {
	if (current_lexeme.value != "if") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != "(") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	expression();
	semantic.Check_If(current_lexeme.line);
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	operator_(1);
	if (current_lexeme.value != "else") {
		return;
	}
	current_lexeme = lexer.get();
	operator_(1);
}

void Parser::while_() {
	if (current_lexeme.value != "while") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != "(") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	expression();
	semantic.Check_If(current_lexeme.line);
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	operator_(1);
}

void Parser::for_() {
	if (current_lexeme.value != "for") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != "(") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	semantic.Create_TID();
	if (current_lexeme.value != ";") {
		description();
		while (current_lexeme.value == ",") {
			current_lexeme = lexer.get();
			description();
		}
	}
	if (current_lexeme.value != ";") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != ";") {
		expression();
		semantic.Check_If(current_lexeme.line);
	}
	if (current_lexeme.value != ";") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != ")") {
		expression();
		semantic.Pop_Stack();
	}
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	operator_();
}

void Parser::print() {
	if (current_lexeme.value != "print") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != "(") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value == ")") {
		current_lexeme = lexer.get();
		return;
	}
	if (current_lexeme.type == 20) {
		current_lexeme = lexer.get();
		if (current_lexeme.value != ")") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
	}
	else {
		expression();
		semantic.Pop_Stack();
		if (current_lexeme.value != ")") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
	}
}

void Parser::input() {
	if (current_lexeme.value != "input") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != "(") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	semantic.Check_ID(current_lexeme);
	if (current_lexeme.type != 0) {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value == ")") {
		current_lexeme = lexer.get();
		return;
	}
	if (current_lexeme.value != "[") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	int line = current_lexeme.line;
	expression();
	StructStack res = semantic.Pop_Stack();
	if (res.type > 2) throw "Inside [] is an invalid type in string " + std::to_string(line);
	if (current_lexeme.value != "]") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
}

void Parser::map_delete() {
	if (current_lexeme.type != 0) {
		throw current_lexeme;
	}
	string type_lex = semantic.Check_ID(current_lexeme);
	if (type_lex[0] != 'a' && type_lex.size() > 6) return;
	current_lexeme = lexer.get();
	if (current_lexeme.value != ".") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != "delete") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != "(") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	int line = current_lexeme.line;
	expression();
	StructStack res = semantic.Pop_Stack();
	if (res.type > 2) throw "Inside [] is an invalid type in string " + std::to_string(line);
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
}

void Parser::expression() {
	//cout << "expr " << current_lexeme.value << "\n";
	expression1();
	while (current_lexeme.value == ",") {
		current_lexeme = lexer.get();
		expression1();
	}
}

void Parser::expression1() {
	expression2();
	while (current_lexeme.value == "=") {
		current_lexeme = lexer.get();
		expression2();
	}
}

void Parser::expression2() {
	//cout << "expr 2";
	expression3();
	while (current_lexeme.value == "||") {
		current_lexeme = lexer.get();
		expression3();
	}
}

void Parser::expression3() {
	//cout << "expr 3";
	expression4();
	while (current_lexeme.value == "&&") {
		current_lexeme = lexer.get();
		expression4();
	}
}

void Parser::expression4() {
	//cout << "expr 4";
	expression5();
	if (current_lexeme.value == "==" || current_lexeme.value == "!=") {
		current_lexeme = lexer.get();
		expression5();
	}
}

void Parser::expression5() {
	//cout << "expr 5";
	expression6();
	if (current_lexeme.value == ">" || current_lexeme.value == "<" || current_lexeme.value == ">=" || current_lexeme.value == "<=") {
		current_lexeme = lexer.get();
		expression6();
	}
}

void Parser::expression6() {
	//cout << "expr 6";
	expression7();
	while (current_lexeme.value == "+" || current_lexeme.value == "-") {
		current_lexeme = lexer.get();
		expression7();
	}
}

void Parser::expression7() {
	//cout << "expr 7";
	expression8();
	while (current_lexeme.value == "*" || current_lexeme.value == "/" || current_lexeme.value == "%") {
		current_lexeme = lexer.get();
		expression8();
	}
}

void Parser::expression8() {
	//cout << "expr 8";
	if (current_lexeme.value == "!" || current_lexeme.value == "-" || current_lexeme.value == "++" || current_lexeme.value == "--") {
		current_lexeme = lexer.get();
		expression9();
	}
	else {
		expression9();
	}
}

void Parser::expression9() {
	//cout << "expr 9" << current_lexeme.value << "\n";
	if (current_lexeme.value == "(") {
		current_lexeme = lexer.get();
		expression();
		if (current_lexeme.value != ")") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
	}
	else if (current_lexeme.type == 0) {
		//cout << "aaaaaaaa";
		Lexeme last = current_lexeme;
		string type_lex = " ";
		try {
			type_lex = semantic.Check_ID(current_lexeme);
		}
		catch (...) {

		}
		if (type_lex == "int ") {
			semantic.Push_Stack(0, current_lexeme);
		}
		else if (type_lex == "float ") {
			semantic.Push_Stack(1, current_lexeme);
		}
		else if (type_lex == "char ") {
			semantic.Push_Stack(2, current_lexeme);
		}
		else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "int") {
			if (type_lex[0] == 'a') semantic.Push_Stack(3, current_lexeme);
			else semantic.Push_Stack(6, current_lexeme);
		}
		else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "float") {
			if (type_lex[0] == 'a') semantic.Push_Stack(4, current_lexeme);
			else semantic.Push_Stack(7, current_lexeme);
		}
		else {
			if (type_lex[0] == 'a') semantic.Push_Stack(5, current_lexeme);
			else semantic.Push_Stack(8, current_lexeme);
		}
		current_lexeme = lexer.get();
		if (current_lexeme.value == "++" || current_lexeme.value == "--") {
			semantic.Push_Stack(-2 + (current_lexeme.value == "--"), current_lexeme);
			semantic.Check_Uno(current_lexeme.line);
			current_lexeme = lexer.get();
		}
		else if (current_lexeme.value == "[") {
			semantic.Push_Stack(-1, current_lexeme);
			current_lexeme = lexer.get();
			int line = current_lexeme.line;
			expression();
			semantic.Check_Bin(line);
			if (current_lexeme.value != "]") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
		}
		else if (current_lexeme.value == ".") {
			current_lexeme = lexer.get();
			if (current_lexeme.value != "find") {
				throw current_lexeme;
			}
			semantic.Push_Stack(-20, current_lexeme);
			current_lexeme = lexer.get();
			if (current_lexeme.value != "(") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			int line = current_lexeme.line;
			expression();
			semantic.Check_Bin(line);
			if (current_lexeme.value != ")") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
		}
		else if (current_lexeme.value == "(") {
			int line = current_lexeme.line;
			current_lexeme = lexer.get();
			if (current_lexeme.value == ")") {
				semantic.Check_Call(current_lexeme, last.value + " ");
				current_lexeme = lexer.get();
				return;
			}
			string params = "";
			expression();
			{
				StructStack res = semantic.Pop_Stack();
				if (res.type < 0) throw "Incorrect parameters in function call in string " + std::to_string(line);
				if (res.type <= 2) params += "number ";
				if (res.type <= 5) params += "number[] ";
				if (res.type <= 8) params += "map ";
			}
			while (current_lexeme.value == ",") {
				current_lexeme = lexer.get();
				expression();
				StructStack res = semantic.Pop_Stack();
				if (res.type < 0) throw "Incorrect parameters in function call in string " + std::to_string(line);
				if (res.type <= 2) params += "number ";
				if (res.type <= 5) params += "number[] ";
				if (res.type <= 8) params += "map ";
			}
			params.pop_back();
			semantic.Check_Call(last, last.value + " " + params);
			if (current_lexeme.value != ")") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
		}
	}
	else if (current_lexeme.type == 21 || current_lexeme.type == 22 || current_lexeme.type == 23) {
		if (current_lexeme.type == 21) {
			semantic.Push_Stack(2, current_lexeme);
		} else if (current_lexeme.type == 22) {
			semantic.Push_Stack(0, current_lexeme);
		} else {
			semantic.Push_Stack(1, current_lexeme);
		}
		current_lexeme = lexer.get();
	}
	else {
		throw current_lexeme;
	}
}

/*
20 - string
21 - symbol
22 - number int
23 - number float
*/