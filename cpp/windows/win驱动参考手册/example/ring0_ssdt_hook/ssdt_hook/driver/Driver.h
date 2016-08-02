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


#define IOCTL_DIRECT_IN  CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_IN_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_DIRECT_HOOK CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_OUT_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_DIRECT_UNHOOK CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_OUT_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)

// 函数声明

//创建设备对象 辅助函数
NTSTATUS CreateDevice (IN PDRIVER_OBJECT pDriverObject);
//卸载例程函数 设备卸载时I/O管理器中的调用
VOID UnloadDevice (IN PDRIVER_OBJECT pDriverObject);


NTSTATUS CreateDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
NTSTATUS CloseDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
NTSTATUS ControlDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);

NTSTATUS WriteDirectIn(IN PIRP pIrp,ULONG inSize,ULONG outSize);
void Hook();
void UnHook();

ULONG GetCurFunctionAddr(ULONG index);
ULONG SetCurFunctionAddr(ULONG index,ULONG addr);

//定义一个函数指针类型
extern "C" typedef NTSTATUS  (__stdcall *HookNtOpenProcess)( 
	OUT	PHANDLE ProcessHandle, 
	IN	ACCESS_MASK DesiredAccess, 
	IN	POBJECT_ATTRIBUTES ObjectAttributes, 
	IN	PCLIENT_ID ClientId ) ;


//HOOK 函数声明
extern "C" NTSTATUS __stdcall MyNtOpenProcess( 
	OUT	PHANDLE ProcessHandle, 
	IN	ACCESS_MASK DesiredAccess, 
	IN	POBJECT_ATTRIBUTES ObjectAttributes, 
	IN	PCLIENT_ID ClientId );