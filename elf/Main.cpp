#include "Binary.h"
#include "Symbol.h"
#include "Section.h"

int main(int argc, char** argv)
{
    Binary *binary = new Binary();
    // binary -> storebfd = nullptr;
    std::string file;

    if(argc < 2)
    {
        std::cout << "An argument is missing.  Probably a binary " << std::endl;
        return 1;
    }
    file.assign(argv[1]);

    int ret = 0;
    ret = binary -> open_binary_file(file);

    std::cout << "The return value for open binary is: " << ret << std::endl;

    while(ret < 0)
    {
        std::cout << "Please choose a different file. " << std::endl;
        std::cin >> file;
        ret = binary -> open_binary_file(file);
    }

    ret = binary -> load_binary(binary);

    std::cout << "The return value of load binary is: " << ret << std::endl;

    Symbol *symbol = new Symbol(binary);
    // Symbol *symbol;
    std::cout << binary ->testInBinary << std::endl;
    std::cout << symbol -> symbolBinary -> testInBinary << std::endl;
    
    int ret_static = symbol -> load_static_symbols();
    // int ret_dynamic = symbol -> load_dynamic_symbols();
    // ret = symbol -> load_static_symbols(binary);

    std::cout << "The return value of static symbols is: " << ret_static << std::endl;
    // std::cout << "The return value of static symbols is: " << ret_dynamic << std::endl;

    int size = symbol -> symbols.size();
    std::cout << "The size of the symbol vector is: " << size << std::endl;
    for(int i = 0; i < size; i++)
    {
        std::cout << symbol -> symbols[i].get_sym_name() << "     " << symbol -> symbols[i].get_sym_addr() << std::endl;
    }

    return 0;
}