#pragma once
#include <string>
#include <vector>

class Symbol
{
private:

	enum Symbol_Type
	{
		SYM_TYPE_UNKNOWN = 0,
		SYM_TYPE_FUNC = 1
	};

	Symbol_Type s_type;
	std::string s_name;
	unit64_t 	s_address:

public:
	Symbol()
	{
		type = SYM_TYPE_UNKNOWN;
		s_name = "";
		s_address = 0x0;
	}
};