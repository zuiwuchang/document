// Direct3DTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Direct3DTest.h"
#include<Mmsystem.h>

#include"d3dx.h"
#include<dark\debug\ConsoleDebug.hpp>

//攝像機旋轉弧度 步長
#define STEP		(2*D3DX_PI/100)
//攝像機 起始坐標
D3DXVECTOR3 camera_position(0.0f,0.0f,-10.0f);
//攝像機 觀察位置
D3DXVECTOR3 target_position(0.0f,0.0f,0.0f);
//世界正方向
D3DXVECTOR3 world_direction(0.0f,1.0f,0.0f);

//設備接口
IDirect3DDevice9* pDevice; 

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void SurroundY(const float);
void SurroundX(const float);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{	
	//創建窗口 初始化設備 
	if(!d3dx::Create_d3dx_window(&pDevice
		,hInstance
		,WndProc
		,640,480
		,true
		,D3DDEVTYPE_HAL
		)
		)
	{
		return FALSE;
	}

	//其他初始化操作
	d3dx::Initialization(pDevice,640,480);
	
	//進入消息循環
	MSG msg;
	memset(&msg,0,sizeof(MSG));
	float lastTime = (float)timeGetTime(); 
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

			//計算時間間隔
			float currTime  = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			
			d3dx::Draw_scene(pDevice,timeDelta);

			lastTime = currTime;	
		}
	}

	d3dx::Release();

	//釋放設備
	pDevice->Release();

	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hWnd);
		else if( wParam == VK_LEFT )
		{
			SurroundY(STEP);
		}
		else if( wParam == VK_RIGHT )
		{
			SurroundY(-STEP);
		}
		else if( wParam == VK_UP )
		{
			SurroundX(STEP);
		}
		else if( wParam == VK_DOWN )
		{
			SurroundX(-STEP);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void SurroundY(const float step)
{	
	//計算 旋轉 矩陣
	D3DXMATRIX matrix;
	D3DXMatrixRotationY(&matrix,step);
	
	//旋轉 向量	
	D3DXVec3TransformCoord(&camera_position,&camera_position,&matrix);
	
	//計算 攝像機變換 矩陣
	D3DXMatrixLookAtLH(&matrix,&camera_position,&target_position,&world_direction);

	//重設 攝像機
	pDevice->SetTransform(D3DTS_VIEW,&matrix);
}

void SurroundX(const float step)
{	
	//計算 旋轉 矩陣
	D3DXMATRIX matrix;
	D3DXMatrixRotationX(&matrix,step);
	
	//旋轉 向量
	D3DXVec3TransformCoord(&camera_position,&camera_position,&matrix);
	//計算 攝像機變換 矩陣
	D3DXMatrixLookAtLH(&matrix,&camera_position,&target_position,&world_direction);

	//重設 攝像機
	pDevice->SetTransform(D3DTS_VIEW,&matrix);

}