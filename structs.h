#include <stdint.h>

#define IMAGE_NUMBER_OF_DIRECTORY_ENTRIES 16
#define IMAGE_SIZEOF_SHORT_NAME 8

//Directory Entries
#define IMAGE_DIRECTORY_ENTRY_EXPORT            0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT            1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE            2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION        3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY            4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASE_RELOC        5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG            6   // Debug Directory
//IMAGE_DIRECTORY_ENTRY_COPYRIGHT       		7   (X86 usage)
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE        7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBAL_PTR        8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS                9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG        10   // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT    11   // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT                12   // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT    13   // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR    14   // COM Runtime descriptor

typedef struct {
    int16_t EMagic;      /* 00: MZ Header signature */
    //...Тут какая-то почти 'бесполезная' инфа...
    int32_t ELFANew;     /* 3c: Offset to extended header */
} IMAGE_DOS_HEADER;

typedef struct {
    int16_t Machine;
    int16_t NumberOfSections;
    int32_t TimeDateStamp;
    int32_t PointerToSymbolTable;
    int32_t NumberOfSymbols;
    int16_t SizeOfOptionalHeader;
    int16_t Characteristics;
} IMAGE_FILE_HEADER;

typedef struct {
    int32_t VirtualAddress;
    int32_t Size;
} IMAGE_DATA_DIRECTORY;

typedef struct {
    int16_t Magic;
    int8_t MajorLinkerVersion;
    int8_t MinorLinkerVersion;
    int32_t SizeOfCode;
    int32_t SizeOfInitializedData;
    int32_t SizeOfUninitializedData;
    int32_t AddressOfEntryPoint;
    int32_t BaseOfCode;
    int32_t BaseOfData;
} STAND_COFF_FIELDS;

typedef struct {
    int32_t ImageBase;
    int32_t SectionAlignment;
    int32_t FileAlignment;
    int16_t MajorOperatingSystemVersion;
    int16_t MinorOperatingSystemVersion;
    int16_t MajorImageVersion;
    int16_t MinorImageVersion;
    int16_t MajorSubsystemVersion;
    int16_t MinorSubsystemVersion;
    int32_t Win32VersionValue;
    int32_t SizeOfImage;
    int32_t SizeOfHeaders;
    int32_t CheckSum;
    int16_t Subsystem;
    int16_t DllCharacteristics;
    int32_t SizeOfStackReserve;
    int32_t SizeOfStackCommit;
    int32_t SizeOfHeapReserve;
    int32_t SizeOfHeapCommit;
    int32_t LoaderFlags;
    int32_t NumberOfRvaAndSizes;
} WINDOWS_SPECIFIC_FIELDS;

typedef struct {
    STAND_COFF_FIELDS StandCoffFields;
    WINDOWS_SPECIFIC_FIELDS WindowsSpecificFields;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBER_OF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER;

typedef struct {
    int32_t Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER OptionalHeader;
} IMAGE_NT_HEADERS;

typedef struct {
    int8_t Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
        int32_t PhysicalAddress;
        int32_t VirtualSize;
    } Misc;
    int32_t VirtualAddress;
    int32_t SizeOfRawData;
    int32_t PointerToRawData;
    int32_t PointerToRelocations;
    int32_t PointerToLineNumbers;
    int16_t NumberOfRelocations;
    int16_t NumberOfLineNumbers;
    int32_t Characteristics;
} IMAGE_SECTION_HEADER;