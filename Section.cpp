#include "Section.h"
#include <QErrorMessage>

int Section::load_section()
{
    const char *name;
    int section_flags;

    for(store_section = sectionBinary -> storebfd -> sections; store_section; store_section = store_section -> next)
    {
        flagword sec_flag;
        sec_flag = bfd_section_flags(this -> store_section); //store in the class
        
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
        sectionBinary -> sections.back().set_section_name(bfd_section_name(store_section));
        if(sectionBinary -> sections.back().get_section_name() == "")
        {
            sectionBinary -> sections.back().set_section_name("NO_NAME");
        }
        std::cout << "Section Name: " << sectionBinary -> sections.back().get_section_name() << std::endl;

        //fill the class object with the data
        sectionBinary -> sections.back().set_section_size(bfd_section_size(store_section));
        sectionBinary -> sectionsback().set_section_rsize(bfd_section_rsize(store_section));
        
        sectionBinary -> sections.back().set_virt_address(bfd_section_vma(store_section));
        sectionBinary -> sections.back().set_load_address(bfd_section_lma(store_section));
        uint8_t *stuff = new (std::nothrow)uint8_t[sectionBinary -> sections.back().get_section_size()]; //to be used for bytes later.

        
        if(stuff == nullptr)
        {
            std::cout << "No bytes contents for " << sectionBinary -> sections.back().get_section_name() << std::endl;
            QErrorMessage err;
            err.showMessage("No byte contents for one of the sections.");
        }
        sectionBinary -> sections.back().set_bytes((stuff));


        if(!bfd_get_section_contents(sectionBinary -> storebfd, store_section, sectionBinary -> sections.back().get_bytes(), 0, sectionBinary -> sections[sectionBinary -> sections.size() - 1].get_section_size()))
        {
            std::cerr << "Failed to read the section. There is no memory for this section. " << get_section_name() << std::endl;
            QErrorMessage err;
            err.showMessage("Failed to read one of the sections. There is no memory for it.");
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

uint64_t Section::get_load_address() const
{
    return this -> load_address;
}

uint64_t Section::get_section_size() const
{
    return section_size;
}

uint64_t Section::get_section_rsize() const
{
    return this -> section_rsize;
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

void Section::set_load_address(uint64_t load_address)
{
    this -> load_address = load_address;
}

void Section::set_section_size(uint64_t section_size)
{
    this -> section_size = section_size;
}

void Section::set_section_rsize(uint64_t section_rsize)
{
    this -> section_rsize = section_rsize;
}

void Section::set_bytes(uint8_t* bytes)
{
    this -> bytes = bytes;
}
