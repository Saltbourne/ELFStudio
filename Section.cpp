#include "Section.h"

int Section::load_section()
{
    const char *name;
    int section_flags;
    
    for(store_section = sectionBinary -> storebfd -> sections; store_section; store_section = store_section -> next)
    {
        // section_flags = bfd_get_section_flags(this -> sectionBinary -> storebfd, this -> store_section);
        // int whatever = ((void) sectionBinary -> storebfd, (this -> store_section) -> flags);
        flagword sec_flag;
        sec_flag = bfd_section_flags(this -> store_section);
        // std::cout << "The value of the bfd section flags function" << sec_flag << std::endl;
        // std::cout << "The value of section flags is: " << section_flags << " The value of what the macro does is: " << whatever << std::endl;
        
        s_type = NONE;
        if(sec_flag & SEC_CODE)
        {
            s_type = CODE;
        }else if(sec_flag & SEC_DATA)
        {
            s_type = DATA;
        }else if(sec_flag & SEC_NO_FLAGS)
        {
            s_type = NOFLAGS;
        } else { /* do nothing*/}


        sectionBinary -> sections.push_back(Section());
        sectionBinary -> sections[sectionBinary -> sections.size() - 1].set_section_name(bfd_section_name(store_section));
        if(sectionBinary -> sections[sectionBinary -> sections.size() - 1].get_section_name() == "")
        {
            sectionBinary -> sections[sectionBinary -> sections.size() - 1].set_section_name("NO_NAME");
        }
        std::cout << "Section Name: " << sectionBinary -> sections[sectionBinary -> sections.size() - 1].get_section_name() << std::endl;

        //fill the class object with the data
        sectionBinary -> sections[sectionBinary -> sections.size() - 1].set_section_size(bfd_section_size(store_section));
        uint64_t size = sectionBinary -> sections[sectionBinary -> sections.size() - 1].get_section_size();
        std::cout << "Section Size : " << size << std::endl;
        sectionBinary -> sections[sectionBinary -> sections.size() - 1].set_virt_address(bfd_section_vma(store_section));
        uint8_t *stuff = new (std::nothrow)uint8_t[size]; //to be used for bytes later.
        if(stuff == nullptr)
        {
            std::cout << "No bytes contents for " << sectionBinary -> sections[sectionBinary -> sections.size() - 1].get_section_name() << std::endl;
        }
        std::cout << "Section bytes: " << stuff << std::endl;

        sectionBinary -> sections[sectionBinary -> sections.size() - 1].set_bytes((stuff));
        free(stuff);

        // std::cout << "Section line 46" << std::endl;

        if(!bfd_get_section_contents(sectionBinary -> storebfd, store_section, sectionBinary -> sections[sectionBinary -> sections.size() - 1].get_bytes(), 0, sectionBinary -> sections[sectionBinary -> sections.size() - 1].get_section_size()))
        {
            std::cerr << "Failed to read the section. There is no memory for this section. " << get_section_name() << std::endl;
        }
    }

    return 1;
}

//Getter functions
std::string Section::get_section_name() const
{
    return this -> section_name;
}

uint64_t Section::get_virt_address() const
{
    return this -> virt_address;
}

uint64_t Section::get_section_size() const
{
    return section_size;
}

uint8_t* Section::get_bytes() const
{
    return bytes;
}

//Setter functions
void Section::set_section_name(std::string section_name)
{
    this -> section_name = section_name;
}

void Section::set_virt_address(uint64_t virt_address)
{
    this -> virt_address = virt_address;
}

void Section::set_section_size(uint64_t section_size)
{
    this -> section_size = section_size;
}

void Section::set_bytes(uint8_t* bytes)
{
    this -> bytes = bytes;
}