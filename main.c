#include <stdio.h>
#include "structs.h"
#include "status.h"
#include <malloc.h>

#define LEN 100

const char Name[LEN] = {"Name:"};
const char VSize[LEN] = {"VirtualSize:"};
const char VAddress[LEN] = {"VirtualSize:"};
const char SRowData[LEN] = {"SizeOfRowData:"};
const char PTORawData[LEN] = {"PointerToRawData:"};
const char PToReloc[LEN] = {"PointerToRelocations:"};
const char PToLineNum[LEN] = {"PointerToLineNumbers:"};
const char NOfReloc[LEN] = {"NumberOfRelocations:"};
const char NOfLineNum[LEN] = {"NumberOfLineNumbers:"};
const char Characters[LEN] = {"Characteristics:"};

void writeSection(IMAGE_SECTION_HEADER section, FILE *file) {
    fprintf(file, "--------------\n");
    fprintf(file, "%s %8s, ", Name, section.Name);
    fprintf(file, "%s %8X, ", VSize, section.Misc.VirtualSize);
    fprintf(file, "%s %8X, ", VAddress, section.VirtualAddress);
    fprintf(file, "%s %8X, ", SRowData, section.SizeOfRawData);
    fprintf(file, "%s %8X, ", PTORawData, section.PointerToRawData);
    fprintf(file, "%s %X, ", PToReloc, section.PointerToRelocations);
    fprintf(file, "%s %X, ", PToLineNum, section.PointerToLineNumbers);
    fprintf(file, "%s %X, ", NOfReloc, section.NumberOfRelocations);
    fprintf(file, "%s %X, ", NOfLineNum, section.NumberOfLineNumbers);
    fprintf(file, "%s %8X, ", Characters, section.Characteristics);
    fprintf(file, "\n");
}

int main() {

    FILE *FRead;
    FILE *FWrite1;
    FILE *FWrite2;
    IMAGE_DOS_HEADER DosHeader = {0};
    IMAGE_NT_HEADERS ImageNtHeaders = {0};

    if ((FRead = fopen("input.exe", "rb")) == NULL) { //todo close if error
        printf("Cannot open file reading.\n");
        return CANT_OPEN_FILE;
    }

    if ((FWrite1 = fopen("write1.txt", "w+")) == NULL) {
        printf("Cannot open file writing 1.\n");
        fclose(FRead);
        return CANT_OPEN_FILE;
    }

    if ((FWrite2 = fopen("write2.txt", "w+b")) == NULL) {
        printf("Cannot open file writing 2.\n");
        fclose(FRead);
        fclose(FWrite1);
        return CANT_OPEN_FILE;
    }

    fread(&DosHeader.EMagic, sizeof(int16_t), 1, FRead); //todo add checkers (if statements)
    fseek(FRead, 0x3c, SEEK_SET);
    fread(&DosHeader.ELFANew, sizeof(int32_t), 1, FRead);

    if (DosHeader.EMagic != 0x5a4d) {
        printf("Incorrect start of binary\n");
        fclose(FRead);
        fclose(FWrite1);
        fclose(FWrite2);
        return INCORRECT_START;
    }

    fseek(FRead, DosHeader.ELFANew, SEEK_SET);
    fread(&ImageNtHeaders.Signature, sizeof(int32_t), 1, FRead);

    if (ImageNtHeaders.Signature != 0x4550) {
        printf("Incorrect signature\n");
        fclose(FRead);
        fclose(FWrite1);
        fclose(FWrite2);
        return INCORRECT_SIGNATURE;
    }

    fread(&ImageNtHeaders.FileHeader, sizeof(IMAGE_FILE_HEADER), 1, FRead);

    if (!(ImageNtHeaders.FileHeader.Characteristics & 0x0002)) {
        printf("File doesn't executable");
        fclose(FRead);
        fclose(FWrite1);
        fclose(FWrite2);
        return INCORRECT_FILE_EXECUTION;
    }

    fread(&ImageNtHeaders.OptionalHeader.StandCoffFields, sizeof(STAND_COFF_FIELDS), 1, FRead);

    if (ImageNtHeaders.OptionalHeader.StandCoffFields.Magic != 0x10B) {
        printf("File isn't in PE32 format");
        fclose(FRead);
        fclose(FWrite1);
        fclose(FWrite2);
        return INCORRECT_FILE_TYPE;
    }

    fread(&ImageNtHeaders.OptionalHeader.WindowsSpecificFields, sizeof(WINDOWS_SPECIFIC_FIELDS), 1, FRead);

    for (int i = 0; i < IMAGE_NUMBER_OF_DIRECTORY_ENTRIES; i++) {
        fread(&ImageNtHeaders.OptionalHeader.DataDirectory[i], sizeof(IMAGE_DATA_DIRECTORY), 1, FRead);
    }

    if (!ImageNtHeaders.FileHeader.NumberOfSections) {
        fclose(FRead);
        fclose(FWrite1);
        fclose(FWrite2);
        return ERROR_DATA_NOT_READ;
    }

    const int16_t size = ImageNtHeaders.FileHeader.NumberOfSections;
    IMAGE_SECTION_HEADER *sections = malloc(sizeof(IMAGE_SECTION_HEADER) * size);

    for (int i = 0; i < size; ++i) {
        fread(&sections[i], sizeof(IMAGE_SECTION_HEADER), 1, FRead);
        writeSection(sections[i], FWrite1);

    }

    for (int i = 0; i < size; ++i) {
        if (sections[i].Characteristics & 0x00000020) {
            printf("HERE %s\n", sections[i].Name);
            fseek(FRead, sections[i].PointerToRawData, SEEK_SET);
            int8_t *CopyData = malloc(sections[i].SizeOfRawData * sizeof(int8_t));
            fread(CopyData, sections[i].SizeOfRawData * sizeof(int8_t), 1, FRead);
            fwrite(CopyData, sections[i].SizeOfRawData * sizeof(int8_t), 1, FWrite2);

            /*for (int j = 0; j < sections[i].SizeOfRawData; ++j) {
                int8_t data;
                fread(&data, sizeof(int8_t), 1, FRead);
                fwrite(&data, sizeof(int8_t), 1, FWrite2);
            }*/
        }
    }

    fprintf(FWrite1, "--------------\n");
    fprintf(FWrite1, "ImageBase: %X\n", ImageNtHeaders.OptionalHeader.WindowsSpecificFields.ImageBase);
    fprintf(FWrite1, "AddressOfEntryPoint: %X\n", ImageNtHeaders.OptionalHeader.StandCoffFields.AddressOfEntryPoint);


    /*for (int i = 0; i < size; ++i) {
        printf("%s %X %X %X %X %X %X %X %X %X", sections[i].Name, sections[i].Misc.VirtualSize, sections[i].VirtualAddress, sections[i].SizeOfRawData,
               sections[i].PointerToRawData, sections[i].PointerToRelocations, sections[i].PointerToLineNumbers, sections[i].NumberOfRelocations, sections[i].NumberOfLineNumbers,
               sections[i].Characteristics);
        printf("\n");
    }*/

    free(sections);
    fclose(FRead);
    fclose(FWrite1);
    fclose(FWrite2);

    printf("DONE!");
    return OK;
}

