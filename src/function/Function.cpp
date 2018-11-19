#include "Function.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>


std::vector<Function::Description> Function::List = {
		{"Waves", "-cos(x^2+y^2) / exp((x^2+y^2)/8)", "waves.cl"},
		{"Hills", "-cos(x)cos(y) / exp((x^2+y^2)/32)", "hills.cl"}
};


Function::Function (const std::string& string_representation, const std::string& cl_file)
	: str_repr (string_representation), cl_file (cl_file)
{}


Function Function::get (const std::string& name) {
	for (auto& func : List)
		if (func[0] == name)
			return Function (func[1], func[2]);
	throw std::invalid_argument ("No such function: " + name);
}


void Function::getList (NameList* buffer) {
	size_t list_size = List.size ();
	*buffer = (char**) calloc (list_size + 1, sizeof(char*));
	for (size_t i = 0; i < list_size; i++) {
		(*buffer)[i] = (char*) calloc (List[i][0].length () + 1, sizeof(char));
		memcpy ((*buffer)[i], List[i][0].c_str (), List[i][0].length ());
	}
}


void Function::freeList (NameList* buffer) {
	size_t i = 0;
	char *str = nullptr;
	while ((str = (*buffer)[i++])) free (str);
	free (*buffer);
	*buffer = nullptr;
}