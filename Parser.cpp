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
	semantic.Check_Main();
	{
		StructPoliz polka;
		polka.type = TypePoliz::separator_;
		poliz.Push_Poliz(polka);
	}
	{
		StructPoliz polka;
		polka.name = "main";
		polka.type = TypePoliz::operation_;
		poliz.Push_Poliz(polka);
	}
	{
		StructPoliz polka;
		polka.name = "call";
		polka.type = TypePoliz::plus_;
		poliz.Push_Poliz(polka);
	}
}

void Parser::execution_function(){
	semantic.Create_TID();
	Execution execution(semantic);
	execution.Get(*where, "void");
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
				{
					StructPoliz polka;
					polka.name = "";
					polka.type = TypePoliz::separator_;
					where->Push_Poliz(polka);
				}
				semantic.Push_ID(lex, type1, type2);
				current_lexeme = lexer.get();
				{
					StructPoliz polka;
					polka.name = lex.value;
					polka.type = TypePoliz::operation_;
					where->Push_Poliz(polka);
				}
				{
					StructPoliz polka;
					polka.name = "map " + type1 + " " + type2;
					polka.type = TypePoliz::plus_;
					where->Push_Poliz(polka);
				}
			}
			else if (current_lexeme.value == "=") {
				{
					StructPoliz polka;
					polka.name = "";
					polka.type = TypePoliz::separator_;
					where->Push_Poliz(polka);
				}
				semantic.Push_ID(lex, type1, type2);
				map_notitle(lex.value);
				if (current_lexeme.value != ";") {
					throw current_lexeme;
				}
				current_lexeme = lexer.get();
				{
					StructPoliz polka;
					polka.name = lex.value;
					polka.type = TypePoliz::operation_;
					where->Push_Poliz(polka);
				}
				{
					StructPoliz polka;
					polka.name = "map " + type1 + " " + type2;
					polka.type = TypePoliz::plus_;
					where->Push_Poliz(polka);
				}
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
				{
					StructPoliz polka;
					polka.name = lex.value;
					polka.type = TypePoliz::operation_;
					where->Push_Poliz(polka);
				}
				{
					StructPoliz polka;
					polka.name = "var " + type1;
					polka.type = TypePoliz::plus_;
					where->Push_Poliz(polka);
				}
				semantic.Push_ID(lex, type1);
				current_lexeme = lexer.get();
			}
			else if (current_lexeme.value == "=") {
				semantic.Push_ID(lex, type1);
				{
					StructPoliz polka;
					polka.name = lex.value;
					polka.type = TypePoliz::operation_;
					where->Push_Poliz(polka);
				}
				{
					StructPoliz polka;
					polka.name = "var " + type1;
					polka.type = TypePoliz::plus_;
					where->Push_Poliz(polka);
				}
				var_notitle(lex);
				if (current_lexeme.value != ";") {
					throw current_lexeme;
				}
				current_lexeme = lexer.get();
			}
			else if (current_lexeme.value == "[") {
				semantic.Push_ID(lex, "array " + type1);
				array_notitle(lex.value);
				if (current_lexeme.value != ";") {
					throw current_lexeme;
				}
				current_lexeme = lexer.get();
				{
					StructPoliz polka;
					polka.name = lex.value;
					polka.type = TypePoliz::operation_;
					where->Push_Poliz(polka);
				}
				{
					StructPoliz polka;
					polka.name = "array " + type1;
					polka.type = TypePoliz::plus_;
					where->Push_Poliz(polka);
				}
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
	StructPoliz res;
	res.name = lex.value;
	res.type = TypePoliz::operation_;
	where->Push_Poliz(res);
	if (current_lexeme.value == "=") {
		semantic.Push_Stack(0, lex);
		semantic.Push_Stack(-19, current_lexeme);
		current_lexeme = lexer.get();
		expression();
		semantic.Check_Bin(current_lexeme.line, where);
		res.name = "";
		res.type = TypePoliz::separator_;
		where->Push_Poliz(res);
	}
	else {
		throw current_lexeme;
	}
}

void Parser::array_notitle(const string& name) {
	if (current_lexeme.value != "[") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	{
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::separator_;
		where->Push_Poliz(polka);
	}
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
	expression1();
	res = semantic.Pop_Stack();
	if (res.type != 0 && res.type != 1 && res.type != 2) {
		throw "Incorrect expression to array element in string " + std::to_string(current_lexeme.line);
	}
	while (current_lexeme.value == ",") {
		current_lexeme = lexer.get();
		expression1();
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

void Parser::function_notitle(const Lexeme& lex, const string& type1, string type2) {
	if (current_lexeme.value != "(") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	string inner_name = lex.value;
	semantic.Create_TID();
	vector<StructTid> params;
	if (current_lexeme.value != ")") {
		inner_name += parameters(params);
	}
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	semantic.New_Func(lex, inner_name, type1 + " " + type2, params);
	where = &semantic.TF.back().poliz_function;
	block(0);
	where = &poliz;
	semantic.Delete_TID();
}

string Parser::parameters(vector<StructTid>& params) {
	string for_return = " ";
	for_return += parameters_description(params);
	while (current_lexeme.value == ",") {
		current_lexeme = lexer.get();
		for_return += " ";
		for_return += parameters_description(params);
	}
	return for_return;
}

string Parser::parameters_description(vector<StructTid>& params) {
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
		params.push_back(StructTid(current_lexeme.value, type1, type2));
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
			params.push_back(StructTid(prev.value, type1, ""));
			return for_return;
		}
		current_lexeme = lexer.get();
		if (current_lexeme.value != "]") {
			throw current_lexeme;
		}
		for_return += "[]";
		current_lexeme = lexer.get();
		semantic.Push_ID(prev, "array " + type1);
		params.push_back(StructTid(prev.value, "array " + type1, ""));
		return for_return;
	}
}

void Parser::map_notitle(const string& name) {
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
		{
			StructPoliz polka;
			polka.name = "create tid";
			polka.type = TypePoliz::plus_;
			where->Push_Poliz(polka);
		}
	}
	if (current_lexeme.value != "{") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	while (current_lexeme.value != "}") {
		operator_(1);
	}
	current_lexeme = lexer.get();
	if (is_tid_needed) {
		semantic.Delete_TID();
		{
			StructPoliz polka;
			polka.name = "delete tid";
			polka.type = TypePoliz::plus_;
			where->Push_Poliz(polka);
		}
	}
}

void Parser::operator_(bool is_tid_needed) {
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
			StructTf last = semantic.Get_Last_Func();
			string tmp = last.type_back.substr(0, last.type_back.find(' '));
			if (tmp != "void") {
				throw "Returning value type is not correct in string " + std::to_string(current_lexeme.line);
			}
			current_lexeme = lexer.get();
			return;
		}
		expression();
		StructTf last = semantic.Get_Last_Func();
		StructStack res = semantic.Pop_Stack();
		string tmp = last.type_back.substr(0, last.type_back.find(' '));
		//cout << "aaaaaaaaaaaaaaaaaaaa " << last.type_back << " " << res.type << "\n";
		bool is_ok = 0;
		if (res.type >= 0 && res.type <= 2 && (tmp == "int" || tmp == "char" || tmp == "float")) {
			is_ok = 1;
		}
		if (res.type >= 6 && res.type <= 8 && !(tmp == "int" || tmp == "char" || tmp == "float")) {
			is_ok = 1;
		}
		if (!is_ok) {
			throw "Returning value type is not correct in string " + std::to_string(current_lexeme.line);
		}
		if (current_lexeme.value != ";") {
			throw current_lexeme;
		}
		StructPoliz polka;
		polka.name = "return";
		polka.type = TypePoliz::plus_;
		where->Push_Poliz(polka);
		current_lexeme = lexer.get();
	}
	else {
		char* prev = lexer.current;
		Lexeme prev_lex = current_lexeme;
		//cout << prev_lex.value << "\n";
		int place = where->Get_Position();
		try {
			map_delete();
			if (current_lexeme.value != ";") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
		}
		catch (Lexeme lex) {
			// cout << "catch map delete\n";
			lexer.current = prev;
			current_lexeme = prev_lex;
			where->Clear_to_Position(place);
			//cout << prev_lex.value << " " << current_lexeme.value << "\n";
			expression();
			StructPoliz polka;
			polka.type = TypePoliz::separator_;
			where->Push_Poliz(polka);
			if (current_lexeme.value != ";") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
		}
		catch (string e) {
			lexer.current = prev;
			current_lexeme = prev_lex;
			where->Clear_to_Position(place);
			//cout << prev_lex.value << " " << current_lexeme.value << "\n";
			expression();
			StructPoliz polka;
			polka.type = TypePoliz::separator_;
			where->Push_Poliz(polka);
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
		{
			StructPoliz polka;
			polka.name = "";
			polka.type = TypePoliz::separator_;
			where->Push_Poliz(polka);
		}
		if (current_lexeme.value == ";") {
			{
				StructPoliz polka;
				polka.name = lex.value;
				polka.type = TypePoliz::operation_;
				where->Push_Poliz(polka);
			}
			{
				StructPoliz polka;
				polka.name = "map " + type1 + " " + type2;
				polka.type = TypePoliz::plus_;
				where->Push_Poliz(polka);
			}
			return;
		}
		else if (current_lexeme.value == "=") {
			map_notitle(lex.value);
			{
				StructPoliz polka;
				polka.name = lex.value;
				polka.type = TypePoliz::operation_;
				where->Push_Poliz(polka);
			}
			{
				StructPoliz polka;
				polka.name = "map " + type1 + " " + type2;
				polka.type = TypePoliz::plus_;
				where->Push_Poliz(polka);
			}
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
		{
			StructPoliz polka;
			polka.name = "";
			polka.type = TypePoliz::separator_;
			where->Push_Poliz(polka);
		}
		if (current_lexeme.value == ";") {
			{
				StructPoliz polka;
				polka.name = lex.value;
				polka.type = TypePoliz::operation_;
				where->Push_Poliz(polka);
			}
			{
				StructPoliz polka;
				polka.name = "var " + type1;
				polka.type = TypePoliz::plus_;
				where->Push_Poliz(polka);
			}
			//current_lexeme = lexer.get();
			semantic.Push_ID(lex, type1);
			return;
		}
		if (current_lexeme.value == "=") {
			semantic.Push_ID(lex, type1);
			{
				StructPoliz polka;
				polka.name = lex.value;
				polka.type = TypePoliz::operation_;
				where->Push_Poliz(polka);
			}
			{
				StructPoliz polka;
				polka.name = "var " + type1;
				polka.type = TypePoliz::plus_;
				where->Push_Poliz(polka);
			}
			var_notitle(lex);
		}
		else {
			semantic.Push_ID(lex, "array " +  type1);
			array_notitle(lex.value);
			{
				StructPoliz polka;
				polka.name = lex.value;
				polka.type = TypePoliz::operation_;
				where->Push_Poliz(polka);
			}
			{
				StructPoliz polka;
				polka.name = "array " + type1;
				polka.type = TypePoliz::plus_;
				where->Push_Poliz(polka);
			}
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
	where->Blanc();
	int p1 = where->Get_Position();
	{
		StructPoliz polka;
		polka.name = "F!";
		polka.type = TypePoliz::move_;
		where->Push_Poliz(polka);
	}
	semantic.Check_If(current_lexeme.line);
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	operator_(1);
	if (current_lexeme.value != "else") {
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::adress_;
		polka.value_int = where->Get_Position() + 1;
		where->Write_Position(p1, polka);
		return;
	}
	where->Blanc();
	int p2 = where->Get_Position();
	{
		StructPoliz polka;
		polka.name = "!";
		polka.type = TypePoliz::move_;
		where->Push_Poliz(polka);
	}
	{
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::adress_;
		polka.value_int = where->Get_Position() + 1;
		where->Write_Position(p1, polka);
	}
	current_lexeme = lexer.get();
	operator_(1);
	{
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::adress_;
		polka.value_int = where->Get_Position() + 1;
		where->Write_Position(p2, polka);
	}
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
	int a1 = where->Get_Position() + 1;
	expression();
	where->Blanc();
	int p2 = where->Get_Position();
	{
		StructPoliz polka;
		polka.name = "F!";
		polka.type = TypePoliz::move_;
		where->Push_Poliz(polka);
	}
	semantic.Check_If(current_lexeme.line);
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	operator_(1);
	{
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::adress_;
		polka.value_int = a1;
		where->Push_Poliz(polka);
	}
	{
		StructPoliz polka;
		polka.name = "!";
		polka.type = TypePoliz::move_;
		where->Push_Poliz(polka);
	}
	{
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::adress_;
		polka.value_int = where->Get_Position() + 1;
		where->Write_Position(p2, polka);
	}
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
	{
		StructPoliz polka;
		polka.name = "create tid";
		polka.type = TypePoliz::plus_;
		where->Push_Poliz(polka);
	}
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
	int a1 = where->Get_Position() + 1;
	if (current_lexeme.value != ";") {
		expression();
		semantic.Check_If(current_lexeme.line);
	}
	else {
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::operation_;
		polka.value_int = 1;
		polka.value_char = 1;
		polka.value_float = 1;
		where->Push_Poliz(polka);
	}
	where->Blanc();
	int p2 = where->Get_Position();
	{
		StructPoliz polka;
		polka.name = "F!";
		polka.type = TypePoliz::move_;
		where->Push_Poliz(polka);
	}
	where->Blanc();
	int p1 = where->Get_Position();
	{
		StructPoliz polka;
		polka.name = "!";
		polka.type = TypePoliz::move_;
		where->Push_Poliz(polka);
	}
	if (current_lexeme.value != ";") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	int a2 = where->Get_Position() + 1;
	if (current_lexeme.value != ")") {
		expression();
		semantic.Pop_Stack();
	}
	else {
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::operation_;
		polka.value_int = 1;
		polka.value_float = 1;
		polka.value_char = 1;
		where->Push_Poliz(polka);
	}
	{
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::adress_;
		polka.value_int = a1;
		where->Push_Poliz(polka);
	}
	{
		StructPoliz polka;
		polka.name = "!";
		polka.type = TypePoliz::move_;
		where->Push_Poliz(polka);
	}
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	{
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::adress_;
		polka.value_int = where->Get_Position() + 1;
		where->Write_Position(p1, polka);
	}
	operator_();
	{
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::adress_;
		polka.value_int = a2;
		where->Push_Poliz(polka);
	}
	{
		StructPoliz polka;
		polka.name = "!";
		polka.type = TypePoliz::move_;
		where->Push_Poliz(polka);
	}
	{
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::adress_;
		polka.value_int = where->Get_Position() + 1;
		where->Write_Position(p2, polka);
	}
	semantic.Delete_TID();
	{
		StructPoliz polka;
		polka.name = "delete tid";
		polka.type = TypePoliz::plus_;
		where->Push_Poliz(polka);
	}
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
	{
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::separator_;
		where->Push_Poliz(polka);
	}
	if (current_lexeme.type == 20) {
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::operation_;
		polka.type_number = TypeNumber::string_;
		polka.value_string = current_lexeme.value;
		where->Push_Poliz(polka);
		current_lexeme = lexer.get();
		if (current_lexeme.value != ")") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
	}
	else {
		expression1();
		semantic.Pop_Stack();
		if (current_lexeme.value != ")") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
	}
	{
		StructPoliz polka;
		polka.name = "print";
		polka.type = TypePoliz::plus_;
		where->Push_Poliz(polka);
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
	{
		StructPoliz polka;
		polka.name = "";
		polka.type = TypePoliz::separator_;
		where->Push_Poliz(polka);
	}

	current_lexeme = lexer.get();
	Lexeme lex = current_lexeme;
	semantic.Check_ID(current_lexeme);
	if (current_lexeme.type != 0) {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	{
		StructPoliz polka;
		polka.name = lex.value;
		polka.type = TypePoliz::operation_;
		where->Push_Poliz(polka);
	}
	{
		semantic.Push_Stack(0, lex);
	}
	if (current_lexeme.value == ")") {
		{
			string str = semantic.Check_ID(lex);
			if (str != "int " && str != "float " && str != "char ") {
				throw "Can not input array or map in string " + std::to_string(lex.line);
			}
			StructPoliz polka;
			polka.name = "input";
			polka.type = TypePoliz::plus_;
			where->Push_Poliz(polka);
		}
		current_lexeme = lexer.get();
		return;
	}
	if (current_lexeme.value != "[") {
		throw current_lexeme;
	}
	semantic.Push_Stack(-1, current_lexeme);
	current_lexeme = lexer.get();
	int line = current_lexeme.line;
	expression();
	semantic.Check_Bin(line, where);
	{
		StructPoliz polka;
		polka.name = "input";
		polka.type = TypePoliz::plus_;
		where->Push_Poliz(polka);
	}
	/*StructStack res = semantic.Pop_Stack();
	if (res.type > 2) throw "Inside [] is an invalid type in string " + std::to_string(line);*/
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
	// cout << "map delete: " << current_lexeme.value << '\n';
	if (current_lexeme.type != 0) {
		throw current_lexeme;
	}
	string type_lex = semantic.Check_ID(current_lexeme);
	if (!(type_lex[0] != 'a' && type_lex.size() > 6)) return;
	{
		StructPoliz polka;
		polka.name = current_lexeme.value;
		polka.type = TypePoliz::operation_;
		where->Push_Poliz(polka);
	}
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
	if (res.type > 2) throw "Inside () is an invalid type in string " + std::to_string(line);
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	{
		StructPoliz polka;
		polka.name = "delete";
		polka.type = TypePoliz::plus_;
		where->Push_Poliz(polka);
	}
}

void Parser::expression() {
	//cout << "expr " << current_lexeme.value << "\n";
	expression1();
	while (current_lexeme.value == ",") {
		semantic.Push_Stack(-21, current_lexeme);
		current_lexeme = lexer.get();
		expression1();
		semantic.Check_Bin(current_lexeme.line, where);
	}
}

void Parser::expression1() {
	expression2();
	if (current_lexeme.value == "=") {
		semantic.Push_Stack(-19, current_lexeme);
		current_lexeme = lexer.get();
		expression2();
		semantic.Check_Bin(current_lexeme.line, where);
	}
}

void Parser::expression2() {
	//cout << "expr 2";
	expression3();
	while (current_lexeme.value == "||") {
		semantic.Push_Stack(-18, current_lexeme);
		current_lexeme = lexer.get();
		expression3();
		semantic.Check_Bin(current_lexeme.line, where);
	}
}

void Parser::expression3() {
	//cout << "expr 3";
	expression4();
	while (current_lexeme.value == "&&") {
		semantic.Push_Stack(-17, current_lexeme);
		current_lexeme = lexer.get();
		expression4();
		semantic.Check_Bin(current_lexeme.line, where);
	}
}

void Parser::expression4() {
	//cout << "expr 4";
	expression5();
	if (current_lexeme.value == "==" || current_lexeme.value == "!=") {
		if (current_lexeme.value == "==") {
			semantic.Push_Stack(-11, current_lexeme);
		}
		if (current_lexeme.value == "!=") {
			semantic.Push_Stack(-12, current_lexeme);
		}
		current_lexeme = lexer.get();
		expression5();
		semantic.Check_Bin(current_lexeme.line, where);
	}
}

void Parser::expression5() {
	//cout << "expr 5";
	expression6();
	if (current_lexeme.value == ">" || current_lexeme.value == "<" || current_lexeme.value == ">=" || current_lexeme.value == "<=") {
		if (current_lexeme.value == ">") {
			semantic.Push_Stack(-13, current_lexeme);
		}
		if (current_lexeme.value == "<") {
			semantic.Push_Stack(-14, current_lexeme);
		}
		if (current_lexeme.value == ">=") {
			semantic.Push_Stack(-15, current_lexeme);
		}
		if (current_lexeme.value == "<=") {
			semantic.Push_Stack(-16, current_lexeme);
		}
		current_lexeme = lexer.get();
		expression6();
		semantic.Check_Bin(current_lexeme.line, where);
	}
}

void Parser::expression6() {
	//cout << "expr 6";
	expression7();
	while (current_lexeme.value == "+" || current_lexeme.value == "-") {
		if (current_lexeme.value == "+") {
			semantic.Push_Stack(-6, current_lexeme);
		}
		if (current_lexeme.value == "-") {
			semantic.Push_Stack(-7, current_lexeme);
		}
		current_lexeme = lexer.get();
		expression7();
		semantic.Check_Bin(current_lexeme.line, where);
	}
}

void Parser::expression7() {
	//cout << "expr 7";
	expression8();
	while (current_lexeme.value == "*" || current_lexeme.value == "/" || current_lexeme.value == "%") {
		if (current_lexeme.value == "*") {
			semantic.Push_Stack(-8, current_lexeme);
		}
		if (current_lexeme.value == "/") {
			semantic.Push_Stack(-9, current_lexeme);
		}
		if (current_lexeme.value == "%") {
			semantic.Push_Stack(-23, current_lexeme);
		}
		current_lexeme = lexer.get();
		expression8();
		semantic.Check_Bin(current_lexeme.line, where);
	}
}

void Parser::expression8() {
	//cout << "expr 8";
	if (current_lexeme.value == "!" || current_lexeme.value == "-" || current_lexeme.value == "++" || current_lexeme.value == "--") {
		if (current_lexeme.value == "!") {
			semantic.Push_Stack(-10, current_lexeme);
		}
		if (current_lexeme.value == "-") {
			semantic.Push_Stack(-22, current_lexeme);
		}
		if (current_lexeme.value == "++") {
			semantic.Push_Stack(-4, current_lexeme);
		}
		if (current_lexeme.value == "--") {
			semantic.Push_Stack(-5, current_lexeme);
		}
		current_lexeme = lexer.get();
		expression9();
		semantic.Check_Uno(current_lexeme.line, where);
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
		// cout << "current_lexeme: " << "\"" << current_lexeme.value << "\"" << current_lexeme.type << "\n";
		try {
			type_lex = semantic.Check_ID(current_lexeme);
			//cout << "type: \"" << type_lex << "\"" << '\n';
			{
				StructPoliz polka;
				polka.name = current_lexeme.value;
				polka.type = TypePoliz::operation_;
				where->Push_Poliz(polka);
			}
		}
		catch (std::string e) {
			//cout << "catch\n";
		}
		current_lexeme = lexer.get();
		if (current_lexeme.value == "++" || current_lexeme.value == "--") {
			semantic.Push_Stack(-2 + (current_lexeme.value == "--"), current_lexeme);
			if (type_lex == "int ") {
				semantic.Push_Stack(0, last);
			}
			else if (type_lex == "float ") {
				semantic.Push_Stack(1, last);
			}
			else if (type_lex == "char ") {
				semantic.Push_Stack(2, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "int " || type_lex.substr(type_lex.find(" ", 0) + 1) == "int") {
				if (type_lex[0] == 'a') semantic.Push_Stack(3, last);
				else semantic.Push_Stack(6, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "float " || type_lex.substr(type_lex.find(" ", 0) + 1) == "float") {
				if (type_lex[0] == 'a') semantic.Push_Stack(4, last);
				else semantic.Push_Stack(7, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "char " || type_lex.substr(type_lex.find(" ", 0) + 1) == "char") {
				if (type_lex[0] == 'a') semantic.Push_Stack(5, last);
				else semantic.Push_Stack(8, last);
			}
			else {
				ofstream out("debugging.txt", std::ios::app);
				out << type_lex << "\n";
				out.close();
				throw string("error type");
			}
			// if (last.value == "counter") cout << "BBBBBBBB " << last.value << " " << -2 + (current_lexeme.value == "--") << std::endl;
			semantic.Check_Uno(current_lexeme.line, where);
			current_lexeme = lexer.get();
		}
		else if (current_lexeme.value == "[") {
			if (type_lex == "int ") {
				semantic.Push_Stack(0, last);
			}
			else if (type_lex == "float ") {
				semantic.Push_Stack(1, last);
			}
			else if (type_lex == "char ") {
				semantic.Push_Stack(2, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "int " || type_lex.substr(type_lex.find(" ", 0) + 1) == "int") {
				if (type_lex[0] == 'a') semantic.Push_Stack(3, last);
				else semantic.Push_Stack(6, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "float " || type_lex.substr(type_lex.find(" ", 0) + 1) == "float") {
				if (type_lex[0] == 'a') semantic.Push_Stack(4, last);
				else semantic.Push_Stack(7, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "char " || type_lex.substr(type_lex.find(" ", 0) + 1) == "char") {
				if (type_lex[0] == 'a') semantic.Push_Stack(5, last);
				else semantic.Push_Stack(8, last);
			}
			else {
				ofstream out("debugging.txt", std::ios::app);
				out << type_lex << "\n";
				out.close();
				throw string("error type " + std::to_string(current_lexeme.line));
			}
			semantic.Push_Stack(-1, current_lexeme);
			current_lexeme = lexer.get();
			int line = current_lexeme.line;
			expression();
			semantic.Check_Bin(line, where);
			if (current_lexeme.value != "]") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
		}
		else if (current_lexeme.value == ".") {
			// cout << "type: \"" << type_lex.substr(type_lex.find(" ", 0) + 1) << "\"" << "\n";
			if (type_lex == "int ") {
				semantic.Push_Stack(0, last);
			}
			else if (type_lex == "float ") {
				semantic.Push_Stack(1, last);
			}
			else if (type_lex == "char ") {
				semantic.Push_Stack(2, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "int " || type_lex.substr(type_lex.find(" ", 0) + 1) == "int") {
				if (type_lex[0] == 'a') semantic.Push_Stack(3, last);
				else semantic.Push_Stack(6, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "float " || type_lex.substr(type_lex.find(" ", 0) + 1) == "float") {
				if (type_lex[0] == 'a') semantic.Push_Stack(4, last);
				else semantic.Push_Stack(7, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "char " || type_lex.substr(type_lex.find(" ", 0) + 1) == "char") {
				if (type_lex[0] == 'a') semantic.Push_Stack(5, last);
				else semantic.Push_Stack(8, last);
			}
			else {
				ofstream out("debugging.txt", std::ios::app);
				out << type_lex << "\n";
				out.close();
				throw string("error type");
			}
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
			semantic.Check_Bin(line, where);
			if (current_lexeme.value != ")") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
		}
		else if (current_lexeme.value == "(") {
			int line = current_lexeme.line;
			// cout << "AAAAA " << last.value << std::endl;
			current_lexeme = lexer.get();
			{
				StructPoliz polka;
				polka.name = "";
				polka.type = TypePoliz::separator_;
				where->Push_Poliz(polka);
			}
			if (current_lexeme.value == ")") {
				type_lex = semantic.Check_Call(current_lexeme, last.value);
				if (type_lex == "int ") {
					semantic.Push_Stack(0, last);
				}
				else if (type_lex == "float ") {
					semantic.Push_Stack(1, last);
				}
				else if (type_lex == "char ") {
					semantic.Push_Stack(2, last);
				}
				else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "int " || type_lex.substr(type_lex.find(" ", 0) + 1) == "int") {
					if (type_lex[0] == 'a') semantic.Push_Stack(3, last);
					else semantic.Push_Stack(6, last);
				}
				else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "float " || type_lex.substr(type_lex.find(" ", 0) + 1) == "float") {
					if (type_lex[0] == 'a') semantic.Push_Stack(4, last);
					else semantic.Push_Stack(7, last);
				}
				else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "char " || type_lex.substr(type_lex.find(" ", 0) + 1) == "char") {
					if (type_lex[0] == 'a') semantic.Push_Stack(5, last);
					else semantic.Push_Stack(8, last);
				}
				else {
					ofstream out("debugging.txt", std::ios::app);
					out << type_lex << "\n";
					out.close();
					throw string("error type");
				}
				{
					StructPoliz polka;
					polka.name = last.value;
					polka.type = TypePoliz::operation_;
					where->Push_Poliz(polka);
				}
				{
					StructPoliz polka;
					polka.name = "call";
					polka.type = TypePoliz::plus_;
					where->Push_Poliz(polka);
				}
				current_lexeme = lexer.get();
				return;
			}
			string params = "";
			expression1();
			{
				StructStack res = semantic.Pop_Stack();
				if (res.type < 0) throw "Incorrect parameters in function call in string " + std::to_string(line);
				else if (res.type <= 2) params += "number ";
				else if (res.type <= 5) params += "number[] ";
				else if (res.type <= 8) params += "map ";
			}
			while (current_lexeme.value == ",") {
				current_lexeme = lexer.get();
				expression1();
				StructStack res = semantic.Pop_Stack();
				if (res.type < 0) throw "Incorrect parameters in function call in string " + std::to_string(line);
				else if (res.type <= 2) params += "number ";
				else if (res.type <= 5) params += "number[] ";
				else if (res.type <= 8) params += "map ";
			}
			params.pop_back();
			type_lex = semantic.Check_Call(last, last.value + " " + params);
			// cout << "RARRARARAR\n";
			{
				StructPoliz polka;
				polka.name = last.value + " " + params;
				polka.type = TypePoliz::operation_;
				where->Push_Poliz(polka);
			}
			// cout << "type: \"" << type_lex.substr(type_lex.find(" ", 0) + 1) << "\"" << "\n";
			if (type_lex == "int ") {
				semantic.Push_Stack(0, last);
			}
			else if (type_lex == "float ") {
				semantic.Push_Stack(1, last);
			}
			else if (type_lex == "char ") {
				semantic.Push_Stack(2, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "int " || type_lex.substr(type_lex.find(" ", 0) + 1) == "int") {
				if (type_lex[0] == 'a') semantic.Push_Stack(3, last);
				else semantic.Push_Stack(6, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "float " || type_lex.substr(type_lex.find(" ", 0) + 1) == "float") {
				if (type_lex[0] == 'a') semantic.Push_Stack(4, last);
				else semantic.Push_Stack(7, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "char " || type_lex.substr(type_lex.find(" ", 0) + 1) == "char") {
				if (type_lex[0] == 'a') semantic.Push_Stack(5, last);
				else semantic.Push_Stack(8, last);
			}
			else {
				ofstream out("debugging.txt", std::ios::app);
				out << type_lex << "\n";
				out.close();
				throw string("error type");
			}
			if (current_lexeme.value != ")") {
				throw current_lexeme;
			}
			{
				StructPoliz polka;
				polka.name = "call";
				polka.type = TypePoliz::plus_;
				where->Push_Poliz(polka);
			}
			current_lexeme = lexer.get();
		}
		else {
			// cout << "type: \"" << type_lex.substr(type_lex.find(" ", 0) + 1) << "\"" << "\n";
			if (type_lex == "int ") {
				semantic.Push_Stack(0, last);
			}
			else if (type_lex == "float ") {
				semantic.Push_Stack(1, last);
			}
			else if (type_lex == "char ") {
				semantic.Push_Stack(2, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "int " || type_lex.substr(type_lex.find(" ", 0) + 1) == "int") {
				if (type_lex[0] == 'a') semantic.Push_Stack(3, last);
				else semantic.Push_Stack(6, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "float " || type_lex.substr(type_lex.find(" ", 0) + 1) == "float") {
				if (type_lex[0] == 'a') semantic.Push_Stack(4, last);
				else semantic.Push_Stack(7, last);
			}
			else if (type_lex.substr(type_lex.find(" ", 0) + 1) == "char " || type_lex.substr(type_lex.find(" ", 0) + 1) == "char") {
				if (type_lex[0] == 'a') semantic.Push_Stack(5, last);
				else semantic.Push_Stack(8, last);
			}
			else {
				ofstream out("debugging.txt", std::ios::app);
				out << type_lex << "\n";
				out.close();
				throw string("error type");
			}
		}
	}
	else if (current_lexeme.type == 21 || current_lexeme.type == 22 || current_lexeme.type == 23) {
		if (current_lexeme.type == 21) {
			{
				StructPoliz polka;
				polka.type = TypePoliz::operation_;
				polka.value_char = current_lexeme.value[1];
				if (current_lexeme.value == "\n") {
					polka.value_char = '\n';
				}
				polka.value_int = polka.value_char;
				polka.value_float = polka.value_char;
				polka.type_number = TypeNumber::char_;
				where->Push_Poliz(polka);
			}
			semantic.Push_Stack(2, current_lexeme);
		} else if (current_lexeme.type == 22) {
			{
				StructPoliz polka;
				polka.type = TypePoliz::operation_;
				polka.value_int = stoi(current_lexeme.value);
				polka.value_char = polka.value_int;
				polka.value_float = polka.value_int;
				polka.type_number = TypeNumber::int_;
				where->Push_Poliz(polka);
			}
			semantic.Push_Stack(0, current_lexeme);
		} else {
			{
				StructPoliz polka;
				polka.type = TypePoliz::operation_;
				polka.value_float = stof(current_lexeme.value);
				polka.value_int = polka.value_float;
				polka.value_char = polka.value_float;
				polka.type_number = TypeNumber::float_;
				where->Push_Poliz(polka);
			}
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