#include "Binary.h"

int Binary::open_binary_file(std::string filename)
{
    //check to see if the magic value has been set.  This is required for opening.
    static int magic = 1;
    if(!magic)
    {
        bfd_init();
    }

    //open the file with the bfd open read function.  Target is set to NULL
    this -> storebfd = bfd_openr(filename.c_str(), NULL);

    if(!this -> storebfd)
    {
        std::cerr << "Failed to open the binary : " << filename << bfd_errmsg(bfd_get_error()) << std::endl;
        bfd_close(this -> storebfd);
        return -1;
    }

    int loaded = bfd_check_format(this -> storebfd, bfd_object);
    if(!loaded)
    {
        std::cerr << "Binary is not an executable. " << "File: " << filename << " " << std::endl;
        bfd_close(this -> storebfd);
        return -2;
    }

    if(bfd_get_flavour(this -> storebfd) == bfd_target_unknown_flavour)
    {
        std::cerr << "Binary is not an ELF type.  Please run the system command <file> on " << filename << " to check the file type. "
                << bfd_errmsg(bfd_get_error()) << std::endl;
        bfd_close(this -> storebfd);
        return -3;
    }

    //sucessful opening of the file

    return 1;
}

int Binary::load_binary(Binary *binary)
{
    const bfd_arch_info_type *binary_info;

    //set the entry address to the binary;
    binary -> set_entry_addr(bfd_get_start_address(binary -> storebfd));
    //set the file size
    binary -> set_file_size(bfd_get_file_size(binary -> storebfd));
    //set the flavour of the binary
    // bfd *bfd_h = binary -> storebfd;
    std::string flavour = storebfd -> xvec -> name;
    binary -> set_flavour(flavour);
    //store the type of binary, PE or ELF
    if(storebfd -> xvec -> flavour == bfd_target_coff_flavour)
    {
        //set the binary_type enum to the correct type
        binary -> b_type = Binary::PE;
        std::cerr << "Window's Portable Executable files are not supported.  Please choose an ELF file. " << std::endl;
        return -1;
    } else if(storebfd -> xvec -> flavour == bfd_target_elf_flavour)
    {
        //Continue.
        binary -> b_type = Binary::ELF;
    } else
    {
        //file is not a PE or ELF and thus it is unknown.
        binary -> b_type = Binary::UNKNOWN;
        std::cerr << "File type is not supported.  Only ELF file types are supported for this tool. " << std::endl;
        bfd_close(this -> storebfd);
        return -2;
    }

    //check the architecture.  
    const bfd_arch_info_type *arch_type;
    arch_type = bfd_get_arch_info(storebfd);
    //set the arch name from the printable name of the arch type.
    binary -> set_arch_name(std::string(arch_type -> printable_name));
    if(arch_type -> mach == bfd_mach_i386_i386)
    {
        binary -> set_bits(32);
        binary -> b_arch = Binary::X86;
        std::cerr << "This tool does not support 32 but file types at this point. " << std::endl;
        return -3;
    } else if(arch_type -> mach == bfd_mach_x86_64)
    {
        binary -> set_bits(64);
        binary -> b_arch = Binary::X86_64;
    } else
    {
        //file is not a 32, 64 bit file, so it is not supported
        binary -> set_bits(0);
        binary -> b_arch = Binary::OTHER;
        std::cerr << "Architecture is not supported.  Only 64 bit file types are supported. " << std::endl;
        return -4;
    }

    //Error checking is doing in the main function to prevent tool crashing.

    //file type and architure is correct, return 1 for success.
    return 1;
}
