#pragma once

#include <iostream>
#include <string>
#include <bfd.h>

class Section
{
private:
	enum Section_Type
	{
		SEC_TYPE_NONE = 0,
		SEC_TYPE_CODE = 1,
		SEC_TYPE_DATA = 2
	};

	std::string 		sec_name;
	Section_Type 		sec_type;
	uint64_t 			vma;
	uint64_t			size;
	uint8_t				*bytes;

public:
	Section()
	{
		//default parameters
		type = SEC_TYPE_NONE;
		vma = 0;
		size = 0;
		bytes = NULL;
	}

	uint64_t get_vma();

	uint64_t get_size();

	uint8_t* get_bytes();

	void set_vma(uint64_t vma);

	void set_size(uint64_t size);
};