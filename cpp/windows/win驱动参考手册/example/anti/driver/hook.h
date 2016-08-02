#pragma once
#include"all.h"

/************************************************************SSDT 相关*********************************************************/

//获取 ssdt函数当前地址
ULONG GetCurSsdtFunctionAddr(ULONG index);
//获取 ssdt函数当前地址
ULONG SetCurSsdtFunctionAddr(ULONG index,ULONG addr);


/******************************************************进程 EPROCESS结构 相关**************************************************/

//根据 获取EPROCESS 全名
VOID GetProcessFullName(__out WCHAR* name_buffer,__in const ULONG size_buffer,__in const PEPROCESS pEprocess);
//原理同上 只不过是 过滤了路径 只留下了 镜像名
VOID __stdcall GetProcessName(__out WCHAR* name_buffer,__in const ULONG size_buffer,__in const PEPROCESS pEprocess);

//同 PsGetCurrentProcess 只是可能会hook PsGetCurrentProcess 从而使PsGetCurrentProcess功能无效
extern "C" PEPROCESS __stdcall MyPsGetCurrentProcessEx(VOID);