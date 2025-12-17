#pragma once

#include "LexicalAnalyzer.h"
#include "SemanticAnalysis.h"
#include "Poliz.h"

class Parser
{
public:
	void start();
	Parser() = default;
	Parser(LexicalAnalyzer& lexer) : lexer(lexer) {}

private:
	Poliz* where;
	LexicalAnalyzer lexer;
	SemanticAnalysis semantic;
	Lexeme current_lexeme;
	Poliz poliz;
	void program();
	void type();
	void var_notitle(const Lexeme& lex);
	void array_notitle();
	void function_notitle(const Lexeme& lex, const string& type1, string type2 = "");
	void expression();
	string parameters();
	string parameters_description();
	void block(bool is_tid_needed);
	void map_notitle();
	void operator_(bool is_tid_needed = 0);
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

