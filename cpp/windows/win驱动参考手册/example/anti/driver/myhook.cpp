#include"myhook.h"
#include"hook.h"
OldNtQueryInformationProcess RealNtQueryInformationProcess;

#pragma PAGE_MEMORY
extern "C"
NTSTATUS __stdcall MyNtQueryInformationProcess(
  __in       HANDLE ProcessHandle,
  __in       PROCESSINFOCLASS ProcessInformationClass,
  __out      PVOID ProcessInformation,
  __in       ULONG ProcessInformationLength,
  __out_opt  PULONG ReturnLength
)
{
	
	NTSTATUS status = STATUS_SUCCESS;

	status=RealNtQueryInformationProcess(ProcessHandle,ProcessInformationClass,ProcessInformation,ProcessInformationLength,ReturnLength);
 
	if(STATUS_SUCCESS==status && 7==ProcessInformationClass)
	{
		*(ULONG*)ProcessInformation=0;
		KdPrint(("%d\n",*(ULONG*)ProcessInformation));
	}

	
	return status;
}
#pragma PAGE_MEMORY
VOID Hook_NtQueryInformationProcess()
{
	ULONG index=0xEA;
	ULONG my_address=(ULONG)MyNtQueryInformationProcess;
	ULONG cur_address=GetCurSsdtFunctionAddr(index);
	if(my_address!=cur_address)
	{
		cur_address=SetCurSsdtFunctionAddr(index,my_address);
		RealNtQueryInformationProcess=(OldNtQueryInformationProcess)cur_address;
		KdPrint(("Hook ok old NtQueryInformationProcess=0x%08X",cur_address));
	}
	else
	{
		KdPrint(("already Hook "));
	}
}
#pragma PAGE_MEMORY
VOID Unhook_NtQueryInformationProcess()
{
	ULONG index=0xEA;
	ULONG my_address=(ULONG)MyNtQueryInformationProcess;
	ULONG cur_address=GetCurSsdtFunctionAddr(index);
	
	if(my_address==cur_address)
	{
		cur_address=SetCurSsdtFunctionAddr(index,(ULONG)RealNtQueryInformationProcess);
		KdPrint(("Unhook ok old NtQueryInformationProcess=0x%08X",cur_address));
	}
	else
	{
		KdPrint(("no my Hook "));
	}
}

