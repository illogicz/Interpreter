#pragma once

#include "stdafx.h"

class VariableMap {
	private:
		static map<string, unsigned int> name_to_id;
		static map<unsigned int, string> id_to_name;
		static unsigned int count;
	public:
		static unsigned int get_id(const string& name);
		static string get_name(unsigned int id);

};