// Direct3DTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Direct3DTest.h"

#include"d3dx.h"
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{	
	//創建窗口 初始化設備
	IDirect3DDevice9* pDevice; 
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

	//進入消息循環
	MSG msg;
	memset(&msg,0,sizeof(MSG));
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
			pDevice->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0,10.f,0);

			//將內存 緩衝區 交換到屏幕
			pDevice->Present(0,0,0,0);
		}
	}

	//釋放設備
	pDevice->Release();

	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT: 
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

