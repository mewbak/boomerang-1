#pragma region License
/*
 * This file is part of the Boomerang Decompiler.
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 */
#pragma endregion License
#pragma once


#include "boomerang/ifc/IFileLoader.h"
#include "boomerang/util/ByteUtil.h"

struct Elf32_Ehdr;
struct Elf32_Phdr;
struct Elf32_Shdr;
struct Elf32_Rel;
struct Elf32_Sym;
struct Translated_ElfSym;
class BinaryImage;
class BinarySymbolTable;
class QFile;
class BinarySection;


/**
 * File loader for loading 32 bit binary ELF files.
 */
class ElfBinaryLoader : public IFileLoader
{
    typedef std::map<Address, QString, std::less<Address> > RelocMap;

public:
    ElfBinaryLoader();
    virtual ~ElfBinaryLoader() override;

    /// \copydoc IFileLoader::initialize
    void initialize(BinaryImage *image, BinarySymbolTable *symbols) override;

    /// \copydoc IFileLoader::canLoad
    int canLoad(QIODevice& fl) const override;

    /// \copydoc IFileLoader::loadFromMemory
    /// Note that empty sections will not be added to the image.
    bool loadFromMemory(QByteArray& img) override;

    /// \copydoc IFileLoader::unload
    void unload() override;

    /// \copydoc IFileLoader::close
    void close() override;

    /// \copydoc IFileLoader::getFormat
    LoadFmt getFormat() const override;

    /// \copydoc IFileLoader::getMachine
    Machine getMachine() const override;

    /// \copydoc IFileLoader::getMainEntryPoint
    /// (this should be a label in elf binaries generated by compilers).
    virtual Address getMainEntryPoint() override;

    /// \copydoc IFileLoader::getEntryPoint
    virtual Address getEntryPoint() override;

    /// \copydoc IFileLoader::isRelocationAt
    bool isRelocationAt(Address addr) override;

private:
    /// Reset internal state, except for those that keep track of which member
    /// we're up to
    void init();

    /// \returns true if this file is a shared library file.
    bool isLibrary() const;

    /// Return a list of library names which the binary file depends on
    QStringList getDependencyList();

    // Apply relocations; important when compiled without -fPIC
    void applyRelocations();

    /// Not meant to be used externally, but sometimes you just have to have it.
    /// Like a replacement for elf_strptr().
    /// If the string pointer could not be found, this function returns nullptr.
    const char *getStrPtr(int sectionIdx, int offset); // Calc string pointer


    /// FIXME: the below assumes a fixed delta
    HostAddress nativeToHostAddress(Address addr);

    /// Add appropriate symbols to the symbol table.
    /// \p secIndex is the section index of the symbol table.
    void addSymbolsForSection(int secIndex);

    /// FIXME: this function is way off the rails. It seems to always overwrite the relocation entry with the 32 bit value
    /// from the symbol table. Totally invalid for SPARC, and most X86 relocations!
    /// So currently not called
    void addRelocsAsSyms(uint32_t secIndex);

    /// Search the .rel[a].plt section for an entry with symbol table index i.
    /// If found, return the native address of the associated PLT entry.
    /// A linear search will be needed. However, starting at offset i and searching backwards with wraparound should
    /// typically minimise the number of entries to search
    Address findRelPltOffset(int i);

    // Internal elf reading methods // TODO replace by Util::swapEndian

    SWord elfRead2(const SWord *ps) const; // Read a 16 bit value, respecting source endianness
    DWord elfRead4(const DWord *pi) const; // Read a 32 bit value, respecting source endianness
    void elfWrite4(DWord *pi, DWord val);  // Write an 32 bit value, respecting destination endianness

    /**
     * Mark all imported symbols as such.
     * This function relies on the fact that the symbols are sorted by address,
     * and that Elf PLT entries have successive addresses beginning soon after m_PltMin
     */
    void markImports();

    void processSymbol(Translated_ElfSym& sym, int e_type, int i);

private:
    size_t m_loadedImageSize = 0;               ///< Size of image in bytes
    Byte *m_loadedImage = nullptr;              ///< Pointer to the loaded image

    Elf32_Ehdr *m_elfHeader   = nullptr;        ///< ELF header
    Elf32_Phdr *m_programHdrs = nullptr;        ///< Pointer to program headers
    Elf32_Shdr *m_sectionHdrs = nullptr;        ///< Array of section header structs

    const char *m_strings = nullptr;            ///< Pointer to the string section
    Endian m_endian = Endian::Little;

    const Elf32_Rel *m_relocSection  = nullptr; ///< Pointer to the relocation section
    const Elf32_Sym *m_symbolSection = nullptr; ///< Pointer to loaded symbol section

    bool m_relocHasAddend = false;              ///< true if reloc table has addend
    Address m_lastAddr = Address::INVALID;      ///< Save last address looked up
    int m_lastSize = 0;                         ///< Size associated with that name
    Address m_pltMin = Address::INVALID;        ///< Min address of PLT table
    Address m_pltMax = Address::INVALID;        ///< Max address (1 past last) of PLT
    Address *m_importStubs = nullptr;           ///< An array of import stubs
    Address m_baseAddr = Address::INVALID;      ///< Base image virtual address
    Address m_firstExtern = Address::INVALID;   ///< where the first extern will be placed
    Address m_nextExtern = Address::INVALID;    ///< where the next extern will be placed
    uint32 *m_shLink = nullptr;                 ///< pointer to array of sh_link values
    uint32 *m_shInfo = nullptr;                 ///< pointer to array of sh_info values

    std::vector<struct SectionParam> m_elfSections;
    BinaryImage *m_binaryImage = nullptr;
    BinarySymbolTable *m_symbols = nullptr;
};
