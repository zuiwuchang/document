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

#define DRIVER_NAME L"\\Device\\Anti"
#define DRIVER_LINK_NAME L"\\??\\Anti"

#ifndef MAX_PATH
#define MAX_PATH 256
#endif



