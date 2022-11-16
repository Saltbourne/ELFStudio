#pragma once
#include "Section.h"

class Symbol
{
public:
    enum Symbol_Type
    {
        NONE    = 0,
        FUNCTION = 1
    };

    //Define Symbol Type
    Symbol_Type sym_type;

    //Link back to the binary class
    Binary *symbolBinary;

    //store the symbol table in the class when finished.
    asymbol **store_static_symbols = nullptr;
    asymbol **store_dynamic_symbols = nullptr;

    //Symbols vector
    std::vector<Symbol> symbols;

    //Constructor
    Symbol() : sym_type(NONE), sym_name(), sym_addr(0) {}
    Symbol(Binary *binary)
    {
        symbolBinary = binary;
    }

    //load the Symbols
    int load_static_symbols();
    int load_dynamic_symbols();

    //Getter functions
    std::string get_sym_name();

    uint64_t get_sym_addr();

    //Setter functions
    void set_sym_name(std::string symbol_name);
    void set_sym_addr(uint64_t symbol_address);



private:

    std::string     sym_name;
    uint64_t        sym_addr;
};