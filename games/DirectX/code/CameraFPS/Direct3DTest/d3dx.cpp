//d3dx.cpp
#include"stdafx.h"
#include"d3dx.h"
#include"Resource.h"

#include<boost\lexical_cast.hpp>

#include"model_scene.h"
#include"model_teapot.h"
#include"model_box.h"
	
//光源 定義
#define USING_LIGHT_ONE	0


//窗口
#define D3DX_WINDOW_TITLE	L"D3DX Window"
#define D3DX_WINDOW_CLASS	L"D3DX Window Class"


d3dx::Device::Device()
{
	device_ptr_			=	NULL;
	vertexs_buffer_ptr_	=	NULL;
	vertexs_buffer_ptr_	=	NULL;
	font_ptr			=	NULL;

}
d3dx::Device::~Device()
{
	Release();
}

bool d3dx::Device::CreateDeviceWindow(HINSTANCE hInstance
	,WNDPROC window_fun
	,int width
	,int height
	,bool show_window
	,D3DDEVTYPE device_type		
	)
{
	width_		=	width;
	height_	=	height;

	//註冊窗口類
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= window_fun;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT3DTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_DIRECT3DTEST);;
	wcex.lpszClassName	= D3DX_WINDOW_CLASS;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);
	
	//創建窗口 並顯示
	DWORD style	=	WS_EX_TOPMOST;				//頂層窗口
	style	|=	WS_SYSMENU | WS_MINIMIZEBOX;	//最小化 關閉 按鈕
	HWND hwnd	=	CreateWindow(D3DX_WINDOW_CLASS, D3DX_WINDOW_TITLE
		,style
		,0, 0, width, height
		,0 /*parent hwnd*/, 0 /* menu */, hInstance, 0 /*extra*/); 
	if(!hwnd)
	{
		return false;
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//開始初始化 Direct3D
	//1	獲取IDirect3D9接口 指針
	IDirect3D9* direct_ptr	=	Direct3DCreate9(D3D_SDK_VERSION);
	
	if(!direct_ptr)
	{
		return false;
	}

	//2 獲取硬件特性
	D3DCAPS9 caps;
	direct_ptr->GetDeviceCaps(D3DADAPTER_DEFAULT, device_type, &caps);

	int vp = 0;
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp	=	D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp	=	D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	
	//3	填充 D3DPRESENT_PARAMETERS
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = show_window;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	//4 創建 設備
	HRESULT hr	=	direct_ptr->CreateDevice(D3DADAPTER_DEFAULT
		,device_type
		,hwnd
		,vp
		,&d3dpp
		,&device_ptr_
		);
	if(FAILED(hr))
	{
		//使用16位深 重試
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		hr	=	direct_ptr->CreateDevice(D3DADAPTER_DEFAULT
			,device_type
			,hwnd
			,vp
			,&d3dpp
			,&device_ptr_
			);
		if(FAILED(hr))
		{
			direct_ptr->Release();
			direct_ptr	=	NULL;
			return false;
		}
	}

	//創建 文本繪製接口
	D3DXFONT_DESC font_desc;
	memset(&font_desc,0,sizeof(font_desc));
	font_desc.Height	=	25;
	font_desc.Weight	=	12;
	font_desc.Weight	=	500;	//[0,1000]
	font_desc.MipLevels	=	D3DX_DEFAULT;
	font_desc.Italic	=	FALSE;
	font_desc.CharSet	=	DEFAULT_CHARSET;
	font_desc.OutputPrecision	=	0;
	font_desc.Quality			=	0;
	font_desc.PitchAndFamily	=	0;
	wcscpy_s(font_desc.FaceName,L"宋体");

	D3DXCreateFontIndirect(device_ptr_,&font_desc,&font_ptr);
	if(!font_ptr)
	{
		Release();
		return false;
	}

	//釋放IDirect3D9接口 指針
	direct_ptr->Release();

	//進行其他初始化操作
	return Initialization();
	
}

bool d3dx::Device::Initialization()
{
	D3DXMATRIX matrix;

	//創建場景
	d3dx::ModlePtr_t modle_ptr = boost::make_shared<ModelScene>();
	modle_ptr->Create(device_ptr_);
	D3DXMatrixTranslation(&matrix,0,1,0);
	modle_ptr->Matrix(matrix);
	models_[D3DX_MODEL_SCENE_ID]	=	modle_ptr;
	
	//創建茶壺
	modle_ptr =	boost::make_shared<ModelTeapot>();
	modle_ptr->Create(device_ptr_);
	models_[D3DX_MODEL_TEAPOT_ID]	=	modle_ptr;

	//創建 盒子D3DX_MODEL_BOX_ID
	modle_ptr =	boost::make_shared<ModelBox>();
	modle_ptr->Create(device_ptr_);
	D3DXMatrixTranslation(&matrix,0,1,0);
	modle_ptr->Matrix(matrix);
	models_[D3DX_MODEL_BOX_ID]	=	modle_ptr;

	//創建XFILE模型
	modle_ptr =	boost::make_shared<ModelX>();
	modle_ptr->Create(device_ptr_);
	D3DXMatrixScaling(&matrix,0.3f,0.3f,0.3f);
	modle_ptr->Matrix(matrix);
	models_[D3DX_MODEL_XFILE_ID]	=	modle_ptr;

	//創建攝像機
	camera.Create(device_ptr_);
	camera.ComputeViewMatrix(&matrix);
	device_ptr_->SetTransform(D3DTS_VIEW,&matrix);

	//投影矩陣
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj
		,D3DX_PI * 0.25f		//45度 觀察夾角
		,(float)width_/(float)(height_)
		,1.0f
		,1000.0f
		);
	device_ptr_->SetTransform(D3DTS_PROJECTION,&proj);
	
	//設置繪製模式
	//device_ptr_->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);	//只繪製線框
	
	//設置光照
	BOOST_AUTO(light_ptr,d3dx::CreateDirectionLight(D3DXVECTOR3(1,-1,1),D3DCOLOR_XRGB(255,255,255)));
	device_ptr_->SetLight(USING_LIGHT_ONE,light_ptr.get());
	device_ptr_->LightEnable(USING_LIGHT_ONE,TRUE);
	lights_[USING_LIGHT_ONE]	=	light_ptr;

	//device_ptr_->SetRenderState(D3DRS_LIGHTING,FALSE);		//禁用光照
	device_ptr_->SetRenderState(D3DRS_NORMALIZENORMALS,TRUE);	//規範化所有法線
	device_ptr_->SetRenderState(D3DRS_SPECULARENABLE,TRUE);		//啟用鏡面光

	return true;
}

void d3dx::Device::EnterMessageLoop()
{
	MSG msg;
	memset(&msg,0,sizeof(MSG));
	DWORD time_begin = GetTickCount(); 
	DWORD time_now;
	DWORD time_interval;

	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			//Idle time 繪製場景

			//計算 場景繪製 時間間隔
			time_now		=	GetTickCount(); 
			time_interval	=	time_now - time_begin;
			
			DrawScene(time_interval);

			time_begin = time_now;	
		}
	}

}

void d3dx::Device::DrawScene(const DWORD time_interval)
{
	//計算FPS
	static DWORD time			=	0;
	static DWORD frame_count	=	0;  
	static std::string fps;
	
	++frame_count;
	time	+=	time_interval;
	if(time >= 1000)
	{

		frame_count	*=	1000;
		frame_count	/=	time;
		fps	=	boost::lexical_cast<std::string>(frame_count);
		fps			+=	"fps";
		time		=	0;
		frame_count	=	0;

	}
	
	//清除背景
	device_ptr_->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,0xffffffff,1.0f,0);
	
	//繪製場景
	device_ptr_->BeginScene();

		//繪製 場景
		models_[D3DX_MODEL_SCENE_ID]->DrawModel();
		
		//繪製 茶壺
		models_[D3DX_MODEL_TEAPOT_ID]->DrawModel();

		//繪製 茶壺 倒影
		models_[D3DX_MODEL_SCENE_ID]->DrawMirror(*models_[D3DX_MODEL_TEAPOT_ID]);

		//繪製 茶壺 陰影
		models_[D3DX_MODEL_SCENE_ID]->DrawShadow(*models_[D3DX_MODEL_TEAPOT_ID],D3DXVECTOR4(-1,1,-1,0));

		//繪製盒子
		models_[D3DX_MODEL_BOX_ID]->DrawModel();

		//繪製XFile模型
		models_[D3DX_MODEL_XFILE_ID]->DrawModel();

		//繪製fps
		RECT rect;
		rect.left	=	10;
		rect.right	=	110;
		rect.top	=	10;
		rect.bottom	=	50;
		font_ptr->DrawTextA(NULL
			,fps.data()
			,fps.size()
			,&rect
			,DT_TOP | DT_LEFT
			,D3DCOLOR_XRGB(255,0,0)
			);

	device_ptr_->EndScene();

	//交換緩衝區到 屏幕
	device_ptr_->Present(0,0,0,0);
}

void d3dx::Device::Release()
{
	if(font_ptr)
	{
		font_ptr->Release();
		font_ptr	=	NULL;
	}

	if(vertexs_buffer_ptr_)
	{
		vertexs_buffer_ptr_->Release();
		vertexs_buffer_ptr_	=	NULL;
	}

	if(indexs_buffer_ptr_)
	{
		indexs_buffer_ptr_->Release();
		indexs_buffer_ptr_	=	NULL;
	}

	if(device_ptr_)
	{
		device_ptr_->Release();
		device_ptr_	=	NULL;
	}


}




