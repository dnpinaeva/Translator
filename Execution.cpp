#include "Execution.h"
#include "Poliz.h"

void Execution::return_(const Poliz& poliz, int& i) {

}

void Execution::map_(const Poliz& poliz, int& i) {

}

void Execution::array_(const Poliz& poliz, int& i) {

}

void Execution::var_(const Poliz& poliz, int& i) {

}

void Execution::expression_(const Poliz& poliz, int& i) {

}

void Execution::input_(const Poliz& poliz, int& i) {

}

void Execution::print_(const Poliz& poliz, int& i) {

}

void Execution::delete_(const Poliz& poliz, int& i) {

}

void Execution::call_(const Poliz& poliz, int& i) {

}

StructValue Execution::Get(const Poliz& poliz) {
	int i = 0;
	for (; i < poliz.data_.size();++i) {
		auto el = poliz.data_[i];
		if (el.type == TypePoliz::separator_ || el.type == TypePoliz::adress_) {
			++i;
			continue;
		}
		if (el.type == TypePoliz::move_) {
			i = poliz.data_[i - 1].value_int;
			continue;
		}
		if (el.type == TypePoliz::operation_) {
			operations.push_back(el);
			continue;
		}
		if (el.type == TypePoliz::plus_) {
			if (el.name == "return") {
				return_(poliz, i);
				continue;
			}
			else if (el.name.substr(0, 4) == "map ") {
				map_(poliz, i);
				continue;
			}
			else if (el.name.substr(0, 4) == "var ") {
				var_(poliz, i);
				continue;
			}
			else if (el.name.substr(0, 6) == "array ") {
				array_(poliz, i);
				continue;
			}
			else if (el.name == "input") {
				input_(poliz, i);
				continue;
			}
			else if (el.name == "print") {
				print_(poliz, i);
				continue;
			}
			else if (el.name == "delete") {
				delete_(poliz, i);
				continue;
			}
			else if (el.name == "call") {
				call_(poliz, i);
				continue;
			}
			else {
				expression_(poliz, i);
				continue;
			}
		}
	}
	return StructValue();
}