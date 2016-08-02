// Direct3DTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Direct3DTest.h"
#include<Mmsystem.h>

#include"d3dx.h"
#include<dark\debug\ConsoleDebug.hpp>

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//設備實例
d3dx::Device device;

//攝像機旋轉弧度 步長
#define SURROUND_STEP	(2*D3DX_PI/100)
#define MOVE_STEP		1


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{	
	
	device.CreateDeviceWindow(hInstance
		,WndProc
		,640,480
		,true
		,D3DDEVTYPE_HAL
		);

	if(device)
	{
		//dark::debug::ConsoleDebug::instance();

		//改變Device默認的 攝像機位置
		D3DXMATRIX matrix;
		device.camera.Position(D3DXVECTOR3(0,0,-15));
		device.camera.ComputeViewMatrix(&matrix);
		device->SetTransform(D3DTS_VIEW,&matrix);

		device.EnterMessageLoop();
	}
	
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
			device.camera.RotateUp(-SURROUND_STEP);
			device.camera.Refresh();
		}
		else if( wParam == VK_RIGHT )
		{
			device.camera.RotateUp(SURROUND_STEP);
			device.camera.Refresh();
		}
		else if( wParam == VK_UP )
		{
			device.camera.RotateRight(-SURROUND_STEP);
			device.camera.Refresh();
		}
		else if( wParam == VK_DOWN )
		{
			device.camera.RotateRight(SURROUND_STEP);
			device.camera.Refresh();
		}
		else if( wParam == VK_OEM_PLUS )
		{
			device.camera.RotateFront(-SURROUND_STEP);
			device.camera.Refresh();
		}
		else if( wParam == VK_OEM_MINUS )
		{
			device.camera.RotateFront(SURROUND_STEP);
			device.camera.Refresh();
		}

		else if( wParam == 'W' )
		{
			device.camera.MoveFront(MOVE_STEP);
			device.camera.Refresh();
		}
		else if( wParam == 'S' )
		{
			device.camera.MoveFront(-MOVE_STEP);
			device.camera.Refresh();
		}
		else if( wParam == 'A' )
		{
			device.camera.MoveRight(-MOVE_STEP);
			device.camera.Refresh();
		}
		else if( wParam == 'D' )
		{
			device.camera.MoveRight(MOVE_STEP);
			device.camera.Refresh();
		}
		else if( wParam == 'Q' )
		{
			device.camera.MoveUp(MOVE_STEP);
			device.camera.Refresh();
		}
		else if( wParam == 'E' )
		{
			device.camera.MoveUp(-MOVE_STEP);
			device.camera.Refresh();
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}