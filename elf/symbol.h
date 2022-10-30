#pragma once
#include <string>
#include <vector>

#include "Section.h"

class Symbol
{
public:
	enum Symbol_Type
	{
		UNKOWN 		= 0,
		FUNCTION 	= 1
	} sym_type;

	Symbol() : sym_type(UNKNOWN), sym_name(), sym_address(0) {}

/* *************** Getter functions ************************ */
	std::string 	get_symbol_name();
	uint64_t		get_symbol_address();

/* *************** Setter functions ************************ */
	void 		set_symbol_name(std::string sym_name);
	void		set_symbol_address(uint64_t sym_address);	

private:

	std::string		sym_name;
	uint64_t		sym_address;
};