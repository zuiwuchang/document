//d3dx.h	一些d3dx編程的複製函數
#ifndef _DARK_D3DX_
#define _DARK_D3DX_


#include"d3dx_utility.h"
#include"d3dx_model_base.h"
#include"d3dx_CameraFPS.h"
#include"ModelX.h"

//定義模型 id
#define D3DX_MODEL_SCENE_ID			0
#define D3DX_MODEL_TEAPOT_ID		1
#define D3DX_MODEL_BOX_ID			2

#define D3DX_MODEL_XFILE_ID			3

namespace d3dx
{
	//設備 class
	class Device
		: boost::noncopyable
	{
	private:
		IDirect3DDevice9*		device_ptr_;
		IDirect3DVertexBuffer9*	vertexs_buffer_ptr_;
		IDirect3DIndexBuffer9*	indexs_buffer_ptr_;
		LPD3DXFONT font_ptr;

		int width_;
		int height_;
	public:
		Device();
		~Device();

		//創建窗口 IDirect3DDevice9 接口 必須第一個被調用的初始化函數
		bool CreateDeviceWindow(HINSTANCE hInstance		//應用程序實例
			,WNDPROC window_fun							//窗口函數
			,int width = 640							//窗口寬
			,int height = 480							//窗口高
			,bool show_window = true						//窗口化
			,D3DDEVTYPE device_type= D3DDEVTYPE_HAL			//使用 硬件/軟件 運算
			);

		//重載 -> 以方便調用 IDirect3DDevice9接口的方法
		inline IDirect3DDevice9* operator->()
		{
			return device_ptr_;
		}
		inline operator bool()
		{
			return device_ptr_ != NULL;
		}

	protected:
		//應該修改此函數 在裡面增加 初始化操作
		//CreateDeviceWindow 成功後被自動調用
		bool Initialization();

	public:
		//進入消息循環 應該被調用的第二個 函數
		void EnterMessageLoop();

	protected:
		//應該修改此函數 以改變場景繪製 time_interval 是距離上次繪製的 時間間隔
		void DrawScene(const DWORD time_interval);
		
		//應該修改此函數 在裡面增加 資源釋放操作
		void Release();
		
	private:
		//保存 光源
		boost::unordered_map<unsigned int,boost::shared_ptr<D3DLIGHT9>> lights_;

	public:
		//保存 模型
		boost::unordered_map<unsigned int,ModlePtr_t> models_;
		
		//FPS 攝像機
		d3dx::CameraFPS camera;


	};	
};

#endif