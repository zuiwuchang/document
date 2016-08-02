#include"all.h"
#include"control.h"
#include"myhook.h"
extern WCHAR protected_name[MAX_PATH];
extern BOOLEAN is_hook;
#pragma INIT_MEMORY
VOID InitDriver()
{
	is_hook=FALSE;

	NTSTATUS status;

	OBJECT_ATTRIBUTES object_attributes;
	IO_STATUS_BLOCK io_status;

	HANDLE handle_file;
	UNICODE_STRING file_symbol;
	//初始化文件符号名 或者用设备名 \device\harddiskvolume1\test.txt
	RtlInitUnicodeString(&file_symbol,L"\\??\\c:\\anti.txt");

	//初始化设备属性
	InitializeObjectAttributes(&object_attributes,&file_symbol,OBJ_CASE_INSENSITIVE,NULL,NULL);

	//创建文件
	status=ZwCreateFile(&handle_file,
		GENERIC_READ,//写
		&object_attributes,
		&io_status,
		NULL,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,
		FILE_OPEN,	//创建文件 FILE_OPEN(打开文件)
		FILE_SYNCHRONOUS_IO_NONALERT,NULL,0);

	if(STATUS_SUCCESS==status)
	{
		KdPrint(("打开文件成功\n"));
	}
	else
	{
		KdPrint(("打开文件失败\n"));
	}

	if(STATUS_SUCCESS==status)
	{
		//文件操作
		
		FILE_STANDARD_INFORMATION file_standard_information;
		
		if(STATUS_SUCCESS==ZwQueryInformationFile(handle_file,&io_status,&file_standard_information,sizeof(FILE_STANDARD_INFORMATION),FileStandardInformation))
		{
			LONGLONG file_long=file_standard_information.EndOfFile.QuadPart;
			
			if(file_long>MAX_PATH)
			{
				file_long=MAX_PATH;

			}
			KdPrint(("文件长度为%d字节\n",file_long));

			//设置读入位置
			LARGE_INTEGER byte_offset;
			byte_offset.QuadPart=2;

			//读文件
			RtlZeroMemory(protected_name,sizeof(protected_name));

			if(STATUS_SUCCESS==ZwReadFile(handle_file,NULL,NULL,NULL,&io_status,protected_name,(ULONG)file_long,&byte_offset,0))
			{
				DbgPrint("Anti进程名:%S\n",protected_name);
				Hook();
			}

		}
		//关闭文件
		if(STATUS_SUCCESS==ZwClose(handle_file))
		{
			KdPrint(("关闭文件成功\n"));
		}
	}
}

#pragma PAGE_MEMORY
VOID UnloadDriver()
{
	UnHook();
}

#pragma PAGE_MEMORY
NTSTATUS ControlInstructions(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp,ULONG inSize,ULONG outSize)
{
	NTSTATUS status = STATUS_SUCCESS;

	//获取用户层传入的 输入缓冲区的地址
	PVOID lp_input_buffer=pIrp->AssociatedIrp.SystemBuffer,
		lp_output_buffer=NULL;

	pIrp->IoStatus.Information=0;
	ULONG* size_returned=&pIrp->IoStatus.Information;
	
	if(lp_input_buffer && inSize>3)
	{
		//如果传入了输出缓冲区
		if(pIrp->MdlAddress)
		{
			//锁定输出缓冲区 并映射到内核以供 驱动使用
			lp_output_buffer=MmGetSystemAddressForMdlSafe(pIrp->MdlAddress,NormalPagePriority );
		}

		ULONG control=0;//获取操作码
		
		__asm
		{
			push eax;
			push ebx;

			mov eax,lp_input_buffer;
			mov ebx,[eax];
			mov control,ebx;

			add eax,4;
			mov lp_input_buffer,eax;

			pop ebx;
			pop eax;
		}
		
		inSize-=4;
		switch(control)
		{	
		case 10000:
			Examplate_Control(lp_input_buffer,inSize,lp_output_buffer,outSize,size_returned);
			break;

		}
	}
	else
	{
		status=STATUS_BUFFER_ALL_ZEROS;
	}

	return status;
}

#pragma PAGE_MEMORY
NTSTATUS Examplate_Control(IN PVOID in_buffer,IN ULONG size_inbuffer,IN PVOID out_buffer,IN ULONG size_outbuffer,OUT ULONG* size_returned)
{
	NTSTATUS status = STATUS_SUCCESS;
	//进行一些操作 如果有额外参数读入
	WCHAR buffer[MAX_BUFFER]={0};
	RtlCopyMemory(buffer,in_buffer,size_inbuffer);
	KdPrint(("size=%d inParam:%S \n",size_inbuffer,buffer));
	
	//如果需要传出参数
	if(out_buffer)
	{
		UNICODE_STRING unicode_string;
		RtlInitUnicodeString(&unicode_string,L"写入数据成功");
		RtlCopyMemory(out_buffer,unicode_string.Buffer,unicode_string.Length);
		*size_returned=unicode_string.Length;

		KdPrint(("size=%d outParam:%wZ",*size_returned,&unicode_string));	
	}

	
	/*LinkList<ULONG> link;
	link.init();

	for(ULONG i=0;i<10;++i)
	{
		link.push_back(i);
	}

	LinkList<ULONG>::POS pos=link.begin();
	while(pos)
	{
		DbgPrint("%d\n",pos->data);
		link.next(pos);
	}*/

	return status;
}