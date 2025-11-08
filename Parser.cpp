#include "Parser.h"

/* при первом заходе в функцию
* и при возвращении в неё из других функций считается,
* что лексема уже считана
*/


void Parser::program() {
	while (current_lexeme.type != 6) {
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