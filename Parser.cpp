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
	while (current_lexeme.type != 6) {
		if (current_lexeme.value == "map") {
			//cout << "bbbbbbbbbbbbb map\n";
			current_lexeme = lexer.get();
			if (current_lexeme.value != "<") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			type();
			if (current_lexeme.value != ",") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			type();
			if (current_lexeme.value != ">") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			if (current_lexeme.type != 0) {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			//cout << "current " << current_lexeme.value << "\n";
			if (current_lexeme.value == ";") {
				current_lexeme = lexer.get();
			}
			else if (current_lexeme.value == "=") {
				map_notitle();
				if (current_lexeme.value != ";") {
					throw current_lexeme;
				}
				current_lexeme = lexer.get();
			}
			else {
				function_notitle();
			}
		}
		else if (current_lexeme.value == "void") {
			current_lexeme = lexer.get();
			if (current_lexeme.type != 0) {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			function_notitle();
		}
		else {
			type();
			if (current_lexeme.type != 0) {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			if (current_lexeme.value == ";") {
				current_lexeme = lexer.get();
			}
			else if (current_lexeme.value == "=") {
				var_notitle();
				if (current_lexeme.value != ";") {
					throw current_lexeme;
				}
				current_lexeme = lexer.get();
			}
			else if (current_lexeme.value == "[") {
				array_notitle();
				if (current_lexeme.value != ";") {
					throw current_lexeme;
				}
				current_lexeme = lexer.get();
			}
			else {
				function_notitle();
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

void Parser::var_notitle() {
	if (current_lexeme.value == "=") {
		current_lexeme = lexer.get();
		expression();
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
	while (current_lexeme.value == ",") {
		current_lexeme = lexer.get();
		expression();
	}
	if (current_lexeme.value != "}") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	return;
}

void Parser::function_notitle() {
	if (current_lexeme.value != "(") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != ")") {
		parameters();
	}
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	block();
}

void Parser::parameters() {
	parameters_description();
	while (current_lexeme.value == ",") {
		current_lexeme = lexer.get();
		parameters_description();
	}
	return;
}

void Parser::parameters_description() {
	if (current_lexeme.value == "map") {
		current_lexeme = lexer.get();
		if (current_lexeme.value != "<") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		type();
		if (current_lexeme.value != ",") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		type();
		if (current_lexeme.value != ">") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		if (current_lexeme.type != 0) {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
	}
	else {
		type();
		if (current_lexeme.type != 0) {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		if (current_lexeme.value != "[") {
			return;
		}
		current_lexeme = lexer.get();
		if (current_lexeme.value != "]") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
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
	if (current_lexeme.value != ",") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	//cout << "aaaaaaaaaaaaa\n";
	expression1();
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
		if (current_lexeme.value != ",") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		expression1();
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

void Parser::block() {
	if (current_lexeme.value != "{") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	while (current_lexeme.value != "}") {
		operator_();
	}
	current_lexeme = lexer.get();
}

void Parser::operator_() {
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
		block();
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
		type();
		if (current_lexeme.value != ",") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		type();
		if (current_lexeme.value != ">") {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		if (current_lexeme.type != 0) {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
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
		type();
		if (current_lexeme.type != 0) {
			throw current_lexeme;
		}
		current_lexeme = lexer.get();
		if (current_lexeme.value == ";") {
			//current_lexeme = lexer.get();
			return;
		}
		if (current_lexeme.value == "=") {
			var_notitle();
		}
		else {
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
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	operator_();
	if (current_lexeme.value != "else") {
		return;
	}
	current_lexeme = lexer.get();
	operator_();
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
	if (current_lexeme.value != ")") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	operator_();
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
	}
	if (current_lexeme.value != ";") {
		throw current_lexeme;
	}
	current_lexeme = lexer.get();
	if (current_lexeme.value != ")") {
		expression();
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
	expression();
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
	expression();
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
		current_lexeme = lexer.get();
		if (current_lexeme.value == "++" || current_lexeme.value == "--") {
			current_lexeme = lexer.get();
		}
		else if (current_lexeme.value == "[") {
			current_lexeme = lexer.get();
			expression();
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
			current_lexeme = lexer.get();
			if (current_lexeme.value != "(") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
			expression();
			if (current_lexeme.value != ")") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
		}
		else if (current_lexeme.value == "(") {
			current_lexeme = lexer.get();
			if (current_lexeme.value == ")") {
				current_lexeme = lexer.get();
				return;
			}
			expression();
			while (current_lexeme.value == ",") {
				current_lexeme = lexer.get();
				expression();
			}
			if (current_lexeme.value != ")") {
				throw current_lexeme;
			}
			current_lexeme = lexer.get();
		}
	}
	else if (current_lexeme.type == 21 || current_lexeme.type == 22) {
		current_lexeme = lexer.get();
	}
	else {
		throw current_lexeme;
	}
}