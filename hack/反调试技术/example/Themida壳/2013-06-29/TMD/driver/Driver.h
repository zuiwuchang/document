//driver.h
//驱动的初始化与卸载等 如非必要不应该修改
#pragma once
#include"all.h"


//程序员自定义扩展设备结构 以描述不同设备的信息
typedef struct _DEVICE_EXTENSION {
	PDEVICE_OBJECT pDevice;
	UNICODE_STRING ustrDeviceName;		//设备名称
	UNICODE_STRING ustrSymLinkName;		//符号链接名
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


// 函数声明

//创建设备对象 辅助函数
#pragma INIT_MEMORY
NTSTATUS CreateDevice (IN PDRIVER_OBJECT pDriverObject);
//卸载例程函数 设备卸载时I/O管理器中的调用
#pragma PAGE_MEMORY
VOID UnloadDevice (IN PDRIVER_OBJECT pDriverObject);

//irp派遣例程
//NTSTATUS DispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
#pragma PAGE_MEMORY
NTSTATUS CreateDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
#pragma PAGE_MEMORY
NTSTATUS CloseDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
#pragma PAGE_MEMORY
NTSTATUS ControlDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);

#pragma PAGE_MEMORY
NTSTATUS WriteDirectIn(IN PIRP pIrp,ULONG inSize,ULONG outSize);
#pragma PAGE_MEMORY
NTSTATUS ReadDirectOut(IN PIRP pIrp,ULONG inSize,ULONG outSize);




