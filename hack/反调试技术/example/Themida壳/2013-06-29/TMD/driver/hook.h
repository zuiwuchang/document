//hook.h
//封装的hook相关的一些辅助函数
#pragma once
#include"all.h"
#define HOOKAPI __stdcall

/************************************************************SSDT 相关*********************************************************/

//获取 ssdt函数当前地址
#pragma PAGE_MEMORY
ULONG GetCurSsdtFunctionAddr(ULONG index);
//获取 ssdt函数当前地址
#pragma PAGE_MEMORY
ULONG SetCurSsdtFunctionAddr(ULONG index,ULONG addr);


/******************************************************进程 EPROCESS结构 相关**************************************************/

//根据 获取EPROCESS 全名
#pragma PAGE_MEMORY
VOID HOOKAPI GetProcessFullName(__out WCHAR* name_buffer,__in const ULONG size_buffer,__in const PEPROCESS pEprocess);
//原理同上 只不过是 过滤了路径 只留下了 镜像名
#pragma PAGE_MEMORY
VOID HOOKAPI GetProcessName(__out WCHAR* name_buffer,__in const ULONG size_buffer,__in const PEPROCESS pEprocess);

//同 PsGetCurrentProcess 只是可能会hook PsGetCurrentProcess 从而使PsGetCurrentProcess功能无效
#pragma PAGE_MEMORY
extern "C" PEPROCESS HOOKAPI MyPsGetCurrentProcessEx(VOID);