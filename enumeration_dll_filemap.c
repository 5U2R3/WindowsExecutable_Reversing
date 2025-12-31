#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

int wmain(int argc, wchar_t** argv)
{
    HANDLE hFile = CreateFileW(
        argv[1],
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    HANDLE hMap = CreateFileMappingW(
        hFile,
        NULL,
        PAGE_READONLY,
        0,
        0,
        NULL
    );

    void* base = MapViewOfFile(
        hMap,
        FILE_MAP_READ,
        0,
        0,
        0
    );

    BYTE* dos_base = (BYTE*)base;
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)dos_base;
    wprintf(L"e_magic = 0x%04X\n", dos->e_magic);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE)
    {
        wprintf(L"It's not PE File format.\n");
    }
    wprintf(L"e_lfanew = 0x%08X\n", dos->e_lfanew);

    BYTE* nt_base = dos_base + dos->e_lfanew;

    DWORD signature;
    memcpy(&signature, nt_base, sizeof(signature));
    if (signature != IMAGE_NT_SIGNATURE)
    {
        wprintf(L"Not PE\\0\\0\n");
        return 0;
    }

    // nt_base+offset
    BYTE* fh_base = nt_base + sizeof(DWORD);
    IMAGE_FILE_HEADER* fh = (IMAGE_FILE_HEADER*)fh_base;

    BYTE* optional_base = nt_base + sizeof(signature) + sizeof(IMAGE_FILE_HEADER);
    WORD magic;
    memcpy(&magic, optional_base, sizeof(magic));

    IMAGE_DATA_DIRECTORY* import_data_directory = NULL;

    if (magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
    {
        wprintf(L"this is 32bit\n");
        IMAGE_OPTIONAL_HEADER32* optional32 = (IMAGE_OPTIONAL_HEADER32*)optional_base;
        import_data_directory = &optional32->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    }
    else if (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
    {
        wprintf(L"this is 64bit\n");
        IMAGE_OPTIONAL_HEADER64* optional64 = (IMAGE_OPTIONAL_HEADER64*)optional_base;
        import_data_directory = &optional64->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    }
    else
    {
        wprintf(L"Unknown OptionalHeader.Magic: 0x%04X\n", magic);
        return 0;
    }

    BYTE* section_table_array_base = optional_base + fh->SizeOfOptionalHeader;
    IMAGE_SECTION_HEADER* section_table_array = (IMAGE_SECTION_HEADER*)section_table_array_base;
    WORD section_num = fh->NumberOfSections;

    if (import_data_directory->VirtualAddress == 0) {
        wprintf(L"No Import Directory\n");
        return 0;
    }

    // Import RVA -> FOA (file pointer)
    DWORD import_table_rva = import_data_directory->VirtualAddress;
    BYTE* import_descriptor_array_base = dos_base + import_table_rva; // fallback
    for (WORD i = 0; i < section_num; i++) {
        DWORD section_va   = section_table_array[i].VirtualAddress;
        DWORD section_vs = section_table_array[i].Misc.VirtualSize;
        DWORD section_ptRD  = section_table_array[i].PointerToRawData;
        DWORD section_sRD = section_table_array[i].SizeOfRawData;

        DWORD span;
        if (section_vs >= section_sRD)
        {
            span = section_vs;
        }
        else
        {
            span = section_sRD;
        }
        if (import_table_rva >= section_va && import_table_rva < section_va + span) {
            import_descriptor_array_base = dos_base + section_ptRD + (import_table_rva - section_va);
            break;
        }
    }

    // インポートテーブルはIMAGE_IMPORT_DESCRIPTORの配列
    IMAGE_IMPORT_DESCRIPTOR* import_descriptor_array = (IMAGE_IMPORT_DESCRIPTOR*)import_descriptor_array_base;

    for (; import_descriptor_array->Name != 0; import_descriptor_array++) {
        DWORD dll_name_rva = import_descriptor_array->Name;

        // Name RVA -> FOA (file pointer)
        BYTE* name_ptr = dos_base + dll_name_rva; // fallback
        for (WORD i = 0; i < section_num; i++) {
            DWORD section_va   = section_table_array[i].VirtualAddress;
            DWORD section_vs = section_table_array[i].Misc.VirtualSize;
            DWORD section_ptRD  = section_table_array[i].PointerToRawData;
            DWORD section_sRD = section_table_array[i].SizeOfRawData;

            DWORD span;
            if (section_vs >= section_sRD)
            {
                span = section_vs;
            }
            else
            {
                span = section_sRD;
            }
            if (dll_name_rva >= section_va && dll_name_rva < section_va + span) {
                name_ptr = dos_base + section_ptRD + (dll_name_rva - section_va);
                break;
            }
        }

        char* dll = (char*)name_ptr;
        wprintf(L"%hs\n", dll);
    }

    UnmapViewOfFile(base);
    CloseHandle(hMap);
    CloseHandle(hFile);
    return 0;
}
