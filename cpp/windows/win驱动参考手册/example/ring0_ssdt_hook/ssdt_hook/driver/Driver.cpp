//driver.cpp

#include "Driver.h"

//导入KeServiceDescriptorTable表
extern "C" ULONG KeServiceDescriptorTable;
//记录原函数地址
HookNtOpenProcess realNtOpenProcess;
//记录被保护进程 pid
HANDLE protected_pid;
//记录是否被保护
bool isHook=false;

#pragma INIT_MEMORY
extern "C" NTSTATUS DriverEntry (IN PDRIVER_OBJECT pDriverObject,IN PUNICODE_STRING pRegistryPath	) 
{
	KdPrint(("DriverEntry begin\n"));
	//__asm int 3;
	
	NTSTATUS status;

	//注册设备卸载例程
	pDriverObject->DriverUnload = UnloadDevice;

	////注册irp例程
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseDispatchRoutine;

	
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ControlDispatchRoutine;
	//创建驱动设备对象
	status = CreateDevice(pDriverObject);

	KdPrint(("DriverEntry end\n"));
	return status;
}


#pragma INIT_MEMORY
ULONG GetCurFunctionAddr(ULONG index)
{
	ULONG function_addr;
	__asm
	{
		push eax;
		push ebx;
			mov eax,KeServiceDescriptorTable;
			mov eax,[eax];
			mov ebx,index;	//函数索引
			shl ebx,2;		//计算函数偏移 左移两位 相当于*4 或 imul ebx,ebx,4 
			add eax,ebx;	
			mov eax,[eax];
			mov function_addr,eax;
		pop ebx;
		pop eax;
	}
	return function_addr;
}

#pragma INIT_MEMORY
ULONG SetCurFunctionAddr(ULONG index,ULONG addr)
{
	ULONG function_addr;
	__asm
	{
		push eax;
		push ebx;

		//去除页面保护
		cli;
			mov eax,cr0;
			and eax,not 10000h; //and eax,0FFFEFFFFh
			mov cr0,eax;

			mov eax,KeServiceDescriptorTable;
			mov eax,[eax];
			mov ebx,index;	//函数索引
			shl ebx,2;		//计算函数偏移 左移两位 相当于*4 或 imul ebx,ebx,4 
			add eax,ebx;
			mov ebx,[eax];
			mov function_addr,ebx;

			mov ebx,addr;
			mov [eax],ebx;
		
		//恢复页面保户
		cli;
			mov eax,cr0;
			or eax,10000h; 
			mov cr0,eax ;
			sti;

		pop ebx;
		pop eax;
	}
	return function_addr;
}

#pragma INIT_MEMORY
NTSTATUS CreateDevice (IN PDRIVER_OBJECT pDriverObject) 
{
	KdPrint(("CreateDevice begin\n"));

	NTSTATUS status;
	//设备对象 指针
	PDEVICE_OBJECT pDevObj;
	//设备对象扩展结构 指针
	PDEVICE_EXTENSION pDevExt;
	
	//设备名称
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName,L"\\Device\\SSDT_HOOK");
	
	//创建设备
	status = IoCreateDevice( pDriverObject,			//驱动对象
						sizeof(DEVICE_EXTENSION),	//设备扩展结构大小
						&(UNICODE_STRING)devName,	//设备名 或 NULL
						FILE_DEVICE_UNKNOWN,		//设备类型 FILE_DEVICE_UNKNOWN 未知虚拟设备，且为独占(既只能被一个应用程序使用) 
						0, TRUE,
						&pDevObj );					//设备地址 out

	if (!NT_SUCCESS(status))
		return status;

	//以直接的方式读写(既不使用缓冲区)
	pDevObj->Flags |= DO_DIRECT_IO;

	//填充扩展结构数据
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	pDevExt->ustrDeviceName = devName;
	
	//符号链接名
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName,L"\\??\\SSDT_HOOK");
	
	//创建符号链接
	pDevExt->ustrSymLinkName = symLinkName;
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice( pDevObj );
		return status;
	}
	
	KdPrint(("CreateDevice sucess and end\n"));
	return STATUS_SUCCESS;
}



#pragma PAGE_MEMORY
VOID UnloadDevice (IN PDRIVER_OBJECT pDriverObject) 
{
	KdPrint(("UnloadDevice begin\n"));
	
	//获取设备链表 加载的设备被自动保存在一个链表中
	PDEVICE_OBJECT	pNextObj = pDriverObject->DeviceObject;
	
	//遍历设备链表
	while (pNextObj) 
	{
		//获取设备扩展结构
		PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pNextObj->DeviceExtension;

		//删除符号链接
		UNICODE_STRING pLinkName = pDevExt->ustrSymLinkName;
		IoDeleteSymbolicLink(&pLinkName);
		
		//删除设备
		IoDeleteDevice(pNextObj);

		//指向链表下一节点
		pNextObj = pNextObj->NextDevice;
	}

	KdPrint(("UnloadDevice end\n"));
}




#pragma PAGE_MEMORY
NTSTATUS DispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp) 
{
	KdPrint(("DispatchRoutine begin\n"));
	
	//...根据要实现的功能
	//...完成一些事情
	//...代码省略

	NTSTATUS status = STATUS_SUCCESS;
	//设置完成irp状态 是否成功等
	pIrp->IoStatus.Status = status;
	//设置irp实际操作字节数
	pIrp->IoStatus.Information = 0;	// bytes xfered
	//指示完成此irp
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	
	KdPrint(("DispatchRoutine end\n"));
	return status;
}
#pragma PAGE_MEMORY
NTSTATUS CreateDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp)
{
	KdPrint(("CreateDispatchRoutine begin\n"));
	
	NTSTATUS status = STATUS_SUCCESS;
	//...根据要实现的功能
	//...完成一些事情
	//...代码省略

	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("CreateDispatchRoutine end\n"));
	return status;
}
#pragma PAGE_MEMORY
NTSTATUS CloseDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp)
{
	KdPrint(("CloseDispatchRoutine begin\n"));
	
	NTSTATUS status = STATUS_SUCCESS;
	UnHook();
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("CloseDispatchRoutine end\n"));
	return status;
}
#pragma PAGE_MEMORY
NTSTATUS ControlDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp)
{
	KdPrint(("ControlDispatchRoutine begin\n"));
	
	NTSTATUS status = STATUS_NOT_SUPPORTED;
	pIrp->IoStatus.Information=0; //设置返回的实际操作字节数 无实意(既程序员自定)

	//从irp堆栈结构中 获取设备对象的 irp 结构
	PIO_STACK_LOCATION lpIoStack=IoGetCurrentIrpStackLocation(pIrp);

	//如果成功获取到 irp 结构
	if(lpIoStack)
	{
		//判断控制码
		switch(lpIoStack->Parameters.DeviceIoControl.IoControlCode)
		{
		case IOCTL_DIRECT_IN://写如到驱动
			status=WriteDirectIn(pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
			break;
		case IOCTL_DIRECT_HOOK://从驱动读数据到用户层
			status=STATUS_SUCCESS;
			Hook();
			break;
		case IOCTL_DIRECT_UNHOOK:
			status=STATUS_SUCCESS;
			UnHook();
			break;
		}

	}

	//设置pIrp状态 否则驱动无法继续进行其他工作
	pIrp->IoStatus.Status=STATUS_SUCCESS;//返回成功
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("ControlDispatchRoutine end\n"));
	return status;
}


#pragma PAGE_MEMORY
NTSTATUS WriteDirectIn(IN PIRP pIrp,ULONG inSize,ULONG outSize)
{
	KdPrint(("WriteDirectIn begin\n"));
	NTSTATUS status = STATUS_SUCCESS;

	//获取用户层传入的 输入缓冲区内容拷贝 的地址
	PVOID lp_input_buffer=pIrp->AssociatedIrp.SystemBuffer;

	__asm
	{
		push eax;
		mov eax,lp_input_buffer;
		mov eax,[eax];
		mov protected_pid,eax;
		pop eax;
	}
	KdPrint(("WriteDirectIn pid=%d\n",protected_pid));
	
	
	pIrp->IoStatus.Information=4;

	KdPrint(("WriteDirectIn end%d\n"));

	return status;
}


#pragma PAGE_MEMORY
void Hook()
{
	KdPrint(("HOOK begin\n"));
	
	if(!isHook)
	{
		ULONG index=0xbe;
		realNtOpenProcess=(HookNtOpenProcess)SetCurFunctionAddr(index,(ULONG)MyNtOpenProcess);
		isHook=true;
	}
	KdPrint(("HOOK end%d\n"));
	
}

#pragma PAGE_MEMORY
void UnHook()
{
	KdPrint(("UNHOOK begin\n"));
	
	ULONG index=0xbe;
	if(isHook)
	{
		SetCurFunctionAddr(index,(ULONG)realNtOpenProcess);
		isHook=false;
	}
	KdPrint(("UNHOOK end%d\n"));
	
}

#pragma PAGE_MEMORY
extern "C" NTSTATUS __stdcall MyNtOpenProcess( 
	OUT	PHANDLE ProcessHandle, 
	IN	ACCESS_MASK DesiredAccess, 
	IN	POBJECT_ATTRIBUTES ObjectAttributes, 
	IN	PCLIENT_ID ClientId ) 
{ 
	NTSTATUS status=STATUS_SUCCESS; 
	
	//调用原 NtOpenProcess
	status=(NTSTATUS)realNtOpenProcess( ProcessHandle, DesiredAccess, ObjectAttributes, ClientId ); 	

	//如果 NtOpenProcess 调用成功
	if(ClientId) 
	{ 
		//获取被打开进程 pid
		HANDLE pid = ClientId->UniqueProcess; 	 

		// 如果是被保护的PID，则拒绝访问，并将句柄设置为空 
		if(pid == protected_pid) 
		{ 
			ProcessHandle=NULL;
			status=STATUS_ACCESS_DENIED;
			//PsLookupProcessByProcessId((ULONG)PID,&EP);
			/*获取调用此函数的 未公开结构 以获得其进程名
			EP=PsGetCurrentProcess();			 
			KdPrint((" ACESS Process Name  --:%s--   \n",(PTSTR)((ULONG)EP+0x174)));
			*/
		} 
	} 

	return status; 
} 