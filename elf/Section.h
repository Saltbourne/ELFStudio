#pragma once
#include "Binary.h"
#include <vector>


class Section
{

public:
    enum Section_Type
    {
        NONE    = 0,
        CODE    = 1,
        DATA    = 2
    };

    //Declare object for Section_Type enum
    Section_Type s_type;

    //Link back to the Binary class
    Binary *sectionBinary;

    //Constructor
    Section() : s_type(NONE), section_name(), virt_address(0), section_size(0), bytes(nullptr) {}
    Section(Binary *binary)
    {
        sectionBinary = binary;
        s_type = NONE;
        section_name = "";
        virt_address = 0;
        section_size = 0;
        bytes = nullptr;
    }

    //

    //Member functions
    int load_section(Binary *binary);

    //Getter functions
    std::string get_section_name()
    {
        return section_name;
    }
    
    uint64_t get_virt_address()
    {
        return virt_address;
    }

    uint64_t get_section_size()
    {
        return section_size;
    }

    uint8_t* get_bytes()
    {
        return bytes;
    }

    //Setter functions
    void set_section_name(std::string section_name)
    {
        this -> section_name = section_name;
    }

    void set_virt_address(uint64_t virt_address)
    {
        this -> virt_address = virt_address;
    }

    void set_section_size(uint64_t section_size)
    {
        this -> section_size = section_size;
    }

    void set_bytes(uint8_t *bytes)
    {
        this -> bytes = bytes;
    }

private:

    std::string     section_name;
    uint64_t        virt_address;
    uint64_t        section_size;
    uint8_t         *bytes;

    //section vector
    std::vector<std::string> sections;

};