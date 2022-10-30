#pragma once

#include "Binary.h"
#include <iostream>
#include <bfd.h>

Binary::bfd open_binary(std::string &filename)
{
	static int magic = 1;
	if(!magic)
	{
		// The bfd_init() function initializes the magic number once.
		bfd_init();
	}

	bfd *bfd_h;

	bfd_h = bfd_openr(filename, NULL); //There is no targer in this case

	if(!bfd_h)
	{
		//bfd did not open. Print an error message
		std::cerr << "Failed to open binary: " << filename << bfd_errmsg(bfd_get_error()) << std::endl;
		return NULL;
	}

	//Since the file opened check the format
	int loaded;

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
	bfd_h = open_binary(filename);
	if(!bfd_h)
	{
		//Did not load.  Do not proceed.
		return 0;
	}

	Binary *binary;

	binary -> set_binary_entry_address(bfd_get_start_address(bfd_h));

	binary -> ufile_ptr get_binary_file_size(bfd_get_file_size(bfd_h));

	//TODO

	return bfd_h;
}

Section::int load_binary_sections(bfd *bfd_h, Binary *binary)
{
	//TODO
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