//Hookmain.cpp

#include "stdio.h"
#include "conio.h"
#include "windows.h"

#define DEF_DLL_NAME "KeyHook.dll"
#define DEF_HOOKSTART "HookStart"
#define DEF_HOOKSTOP "HookStop" 

typedef void(*PFN_HOOKSTART)();
typedef void(*PFN_HOOKSTOP)();

void main()
{
    HMODULE hDll = NULL;
    PFN_HOOKSTART HookStart = NULL;
    PFN_HOOKSTOP HOOKSTOP = NULL;
    char ch = 0;

    hDll = LoadLibraryA(DEF_DLL_NAME);

    //获取导出函数地址
    HookStart = (PFN_HOOKSTART)GetProcAddress(hDll,DEF_HOOKSTART);
    HOOKSTOP = (PFN_HOOKSTOP)GetProcAddress(hDll,DEF_HOOKSTOP);

    HookStart();

    printf("Q end");
    while (_getch() != 'q');

    HOOKSTOP();
    FreeLibrary(hDll);

    
}
