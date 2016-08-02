// SSDT_HOOK_TEST.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<string>
#include<iostream>
#include<Windows.h>

#define IOCTL_DIRECT_IN  CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_IN_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_DIRECT_HOOK CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_OUT_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_DIRECT_UNHOOK CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_OUT_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)

int _tmain(int argc, _TCHAR* argv[])
{
	std::wcout.imbue(std::locale("chs"));
	std::wstring link_name(L"\\??\\SSDT_HOOK");

	HANDLE handle_file = CreateFile(link_name.c_str(),GENERIC_WRITE | GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);

	if( handle_file ==INVALID_HANDLE_VALUE)
	{
		std::wcout<<L"无法打开指定设备"<<std::endl;
		std::system("pause");
		return 0;
	}
	DWORD d;
	if(!DeviceIoControl(handle_file, IOCTL_DIRECT_HOOK, 0, 0, 0, 0,&d, NULL))
	{
		std::wcout<<L"HOOK失败"<<std::endl;
		std::system("pause");
		return 0;
	}
	int cmd;
	std::wcout<<L"请输入pid"<<std::endl;
	while(std::wcin>>cmd)
	{
		if(cmd)
		{
			if(!DeviceIoControl(handle_file, IOCTL_DIRECT_IN, (LPVOID)&cmd, sizeof(int), 0, 0,&d, NULL))
			{
				std::wcout<<L"写入pid 失败"<<std::endl;;
				break;
			}
		}
		else
		{
			break;
		}
	}
	if(!DeviceIoControl(handle_file, IOCTL_DIRECT_UNHOOK, 0, 0, 0, 0,&d, NULL))
	{
		std::wcout<<L"UnHOOK失败"<<std::endl;;
		CloseHandle(handle_file);
		std::system("pause");
		return 0;
	}
	

	CloseHandle(handle_file);
	std::system("pause");
	return 0;
}

