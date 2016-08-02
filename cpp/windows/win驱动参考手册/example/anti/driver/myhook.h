#pragma once
#include"all.h"

//原函数指针声明
extern "C"
typedef
NTSTATUS (__stdcall *OldNtQueryInformationProcess)(
  __in       HANDLE ProcessHandle,
  __in       PROCESSINFOCLASS ProcessInformationClass,
  __out      PVOID ProcessInformation,
  __in       ULONG ProcessInformationLength,
  __out_opt  PULONG ReturnLength
);


//自定义hook函数

extern "C"
NTSTATUS __stdcall MyNtQueryInformationProcess(
  __in       HANDLE ProcessHandle,
  __in       PROCESSINFOCLASS ProcessInformationClass,
  __out      PVOID ProcessInformation,
  __in       ULONG ProcessInformationLength,
  __out_opt  PULONG ReturnLength
);

VOID Hook_NtQueryInformationProcess();
VOID Unhook_NtQueryInformationProcess();
