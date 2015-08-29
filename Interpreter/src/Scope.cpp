#include "stdafx.h"
#include "Expression.h"
#include "Scope.h"
#include "Utils.h"
#include "VariableMap.h"


void Scope::set(const Variable& v, Value value) {
	if (!defined(v)) {
		if (root) {
			my_error("variable '" + VariableMap::get_name(v.id) + "' is undefined");
		}
		else {
			parent->set(v, value);
		}
	}
	else {
		vars[v.id] = value;
	}
}

Value Scope::get(const Variable& v) {
	if (!defined(v)) {
		if (root) {
			my_error("variable '" + VariableMap::get_name(v.id) + "' is undefined");
		}
		else {
			return parent->get(v);
		}
	}

	return vars[v.id];
}

void Scope::define(const Variable& v, Value value)
{
	if (defined(v)) {
		my_error("variable '" + VariableMap::get_name(v.id) + "' is already defined");
	}
	vars[v.id] = value;
}

bool Scope::defined(const Variable& v) {
	return vars.find(v.id) != vars.end();
}

void Scope::dispose()
{
	for (auto& var : vars) {
		if (var.second.closure) {
			var.second.closure->dispose();
		}
	}
	vars.clear();
}

void Scope::print() {
	for (auto& var : vars) {
		cout << VariableMap::get_name(var.first) << " = ";
		var.second.print();
		cout << std::endl;
	}
};