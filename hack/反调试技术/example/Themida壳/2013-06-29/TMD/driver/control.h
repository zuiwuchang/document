//control.h
//用以扩展 以实现具体功能

//初始化函数 会在DriverEntry中被调用
#pragma INIT_MEMORY
VOID InitDriver();
//卸载函数用以释放自定义资源 在卸载例程UnloadDevice中调用
#pragma PAGE_MEMORY
VOID UnloadDriver();

//用户层传入的控制信息
#pragma PAGE_MEMORY
NTSTATUS ControlInstructions(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp,ULONG inSize,ULONG outSize);

//处理控制信息实例
#pragma PAGE_MEMORY
NTSTATUS Examplate_Control(IN PVOID in_buffer,IN ULONG size_inbuffer,IN PVOID out_buffer,IN ULONG size_outbuffer,OUT ULONG* size_returned);