#pragma once
#include "asm.hpp"
#include "num.hpp"
#include "vector.hpp"

struct BinaryIdentifier {
    uint8_t magic[4];
    uint8_t class_;
    uint8_t data;
    uint8_t version;
    uint8_t os_abi;
    uint8_t abi_version;
    uint8_t pad[7];
};

struct BinaryHeader {
    BinaryIdentifier ident; // File identification
    uint16_t type; // File type
    uint16_t machine; // Machine arch
    uint32_t version; // ELF fmt version
    uint64_t entry; // Entry point
    uint64_t prog_off; // Program header file offset
    uint64_t sect_off; // Section header file offset
    uint32_t flags; // Arch-specific flags
    uint32_t header_size; // ELF header size
    uint32_t prog_entry_size; // Program header entry size
    uint32_t prog_num_entries; // Num program header entries
    uint32_t sect_entry_size; // Section header entry size
    uint32_t sect_num_entries; // Num section header entries
    uint32_t sect_string_index; // Section name strings section
};

struct ProgramHeader {
    uint32_t type; // Entry type
    uint32_t flags; // Access permission flags
    uint64_t off; // File offset of contents
    uint64_t virt_addr; // Virtual address in memory image
    uint64_t phys_addr; // Physical address (not used)
    uint64_t file_size; // Size of contents in file
    uint64_t mem::size; // Size of contents in memory
    uint64_t align; // Alignment in memory and file
};

struct SectionHeader {
    uint32_t name; // Section name (index into the section header string table)
    uint32_t type; // Section type
    uint64_t flags; // Section flags
    uint64_t addr; // Address in memory image
    uint64_t off; // Offset in file
    uint64_t size; // Size in bytes
    uint32_t link; // Index of a related section
    uint32_t info; // Depends on section type
    uint64_t addr_align; // Alignment in bytes
    uint64_t entry_size; // Size of each entry in section
};

typedef Vector<uint8_t> Program;
typedef Vector<uint8_t> Section;

struct Binary {
    BinaryHeader bin_header;
    ProgramHeader prog_header;
    Program prog;
    void init();
    void fini();
    void add_program(Program program);
    Vector<uint8_t> build();
};