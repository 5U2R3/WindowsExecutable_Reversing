; int __fastcall WinMain(HINSTANCE__ *hInstance, HINSTANCE__ *hPrevInstance, char *lpCmdLine, int nCmdShow)
WinMain proc near

hInstance= qword ptr  8
hPrevInstance= qword ptr  10h
lpCmdLine= qword ptr  18h
nCmdShow= dword ptr  20h

mov     [rsp+nCmdShow], r9d
mov     [rsp+lpCmdLine], r8
mov     [rsp+hPrevInstance], rdx
mov     [rsp+hInstance], rcx
push    rdi
sub     rsp, 20h        ; stack point sub
mov     r9d, 40h ; '@'  ; uType
lea     r8, Caption     ; "Hello"
lea     rdx, Text       ; "Hello, World!"
xor     ecx, ecx        ; hWnd
call    cs:__imp_MessageBoxA
xor     eax, eax
add     rsp, 20h
pop     rdi
retn
WinMain endp
