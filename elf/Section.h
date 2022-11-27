#pragma once
#include "Symbol.h"

//forward declaration
class Binary;

//for section flags.
// #define bfd_get_section_flags(bfd, ptr) ((void) bfd, (ptr)->flags)

class Section
{

public:
    enum Section_Type
    {
        NONE    = 0,
        CODE    = 1,
        DATA    = 2, 
        NOFLAGS = 3
    };

    //Declare object for Section_Type enum
    Section_Type s_type;

    //store the asection pointer
    asection *store_section = nullptr;

    //Link back to the Binary class
    Binary *sectionBinary = nullptr;

    //Constructor
    Section() : s_type(NONE), section_name(), virt_address(0), section_size(0), bytes(nullptr) {}
    Section(Binary *binary)
    {
        sectionBinary = binary;
    }

    //

    //Member functions
    int load_section();

    //Getter functions
    std::string get_section_name() const;
    uint64_t get_virt_address() const;
    uint64_t get_section_size() const;
    uint8_t* get_bytes() const;

    //Setter functions
    void set_section_name(std::string section_name);
    void set_virt_address(uint64_t virt_address);
    void set_section_size(uint64_t section_size);
    void set_bytes(uint8_t *bytes);

private:

    std::string     section_name;
    uint64_t        virt_address;
    uint64_t        section_size;
    uint8_t         *bytes;

    //section vector
    std::vector<std::string> sections;

};