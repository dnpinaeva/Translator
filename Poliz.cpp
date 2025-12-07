#include "Poliz.h"

void Poliz::Push_Poliz(StructPoliz push) {
	data_.push_back(push);
}

void Poliz::Blanc() {
	data_.push_back(StructPoliz());
}

void Poliz::Write_Position(int position, StructPoliz to) {
	data_[position] = to;
}