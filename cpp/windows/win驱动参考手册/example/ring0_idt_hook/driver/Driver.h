//driver.h
#pragma once


#ifdef __cplusplus	//若是c++编译 以c符号方式导入
extern "C"
{
#endif

#include <NTDDK.h>

#ifdef __cplusplus
}
#endif 

#define PAGE_MEMORY code_seg("PAGE")		//分页内存
#define LOCKED_MEMORY code_seg()			//非分页
#define INIT_MEMORY code_seg("INIT")		//INIT内存 此内存指明只在加载驱动时加载 驱动加载成功后自动卸载

//程序员自定义扩展设备结构 以描述不同设备的信息
typedef struct _DEVICE_EXTENSION {
	PDEVICE_OBJECT pDevice;
	UNICODE_STRING ustrDeviceName;	//设备名称
	UNICODE_STRING ustrSymLinkName;	//符号链接名
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

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


//IDT 表结构定义
#pragma pack(push)
#pragma pack(1) //
typedef struct _IDT //IDT基址
{
	USHORT limit; //范围 占8位
	ULONG base; //基地址 占32位 _IDT_ENTRY类型指针
}IDT,*PIDT;

typedef struct _IDT_ENTRY
{
	USHORT offset_low; //中断处理函数地址低16位
	USHORT selector;
	UCHAR  reserved;
	UCHAR  type:4; 
    UCHAR  always0:1;
	UCHAR  dpl:2;
	UCHAR  present:1;
	USHORT offset_high;//中断处理函数地址低16位
}IDT_ENTRY,*PIDT_ENTRY;
#pragma pack(pop)

void IdtHookInt3();
void IdtUnhookInt3();
void MyInt3();
void int3Hookcode();