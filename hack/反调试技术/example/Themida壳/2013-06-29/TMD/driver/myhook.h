#pragma once
#include"all.h"

#pragma PAGE_MEMORY
void Hook();
#pragma PAGE_MEMORY
void UnHook();

//原函数声明
extern "C" 
typedef NTSTATUS  
	(__stdcall *HookNtSetInformationThread)(
    IN HANDLE  ThreadHandle,
    IN THREADINFOCLASS  ThreadInformationClass,
    IN PVOID  ThreadInformation,
    IN ULONG  ThreadInformationLength
    );
extern "C"
typedef NTSTATUS 
	(__stdcall *HookNtQueryInformationProcess)(
  __in       HANDLE ProcessHandle,
  __in       PROCESSINFOCLASS ProcessInformationClass,
  __out      PVOID ProcessInformation,
  __in       ULONG ProcessInformationLength,
  __out_opt  PULONG ReturnLength
);

extern "C"
typedef NTSTATUS  
	(__stdcall *HookNtGetContextThread)(
    IN HANDLE  ThreadHandle,
	IN CONTEXT* lpContext
    );
//HOOK 函数声明
extern "C" 
#pragma PAGE_MEMORY
NTSTATUS 
  __stdcall MyNtSetInformationThread(
    IN HANDLE  ThreadHandle,
    IN THREADINFOCLASS  ThreadInformationClass,
    IN PVOID  ThreadInformation,
    IN ULONG  ThreadInformationLength
    );


extern "C"
#pragma PAGE_MEMORY
NTSTATUS 
	__stdcall MyNtQueryInformationProcess(
	__in       HANDLE ProcessHandle,
	__in       PROCESSINFOCLASS ProcessInformationClass,
	__out      PVOID ProcessInformation,
	__in       ULONG ProcessInformationLength,
	__out_opt  PULONG ReturnLength
);

extern "C" 
#pragma PAGE_MEMORY
NTSTATUS 
  __stdcall MyNtGetContextThread(
    IN HANDLE  ThreadHandle,
	IN CONTEXT* lpContext
    );