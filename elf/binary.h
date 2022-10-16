#pragma once

#include "Section.h"
#include "Symbol.h"
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

	Section *get_text_section();

	std::string		filename;
	Binary_Type		b_type;
	std:string		type_string;
	Binary_Arch		archit;
	std::string 	archit_string;
	unsigned		bits;
	uint64_t		entry_addr;
	std::vector<Section> sections;
	std::vector<Symbol> symbols

public:
	Binary()
	{
		type = BIN_TYPE_OTHER;
		archit = ARCH_NONE;
		bits = 0;
		entry_addr = 0x0;
	}

	int  load_binary   (std::string &fname, Binary *bin, Binary::BinaryType type);
	void unload_binary (Binary *bin);

	std::string set_filename();
	Binary_Type get_Binary_Type();
	std::string set_type_string();
	void set_archit_string(std::string archit_string)
	void set_sections_vector(std::vector<Section> &sections);
	void set_symbols_vector(std::vector<Symbol> &symbols);

};