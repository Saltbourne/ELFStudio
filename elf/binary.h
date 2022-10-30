#pragma once

#include <string>
#include <vector>

#include <bfd.h>

class Binary
{
public:
// Describe the type of Binary
//Can add more binary types later
	enum Binary_Type
	{
		DEFAULT 	= 0,
		ELF   		= 1, 
		PE			= 2
	} binary_type;

//Check if the binary is 32 bits or 64
	enum Binary_Architecture
	{
		UNKOWN 		= 0,
		X86 		= 1,
		X86_64		= 2
	} binary_arch;


	Binary() : binary_type(DEFAULT), binary_arch(UNKNOWN), bits(0), entry_address(0) {}

//Fill the section and symbol vectors
	std::vector<Section> sections;
	std::vector<Symbol> symbols


/* **************** Setters for binary **************** */

	void set_filename(std::string filename);
	void set_type_string(std::string type_string);
	void set_archit_string(std::string archit_string)

/* **************** Getter for binary ****************** */

	std::string get_binary_filename();
	unsigned get_binary_bits();
	uint64_t get_binary_entry_address();

	bfd 	open_binary(std::filename);
	int 	load_binary(std::string &filename, Binary *binary, Binary::Binary_Type binary_type);
	void 	unload_binary(Binary *bin);


private:

	std::string		filename;
//	std::string		type_string;
//	std::string 	archit_string;
	unsigned		bits;
	uint64_t		entry_address;

};