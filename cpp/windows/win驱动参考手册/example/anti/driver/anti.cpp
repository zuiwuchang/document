#include"anti.h"
#include"myhook.h"

#pragma PAGE_MEMORY
NTSTATUS ReadDebugPort(IN PIRP pIrp,ULONG inSize,ULONG outSize)
{
	KdPrint(("ReadDebugPort begin\n"));
	//MyPsGetCurrentProcess();

	NTSTATUS status = STATUS_SUCCESS;

	PVOID lp_output_buffer=NULL;

	//如果传入了输出缓冲区
	if(pIrp->MdlAddress && outSize>3)
	{
		//锁定输出缓冲区 并映射到内核以供 驱动使用
		lp_output_buffer=MmGetSystemAddressForMdlSafe(pIrp->MdlAddress,NormalPagePriority );

		KdPrint(("ReadDebugPort inSize=%d outSize=%d\n",inSize,outSize));
	
		PEPROCESS lp_eprocess=PsGetCurrentProcess();
		__asm
		{
			mov ebx,lp_output_buffer;
			mov eax,lp_eprocess;

			add eax,0xec;			//PEPROCESS	偏移
			mov eax,[eax];
			mov [ebx],eax;

		}
		pIrp->IoStatus.Information=4;
		KdPrint(("ReadDebugPort end%d\n",lp_output_buffer));
	}
	return status;
}

#pragma PAGE_MEMORY
NTSTATUS ReadDebugPortSet(IN PIRP pIrp,ULONG inSize,ULONG outSize)
{
	KdPrint(("WriteDirectIn begin\n"));
	NTSTATUS status = STATUS_SUCCESS;

	//获取用户层传入的 输入缓冲区的地址
	PVOID lp_input_buffer=pIrp->AssociatedIrp.SystemBuffer,
		lp_output_buffer=NULL;

	//如果传入了输出缓冲区
	if(pIrp->MdlAddress)
	{
		//锁定输出缓冲区 并映射到内核以供 驱动使用
		lp_output_buffer=MmGetSystemAddressForMdlSafe(pIrp->MdlAddress,NormalPagePriority );
	}

	
	PEPROCESS lp_eprocess=PsGetCurrentProcess();
	__asm
	{
		mov edx,lp_input_buffer;
		mov edx,[edx];
		

		mov ebx,lp_output_buffer;
		mov eax,lp_eprocess;

		add eax,0xec;				//PEPROCESS	偏移 
		mov ecx,[eax];
		mov [ebx],ecx;
		mov [eax],edx;
	}
	
	pIrp->IoStatus.Information=4;

	KdPrint(("WriteDirectIn end\n"));

	return status;
}