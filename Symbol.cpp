#include "Symbol.h"
#include <QErrorMessage>
#include <cxxabi.h>


int Symbol::load_static_symbols()
{
    asymbol **symbol_table = nullptr;

    std::cout << "Symbol Line 8" << std::endl;

    symbol_table = nullptr;
    std::cout << "Symbol Line 12" << std::endl;

    long storage_needed = bfd_get_symtab_upper_bound(this -> symbolBinary -> storebfd);
    std::cout << "Symbol Line 15" << std::endl;
    if(storage_needed < 0)
    {
        std::cerr << "The symbol table is empty or failed to read the symbol table. " << std::endl;
        std::cerr << bfd_errmsg(bfd_get_error()) << std::endl;
        QErrorMessage err;
        err.showMessage("Failed to read symbols. The file is likely stripped.");
        std::cout << "Symbol Line 20" << std::endl;
        return -1;
    } else if(storage_needed)
    {
        asymbol **symbol_table = new asymbol *[storage_needed];
        std::cout << "Symbol Line 27" << std::endl;
        if(!symbol_table)
        {
            std::cerr << "There is no memory for the symbol table. " << std::endl;
            QErrorMessage err;
            err.showMessage("There is no memory for the symbol table.");
            std::cout << "Symbol Line 32" << std::endl;
            delete[] symbol_table;
            return -2;
        }
        std::cout << "Symbol Line 36" << std::endl;
        long number_symbols = bfd_canonicalize_symtab(symbolBinary -> storebfd, symbol_table);
        std::cout << "Symbol Line 38" << std::endl;
        if(number_symbols <= 0)
        {
            std::cerr << "There are no symbols in the table.  The table could have been stripped." << std::endl;
            QErrorMessage err;
            err.showMessage("There are no symbols in the table.  The table could have been stripped.");
            delete[] symbol_table;
            return -3;
        }
        std::cout << "HELLOOOOOOOO " << number_symbols << std::endl;
        for(long i = 0; i < number_symbols; i++)
        {
            if(symbol_table[i] -> flags & BSF_FUNCTION)
            {
                symbolBinary -> symbols.push_back(Symbol());
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].sym_type = FUNCTION;
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].set_sym_name(std::string(symbol_table[i] -> name));
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].set_sym_addr(bfd_asymbol_value(symbol_table[i]));
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].demangle_symbol(symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].get_sym_name(), symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].get_sym_name().length());
            }
        }
        std::cout << "Symbol line 58 " << std::endl;
    }

    //store the symbol table in the class
    this -> store_static_symbols = symbol_table;
    delete[] symbol_table;
    std::cout << "The static table of symbols has finished loading. " << std::endl;
    return 1;
}

int Symbol::load_dynamic_symbols()
{
    asymbol **dynamic_table;

    dynamic_table = nullptr;
    long num_dy_symbols = bfd_get_dynamic_symtab_upper_bound(symbolBinary -> storebfd);

    if(num_dy_symbols < 0)
    {
        std::cerr << "Dynamic Symbol table was not able to load. " << std::endl;
        QErrorMessage err;
        err.showMessage("Dynamic symbol table was not able to load.");
        return -1;
    } else if(num_dy_symbols)
    {
        asymbol **dynamic_table = new asymbol *[num_dy_symbols];
        if(!dynamic_table)
        {
            std::cerr << "Memory failed to load for the dynamic symbol table. " << std::endl;
            QErrorMessage err;
            err.showMessage("Memory failed to load for the dynamic symbol table.");
            return -2;
        }
        num_dy_symbols = bfd_canonicalize_dynamic_symtab(symbolBinary -> storebfd, dynamic_table);
        if(num_dy_symbols < 0)
        {
            std::cerr << "Failed the read the dynamic symbol table. " << std::endl;
            QErrorMessage err;
            err.showMessage("Failed the read the dynamic symbol table.");
            std::cerr << bfd_errmsg(bfd_get_error()) << std::endl;
            return -3;
        }
        std::cout << "Symbol LIne 94. " << std::endl;
        for(long i = 0; i < num_dy_symbols; i++)
        {
            if(dynamic_table[i] -> flags & BSF_FUNCTION)
            {
                symbolBinary -> symbols.push_back(Symbol());
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].sym_type = FUNCTION;
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].set_sym_name(std::string(dynamic_table[i] -> name));
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].set_sym_addr(bfd_asymbol_value(dynamic_table[i]));
                std::cout << std::string(dynamic_table[i] -> name) << std::endl;
                symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].demangle_symbol(symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].get_sym_name(), symbolBinary -> symbols[symbolBinary -> symbols.size() - 1].get_sym_name().length());

            }
        }
    }
    this -> store_dynamic_symbols = dynamic_table;
    delete[] dynamic_table;
    std::cout << "Dynamic Symbol table is finished loading." << std::endl;
    return 1;
}

/*
 * Recursive function to demangle symbols until it can no longer be demangled.
 * -Rebecca L
 */

void Symbol::demangle_symbol(std::string name, unsigned long len)
{
    if(len == 0)
    {
        std::cout << "Empty String" << std::endl;
        return;
    }
    int status;

    std::cout << "Symbol mangled name: " << name << std::endl;

    char *realname;
    realname = abi::__cxa_demangle(name.c_str(), 0, nullptr, &status);

    if(realname == nullptr)
    {
        std::cout << "line 152" << std::endl;
        std::cout << "Demangle end" << std::endl;
        std::cout << name << std::endl;
        this->set_demangled_name(name);
        std::cout << this->get_demangled_name() << std::endl;
        return;
    }
    std::cout << "Test demangle: **********************  " << realname << std::endl;
    std::string str(realname);
    std::cout << "line 162" << std::endl;
    free(realname);
    this->demangle_symbol(str, str.length());
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

std::string Symbol::get_demangled_name() const
{
    return this -> demangled_name;
}

//Setter functions
void Symbol::set_sym_name(std::string symbol_name)
{
    this -> sym_name = symbol_name;
}

void Symbol::set_demangled_name(std::string demangled_name)
{
    this -> demangled_name = demangled_name;
}

void Symbol::set_sym_addr(uint64_t sym_addr)
{
    this -> sym_addr = sym_addr;
}
