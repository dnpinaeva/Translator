#pragma once

#include "LexicalAnalyzer.h"
#include "SemanticAnalysis.h"
#include "Poliz.h"
#include "Execution.h"

class Parser
{
public:
	void start();
	void execution_function();
	Parser() = default;
	Parser(LexicalAnalyzer& lexer) : lexer(lexer)  {
		where = &poliz;
	}
	Poliz poliz;
	SemanticAnalysis semantic;

private:
	Poliz* where;
	LexicalAnalyzer lexer;
	Lexeme current_lexeme;
	void program();
	void type();
	void var_notitle(const Lexeme& lex);
	void array_notitle(const string& name);
	void function_notitle(const Lexeme& lex, const string& type1, string type2 = "");
	void expression();
	string parameters(vector<StructTid>& params);
	string parameters_description(vector<StructTid>& params);
	void block(bool is_tid_needed);
	void map_notitle(const string& name);
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

