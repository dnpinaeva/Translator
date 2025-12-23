#include "Execution.h"
#include "Poliz.h"
#include <string>
#include <algorithm>

using std::string;

StructValue Execution::return_(const Poliz& poliz, int i) {

	semantic.Delete_TID();
	return StructValue();
}

void Execution::map_(const Poliz& poliz, int i) {
	string type1 = poliz.data_[i].name.substr(4, poliz.data_[i].name.find(" ", 4) - 5);
	string type2 = poliz.data_[i].name.substr(poliz.data_[i].name.find(" ", 4) + 1);
	StructPoliz name = operations.back();
	operations.pop_back();
	semantic.Push_ID(name.name, type1, type2);
	StructValue val;
	while (operations.back().type != TypePoliz::separator_) {
		StructPoliz v2 = get_operation_rvalue();
		StructPoliz v1 = get_operation_rvalue();
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

void Execution::array_(const Poliz& poliz, int i) {
	string type1 = poliz.data_[i].name.substr(4, poliz.data_[i].name.find(" ", 4) - 5);
	StructPoliz name = operations.back();
	operations.pop_back();
	semantic.Push_ID(name.name, type1);
	StructValue val;
	int sz = (int)operations.size();
	while (operations[sz-- - 2].type != TypePoliz::separator_) {
		StructPoliz one = get_operation_rvalue();
		if (type1 == "int") {
			val.value_array_int.push_back(one.value_int);
		}
		else if (type1 == "char") {
			val.value_array_char.push_back(one.value_char);
		}
		else {
			val.value_array_float.push_back(one.value_float);
		}
	}
	int len = operations.back().value_int;
	operations.pop_back();
	operations.pop_back();
	if (type1 == "int") {
		if (len != val.value_array_int.size()) throw "Incorrect len of array " + poliz.data_[i].name;
	}
	if (type1 == "float") {
		if (len != val.value_array_float.size()) throw "Incorrect len of array " + poliz.data_[i].name;
	}
	if (type1 == "char") {
		if (len != val.value_array_char.size()) throw "Incorrect len of array " + poliz.data_[i].name;
	}
	semantic.Push_Value_TID(name.name, val);
}

void Execution::var_(const Poliz& poliz, int i) {
	string type1 = poliz.data_[i].name.substr(4, poliz.data_[i].name.find(" ", 4) - 5);
	StructPoliz name = operations.back();
	operations.pop_back();
	semantic.Push_ID(name.name, type1);
}

StructPoliz Execution::get_operation_rvalue() {       
	StructPoliz one = operations.back();
	operations.pop_back();
	if (one.name != "") {
		StructValue v = *semantic.Get_Value_ID(one.name);
		if (v.type_value == TypeValue::Int) {
			one.type_number = TypeNumber::int_;
			one.value_int = v.value_int;
			one.value_char = v.value_int;
			one.value_float = v.value_int;
		}
		else if (v.type_value == TypeValue::Char) {
			one.type_number = TypeNumber::char_;
			one.value_char = v.value_char;
			one.value_float = v.value_char;
			one.value_int = v.value_char;
		}
		else if (v.type_value == TypeValue::Float) {
			one.type_number = TypeNumber::float_;
			one.value_float = v.value_float;
			one.value_int = v.value_float;
			one.value_char = v.value_float;
		} else if (v.type_value == TypeValue::ArrayInt) {
			if (v.value_array_int.size() <= one.value_int) {
				throw "Array index out of range";
			}
			one.type_number = TypeNumber::int_;
			one.value_int = v.value_array_int[one.value_int];
			one.value_char = one.value_int;
			one.value_float = one.value_int;
		} else if (v.type_value == TypeValue::ArrayChar) {
			if (v.value_array_char.size() <= one.value_int) {
				throw "Array index out of range";
			}
			one.type_number = TypeNumber::char_;
			one.value_char = v.value_array_char[one.value_int];
			one.value_int = one.value_char;
			one.value_float = one.value_char;
		} else if (v.type_value == TypeValue::ArrayFloat) {
			if (v.value_array_float.size() <= one.value_int) {
				throw "Array index out of range";
			}
			one.type_number = TypeNumber::char_;
			one.value_float = v.value_array_float[one.value_int];
			one.value_int = one.value_float;
			one.value_char = one.value_float;
		} else if (v.type_value == TypeValue::MapIntInt) {
			if (!v.value_int_int.find(one.value_int)) {
				throw "This key in map doesn't exist";
			}
			one.type_number = TypeNumber::int_;
			one.value_int = v.value_int_int.get(one.value_int);
			one.value_char = one.value_int;
			one.value_float = one.value_int;
		}
		else if (v.type_value == TypeValue::MapIntFloat) {
			if (!v.value_int_float.find(one.value_int)) {
				throw "This key in map doesn't exist";
			}
			one.type_number = TypeNumber::float_;
			one.value_float = v.value_int_float.get(one.value_int);
			one.value_char = one.value_float;
			one.value_int = one.value_float;
		}
		else if (v.type_value == TypeValue::MapIntChar) {
			if (!v.value_int_char.find(one.value_int)) {
				throw "This key in map doesn't exist";
			}
			one.type_number = TypeNumber::char_;
			one.value_char = v.value_int_char.get(one.value_int);
			one.value_float = one.value_char;
			one.value_int = one.value_char;
		}
		else if (v.type_value == TypeValue::MapCharInt) {
			if (!v.value_char_int.find(one.value_char)) {
				throw "This key in map doesn't exist";
			}
			one.type_number = TypeNumber::int_;
			one.value_int = v.value_char_int.get(one.value_char);
			one.value_char = one.value_int;
			one.value_float = one.value_int;
		}
		else if (v.type_value == TypeValue::MapCharFloat) {
			if (!v.value_char_float.find(one.value_char)) {
				throw "This key in map doesn't exist";
			}
			one.type_number = TypeNumber::float_;
			one.value_float = v.value_char_float.get(one.value_char);
			one.value_char = one.value_float;
			one.value_int = one.value_float;
		}
		else if (v.type_value == TypeValue::MapCharChar) {
			if (!v.value_char_char.find(one.value_char)) {
				throw "This key in map doesn't exist";
			}
			one.type_number = TypeNumber::char_;
			one.value_char = v.value_char_char.get(one.value_char);
			one.value_float = one.value_char;
			one.value_int = one.value_char;
		}
		else if (v.type_value == TypeValue::MapFloatInt) {
			if (!v.value_float_int.find(one.value_float)) {
				throw "This key in map doesn't exist";
			}
			one.type_number = TypeNumber::int_;
			one.value_int = v.value_float_int.get(one.value_float);
			one.value_char = one.value_int;
			one.value_float = one.value_int;
		}
		else if (v.type_value == TypeValue::MapFloatFloat) {
			if (!v.value_float_float.find(one.value_float)) {
				throw "This key in map doesn't exist";
			}
			one.type_number = TypeNumber::float_;
			one.value_float = v.value_float_float.get(one.value_float);
			one.value_char = one.value_float;
			one.value_int = one.value_float;
		}
		else if (v.type_value == TypeValue::MapFloatChar) {
			if (!v.value_float_char.find(one.value_float)) {
				throw "This key in map doesn't exist";
			}
			one.type_number = TypeNumber::char_;
			one.value_char = v.value_float_char.get(one.value_float);
			one.value_float = one.value_char;
			one.value_int = one.value_char;
		}
	}
	one.name = "";
	return one;
}

void Execution::expression_(const Poliz& poliz, int i) {
	string op = poliz.Get_Value(i).name;
	if (op == "[]") { // How to store it as an lvalue ???
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = operations.back();
		operations.pop_back();
		StructValue v = *semantic.Get_Value_ID(one.name);
		if (v.type_value == TypeValue::ArrayInt || v.type_value == TypeValue::MapIntInt || v.type_value == TypeValue::MapFloatInt || v.type_value == TypeValue::MapCharInt) {
			one.type_number = TypeNumber::int_;
		}
		if (v.type_value == TypeValue::ArrayChar || v.type_value == TypeValue::MapIntChar || v.type_value == TypeValue::MapFloatChar || v.type_value == TypeValue::MapCharChar) {
			one.type_number = TypeNumber::char_;
		}
		if (v.type_value == TypeValue::ArrayFloat || v.type_value == TypeValue::MapIntFloat || v.type_value == TypeValue::MapFloatFloat || v.type_value == TypeValue::MapCharFloat) {
			one.type_number = TypeNumber::float_;
		}
		if (two.type_number == TypeNumber::float_) {
			one.value_int = two.value_float;
			one.value_float = two.value_float;
			one.value_char = two.value_float;
		}
		if (two.type_number == TypeNumber::int_) {
			one.value_int = two.value_int;
			one.value_float = two.value_int;
			one.value_char = two.value_int;
		}
		if (two.type_number == TypeNumber::char_) {
			one.value_int = two.value_char;
			one.value_float = two.value_char;
			one.value_char = two.value_char;
		}
		operations.push_back(one);
	}
	else if (op == "a++") {
		StructPoliz one = operations.back();
		StructPoliz two = get_operation_rvalue();
		operations.push_back(two);
		if (one.name == "") {
			throw "++ to not lvalue";
		}
		StructValue* v = semantic.Get_Value_ID(one.name);
		if (v->type_value == TypeValue::Int || v->type_value == TypeValue::Char || v->type_value == TypeValue::Float) {
			v->value_char++;
			v->value_float += 1;
			v->value_int++;
			return;
		}
		if (v->type_value == TypeValue::ArrayInt) {
			v->value_array_int[one.value_int]++;
			return;
		}
		if (v->type_value == TypeValue::ArrayChar) {
			v->value_array_char[one.value_int]++;
			return;
		}
		if (v->type_value == TypeValue::ArrayFloat) {
			v->value_array_float[one.value_int] += 1;
			return;
		}
		if (v->type_value == TypeValue::MapIntInt) {
			int litter = v->value_int_int.get(one.value_int);
			v->value_int_int.insert(one.value_int, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatInt) {
			int litter = v->value_float_int.get(one.value_float);
			v->value_float_int.insert(one.value_float, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharInt) {
			int litter = v->value_char_int.get(one.value_char);
			v->value_char_int.insert(one.value_char, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapIntChar) {
			char litter = v->value_int_char.get(one.value_int);
			v->value_int_char.insert(one.value_int, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatChar) {
			char litter = v->value_float_char.get(one.value_float);
			v->value_float_char.insert(one.value_float, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharChar) {
			char litter = v->value_char_char.get(one.value_char);
			v->value_char_char.insert(one.value_char, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapIntFloat) {
			float litter = v->value_int_float.get(one.value_int);
			litter += 1;
			v->value_int_float.insert(one.value_int, litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatFloat) {
			float litter = v->value_float_float.get(one.value_float);
			litter += 1;
			v->value_float_float.insert(one.value_float, litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharFloat) {
			float litter = v->value_char_float.get(one.value_char);
			litter += 1;
			v->value_char_float.insert(one.value_char, litter);
			return;
		}
	}
	else if (op == "a--") {
		StructPoliz one = operations.back();
		StructPoliz two = get_operation_rvalue();
		operations.push_back(two);
		if (one.name == "") {
			throw "++ to not lvalue";
		}
		StructValue* v = semantic.Get_Value_ID(one.name);
		if (v->type_value == TypeValue::Int || v->type_value == TypeValue::Char || v->type_value == TypeValue::Float) {
			v->value_char--;
			v->value_float -= 1;
			v->value_int--;
			return;
		}
		if (v->type_value == TypeValue::ArrayInt) {
			v->value_array_int[one.value_int]--;
			return;
		}
		if (v->type_value == TypeValue::ArrayChar) {
			v->value_array_char[one.value_int]--;
			return;
		}
		if (v->type_value == TypeValue::ArrayFloat) {
			v->value_array_float[one.value_int] -= 1;
			return;
		}
		if (v->type_value == TypeValue::MapIntInt) {
			int litter = v->value_int_int.get(one.value_int);
			v->value_int_int.insert(one.value_int, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatInt) {
			int litter = v->value_float_int.get(one.value_float);
			v->value_float_int.insert(one.value_float, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharInt) {
			int litter = v->value_char_int.get(one.value_char);
			v->value_char_int.insert(one.value_char, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapIntChar) {
			char litter = v->value_int_char.get(one.value_int);
			v->value_int_char.insert(one.value_int, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatChar) {
			char litter = v->value_float_char.get(one.value_float);
			v->value_float_char.insert(one.value_float, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharChar) {
			char litter = v->value_char_char.get(one.value_char);
			v->value_char_char.insert(one.value_char, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapIntFloat) {
			float litter = v->value_int_float.get(one.value_int);
			litter -= 1;
			v->value_int_float.insert(one.value_int, litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatFloat) {
			float litter = v->value_float_float.get(one.value_float);
			litter -= 1;
			v->value_float_float.insert(one.value_float, litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharFloat) {
			float litter = v->value_char_float.get(one.value_char);
			litter -= 1;
			v->value_char_float.insert(one.value_char, litter);
			return;
		}
	}
	else if (op == "++a") {
		StructPoliz one = operations.back();
		if (one.name == "") {
			throw "++ to not lvalue";
		}
		StructValue* v = semantic.Get_Value_ID(one.name);
		if (v->type_value == TypeValue::Int || v->type_value == TypeValue::Char || v->type_value == TypeValue::Float) {
			v->value_char++;
			v->value_float += 1;
			v->value_int++;
			return;
		}
		if (v->type_value == TypeValue::ArrayInt) {
			v->value_array_int[one.value_int]++;
			return;
		}
		if (v->type_value == TypeValue::ArrayChar) {
			v->value_array_char[one.value_int]++;
			return;
		}
		if (v->type_value == TypeValue::ArrayFloat) {
			v->value_array_float[one.value_int] += 1;
			return;
		}
		if (v->type_value == TypeValue::MapIntInt) {
			int litter = v->value_int_int.get(one.value_int);
			v->value_int_int.insert(one.value_int, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatInt) {
			int litter = v->value_float_int.get(one.value_float);
			v->value_float_int.insert(one.value_float, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharInt) {
			int litter = v->value_char_int.get(one.value_char);
			v->value_char_int.insert(one.value_char, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapIntChar) {
			char litter = v->value_int_char.get(one.value_int);
			v->value_int_char.insert(one.value_int, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatChar) {
			char litter = v->value_float_char.get(one.value_float);
			v->value_float_char.insert(one.value_float, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharChar) {
			char litter = v->value_char_char.get(one.value_char);
			v->value_char_char.insert(one.value_char, ++litter);
			return;
		}
		if (v->type_value == TypeValue::MapIntFloat) {
			float litter = v->value_int_float.get(one.value_int);
			litter += 1;
			v->value_int_float.insert(one.value_int, litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatFloat) {
			float litter = v->value_float_float.get(one.value_float);
			litter += 1;
			v->value_float_float.insert(one.value_float, litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharFloat) {
			float litter = v->value_char_float.get(one.value_char);
			litter += 1;
			v->value_char_float.insert(one.value_char, litter);
			return;
		}
	}
	else if (op == "--a") {
		StructPoliz one = operations.back();
		if (one.name == "") {
			throw "++ to not lvalue";
		}
		StructValue* v = semantic.Get_Value_ID(one.name);
		if (v->type_value == TypeValue::Int || v->type_value == TypeValue::Char || v->type_value == TypeValue::Float) {
			v->value_char--;
			v->value_float -= 1;
			v->value_int--;
			return;
		}
		if (v->type_value == TypeValue::ArrayInt) {
			v->value_array_int[one.value_int]--;
			return;
		}
		if (v->type_value == TypeValue::ArrayChar) {
			v->value_array_char[one.value_int]--;
			return;
		}
		if (v->type_value == TypeValue::ArrayFloat) {
			v->value_array_float[one.value_int] -= 1;
			return;
		}
		if (v->type_value == TypeValue::MapIntInt) {
			int litter = v->value_int_int.get(one.value_int);
			v->value_int_int.insert(one.value_int, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatInt) {
			int litter = v->value_float_int.get(one.value_float);
			v->value_float_int.insert(one.value_float, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharInt) {
			int litter = v->value_char_int.get(one.value_char);
			v->value_char_int.insert(one.value_char, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapIntChar) {
			char litter = v->value_int_char.get(one.value_int);
			v->value_int_char.insert(one.value_int, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatChar) {
			char litter = v->value_float_char.get(one.value_float);
			v->value_float_char.insert(one.value_float, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharChar) {
			char litter = v->value_char_char.get(one.value_char);
			v->value_char_char.insert(one.value_char, --litter);
			return;
		}
		if (v->type_value == TypeValue::MapIntFloat) {
			float litter = v->value_int_float.get(one.value_int);
			litter -= 1;
			v->value_int_float.insert(one.value_int, litter);
			return;
		}
		if (v->type_value == TypeValue::MapFloatFloat) {
			float litter = v->value_float_float.get(one.value_float);
			litter -= 1;
			v->value_float_float.insert(one.value_float, litter);
			return;
		}
		if (v->type_value == TypeValue::MapCharFloat) {
			float litter = v->value_char_float.get(one.value_char);
			litter -= 1;
			v->value_char_float.insert(one.value_char, litter);
			return;
		}
	}
	else if (op == "+") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
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
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
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
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
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
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
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
	else if (op == "!") {
		StructPoliz one = get_operation_rvalue();
		one.value_int = !one.value_int;
		one.value_char = one.value_int;
		one.value_float = one.value_int;
		operations.push_back(one);
}
	else if (op == "==") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
		int tmp;
		if (one.type_number == TypeNumber::float_ || one.type_number == TypeNumber::float_) {
			tmp = one.value_float == two.value_float;
		}
		else {
			tmp = one.value_int == two.value_int;
		}
		StructPoliz res;
		res.type = TypePoliz::operation_;
		res.type_number = TypeNumber::int_;
		res.value_int = tmp;
		res.value_char = tmp;
		res.value_float = tmp;
		operations.push_back(res);
		}
	else if (op == "!=") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
		int tmp;
		if (one.type_number == TypeNumber::float_ || one.type_number == TypeNumber::float_) {
			tmp = one.value_float == two.value_float;
		}
		else {
			tmp = one.value_int == two.value_int;
		}
		StructPoliz res;
		res.type = TypePoliz::operation_;
		res.type_number = TypeNumber::int_;
		res.value_int = tmp;
		res.value_char = tmp;
		res.value_float = tmp;
		operations.push_back(res);
	}
	else if (op == ">") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
		int tmp;
		if (one.type_number == TypeNumber::float_ || one.type_number == TypeNumber::float_) {
			tmp = one.value_float == two.value_float;
		}
		else {
			tmp = one.value_int == two.value_int;
		}
		StructPoliz res;
		res.type = TypePoliz::operation_;
		res.type_number = TypeNumber::int_;
		res.value_int = tmp;
		res.value_char = tmp;
		res.value_float = tmp;
		operations.push_back(res);
	}
	else if (op == "<") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
		int tmp;
		if (one.type_number == TypeNumber::float_ || one.type_number == TypeNumber::float_) {
			tmp = one.value_float == two.value_float;
		}
		else {
			tmp = one.value_int == two.value_int;
		}
		StructPoliz res;
		res.type = TypePoliz::operation_;
		res.type_number = TypeNumber::int_;
		res.value_int = tmp;
		res.value_char = tmp;
		res.value_float = tmp;
		operations.push_back(res);
	}
	else if (op == ">=") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
		int tmp;
		if (one.type_number == TypeNumber::float_ || one.type_number == TypeNumber::float_) {
			tmp = one.value_float == two.value_float;
		}
		else {
			tmp = one.value_int == two.value_int;
		}
		StructPoliz res;
		res.type = TypePoliz::operation_;
		res.type_number = TypeNumber::int_;
		res.value_int = tmp;
		res.value_char = tmp;
		res.value_float = tmp;
		operations.push_back(res);
	}
	else if (op == "<=") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
		int tmp;
		if (one.type_number == TypeNumber::float_ || one.type_number == TypeNumber::float_) {
			tmp = one.value_float == two.value_float;
		}
		else {
			tmp = one.value_int == two.value_int;
		}
		StructPoliz res;
		res.type = TypePoliz::operation_;
		res.type_number = TypeNumber::int_;
		res.value_int = tmp;
		res.value_char = tmp;
		res.value_float = tmp;
		operations.push_back(res);
	}
	else if (op == "&&") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
		int tmp = (one.value_int && two.value_int);
		StructPoliz res;
		res.type = TypePoliz::operation_;
		res.type_number = TypeNumber::int_;
		res.value_int = tmp;
		res.value_char = tmp;
		res.value_float = tmp;
		operations.push_back(res);
	}
	else if (op == "||") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
		int tmp = (one.value_int || two.value_int);
		StructPoliz res;
		res.type = TypePoliz::operation_;
		res.type_number = TypeNumber::int_;
		res.value_int = tmp;
		res.value_char = tmp;
		res.value_float = tmp;
		operations.push_back(res);
	}
	else if (op == "=") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = operations.back();
		operations.pop_back();
		operations.push_back(one);
		StructValue* v = semantic.Get_Value_ID(one.name);
		if (v->type_value == TypeValue::Int || v->type_value == TypeValue::Char || v->type_value == TypeValue::Float) {
			v->value_char = two.value_char;
			v->value_float = two.value_float;
			v->value_int = two.value_int;
			return;
		}
		if (v->type_value == TypeValue::ArrayInt) {
			v->value_array_int[one.value_int] = two.value_int;
			return;
		}
		if (v->type_value == TypeValue::ArrayChar) {
			v->value_array_char[one.value_int] = two.value_char;
			return;
		}
		if (v->type_value == TypeValue::ArrayFloat) {
			v->value_array_float[one.value_int] = two.value_float;
			return;
		}
		if (v->type_value == TypeValue::MapIntInt) {
			v->value_int_int.insert(one.value_int, two.value_int);
			return;
		}
		if (v->type_value == TypeValue::MapFloatInt) {
			v->value_float_int.insert(one.value_float, two.value_int);
			return;
		}
		if (v->type_value == TypeValue::MapCharInt) {
			v->value_char_int.insert(one.value_char, two.value_int);
			return;
		}
		if (v->type_value == TypeValue::MapIntChar) {
			v->value_int_char.insert(one.value_int, two.value_char);
			return;
		}
		if (v->type_value == TypeValue::MapFloatChar) {
			v->value_float_char.insert(one.value_float, two.value_char);
			return;
		}
		if (v->type_value == TypeValue::MapCharChar) {
			v->value_char_char.insert(one.value_char, two.value_char);
			return;
		}
		if (v->type_value == TypeValue::MapIntFloat) {
			v->value_int_float.insert(one.value_int, two.value_float);
			return;
		}
		if (v->type_value == TypeValue::MapFloatFloat) {
			v->value_float_float.insert(one.value_float, two.value_float);
			return;
		}
		if (v->type_value == TypeValue::MapCharFloat) {
			v->value_char_float.insert(one.value_char, two.value_float);
			return;
		}
	}
	else if (op == "find") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = operations.back();
		operations.pop_back();
		StructValue* v = semantic.Get_Value_ID(one.name);
		int tmp;
		if (v->type_value == TypeValue::MapIntInt) {
			tmp = v->value_int_int.find(two.value_int);
		}
		if (v->type_value == TypeValue::MapFloatInt) {
			tmp = v->value_float_int.find(two.value_float);
		}
		if (v->type_value == TypeValue::MapCharInt) {
			tmp = v->value_char_int.find(two.value_char);
		}
		if (v->type_value == TypeValue::MapIntChar) {
			tmp = v->value_int_char.find(two.value_int);
		}
		if (v->type_value == TypeValue::MapFloatChar) {
			tmp = v->value_float_char.find(two.value_float);
		}
		if (v->type_value == TypeValue::MapCharChar) {
			tmp = v->value_char_char.find(two.value_char);
		}
		if (v->type_value == TypeValue::MapIntFloat) {
			tmp = v->value_int_float.find(two.value_int);
		}
		if (v->type_value == TypeValue::MapFloatFloat) {
			tmp = v->value_float_float.find(two.value_float);
		}
		if (v->type_value == TypeValue::MapCharFloat) {
			tmp = v->value_char_float.find(two.value_char);
		}
		StructPoliz res;
		res.type = TypePoliz::operation_;
		res.type_number = TypeNumber::int_;
		res.value_int = tmp;
		res.value_char = tmp;
		res.value_float = tmp;
		operations.push_back(res);
		}
	else if (op == ",") {
		StructPoliz two = operations.back();
		operations.pop_back();
		StructPoliz one = operations.back();
		operations.pop_back();
		operations.push_back(two);
		}
	else if (op == "-a") {
		StructPoliz one = get_operation_rvalue();
		one.value_char = -one.value_char;
		one.value_float = -one.value_float;
		one.value_int = -one.value_int;
		operations.push_back(one);
		}
	else if (op == "%") {
		StructPoliz two = get_operation_rvalue();
		StructPoliz one = get_operation_rvalue();
		StructPoliz res;
		res.type = TypePoliz::operation_;
		if (one.type_number == TypeNumber::int_ || two.type_number == TypeNumber::int_) {
			res.type_number = TypeNumber::int_;
			res.value_int = one.value_int % two.value_int;
			res.value_char = res.value_int;
			res.value_float = res.value_int;
			operations.push_back(res);
			return;
		}
		res.type_number = TypeNumber::char_;
		res.value_char = one.value_char % two.value_char;
		res.value_float = res.value_char;
		res.value_int = res.value_char;
		operations.push_back(res);
		return;
	}
}

void Execution::input_(const Poliz& poliz, int i) {
	StructPoliz one = operations.back();
	operations.pop_back();
	StructValue* v = semantic.Get_Value_ID(one.name);
	if (v->type_value == TypeValue::Int) {
		cin >> v->value_int;
	}
	else if (v->type_value == TypeValue::Char) {
		cin >> v->value_char;
	}
	else if (v->type_value == TypeValue::Float) {
		cin >> v->value_float;
	}
	else if (v->type_value == TypeValue::ArrayInt) {
		if (v->value_array_int.size() >= v->value_int) throw "Index of array out of range";
		cin >> v->value_array_int[v->value_int];
	}
	else if (v->type_value == TypeValue::ArrayFloat) {
		if (v->value_array_float.size() >= v->value_int) throw "Index of array out of range";
		cin >> v->value_array_float[v->value_int];
	}
	else if (v->type_value == TypeValue::ArrayChar) {
		if (v->value_array_char.size() >= v->value_int) throw "Index of array out of range";
		cin >> v->value_array_char[v->value_int];
	}
	else if (v->type_value == TypeValue::MapIntInt) {
		int k; cin >> k;
		v->value_int_int.insert(v->value_int, k);
	}
	else if (v->type_value == TypeValue::MapIntFloat) {
		float k; cin >> k;
		v->value_int_float.insert(v->value_int, k);
	}
	else if (v->type_value == TypeValue::MapIntChar) {
		char k; cin >> k;
		v->value_int_char.insert(v->value_int, k);
	}
	else if (v->type_value == TypeValue::MapCharInt) {
		int k; cin >> k;
		v->value_char_int.insert(v->value_char, k);
	}
	else if (v->type_value == TypeValue::MapCharFloat) {
		float k; cin >> k;
		v->value_char_float.insert(v->value_char, k);
	}
	else if (v->type_value == TypeValue::MapCharChar) {
		char k; cin >> k;
		v->value_char_char.insert(v->value_char, k);
	}
	else if (v->type_value == TypeValue::MapFloatInt) {
		int k; cin >> k;
		v->value_float_int.insert(v->value_float, k);
	}
	else if (v->type_value == TypeValue::MapFloatFloat) {
		float k; cin >> k;
		v->value_float_float.insert(v->value_float, k);
	}
	else if (v->type_value == TypeValue::MapFloatChar) {
		char k; cin >> k;
		v->value_float_char.insert(v->value_float, k);
	}
}

void Execution::print_(const Poliz& poliz, int i) {
	StructPoliz t = poliz.Get_Value(i);
	if (t.value_string != "") {
		cout << t.value_string;
		return;
	}
	StructPoliz one = operations.back();
	operations.pop_back();
	if (one.name == "") {
		if (one.type_number == TypeNumber::float_) {
			cout << one.value_float;
		}
		else if (one.type_number == TypeNumber::char_) {
			cout << one.value_char;
		}
		else if (one.type_number == TypeNumber::int_) {
			cout << one.value_int;
		}
		else {
			cout << one.value_string;
		}
	}
	else {
		StructValue v = *semantic.Get_Value_ID(one.name);
		if (v.type_value == TypeValue::Int) {
			cout << v.value_int;
		}
		else if (v.type_value == TypeValue::Char) {
			cout << v.value_char;
		}
		else if (v.type_value == TypeValue::Float) {
			cout << v.value_float;
		}
		else if (v.type_value == TypeValue::ArrayInt) {
			cout << "[";
			for (auto& to : v.value_array_int) cout << to << " ";
			cout << "]";
		}
		else if (v.type_value == TypeValue::ArrayChar) {
			cout << "[";
			for (auto& to : v.value_array_char) cout << to << " ";
			cout << "]";
		}
		else if (v.type_value == TypeValue::ArrayFloat) {
			cout << "[";
			for (auto& to : v.value_array_float) cout << to << " ";
			cout << "]";
		}
		else if (v.type_value == TypeValue::MapIntInt) {
			v.value_int_int.print_treap();
		}
		else if (v.type_value == TypeValue::MapIntFloat) {
			v.value_int_float.print_treap();
		}
		else if (v.type_value == TypeValue::MapIntChar) {
			v.value_int_char.print_treap();
		}
		else if (v.type_value == TypeValue::MapCharInt) {
			v.value_char_int.print_treap();
		}
		else if (v.type_value == TypeValue::MapCharFloat) {
			v.value_char_float.print_treap();
		}
		else if (v.type_value == TypeValue::MapCharChar) {
			v.value_char_char.print_treap();
		}
		else if (v.type_value == TypeValue::MapFloatInt) {
			v.value_float_int.print_treap();
		}
		else if (v.type_value == TypeValue::MapFloatFloat) {
			v.value_float_float.print_treap();
		}
		else if (v.type_value == TypeValue::MapFloatChar) {
			v.value_float_char.print_treap();
		}
	}
	return;
}


void Execution::delete_(const Poliz& poliz, int i) {
	StructPoliz key = get_operation_rvalue();
	StructPoliz one = operations.back();
	operations.pop_back();
	StructValue* v = semantic.Get_Value_ID(one.name);
	if (v->type_value == TypeValue::MapIntInt || v->type_value == TypeValue::MapIntFloat || v->type_value == TypeValue::MapIntChar) {
		v->value_int_int.erase(key.value_int);
		v->value_int_char.erase(key.value_int);
		v->value_int_float.erase(key.value_int);
	}
	if (v->type_value == TypeValue::MapFloatInt || v->type_value == TypeValue::MapFloatFloat || v->type_value == TypeValue::MapFloatChar) {
		v->value_float_int.erase(key.value_float);
		v->value_float_char.erase(key.value_float);
		v->value_float_float.erase(key.value_float);
	}
	if (v->type_value == TypeValue::MapCharInt || v->type_value == TypeValue::MapCharFloat || v->type_value == TypeValue::MapCharChar) {
		v->value_char_int.erase(key.value_char);
		v->value_char_char.erase(key.value_char);
		v->value_char_float.erase(key.value_char);
	}
}

void Execution::call_(const Poliz& poliz, int ii) {
	StructPoliz Func = operations.back();
	operations.pop_back();
	vector<string> names;
	vector<StructValue> params_val;
	int i = operations.size();
	--i;
	while (operations[i].type != TypePoliz::separator_) {
		StructPoliz one = operations[i--];
		if (one.name != "") {
			StructValue v = *semantic.Get_Value_ID(one.name);
			if (v.type_value == TypeValue::ArrayInt || v.type_value == TypeValue::ArrayChar || v.type_value == TypeValue::ArrayFloat) {
				if (v.value_int == 0) {
					names.push_back("number[]");
					params_val.push_back(v);
					continue;
				}
			}
			else if (v.type_value != TypeValue::Int && v.type_value != TypeValue::Char || v.type_value != TypeValue::Float)  {
				if (v.value_int == 0 && v.value_float == 0.0) {
					names.push_back("map");
					params_val.push_back(v);
					continue;
				}
			}
		}
		one = get_operation_rvalue();
		names.push_back("number");
		StructValue v;
		v.value_int = one.value_int;
		v.value_char = one.value_char;
		v.value_float = one.value_float;
		params_val.push_back(v);
	}
	std::reverse(names.begin(), names.end());
	std::reverse(params_val.begin(), params_val.end());
	string inner_name = Func.name + " ";
	for (auto& to : names) inner_name += to + " ";
	for (auto& el : semantic.TF) {
		if (el.inner_name == inner_name) {
			for (int i = 0; i < el.params.size(); ++i) {
				semantic.Push_ID(el.params[i].name, el.params[i].type, el.params[i].type_map);
				semantic.Push_Value_TID(el.params[i].name, params_val[i]);
			}
			StructValue res = Get(el.poliz_function);
			if (el.type_back != "void") {
				StructPoliz polka;
				polka.type = TypePoliz::operation_; 
				if (res.type_value == TypeValue::Int) {
					polka.type_number = TypeNumber::int_;
					polka.value_int = res.value_int;
					polka.value_char = polka.value_int;
					polka.value_float = polka.value_int;
				}
				else if (res.type_value == TypeValue::Char) {
					polka.type_number = TypeNumber::char_;
					polka.value_int = res.value_char;
					polka.value_char = res.value_char;
					polka.value_float = res.value_char;
				}
				else if (res.type_value == TypeValue::Float) {
					polka.type_number = TypeNumber::float_;
					polka.value_int = res.value_char;
					polka.value_char = res.value_char;
					polka.value_float = res.value_char;
				}
				operations.push_back(polka);
				return;
			}
		}
	}
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