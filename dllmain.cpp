// dllmain.cpp : 定义 DLL 应用程序的入口点。
//KeyHook.cpp
#include "pch.h"
#include "windows.h"
#include "stdio.h"

#define DEF_PROCESS_NAME "notepad.exe"

HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
HWND g_hWnd = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,//hModule参数：指向DLL本身的实例句柄
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hInstance = hModule;
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

//lParam 和 wParam 是宏定义，一般在消息函数中带这两个类型的参数，通常用来存储窗口消息的参数。
//wParam 通常用来存储小段信息，如标志.lParam 通常用于存储消息所需的对象
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    char szPath[MAX_PATH] = { 0, };//全文件名的字符长度是有限制的，MAX_PATH，260
    char *p = NULL;

    if ( nCode >= 0 )
    {
        // 输入参数lParam的第三十一bit代表键盘被按下或者释放，0代表按下，1代表释放
        if ( !(lParam & 0x80000000) )
        {
            //LPSTR被定义成是一个指向以NULL(‘\0’)结尾的32位ANSI字符数组指针，而LPWSTR是一个指向以NULL结尾的64位双字节字符数组指针
            GetModuleFileNameA(NULL, szPath, MAX_PATH);
            p = strrchr(szPath, '\\');//strrchr 参数 str 所指向的字符串中搜索第一次出现字符 c（一个无符号字符）的位置

            //比较进程名
            if (!_stricmp(p + 1, DEF_PROCESS_NAME))
                return 1;
        }
    }
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

#ifdef __cplusplus
extern "C"
#endif // __cplusplus

//__declspec(dllexport)用于Windows中的动态库中，声明导出函数、类、对象等供外面调用。
//即将函数、类等声明为导出函数，供其它程序调用，作为动态库的对外接口函数、类等

/*dllexport
字面意思暴露dll中的变量或方法
编译dll文件的时候,在dll头文件声明的变量名称前添加dllexport。表明这些东西可以被其他工程使用，
即是把 dll中的相关代码(类，函数，全局变量)暴露出来为以后其他应用程序使用。

dllimport
字面意思插入dll中的变量或方法
是在其他工程需要使用dll内相关内容时使用的关键字。当其他工程要使用dll 内部代码(类，函数，全局变量)时，
只需要在dll头文件中声明的变量名称前添加dllimport关键字即可,作用是把dll中的相关代码插入到应用程序中。

_declspec(dllexport)与_declspec(dllimport)是相互呼应，只有在dll内部用dllexport作了声明，才能在外部函数中用dllimport导入相关代码。
*/
__declspec(dllexport) void HookStart()
{
    g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hInstance, 0);
}
__declspec(dllexport) void HookStop()
{
    if (g_hHook)
    {
        UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
    }
}
#ifdef __cpluspuls
#endif