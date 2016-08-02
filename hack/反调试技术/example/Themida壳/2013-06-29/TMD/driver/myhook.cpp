#include"myhook.h"
#include"hook.h"

BOOLEAN is_hook;

//记录保护进程名
WCHAR protected_name[MAX_PATH];

HookNtSetInformationThread RealNtSetInformationThread;
HookNtQueryInformationProcess RealNtQueryInformationProcess;
HookNtGetContextThread RealNtGetContextThread;

#pragma PAGE_MEMORY
void Hook()
{
	if(!is_hook)
	{
		ULONG index=0x14F;
		RealNtSetInformationThread=(HookNtSetInformationThread)SetCurSsdtFunctionAddr(index,(ULONG)MyNtSetInformationThread);
		
		index=0xEA;
		RealNtQueryInformationProcess=(HookNtQueryInformationProcess)SetCurSsdtFunctionAddr(index,(ULONG)MyNtQueryInformationProcess);
		
		index=0x87;
		RealNtGetContextThread=(HookNtGetContextThread)SetCurSsdtFunctionAddr(index,(ULONG)MyNtGetContextThread);
		

		is_hook=TRUE;	
	}
}

#pragma PAGE_MEMORY
void UnHook()
{
	if(is_hook)
	{
		ULONG my=(ULONG)MyNtSetInformationThread;
		ULONG index=0x14F;
		ULONG now=GetCurSsdtFunctionAddr(index);
		if(my==now)
		{
			SetCurSsdtFunctionAddr(index,(ULONG)RealNtSetInformationThread);
		}

		my=(ULONG)MyNtQueryInformationProcess;
		index=0xEA;
		now=GetCurSsdtFunctionAddr(index);
		if(my==now)
		{
			SetCurSsdtFunctionAddr(index,(ULONG)RealNtQueryInformationProcess);
		}

		my=(ULONG)MyNtGetContextThread;
		index=0x87;
		now=GetCurSsdtFunctionAddr(index);
		if(my==now)
		{
			SetCurSsdtFunctionAddr(index,(ULONG)RealNtGetContextThread);
		}

		is_hook=FALSE;
	}
}

extern "C" 
#pragma PAGE_MEMORY
NTSTATUS 
  __stdcall MyNtSetInformationThread(
    IN HANDLE  ThreadHandle,
    IN THREADINFOCLASS  ThreadInformationClass,
    IN PVOID  ThreadInformation,
    IN ULONG  ThreadInformationLength
    )
{
	NTSTATUS status=STATUS_SUCCESS; 
	if(ThreadInformationClass==ThreadHideFromDebugger /*0x11*/)
	{
		PEPROCESS pEprocess=PsGetCurrentProcess();
		WCHAR image_name[MAX_PATH]={0};
		GetProcessName(image_name,sizeof(image_name),pEprocess);
		
		UNICODE_STRING name,pro_ame;
		RtlInitUnicodeString(&name,image_name);
		RtlInitUnicodeString(&pro_ame,protected_name);
		
		if(RtlEqualUnicodeString(&name,&pro_ame,TRUE))
		{
			return status;
		}
	}

	return RealNtSetInformationThread(ThreadHandle,ThreadInformationClass,ThreadInformation,ThreadInformationLength);

}

extern "C"
#pragma PAGE_MEMORY
NTSTATUS 
	__stdcall MyNtQueryInformationProcess(
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
	}
	
	return status;
}

extern "C" 
#pragma PAGE_MEMORY
NTSTATUS 
  __stdcall MyNtGetContextThread(
    IN HANDLE  ThreadHandle,
	IN CONTEXT* lpContext
    )
{
	NTSTATUS status=STATUS_SUCCESS; 

	status=RealNtGetContextThread(ThreadHandle,lpContext);
 
	PEPROCESS pEprocess=PsGetCurrentProcess();
	WCHAR image_name[MAX_PATH]={0};
	GetProcessName(image_name,sizeof(image_name),pEprocess);
	
	UNICODE_STRING name,pro_ame;
	RtlInitUnicodeString(&name,image_name);
	RtlInitUnicodeString(&pro_ame,protected_name);
	
	
	if(RtlEqualUnicodeString(&name,&pro_ame,TRUE))
	{
		lpContext->Dr0=0;
		lpContext->Dr1=0;
		lpContext->Dr2=0;
		lpContext->Dr3=0;
		lpContext->Dr6=0;
		lpContext->Dr7=0;

	}

	return status;
}