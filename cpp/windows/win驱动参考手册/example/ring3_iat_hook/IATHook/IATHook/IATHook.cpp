// IATHook.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<Windows.h>
#include<iostream>
//hook 代码
void iatHook();
//替换的自定义函数
VOID WINAPI hookSleep(__in DWORD dwMilliseconds);
int _tmain(int argc, _TCHAR* argv[])
{
	iatHook();
	Sleep(100);
	system("pause");
	return 0;
}

void iatHook()
{
	//取得DOS头 既基址 
	DWORD image_base=(DWORD)GetModuleHandle(NULL);
	PIMAGE_DOS_HEADER dos_header=(PIMAGE_DOS_HEADER)image_base;

	//取得PE头
	PIMAGE_NT_HEADERS pe_header=(PIMAGE_NT_HEADERS)(image_base+dos_header->e_lfanew);

	//取得导入表信息 IMAGE_DIRECTORY_ENTRY_IMPORT值为1 表示import tabale
	PIMAGE_IMPORT_DESCRIPTOR import_descriptor=(PIMAGE_IMPORT_DESCRIPTOR)(image_base+pe_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	
	//定义用于保存函数信息的局部变量
	PIMAGE_THUNK_DATA thunk_name,thunk_addr;   
	//遍历整个 输入表 
	for (PIMAGE_IMPORT_DESCRIPTOR iterator=import_descriptor; iterator->FirstThunk; iterator++)   
	{      
		std::cout<<"导入模块:"<<(char*)(image_base+iterator->Name)<<std::endl;
		   
		//遍历IAT信息  PIMAGE_THUNK_DATA基址	
		for (thunk_name = (PIMAGE_THUNK_DATA) (image_base + iterator->OriginalFirstThunk),thunk_addr=(PIMAGE_THUNK_DATA) (image_base + iterator->FirstThunk); 
			thunk_addr->u1.Function;++thunk_name,
			++thunk_addr)
		
		{  
			//取得函数名称。hint/name表前两个字节是函数的序号，后4个字节是函数名称字符串的地址
			DWORD address_data=image_base+thunk_name->u1.AddressOfData;
            char* function_name = (char*)(address_data+2);

			//取得存放函数地址的指针
			DWORD function_addr = thunk_addr->u1.Function;
			
			std::cout<<std::hex<<function_addr<<"   "<<function_name<<std::endl;
			
			if((DWORD)Sleep==function_addr)
			{   
				DWORD pSleep=(DWORD)&thunk_addr->u1.Function; 
				
				MEMORY_BASIC_INFORMATION mbi_thunk;  
				 //查询页信息。  
				VirtualQuery((PVOID)pSleep, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION));
				//改动页保护属性为读写执行
				VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize,PAGE_EXECUTE_READWRITE, &mbi_thunk.Protect);
				__asm
				{
						mov ebx,pSleep 
						lea eax,hookSleep;
						mov [ebx],eax
				}
				//恢复页面保护
				VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize, mbi_thunk.Protect, NULL);

			}
			
		}   
	}   
}

VOID WINAPI hookSleep(__in DWORD dwMilliseconds)
{
	MessageBox(NULL,L"hook 成功",L"test",MB_OK);
}