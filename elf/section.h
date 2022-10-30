#pragma once

#include <iostream>
#include <string>
#include <bfd.h>

class Section
{
public:

	//Link section back to the binary
	Binary			*binary;

	enum Section_Type
	{
		NONE 	= 0,
		CODE	= 1,
		DATA 	= 2
	} section_type;

	Section() : binary(NULL), section_type(NONE), section_name(), vma(0), section_size(0), bytes(NULL) {}

/* ********************** Getter functions ************************** */
	std::string		get_section_name();
	uint64_t		get_section_vma();
	uint64_t		get_section_size();
	uint8_t			get_section_bytes();

/* ********************** Setter functions ************************** */
	void 	set_section_name(std::string name);
	void	set_section_vma(uint64_t vma);
	void 	set_section_size(uint64_t size);
	void	set_section_bytes(uint8_t *bytes);

private:

	std::string		section_name;
	uint64_t		vma;
	uint64_t		section_size;
	uint8_t			*section_bytes;
};