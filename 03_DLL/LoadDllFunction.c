// cl.exe /nologo /utf-8 /Zi /Od /Ob0 /RTC1 /GS /sdl /W3 /MDd <source.c> /link /INCREMENTAL /DEBUG:FULL
// cl.exe /nologo /utf-8 /O2 /GL /Ob2 /GS /guard:cf /Qspectre /sdl /Gy /Gw /GF /W3 /MD <source.c> /link /LTCG /INCREMENTAL:NO /OPT:REF /OPT:ICF /DEBUG:NONE

#include <windows.h>

typedef void (WINAPI* HelloWorldFunctionPointer)();

int main(int argc, char *argv[])
{

    HMODULE hModule = GetModuleHandleA("ExportFunction.dll");

    if (hModule == NULL)
    {
        hModule = LoadLibraryA("ExportFunction.dll");
    }

    PVOID pHelloWorld = GetProcAddress(hModule, "HelloWorld");

    HelloWorldFunctionPointer HelloWorld = (HelloWorldFunctionPointer)pHelloWorld;

    HelloWorld();

    return 0;
}
