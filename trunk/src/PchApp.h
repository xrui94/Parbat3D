#ifndef PCH_APP_H
#define PCH_APP_H

/*
	This header provides program-wide declarations and imports.  It should only
	include system/third-party headers, as changes will require re-building all
	modules.
	
	Each .cpp file in the project should #include this file before any other
	includes, declarations, or code.
*/ 

#include <gtkmm.h>

#ifdef __WINE__
#ifndef _WIN32
#define __stdcall __attribute__((ms_abi)) 
#define __cdecl __attribute__((ms_abi)) 
#define _stdcall __attribute__((ms_abi)) 
#define _cdecl __attribute__((ms_abi)) 
#define __fastcall __attribute__((ms_abi)) 
#define _fastcall __attribute__((ms_abi)) 
#define __declspec(x) __declspec_##x 
#define __declspec_align(x) __attribute__((aligned(x))) 
#define __declspec_allocate(x) __attribute__((section(x))) 
#define __declspec_deprecated __attribute__((deprecated)) 
#define __declspec_dllimport __attribute__((dllimport)) 
#define __declspec_dllexport __attribute__((dllexport)) 
#define __declspec_naked __attribute__((naked)) 
#define __declspec_noinline __attribute__((noinline)) 
#define __declspec_noreturn __attribute__((noreturn)) 
#define __declspec_nothrow __attribute__((nothrow)) 
#define __declspec_novtable __attribute__(()) 
#define __declspec_selectany __attribute__((weak)) 
#define __declspec_thread __thread 
#define __int8 char 
#define __int16 short
#define __int32 int 
#define __int64 long 

#endif // !_WIN32
#endif // __WINE__

// 不要在非 Windows 平台强制定义 WIN32/WIN64 宏，避免污染第三方头的条件编译判定。



// We use std::string all over the place, but other std:: namespaces should be
// explicit
#include <string>
using std::string;

// We use a fair bit of stuff from the C/C++ standard libraries
#include <cassert>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

#include <sstream>
#include <iostream>   
#include <fstream>
#include <sys/types.h>

#include <limits>
#include <algorithm>


// C++ standard containers
#include <vector>
#include <list>
#include <queue>
#include <deque>

// OpenGL 与平台相关头文件
#ifdef _WIN32
    // 在 Windows 上，必须先包含 windows.h，再包含 OpenGL 头
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #ifndef WINVER
        #define WINVER 0x0601
    #endif
    #ifndef _WIN32_IE
        #define _WIN32_IE 0x0900
    #endif
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0601
    #endif
    #include <windows.h>
    #include <commctrl.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#else
    // 在非 Windows（Linux/WSL）上，仅包含标准 OpenGL 头
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

// And GDAL
//#define CPL_STDCALL __stdcall
//#define CPL_DISABLE_STDCALL 1
#include <gdal_priv.h>


#endif // include guard

