//driver.h
#pragma once
#include"all.h"


//程序员自定义扩展设备结构 以描述不同设备的信息
typedef struct _DEVICE_EXTENSION {
	PDEVICE_OBJECT pDevice;
	UNICODE_STRING ustrDeviceName;	//设备名称
	UNICODE_STRING ustrSymLinkName;	//符号链接名
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

//直接读写
#define IOCTL_DIRECT_IN  CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_IN_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_DIRECT_OUT CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_OUT_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)



// 函数声明

//创建设备对象 辅助函数
NTSTATUS CreateDevice (IN PDRIVER_OBJECT pDriverObject);
//卸载例程函数 设备卸载时I/O管理器中的调用
VOID UnloadDevice (IN PDRIVER_OBJECT pDriverObject);

//irp派遣例程
NTSTATUS DispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);


NTSTATUS CreateDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
NTSTATUS CloseDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
NTSTATUS ControlDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);

NTSTATUS WriteDirectIn(IN PIRP pIrp,ULONG inSize,ULONG outSize);
NTSTATUS ReadDirectOut(IN PIRP pIrp,ULONG inSize,ULONG outSize);

//初始化数据
VOID initData();


