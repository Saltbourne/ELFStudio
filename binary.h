#pragma once

#include <string>
#include <vector>
#include <iostream>

class Binary
{
private:
	enum Binary_Type
	{
		BIN_TYPE_OTHER = 0,
		BIN_TYPE_ELF   = 1
	};

	enum Binary_Arch 
	{
		ARCH_NONE = 0,
		ARCH_x86 = 1
	};
}