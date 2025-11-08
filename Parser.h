#pragma once

#include "LexicalAnalyzer.h"

class Parser
{
	LexicalAnalyzer lexer;
	Lexeme current_lexeme;
	void program();
	void type();
	void var_notitle();
	void array_notitle();
	void function_notitle();
	void expression();
	void parameters();
	void parameters_description();
	void block();
	void map_notitle();
};

