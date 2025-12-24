#ifndef EXECUTION_H
#define EXECUTION_H
#pragma once

#include <vector>
#include "Poliz.h"
#include "SemanticAnalysis.h"

using namespace std;

class Execution
{
public:
	StructValue Get(const Poliz& poliz, string type);
	Execution(SemanticAnalysis& semantic): semantic(semantic) {
	}
private:
	SemanticAnalysis semantic;
	vector<StructPoliz> operations;
	StructValue return_(const Poliz& poliz, int i, string type);
	void map_(const Poliz& poliz, int i);
	void array_(const Poliz& poliz, int i);
	void var_(const Poliz& poliz, int i);
	void expression_(const Poliz& poliz, int i);
	void input_(const Poliz& poliz, int i);
	void print_(const Poliz& poliz, int i);
	void delete_(const Poliz& poliz, int i);
	void call_(const Poliz& poliz, int i);

	StructPoliz get_operation_rvalue();
};

#endif EXECUTION_H