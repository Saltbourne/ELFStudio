#pragma once
#include <iostream>
#include <vector>
#include <bfd.h>
#include "Section.h"
#include "Symbol.h"

class Section;
class Symbol;
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

    //Store symbol and section data in the vectors.
    std::vector<Symbol> symbols;
    std::vector<Section> sections;


    //Member functions
    int open_binary_file(std::string filename);
    int load_binary(Binary *binary);
        
    //getter functions
    std::string get_filename() const;
    std::string get_arch_name() const;
    std::string get_flavour() const;
    unsigned get_bits() const;
    uint64_t get_entry_addr() const;
    ufile_ptr get_file_size() const;

    //Setter functions
    void set_filename(std::string filename);
    void set_arch_name(std::string arch_name);
    void set_flavour(std::string flavour);
    void set_bits(unsigned bits);
    void set_entry_addr(uint64_t entry_addr);
    void set_file_size(ufile_ptr file_size);

private:

    std::string     filename;
    std::string     arch_name;
    std::string     flavour;
    unsigned        bits;
    uint64_t        entry_addr;
    ufile_ptr       file_size;

};