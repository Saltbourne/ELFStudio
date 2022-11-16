#pragma once
#include <iostream>
#include <bfd.h>

class Binary
{
public:
    
    enum Binary_Type
    {
        UNKNOWN = 0,
        ELF     = 1,
        PE      = 2
    };

    enum Binary_Arch
    {
        OTHER   = 0,
        X86     = 1,
        X86_64  = 2
    };

    Binary () : b_type(UNKNOWN), b_arch(OTHER), bits(0), entry_addr(0) {}

    //enum declarations
    Binary_Type     b_type;
    Binary_Arch     b_arch;

    //Store the bfd object in the class
    bfd *storebfd = nullptr;

    //Member functions
    int open_binary_file(std::string filename);
    int load_binary(Binary *binary);
    
    std::string testInBinary = "String is in the binary class";
    
    //getter functions
    std::string get_filename()
    {
        return this -> filename;
    }

    std::string get_arch_name()
    {
        return this -> arch_name;
    }

    std::string get_flavour()
    {
        return this -> flavour;
    }

    unsigned get_bits()
    {
        return this -> bits;
    }

    uint64_t get_entry_addr()
    {
        return this -> entry_addr;
    }

    ufile_ptr get_file_size()
    {
        return this -> file_size;
    }


    //Setter functions
    void set_filename(std::string filename)
    {
        this -> filename = filename;
    }

    void set_arch_name(std::string arch_name)
    {
        this -> arch_name = arch_name;
    }

    void set_flavour(std::string flavour)
    {
        this -> flavour = flavour;
    }

    void set_bits(unsigned bits)
    {
        this -> bits = bits;
    }

    void set_entry_addr(uint64_t entry_addr)
    {
        this -> entry_addr = entry_addr;
    }

    void set_file_size(ufile_ptr file_size)
    {
        this -> file_size = file_size;
    }

private:

    std::string     filename;
    std::string     arch_name;
    std::string     flavour;
    unsigned        bits;
    uint64_t        entry_addr;
    ufile_ptr       file_size;

};