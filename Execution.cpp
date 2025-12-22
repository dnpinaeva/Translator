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
			StructValue v = *semantic.Get_Value_ID(one.name);
			if (v.type_value == TypeValue::Int) {
				if (type1 == "int") {
					v1.value_int = (int)v.value_int;
					v1.type_number = TypeNumber::int_;
				}
				else if (type1 == "char") {
					v1.value_char = (char)v.value_int;
					v1.type_number = TypeNumber::char_;
				}
				else {
					v1.value_float = (float)v.value_int;
					v1.type_number = TypeNumber::float_;
				}
			}
			else if (v.type_value == TypeValue::Char) {
				if (type1 == "int") {
					v1.value_int = (int)v.value_char;
					v1.type_number = TypeNumber::int_;
				}
				else if (type1 == "char") {
					v1.value_char = (char)v.value_char;
					v1.type_number = TypeNumber::char_;
				}
				else {
					v1.value_float = (float)v.value_char;
					v1.type_number = TypeNumber::float_;
				}
			}
			else {
				if (type1 == "int") {
					v1.value_int = (int)v.value_float;
					v1.type_number = TypeNumber::int_;
				}
				else if (type1 == "char") {
					v1.value_char = (char)v.value_float;
					v1.type_number = TypeNumber::char_;
				}
				else {
					v1.value_float = (float)v.value_float;
					v1.type_number = TypeNumber::float_;
				}
			}
		}
		else {
			v1 = one;
		}

		StructPoliz v2;
		if (two.name != "") {
			StructValue v = *semantic.Get_Value_ID(two.name);
			if (v.type_value == TypeValue::Int) {
				if (type2 == "int") {
					v2.value_int = (int)v.value_int;
					v2.type_number = TypeNumber::int_;
				}
				else if (type2 == "char") {
					v2.value_char = (char)v.value_int;
					v2.type_number = TypeNumber::char_;
				}
				else {
					v2.value_float = (float)v.value_int;
					v2.type_number = TypeNumber::float_;
				}
			}
			else if (v.type_value == TypeValue::Char) {
				if (type2 == "int") {
					v2.value_int = (int)v.value_char;
					v2.type_number = TypeNumber::int_;
				}
				else if (type2 == "char") {
					v2.value_char = (char)v.value_char;
					v2.type_number = TypeNumber::char_;
				}
				else {
					v2.value_float = (float)v.value_char;
					v2.type_number = TypeNumber::float_;
				}
			}
			else {
				if (type2 == "int") {
					v2.value_int = (int)v.value_float;
					v2.type_number = TypeNumber::int_;
				}
				else if (type2 == "char") {
					v2.value_char = (char)v.value_float;
					v2.type_number = TypeNumber::char_;
				}
				else {
					v2.value_float = (float)v.value_float;
					v2.type_number = TypeNumber::float_;
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
				StructValue v = *semantic.Get_Value_ID(one.name);
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
				StructValue v = *semantic.Get_Value_ID(one.name);
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
				StructValue v = *semantic.Get_Value_ID(one.name);
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

StructPoliz Execution::get_operation_value() {         // Add arrays
	StructPoliz one = operations.back();
	operations.pop_back();
	if (one.name != "") {
		StructValue v = *semantic.Get_Value_ID(one.name);
		if (v.type_value == TypeValue::Int) {
			one.type_number = TypeNumber::int_;
			one.value_int = v.value_int;
		}
		else if (v.type_value == TypeValue::Char) {
			one.type_number = TypeNumber::char_;
			one.value_char = v.value_char;
		}
		else if (v.type_value == TypeValue::Float) {
			one.type_number = TypeNumber::float_;
			one.value_float = v.value_float;
		}
	}
	one.name = "";
	return one;
}

void Execution::expression_(const Poliz& poliz, int& i) {
	string op = poliz.Get_Value(i).name;
	if (op == "[]") { // How to store it as an lvalue ???
		StructPoliz two = get_operation_value();
		StructPoliz one = operations.back();
		operations.pop_back();
		StructValue v = *semantic.Get_Value_ID(one.name);
		if (v.type_value == TypeValue::ArrayInt) {
			one.type_number = TypeNumber::int_;
		}
		if (v.type_value == TypeValue::ArrayChar) {
			one.type_number = TypeNumber::char_;
		}
		if (v.type_value == TypeValue::ArrayFloat) {
			one.type_number = TypeNumber::float_;
		}

		if (two.type_number == TypeNumber::float_) {
			one.value_int = two.value_float;
		}
		if (two.type_number == TypeNumber::int_) {
			one.value_int = two.value_int;
		}
		if (two.type_number == TypeNumber::char_) {
			one.value_int = two.value_char;
		}
	}
	else if (op == "a++") {
		StructPoliz one = operations.back();
		operations.pop_back();
		if (one.name == "") {
			throw "++ to not lvalue";
		}
		StructValue* v = semantic.Get_Value_ID(one.name);
		if (v->type_value == TypeValue::Int) {

		}
	}
	else if (op == "a--") {

	}
	else if (op == "++a") {

	}
	else if (op == "--a") {

	}
	else if (op == "+") {
		StructPoliz two = get_operation_value();
		StructPoliz one = get_operation_value();
		StructPoliz res;
		res.type = TypePoliz::operation_;
		if (one.type_number == TypeNumber::float_ || two.type_number == TypeNumber::float_) {
			res.type_number = TypeNumber::float_;
			res.value_float = one.value_float + two.value_float;
			res.value_char = res.value_float;
			res.value_int = res.value_float;
			operations.push_back(res);
			return;
		}
		if (one.type_number == TypeNumber::int_ || two.type_number == TypeNumber::int_) {
			res.type_number = TypeNumber::int_;
			res.value_int = one.value_int + two.value_int;
			res.value_char = res.value_int;
			res.value_float = res.value_int;
			operations.push_back(res);
			return;
		}
		res.type_number = TypeNumber::char_;
		res.value_char = one.value_char + two.value_char;
		res.value_float = res.value_char;
		res.value_int = res.value_char;
		operations.push_back(res);
		return;
	}
	else if (op == "-") {
		StructPoliz two = get_operation_value();
		StructPoliz one = get_operation_value();
		StructPoliz res;
		res.type = TypePoliz::operation_;
		if (one.type_number == TypeNumber::float_ || two.type_number == TypeNumber::float_) {
			res.type_number = TypeNumber::float_;
			res.value_float = one.value_float - two.value_float;
			res.value_char = res.value_float;
			res.value_int = res.value_float;
			operations.push_back(res);
			return;
		}
		if (one.type_number == TypeNumber::int_ || two.type_number == TypeNumber::int_) {
			res.type_number = TypeNumber::int_;
			res.value_int = one.value_int - two.value_int;
			res.value_char = res.value_int;
			res.value_float = res.value_int;
			operations.push_back(res);
			return;
		}
		res.type_number = TypeNumber::char_;
		res.value_char = one.value_char - two.value_char;
		res.value_float = res.value_char;
		res.value_int = res.value_char;
		operations.push_back(res);
		return;
	}
	else if (op == "*") {
		StructPoliz two = get_operation_value();
		StructPoliz one = get_operation_value();
		StructPoliz res;
		res.type = TypePoliz::operation_;
		if (one.type_number == TypeNumber::float_ || two.type_number == TypeNumber::float_) {
			res.type_number = TypeNumber::float_;
			res.value_float = one.value_float * two.value_float;
			res.value_char = res.value_float;
			res.value_int = res.value_float;
			operations.push_back(res);
			return;
		}
		if (one.type_number == TypeNumber::int_ || two.type_number == TypeNumber::int_) {
			res.type_number = TypeNumber::int_;
			res.value_int = one.value_int * two.value_int;
			res.value_char = res.value_int;
			res.value_float = res.value_int;
			operations.push_back(res);
			return;
		}
		res.type_number = TypeNumber::char_;
		res.value_char = one.value_char * two.value_char;
		res.value_float = res.value_char;
		res.value_int = res.value_char;
		operations.push_back(res);
		return;
	}
	else if (op == "/") {
		StructPoliz two = get_operation_value();
		StructPoliz one = get_operation_value();
		StructPoliz res;
		res.type = TypePoliz::operation_;
		if (one.type_number == TypeNumber::float_ || two.type_number == TypeNumber::float_) {
			res.type_number = TypeNumber::float_;
			res.value_float = one.value_float / two.value_float;
			res.value_char = res.value_float;
			res.value_int = res.value_float;
			operations.push_back(res);
			return;
		}
		if (one.type_number == TypeNumber::int_ || two.type_number == TypeNumber::int_) {
			res.type_number = TypeNumber::int_;
			res.value_int = one.value_int / two.value_int;
			res.value_char = res.value_int;
			res.value_float = res.value_int;
			operations.push_back(res);
			return;
		}
		res.type_number = TypeNumber::char_;
		res.value_char = one.value_char / two.value_char;
		res.value_float = res.value_char;
		res.value_int = res.value_char;
		operations.push_back(res);
		return;
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
				semantic.Create_TID();
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