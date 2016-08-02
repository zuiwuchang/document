// InlineHook.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<Windows.h>
#include<string>
#include<iostream>
//保存hook的字节
byte code[5]={0};
//hook代码
bool inlineHook();
//自定义 hook 函数
void __stdcall hookSleep(DWORD dwMilliseconds);
int _tmain(int argc, _TCHAR* argv[])
{
	inlineHook();
	Sleep(100);
	system("pause");
	return 0;
}
bool inlineHook()
{
	PVOID address=Sleep;
	
	MEMORY_BASIC_INFORMATION mbi_thunk;  
    //查询页信息。  
	VirtualQuery(address, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION));
	
	//改动页保护属性为读写。
	VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize,PAGE_EXECUTE_READWRITE, &mbi_thunk.Protect);

	//保存原代码
	memcpy(code,(PVOID)address,5);

	//hook
	__asm
	{
		mov eax,hookSleep;
		sub eax,Sleep;
		sub eax,5;

		mov ebx,Sleep
		mov byte ptr [ebx],0xE9;
		mov [ebx+1],eax;
	}
	VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize, mbi_thunk.Protect, NULL);

	return true;
}

//_declspec(naked) //此声明代表生成
void __stdcall hookSleep(DWORD dwMilliseconds)
{
	std::cout<<"hook成功 传入参数为"<<dwMilliseconds<<std::endl;
	if(dwMilliseconds<100)
	{
		std::cout<<"执行hook操作"<<std::endl;
	}
	else
	{
		//跳转执行原函数
		__asm
		{
			//跳转执行原函数
			mov eax,Sleep;
			add eax,5;
			jmp eax;
		}
	}

}