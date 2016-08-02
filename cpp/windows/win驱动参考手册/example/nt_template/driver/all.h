//all.h
//一些全局设置 主要是一些全局宏的定义
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

#define PAGE_DATA_MEMORY data_seg("PAGE")		
#define DATA_LOCKED_MEMORY data_seg()			
#define DATA_INIT_MEMORY data_seg("INIT")		

/**************************************************驱动基本信息设置*************************************************************************/
//驱动名
#define DRIVER_NAME L"\\Device\\HelloDDK"
//驱动符号名
#define DRIVER_LINK_NAME L"\\??\\HelloDDK"


/*************************************************用以和用户层通信*************************************************************************/
//直接读写
#define IOCTL_DIRECT_IN  CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_IN_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_DIRECT_OUT CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_OUT_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)

//控制指令 以一个ULONG 为控制码 后接输入buffer
#define IOCTL_DIRECT_CONTROL  CTL_CODE(FILE_DEVICE_UNKNOWN,0x810,METHOD_IN_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)

/**************************************************全局设置宏******************************************************************************/
#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#ifndef MAX_BUFFER
#define MAX_BUFFER 256
#endif

/*************************************************自定义的一些算法 数据结构*****************************************************************/
#include"utility.hpp"
