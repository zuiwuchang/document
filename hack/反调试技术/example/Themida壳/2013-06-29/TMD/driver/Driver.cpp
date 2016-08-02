//driver.cpp

#include "Driver.h"
#include "control.h"


#pragma INIT_MEMORY
extern "C" NTSTATUS DriverEntry (IN PDRIVER_OBJECT pDriverObject,IN PUNICODE_STRING /*pRegistryPath*/	) 
{
	KdPrint(("DriverEntry begin\n"));
	NTSTATUS status;
	
	//注册设备卸载例程
	pDriverObject->DriverUnload = UnloadDevice;

	////注册irp例程
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseDispatchRoutine;
	//pDriverObject->MajorFunction[IRP_MJ_WRITE] = DispatchRoutine;
	//pDriverObject->MajorFunction[IRP_MJ_READ] = DispatchRoutine;
	
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ControlDispatchRoutine;
	//创建驱动设备对象
	status = CreateDevice(pDriverObject);
	
	//初始化自定义代码
	InitDriver();

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
	RtlInitUnicodeString(&devName,DRIVER_NAME);
	
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
	//申请内存 保存设备名
	USHORT length=wcslen(DRIVER_NAME)*sizeof(WCHAR);
	pDevExt->ustrDeviceName.Buffer=(PWCH)ExAllocatePool(PagedPool,length);
	pDevExt->ustrDeviceName.Length=length;
	pDevExt->ustrDeviceName.MaximumLength=length;
	RtlCopyUnicodeString(&pDevExt->ustrDeviceName,&devName);
	
	//符号链接名
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName,DRIVER_LINK_NAME);
	
	//创建符号链接
	pDevExt->ustrSymLinkName = symLinkName;
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice( pDevObj );
		return status;
	}
	//申请内存 保存 符号链接名
	length=wcslen(DRIVER_LINK_NAME)*sizeof(WCHAR);
	pDevExt->ustrSymLinkName.Buffer=(PWCH)ExAllocatePool(PagedPool,length);
	pDevExt->ustrSymLinkName.Length=pDevExt->ustrSymLinkName.MaximumLength=length;
	RtlCopyUnicodeString(&pDevExt->ustrSymLinkName,&symLinkName);
	
	KdPrint(("CreateDevice sucess and end\n"));
	return STATUS_SUCCESS;
}



#pragma PAGE_MEMORY
VOID UnloadDevice (IN PDRIVER_OBJECT pDriverObject) 
{
	KdPrint(("UnloadDevice begin\n"));
	
	//获取设备链表 加载的设备被自动保存在一个链表中
	PDEVICE_OBJECT	pNextObj = pDriverObject->DeviceObject
		,pTmpObj;

	//遍历设备链表
	while (pNextObj) 
	{
		//获取设备扩展结构
		PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pNextObj->DeviceExtension;

		//释放保存设备名的内存
		ExFreePool(pDevExt->ustrDeviceName.Buffer);
		
		//删除符号链接
		IoDeleteSymbolicLink(&pDevExt->ustrSymLinkName);
		
		//释放保存符号名的内存 
		ExFreePool(pDevExt->ustrSymLinkName.Buffer);
		
		//获取链表下一节点
		pTmpObj= pNextObj->NextDevice;
		//删除设备
		IoDeleteDevice(pNextObj);

		//指向链表下一节点
		pNextObj = pTmpObj;
	}
	
	UnloadDriver();
	KdPrint(("UnloadDevice end\n"));
}




//#pragma PAGE_MEMORY
//NTSTATUS DispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp) 
//{
//	KdPrint(("DispatchRoutine begin\n"));
//	
//	//...根据要实现的功能
//	//...完成一些事情
//	//...代码省略
//
//	NTSTATUS status = STATUS_SUCCESS;
//	//设置完成irp状态 是否成功等
//	pIrp->IoStatus.Status = status;
//	//设置irp实际操作字节数
//	pIrp->IoStatus.Information = 0;	// bytes xfered
//	//指示完成此irp
//	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
//	
//	KdPrint(("DispatchRoutine end\n"));
//	return status;
//}
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
		switch(lpIoStack->Parameters.DeviceIoControl.IoControlCode)
		{
		case IOCTL_DIRECT_IN://写如到驱动
			status=WriteDirectIn(pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
			break;
		case IOCTL_DIRECT_OUT://从驱动读数据到用户层
			status=ReadDirectOut(pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
			break;
		case IOCTL_DIRECT_CONTROL:
			status=ControlInstructions(pDevObj,pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
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

	//获取用户层传入的 输入缓冲区的地址
	PVOID lp_input_buffer=pIrp->AssociatedIrp.SystemBuffer,
		lp_output_buffer=NULL;

	//如果传入了输出缓冲区
	if(pIrp->MdlAddress)
	{
		//锁定输出缓冲区 并映射到内核以供 驱动使用
		lp_output_buffer=MmGetSystemAddressForMdlSafe(pIrp->MdlAddress,NormalPagePriority );
	}

	KdPrint(("WriteDirectIn inSize=%d outSize=%d %S\n",inSize,outSize,(WCHAR*)lp_input_buffer));
	
	UNICODE_STRING unicode_string;
	RtlInitUnicodeString(&unicode_string,L"写入数据成功");
	if(lp_output_buffer)
	{
		RtlCopyMemory(lp_output_buffer,unicode_string.Buffer,unicode_string.Length);
	}
	
	pIrp->IoStatus.Information=unicode_string.Length;

	KdPrint(("WriteDirectIn end\n"));

	return status;
}


#pragma PAGE_MEMORY
NTSTATUS ReadDirectOut(IN PIRP pIrp,ULONG inSize,ULONG outSize)
{
	KdPrint(("ReadDirectOut begin\n"));
	NTSTATUS status = STATUS_SUCCESS;

	
	PVOID lp_output_buffer=NULL;

	//如果传入了输出缓冲区
	if(pIrp->MdlAddress)
	{
		//锁定输出缓冲区 并映射到内核以供 驱动使用
		lp_output_buffer=MmGetSystemAddressForMdlSafe(pIrp->MdlAddress,NormalPagePriority );

		KdPrint(("ReadDirectOut inSize=%d outSize=%d\n",inSize,outSize));
	
		UNICODE_STRING unicode_string;
		RtlInitUnicodeString(&unicode_string,L"读取数据成功");
		RtlCopyMemory(lp_output_buffer,unicode_string.Buffer,unicode_string.Length);
		
		pIrp->IoStatus.Information=unicode_string.Length;

		KdPrint(("ReadDirectOut end\n"));

	}

	return status;
}