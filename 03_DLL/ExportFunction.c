// cl.exe /nologo /utf-8 /LD /Zi /Od /Ob0 /RTC1 /GS /sdl /W4 /MDd <source.c> /link /DEBUG:FULL /INCREMENTAL
// cl.exe /nologo /utf-8 /LD /O2 /GL /GS /sdl /guard:cf /Qspectre /Gy /Gw /GF /W4 /MD <source.c> /link /LTCG /INCREMENTAL:NO /OPT:REF /OPT:ICF /DYNAMICBASE /NXCOMPAT /HIGHENTROPYVA /CETCOMPAT /DEBUG:NONE

#include <Windows.h>

extern __declspec(dllexport) void HelloWorld()
{
    MessageBoxA(NULL, "Hello, DLL World!", "DLL Message", MB_ICONINFORMATION);
}

// Entry point for the DLL
BOOL APIENTRY DllMain ( HMODULE hModule,
                        DWORD dwReason,
                        LPVOID lpReserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
