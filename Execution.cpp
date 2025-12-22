#include "Execution.h"
#include "Poliz.h"
#include <string>

using std::string;

StructValue Execution::return_(const Poliz& poliz, int& i) {

	semantic.Delete_TID();
	return StructValue();
}

void Execution::map_(const Poliz& poliz, int& i) {
	string type1 = poliz.data_[i].name.substr(4, poliz.data_[i].name.find(" ", 4) - 5);
	string type2 = poliz.data_[i].name.substr(poliz.data_[i].name.find(" ", 4) + 1);
	StructPoliz name = operations.back();
	operations.pop_back();
	semantic.Push_ID(name.name, type1, type2);
	StructValue val;
	while (operations.back().type != TypePoliz::separator_) {
		StructPoliz two = operations.back();
		operations.pop_back();
		StructPoliz one = operations.back();
		operations.pop_back();
		StructPoliz v1;
		if (one.name != "") {
			StructValue v = semantic.Get_Value_ID(one.name);
			if (v.type_value == TypeValue::Int) {
				if (type1 == "int") {
					v1.value_int = (int)v.value_int;
				}
				else if (type1 == "char") {
					v1.value_char = (char)v.value_int;
				}
				else {
					v1.value_float = (float)v.value_int;
				}
			}
			else if (v.type_value == TypeValue::Char) {
				if (type1 == "int") {
					v1.value_int = (int)v.value_char;
				}
				else if (type1 == "char") {
					v1.value_char = (char)v.value_char;
				}
				else {
					v1.value_float = (float)v.value_char;
				}
			}
			else {
				if (type1 == "int") {
					v1.value_int = (int)v.value_float;
				}
				else if (type1 == "char") {
					v1.value_char = (char)v.value_float;
				}
				else {
					v1.value_float = (float)v.value_float;
				}
			}
		}
		else {
			v1 = one;
		}

		StructPoliz v2;
		if (two.name != "") {
			StructValue v = semantic.Get_Value_ID(two.name);
			if (v.type_value == TypeValue::Int) {
				if (type2 == "int") {
					v2.value_int = (int)v.value_int;
				}
				else if (type2 == "char") {
					v2.value_char = (char)v.value_int;
				}
				else {
					v2.value_float = (float)v.value_int;
				}
			}
			else if (v.type_value == TypeValue::Char) {
				if (type2 == "int") {
					v2.value_int = (int)v.value_char;
				}
				else if (type2 == "char") {
					v2.value_char = (char)v.value_char;
				}
				else {
					v2.value_float = (float)v.value_char;
				}
			}
			else {
				if (type2 == "int") {
					v2.value_int = (int)v.value_float;
				}
				else if (type2 == "char") {
					v2.value_char = (char)v.value_float;
				}
				else {
					v2.value_float = (float)v.value_float;
				}
			}
		}
		else {
			v2 = two;
		}

		if (type1 == "int") {
			if (type2 == "int") {
				val.value_int_int.insert(v1.value_int, v2.value_int);
			}
			else if (type2 == "char") {
				val.value_int_char.insert(v1.value_int, v2.value_char);
			}
			else {
				val.value_int_float.insert(v1.value_int, v2.value_float);
			}
		}
		else if (type1 == "char") {
			if (type2 == "int") {
				val.value_char_int.insert(v1.value_char, v2.value_int);
			}
			else if (type2 == "char") {
				val.value_char_char.insert(v1.value_char, v2.value_char);
			}
			else {
				val.value_char_float.insert(v1.value_char, v2.value_float);
			}
		}
		else {
			if (type2 == "int") {
				val.value_float_int.insert(v1.value_float, v2.value_int);
			}
			else if (type2 == "char") {
				val.value_float_char.insert(v1.value_float, v2.value_char);
			}
			else {
				val.value_float_float.insert(v1.value_float, v2.value_float);
			}
		}
	}
	operations.pop_back();
	semantic.Push_Value_TID(name.name, val);
}

void Execution::array_(const Poliz& poliz, int& i) {
	string type1 = poliz.data_[i].name.substr(4, poliz.data_[i].name.find(" ", 4) - 5);
	StructPoliz name = operations.back();
	operations.pop_back();
	semantic.Push_ID(name.name, type1);
	StructValue val;
	while (operations.back().type != TypePoliz::separator_) {
		StructPoliz one = operations.back();
		operations.pop_back();
		if (type1 == "int") {
			if (one.name != "") {
				StructValue v = semantic.Get_Value_ID(one.name);
				if (v.type_value == TypeValue::Int) {
					val.value_array_int.push_back((int)v.value_int);
				}
				else if (v.type_value == TypeValue::Char) {
					val.value_array_int.push_back((int)v.value_char);
				}
				else {
					val.value_array_int.push_back((int)v.value_float);
				}
			}
			else {
				val.value_array_int.push_back(one.value_int);
			}
		}
		else if (type1 == "char") {
			if (one.name != "") {
				StructValue v = semantic.Get_Value_ID(one.name);
				if (v.type_value == TypeValue::Int) {
					val.value_array_char.push_back((char)v.value_int);
				}
				else if (v.type_value == TypeValue::Char) {
					val.value_array_char.push_back((char)v.value_char);
				}
				else {
					val.value_array_char.push_back((char)v.value_float);
				}
			}
			else {
				val.value_array_char.push_back(one.value_char);
			}
		}
		else {
			if (one.name != "") {
				StructValue v = semantic.Get_Value_ID(one.name);
				if (v.type_value == TypeValue::Int) {
					val.value_array_float.push_back((float)v.value_int);
				}
				else if (v.type_value == TypeValue::Char) {
					val.value_array_float.push_back((float)v.value_char);
				}
				else {
					val.value_array_float.push_back((float)v.value_float);
				}
			}
			else {
				val.value_array_float.push_back(one.value_float);
			}
		}
	}
	int len;
	if (type1 == "int") {
		len = val.value_array_int.back();
		val.value_array_int.pop_back();
		if (len != val.value_array_int.size()) {
			throw "Incorrect array lenth";
		}
	}
	else if (type1 == "char") {
		len = val.value_array_char.back();
		val.value_array_char.pop_back();
		if (len != val.value_array_char.size()) {
			throw "Incorrect array lenth";
		}
	}
	else {
		len = val.value_array_float.back();
		val.value_array_float.pop_back();
		if (len != val.value_array_float.size()) {
			throw "Incorrect array lenth";
		}
	}
	operations.pop_back();
	semantic.Push_Value_TID(name.name, val);
}

void Execution::var_(const Poliz& poliz, int i) {
	string type1 = poliz.data_[i].name.substr(4, poliz.data_[i].name.find(" ", 4) - 5);
	StructPoliz name = operations.back();
	operations.pop_back();
	semantic.Push_ID(name.name, type1);

}

void Execution::expression_(const Poliz& poliz, int& i) {
	if (poliz.Get_Value(i).name == "[]") {

	}
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
	semantic.Create_TID();
	int i = 0;
	for (; i < poliz.data_.size();++i) {
		auto el = poliz.data_[i];
		if (el.type == TypePoliz::separator_ || el.type == TypePoliz::adress_) {
			if (el.type == TypePoliz::separator_) {
				operations.push_back(el);
			}
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
				return return_(poliz, i);
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
	semantic.Delete_TID();
	return StructValue();
}