#include "stdafx.h"

#include "Utils.h"
#include "VariableMap.h"

map<string, unsigned int> VariableMap::name_to_id = {};
map<unsigned int, string> VariableMap::id_to_name = {};
unsigned int VariableMap::count = 0;

unsigned int VariableMap::get_id(const string& name)
{
	if (!name_to_id.count(name)) {
		name_to_id[name] = count;
		id_to_name[count++] = name;
	}
	return name_to_id[name];
}

string VariableMap::get_name(unsigned int id)
{
	if (!id_to_name.count(id)) {
		my_error("internal error: undefined variable id");
	}
	return id_to_name[id];
} 
