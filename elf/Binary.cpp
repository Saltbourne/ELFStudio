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

Binary::bfd load_bfd_binary(std::string &filename, Binary *binary, Binary::Binary_Type binary_type)
{

	bfd *bfd_h;
	bfd_h = open_binary(filename);
	if(!bfd_h)
	{
		//Did not load.
		return 0;
	}

	

	return bfd_h;
}