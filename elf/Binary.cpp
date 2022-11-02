#pragma once

#include "Binary.h"
#include <iostream>
#include <bfd.h>

Binary::bfd open_binary(std::string &filename)
{
	//use static so magic is only initialized once.
	static int magic = 1;
	if(!magic)
	{
		// The bfd_init() function initializes the magic number once.
		bfd_init();
	}

	bfd *bfd_h;

	bfd_h = bfd_openr(filename, NULL); //There is no target (another bfd would be target) in this case

	if(!bfd_h)
	{
		//bfd did not open. Print an error message
		std::cerr << "Failed to open binary: " << filename << bfd_errmsg(bfd_get_error()) << std::endl;
		return NULL;
	}

	//Since the file opened check the format
	int loaded;

	//send a bfd_object to fill bfd_h with data.  Otherwise "unknown" file type is used
	loaded = bfd_check_format(bfd_h, bfd_object);
	if(!loaded)
	{
		std::cerr << "Binary is not an executable " << filename << bfd_errmsg(bfd_get_error()) << std::endl;
		return NULL;
	}

	if(bfd_get_flavour(bfd_h) == bfd_target_unknown_flavour)
	{
		std::cerr << "UNKNOWN file type for binary.  Not supported " << bfd_errmsg(bfd_get_error()) << std::endl;
	}

	//successful open return the pointer

	return bfd_h;

}

Binary::int load_bfd_binary(std::string &filename, Binary *binary, Binary::Binary_Type binary_type)
{

	bfd *bfd_h;
	const bfd_arch_info_type *binary_info;  //get the architecture info

	bfd_h = open_binary(filename);
	if(!bfd_h)
	{
		//Did not load.  Do not proceed.
		return 0;
	}

	Binary *binary;

	binary -> set_binary_entry_address(bfd_get_start_address(bfd_h));

	binary -> set_binary_file_size(bfd_get_file_size(bfd_h));

	binary -> set_binary_flavour(bfd_h -> xvec -> name);

	//check the binary type

	if(bfd_target_elf_flavour)
		binary_type = Binary::ELF;
	
	else if(bfd_target_coff_flavour)
		binary_type = Binary::PE;

	else if(bfd_target_unknown_flavour)
	{
		std::cerr << "Unknown file type.  Not a ELF or PE: " << bfd_h -> xvec -> name << std::endl;

		//not successful load
		return -1;
	}

	//check the architecture type
	binary_info = bfd_get_arch_info(bfd_h);
	binary -> arch_name = set_binary_arch_name(binary_info -> printable_name);
	if(binary_info -> mach == bfd_mach_i386_i386)
	{
		binary -> binary_arch = Binary::X86;
		binary -> set_binary_bits(32);
	}else if(binary_info -> mach == bfd_mach_x86_64)
	{
		binary -> binary_arch = Binary::X86_64;
		binary -> set_binary_bits(64);
	}else
	{
		std::cerr << "Architecture " << binary_info -> printable_name << " is not supported." << std::endl;
		return -1;
	}

	load_binary_static_symbols(bfd_h, binary);
	load_binary_dynamic_symbols(bfd_h, binary);

	return 0;
}

Section::int load_binary_sections(bfd *bfd_h, Binary *binary)
{
	const char *section_name;
	asection *bfd_section;
	Section *section;

	uint64_t size, vma;
	std::string name;

	//loop through all of the sections
	for(bfd_section = bfd_h -> sections; bfd_section;bfd_section -> next)
	{
		section_type = NONE;

		binary -> sections.push_back(Section());
		section = &bin -> sections.back();

		vma = bfd_section_vma(bfd_section);
		size = bfd_section_size(bfd_section);
		name = bfd_section_name(bfd_section);

		section -> set_section_vma(vma);
		section -> set_section_size(size);
		section -> set_section_name(name);
		
		section -> set_section_bytes(uint8_t*)malloc(section -> set_section_size(bfd_section_size(bfd_section)));

	}
	return 0; //successfully loaded
}


Symbol::load_binary_static_symbols(bfd *bfd_h, Binary *binary)
{
	//TODO
	return 0;  //successfully loaded
}

Symbol::int load_binary_dynamic_symbols(bfd *bfd_h, Binary *binary)
{
	//TODO
	return 0;  successfully loaded
}