#include "stdafx.h"
#include "Expression.h"
#include "Scope.h"
#include "VariableMap.h"

int Scope::count = 0;


void Scope::set(const Variable& v, Value value) {
	if (!defined(v))
		if (root)
			error("variable '" + VariableMap::get_name(v.id) + "' is undefined");
		else
			parent->set(v, value);
	else
		vars[v.id] = value;
}

Value Scope::get(const Variable& v) {
	if (!defined(v))
		if (root)
			error("variable '" + VariableMap::get_name(v.id) + "' is undefined");
		else
			return parent->get(v);
	return vars[v.id];
}

void Scope::define(const Variable& v, Value value)
{
	vars[v.id] = value;
}

bool Scope::defined(const Variable& v) {
	return vars.find(v.id) != vars.end();
}

void Scope::dispose()
{
	if (disposed) return;
	disposed = true;
	vars.clear();
	msg("dispose");
	for (auto& child : children) {
		if (auto spt = child.lock()){
			spt->dispose();
		}
	}
	children.clear();
}


Scope::Sptr Scope::makeChild()
{
	auto child = make_shared<Scope>(shared_from_this());
	children.push_front(child);
	return child;
}

void Scope::print() {
	for (auto& var : vars) {
		cout << VariableMap::get_name(var.first) << " = " << string(var.second) << endl;
	}
};

void Scope::msg(const string& type) const
{
	printf("scope %s,  	%d %d %d\n", type.c_str(), depth, count, this);
}