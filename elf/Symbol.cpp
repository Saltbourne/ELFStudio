#include "Symbol.h"


int Symbol::load_static_symbols()
{
    // Symbol *symbol;
    asymbol **symbol_table = nullptr;

    std::cout << "Symbol Line 8" << std::endl;

    symbol_table = nullptr;
    std::cout << "Symbol Line 12" << std::endl;
    // std::cout << symbolBinary -> testInBinary << std::endl;
    
    long storage_needed = bfd_get_symtab_upper_bound(this -> symbolBinary -> storebfd);
    std::cout << "Symbol Line 15" << std::endl;
    if(storage_needed < 0)
    {
        std::cerr << "The symbol table is empty or failed to read the symbol table. " << std::endl;
        std::cerr << bfd_errmsg(bfd_get_error()) << std::endl;
        std::cout << "Symbol Line 20" << std::endl;
        return -1;
    } else if(storage_needed)
    {
        //allocate the memory for the symbol table
        // symbol_table = (asymbol**)malloc(storage_needed);
        asymbol **symbol_table = new asymbol *[storage_needed];
        std::cout << "Symbol Line 27" << std::endl;
        if(!symbol_table)
        {
            std::cerr << "There is no memory for the symbol table. " << std::endl;
            // free(symbol_table);
            std::cout << "Symbol Line 32" << std::endl;
            delete[] symbol_table;
            return -2;
        }
        std::cout << "Symbol Line 36" << std::endl;
        long number_symbols = bfd_canonicalize_symtab(symbolBinary -> storebfd, symbol_table);
        std::cout << "Symbol Line 38" << std::endl;
        if(number_symbols < 0)
        {
            std::cerr << "There are no symbols in the table.  The table could have been stripped." << std::endl;
            delete[] symbol_table;
            return -3;
        }

        for(long i = 0; i < number_symbols; i++)
        {
            if(symbol_table[i] -> flags & BSF_FUNCTION && !(symbol_table[i] -> flags & BSF_WEAK))
            {
                symbolBinary -> symbols.push_back(Symbol());
                // symbolBinary-> symbols.back();
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].sym_type = FUNCTION;
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].set_sym_name(std::string(symbol_table[i] -> name));
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].set_sym_addr(bfd_asymbol_value(symbol_table[i]));
            }
        }
    }
    
    //store the symbol table in the class
    this -> store_static_symbols = symbol_table;
    // free(symbol_table);
    // delete[] symbol_table;
    return 1;
}

int Symbol::load_dynamic_symbols()
{
    Symbol *symbol;
    asymbol **dynamic_table;

    dynamic_table = nullptr;
    long num_dy_symbols = bfd_get_dynamic_symtab_upper_bound(symbolBinary -> storebfd);

    if(num_dy_symbols < 0)
    {
        std::cerr << "Dynamic Symbol table was not able to load. " << std::endl;
        return -1;
    } else if(num_dy_symbols)
    {
        dynamic_table = (asymbol**)malloc(num_dy_symbols);
        if(!dynamic_table)
        {
            std::cerr << "Memory failed to load for the dynamic symbol table. " << std::endl;
            return -2;
        }
        num_dy_symbols = bfd_canonicalize_dynamic_symtab(symbolBinary -> storebfd, dynamic_table);
        if(num_dy_symbols < 0)
        {
            std::cerr << "Failed the read the dynamic symbol table. " << std::endl;
            std::cerr << bfd_errmsg(bfd_get_error()) << std::endl;
            return -3;
        }

        for(long i = 0; i < num_dy_symbols; i++)
        {
            if(dynamic_table[i] -> flags & BSF_FUNCTION)
            {
                symbolBinary -> symbols.push_back(Symbol());
                // symbol = &symbol -> symbols.back();
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].sym_type = FUNCTION;
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].set_sym_name(std::string(dynamic_table[i] -> name));
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].set_sym_addr(bfd_asymbol_value(dynamic_table[i]));
            }
        }
    }

    //store the dynamic symbol table in the symbol class.
    symbol -> store_dynamic_symbols = dynamic_table;
    delete[] dynamic_table;
    // free(dynamic_table);
    return 1;
}

//Getter functions
std::string Symbol::get_sym_name() const
{
    return this -> sym_name;
}

uint64_t Symbol::get_sym_addr() const
{
    return this -> sym_addr;
}

//Setter functions
void Symbol::set_sym_name(std::string symbol_name)
{
    this -> sym_name = symbol_name;
}

void Symbol::set_sym_addr(uint64_t sym_addr)
{
    this -> sym_addr = sym_addr;
}