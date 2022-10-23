#pragma once

#include "Section.h"
#include "Symbol.h"
#include <string>
#include <vector>
#include <iostream>

class Binary
{
private:

	std::string		filename;
	std::string		type_string;
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

	Binary_Type		bin_type;
	Binary_Arch		bin_arch;

	Section *get_text_section();

	int  load_binary   (std::string &fname, Binary *bin, Binary::BinaryType type);
	void unload_binary (Binary *bin);

	std::string get_filename();
	Binary_Type get_Binary_Type();
	void set_filename(std::string filename);
	void set_type_string(std::string type_string);
	void set_archit_string(std::string archit_string)

	//using pointers for the vector since they can be empty.
	//References can not be null.
	void set_sections_vector(std::vector<Section> *sections);
	void set_symbols_vector(std::vector<Symbol> *symbols);

};