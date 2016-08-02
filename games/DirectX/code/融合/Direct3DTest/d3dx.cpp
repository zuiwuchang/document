//d3dx.cpp
#include"stdafx.h"
#include"d3dx.h"
#include"Resource.h"

//自定義 點 描述
const DWORD d3dx::Vertex::FVF	=	D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

//D3DX模型 定義
#define MODEL_D3DX_TEAPOT_ID		0

#define MODEL_D3DX_TEAPOT_MATRIX	0

#define MODEL_D3DX_TEAPOT_MATERIAL	0

#define MODEL_D3DX_TEAPOT_TEXTURES	0

//模型 定義
#define MODEL_TEST_COUNT	6

#define MODEL_TEST_0_ID	1
#define MODEL_TEST_1_ID	2
#define MODEL_TEST_2_ID	3
#define MODEL_TEST_3_ID	4
#define MODEL_TEST_4_ID	5
#define MODEL_TEST_5_ID	6

#define MODEL_TEST_0_MATRIX	1
#define MODEL_TEST_1_MATRIX	2
#define MODEL_TEST_2_MATRIX	3
#define MODEL_TEST_3_MATRIX	4
#define MODEL_TEST_4_MATRIX	5
#define MODEL_TEST_5_MATRIX	6

#define MODEL_TEST_MATERIAL	1

#define MODEL_TEST_TEXTURES	1

//光源 定義
#define USING_LIGHT_ONE	0

//紋理 定義
#define TEXTURES_DEFAULT	0

//窗口
#define D3DX_WINDOW_TITLE	L"D3DX Window"
#define D3DX_WINDOW_CLASS	L"D3DX Window Class"

d3dx::Device::Device()
{
	device_ptr_		=	NULL;
	vertexs_buffer_ptr_	=	NULL;
	vertexs_buffer_ptr_		=	NULL;
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
			return false;
		}
	}

	//釋放IDirect3D9接口 指針
	direct_ptr->Release();

	//進行其他初始化操作
	return Initialization();
	
}

bool d3dx::Device::Initialization()
{
	//創建 頂點 緩存
	device_ptr_->CreateVertexBuffer(4 * sizeof(Vertex)
		,D3DUSAGE_WRITEONLY
		,Vertex::FVF
		,D3DPOOL_MANAGED
		,&vertexs_buffer_ptr_
		,0
		);
	
	//設置使用的點信息
	Vertex* vertexs_ptr;
	vertexs_buffer_ptr_->Lock(0
		,0
		,(void**)&vertexs_ptr
		,0);
	D3DXVECTOR3 point0(-1,1,0)
		,point1(1,1,0)
		,point2(1,-1,0)
		,point3(-1,-1,0)
		,vector
		;
	//計算點 法線
	d3dx::ComputeNormal(&vector,point0,point1,point2);
	
	//計算紋理坐標 並設置點數據
	vertexs_ptr[0]	=	Vertex(point0,vector,D3DXVECTOR2(0,0));
	vertexs_ptr[1]	=	Vertex(point1,vector,D3DXVECTOR2(1,0));
	vertexs_ptr[2]	=	Vertex(point2,vector,D3DXVECTOR2(1,1));
	vertexs_ptr[3]	=	Vertex(point3,vector,D3DXVECTOR2(0,1));
		
	vertexs_buffer_ptr_->Unlock();
	
	//參加 索引 緩存
	device_ptr_->CreateIndexBuffer(6 * 2 * 3 * sizeof(WORD)
		,D3DUSAGE_WRITEONLY
		,D3DFMT_INDEX16
		,D3DPOOL_MANAGED
		,&indexs_buffer_ptr_
		,0
		);
	//寫入索引 建模
	WORD* indexs_ptr;
	indexs_buffer_ptr_->Lock(0
		,0
		,(void**)&indexs_ptr
		,0
		);

	indexs_ptr[0]	=	0;
	indexs_ptr[1]	=	1;
	indexs_ptr[2]	=	2;
	indexs_ptr[3]	=	0;
	indexs_ptr[4]	=	2;
	indexs_ptr[5]	=	3;

	

	indexs_buffer_ptr_->Unlock();

	//將模型 存儲到 map
	d3dx::Device::ModelIndex model_index;
	model_index.base_point_			=	0;
	model_index.point_count_		=	4;
	model_index.primitive_count_	=	2;
	
	//材質
	materials_[MODEL_TEST_MATERIAL]	=	d3dx::white_material;

	D3DXMATRIX matrix,matrix1;
	D3DXMatrixTranslation(&matrix1,0,0,-1);
	matrixs_[MODEL_TEST_0_MATRIX]	=	matrix1;
	
	D3DXMatrixRotationY(&matrix,D3DX_PI);
	matrix	= matrix1 * matrix;
	matrixs_[MODEL_TEST_1_MATRIX]	=	matrix;

	D3DXMatrixRotationY(&matrix,D3DX_PI/2);
	matrix	= matrix1 * matrix;
	matrixs_[MODEL_TEST_2_MATRIX]	=	matrix;

	D3DXMatrixRotationY(&matrix,-D3DX_PI/2);
	matrix	= matrix1 * matrix;
	matrixs_[MODEL_TEST_3_MATRIX]	=	matrix;
	
	D3DXMatrixRotationX(&matrix,D3DX_PI/2);
	matrix	= matrix1 * matrix;
	matrixs_[MODEL_TEST_4_MATRIX]	=	matrix;

	D3DXMatrixRotationX(&matrix,-D3DX_PI/2);
	matrix	= matrix1 * matrix;
	matrixs_[MODEL_TEST_5_MATRIX]	=	matrix;
	
	models_[MODEL_TEST_0_ID]	=	model_index;

	//創建茶壺
	d3dx::ID3DXMeshPtr	mesh_ptr	=	CreateTeapot();
	if(mesh_ptr)
	{
		//大小 位置
		D3DXMATRIX matrix;
		D3DXMatrixTranslation(&matrix,0,0,0);
		matrixs_[MODEL_D3DX_TEAPOT_MATRIX]	=	matrix;

		//材質
		materials_[MODEL_D3DX_TEAPOT_MATERIAL]	=	d3dx::yellow_material;

		//存儲到 map
		models_d3dx_[MODEL_D3DX_TEAPOT_ID]	=	mesh_ptr;
	}

	//啟用紋理
	BOOST_AUTO(texture,CreateTexture(L"dx5_logo.bmp"));
	if(texture)
	{
		textures_[MODEL_D3DX_TEAPOT_TEXTURES]	= texture;
	}
	texture	=	CreateTexture(L"crate.png");
	if(texture)
	{
		textures_[MODEL_TEST_TEXTURES]	= texture;
	}

	//攝像機 位置
	D3DXVECTOR3 camera_position(0.0f,0.0f,-5.0f);
	//攝像機 觀察位置
	D3DXVECTOR3 target_position(0.0f,0.0f,0.0f);
	//世界正方向
	D3DXVECTOR3 world_direction(0.0f,1.0f,0.0f);

	//設置攝像機
	matrix;
	D3DXMatrixLookAtLH(&matrix,&camera_position,&target_position,&world_direction);
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
	BOOST_AUTO(light_ptr,d3dx::CreateDirectionLight(D3DXVECTOR3(2, -6,2),D3DCOLOR_XRGB(255,255,255)));
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
	//清除背景
	device_ptr_->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0xffffffff,1.0f,0);
	
	//繪製場景
	device_ptr_->BeginScene();

		//繪製 ID3DXMesh 模型
		//設置紋理
		device_ptr_->SetTexture(0,textures_[MODEL_D3DX_TEAPOT_TEXTURES].get());

		//設置 位置 大小
		device_ptr_->SetTransform(D3DTS_WORLD,&matrixs_[MODEL_D3DX_TEAPOT_MATRIX]);
		
		//設置材質
		device_ptr_->SetMaterial(&materials_[MODEL_D3DX_TEAPOT_MATERIAL]);

		//繪製
		models_d3dx_[MODEL_D3DX_TEAPOT_ID]->DrawSubset(0);

		//設置 數據源
		device_ptr_->SetStreamSource(0,vertexs_buffer_ptr_,0,sizeof(Vertex));
		device_ptr_->SetIndices(indexs_buffer_ptr_);
		device_ptr_->SetFVF(Vertex::FVF);

		//繪製 點 模型
		device_ptr_->SetTexture(0,textures_[MODEL_TEST_TEXTURES].get());
		
		//開啟融合
		device_ptr_->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		//設置融合因子
		device_ptr_->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		device_ptr_->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA  );

		device_ptr_->SetMaterial(&materials_[MODEL_TEST_MATERIAL]);
		for(int i = MODEL_TEST_0_MATRIX;i < MODEL_TEST_0_MATRIX + MODEL_TEST_COUNT;++i)
		{
			device_ptr_->SetTransform(D3DTS_WORLD,&matrixs_[i]);

			device_ptr_->DrawIndexedPrimitive(models_[MODEL_TEST_0_ID].primitive_type
					,models_[MODEL_TEST_0_ID].base_point_
					,0
					,models_[MODEL_TEST_0_ID].point_count_
					,0
					,models_[MODEL_TEST_0_ID].primitive_count_
					);
		}

		device_ptr_->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	device_ptr_->EndScene();

	//交換緩衝區到 屏幕
	device_ptr_->Present(0,0,0,0);
}

void d3dx::Device::Release()
{

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

d3dx::ID3DXTexturePtr d3dx::Device::CreateTexture(LPCWSTR file_path)
{
	d3dx::ID3DXTexturePtr rs;

	IDirect3DTexture9* texture_ptr	=	NULL;
	D3DXCreateTextureFromFile(device_ptr_,file_path,&texture_ptr);

	if(texture_ptr)
	{
		rs	=	d3dx::ID3DXTexturePtr(texture_ptr,d3dx::SharedPtrRelease<IDirect3DTexture9>);
	}

	return rs;
}

d3dx::ID3DXMeshPtr d3dx::Device::CreateTeapot(__out  LPD3DXBUFFER *ppAdjacency)
{
	d3dx::ID3DXMeshPtr rs;

	LPD3DXMESH mesh_ptr	=	NULL;
	D3DXCreateTeapot(device_ptr_
		,&mesh_ptr
		,ppAdjacency);

	if(mesh_ptr)
	{
		rs	=	d3dx::ID3DXMeshPtr(mesh_ptr,d3dx::SharedPtrRelease<ID3DXMesh>);
	}

	return rs;
}



