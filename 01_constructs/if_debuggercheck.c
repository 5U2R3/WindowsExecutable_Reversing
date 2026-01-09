// cl.exe /nologo /utf-8 /Zi /Od /Ob0 /RTC1 /GS /sdl /W3 /MDd <source.c> /link /INCREMENTAL /DEBUG:FULL
// cl.exe /nologo /utf-8 /O2 /GL /Ob2 /GS /guard:cf /Qspectre /sdl /Gy /Gw /GF /W3 /MD <source.c> /link /LTCG /INCREMENTAL:NO /OPT:REF /OPT:ICF /DEBUG:NONE

#include <windows.h>

int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpszCmdLine, 
    int nCmdShow)
{
    if (IsDebuggerPresent() == 0)
    {
        MessageBox(
            NULL,
            TEXT("Debugger No Detected..."),
            TEXT("Debugger Check"),
            MB_OK | MB_ICONINFORMATION
        );
    }
    else
    {
        MessageBox(
            NULL,
            TEXT("Debugger Detected !"),
            TEXT("Debugger Check"),
            MB_OK | MB_ICONEXCLAMATION
        );
    }

return 0;
}
