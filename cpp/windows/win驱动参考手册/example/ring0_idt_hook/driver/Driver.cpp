//driver.cpp

#include "Driver.h"
#include<utility.hpp>

#pragma PAGE_MEMORY
ULONG int3_function_address;
ULONG int3_function_address_9;
#pragma INIT_MEMORY
extern "C" NTSTATUS DriverEntry (IN PDRIVER_OBJECT pDriverObject,IN PUNICODE_STRING pRegistryPath	) 
{
	KdPrint(("DriverEntry begin\n"));

	NTSTATUS status;
	
	//获取idt基地址
	IDT idt;
	__asm sidt idt;
	PIDT_ENTRY idt_base=(PIDT_ENTRY)idt.base; 
	ULONG idt_function_address;
	//获取所有中断函数地址
	for (int i=0;i<0x100;i++)
	{
		idt_function_address=idt_base[i].offset_high;
		idt_function_address<<=16;
		idt_function_address|=idt_base[i].offset_low;
		KdPrint(("index=%3X hight=%4X low=%4X address=%8X\n",i,idt_base[i].offset_high,idt_base[i].offset_low,idt_function_address));
	}	

	//保存 int 3 中断地址
	int3_function_address=idt_base[3].offset_high;
	int3_function_address<<=16;
	int3_function_address|=idt_base[3].offset_low;
	
	int3_function_address_9=int3_function_address+9;
	//hook int 3
	IdtHookInt3();
	


	//注册设备卸载例程
	pDriverObject->DriverUnload = UnloadDevice;

	////注册irp例程
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = DispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ] = DispatchRoutine;
	
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ControlDispatchRoutine;
	//创建驱动设备对象
	status = CreateDevice(pDriverObject);

	KdPrint(("DriverEntry end\n"));
	return status;
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
	RtlInitUnicodeString(&devName,L"\\Device\\MyDDKDevice");
	
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
	RtlInitUnicodeString(&symLinkName,L"\\??\\HelloDDK");
	
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
	IdtUnhookInt3();
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
	//...根据要实现的功能
	//...完成一些事情
	//...代码省略

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
		//switch(lpIoStack->Parameters.DeviceIoControl.IoControlCode)
		//{
		//case IOCTL_DIRECT_IN://写如到驱动
		//	break;
		//case IOCTL_DIRECT_OUT://从驱动读数据到用户层
		//	break;
		//}

	}

	//设置pIrp状态 否则驱动无法继续进行其他工作
	pIrp->IoStatus.Status=STATUS_SUCCESS;//返回成功
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("ControlDispatchRoutine end\n"));
	return status;
}



#pragma PAGE_MEMORY
void IdtHookInt3()
{
	__asm
	{
		//去除页面保护
		cli;
			mov eax,cr0;
			and eax,not 10000h; //and eax,0FFFEFFFFh
			mov cr0,eax;

			mov eax,int3_function_address;
			mov BYTE ptr [eax],0xE9;
			inc eax;
			lea ebx,MyInt3;
			sub ebx,int3_function_address;
			sub ebx,5;
			mov [eax],ebx;


		//恢复页面保户
		cli;
			mov eax,cr0;
			or eax,10000h; 
			mov cr0,eax ;
			sti;
	}
}
#pragma PAGE_MEMORY
void IdtUnhookInt3()
{
	__asm
	{
		//去除页面保护
		cli;
			mov eax,cr0;
			and eax,not 10000h; //and eax,0FFFEFFFFh
			mov cr0,eax;

			mov eax,int3_function_address;	
			lea ebx,int3Hookcode;
			mov ecx,[ebx];
			mov [eax],ecx;
			add ebx,4;
			add eax,4;
			mov ecx,[ebx];
			mov [eax],ecx;
			add ebx,4;
			add eax,4;
			mov bl,[ebx];
			mov byte ptr [eax],bl;
			


		//恢复页面保户
		cli;
			mov eax,cr0;
			or eax,10000h; 
			mov cr0,eax ;
			sti;
	}
}

#pragma PAGE_MEMORY
__declspec(naked)
void MyInt3()
{	
	__asm
	{
		pushad;
	    pushfd;
	} 
	//在这里添加自己的条件过滤代码
	//获取进程上下文	
	PsGetCurrentProcess();
	//ep+=0x16c;

	//KdPrint(("%d",ep));
	/*if (strcmp((PTSTR)((ULONG)EP+0x16c),"notepad.exe")==0)
	{
		KdPrint(("保护"));	
		__asm iretd;
	}*/

	__asm 
	{
		popfd;
		popad;

		//执行被hook的原指令
		push 0;
		mov word ptr [esp+2],0;
		jmp int3_function_address_9;
	 }
}
#pragma PAGE_MEMORY
void __declspec(naked)  int3Hookcode()
{
	__asm
	{
		push 0;
		mov WORD ptr [esp+2], 0;
	}
}