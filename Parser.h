#pragma once

#include "LexicalAnalyzer.h"

class Parser
{
public:
	void start();
	Parser() = default;
	Parser(LexicalAnalyzer& lexer) : lexer(lexer) {}

private:
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
	void operator_();
	void description();
	void if_();
	void for_();
	void while_();
	void print();
	void input();
	void map_delete();

	void expression1();
	void expression2();
	void expression3();
	void expression4();
	void expression5();
	void expression6();
	void expression7();
	void expression8();
	void expression9();
};

