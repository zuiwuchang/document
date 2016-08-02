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
#define SURROUND_STEP		(2*D3DX_PI/100)
//攝像機 起始坐標
D3DXVECTOR3 camera_position(0.0f,0.0f,-15.0f);
//攝像機 觀察位置
D3DXVECTOR3 target_position(0.0f,0.0f,0.0f);
//世界正方向
D3DXVECTOR3 world_direction(0.0f,1.0f,0.0f);

void SurroundY(const float);
void SurroundX(const float);

//移動 步長
#define MOVE_STEP	0.1f


void MoveNoteX(const int step);
void MoveNoteZ(const int step);

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
		//改變Device默認的 攝像機位置
		D3DXMATRIX matrix;
		D3DXMatrixLookAtLH(&matrix,&camera_position,&target_position,&world_direction);
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
			SurroundY(SURROUND_STEP);
		}
		else if( wParam == VK_RIGHT )
		{
			SurroundY(-SURROUND_STEP);
		}
		else if( wParam == VK_UP )
		{
			SurroundX(SURROUND_STEP);
		}
		else if( wParam == VK_DOWN )
		{
			SurroundX(-SURROUND_STEP);
		}

		else if( wParam == 'W' )
		{
			D3DXMATRIX matrix;
			D3DXMatrixTranslation(&matrix,0,0,MOVE_STEP);
			matrix	=	device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix() * matrix;
			device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix(matrix);
		}
		else if( wParam == 'S' )
		{
			D3DXMATRIX matrix;
			D3DXMatrixTranslation(&matrix,0,0,-MOVE_STEP);
			matrix	=	device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix() * matrix;
			device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix(matrix);
		}
		else if( wParam == 'A' )
		{
			D3DXMATRIX matrix;
			D3DXMatrixTranslation(&matrix,-MOVE_STEP,0,0);
			matrix	=	device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix() * matrix;
			device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix(matrix);
		}
		else if( wParam == 'D' )
		{
			D3DXMATRIX matrix;
			D3DXMatrixTranslation(&matrix,MOVE_STEP,0,0);
			matrix	=	device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix() * matrix;
			device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix(matrix);
		}
		else if( wParam == 'Q' )
		{
			D3DXMATRIX matrix;
			D3DXMatrixTranslation(&matrix,0,MOVE_STEP,0);
			matrix	=	device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix() * matrix;
			device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix(matrix);
		}
		else if( wParam == 'E' )
		{
			D3DXMATRIX matrix;
			D3DXMatrixTranslation(&matrix,0,-MOVE_STEP,0);
			matrix	=	device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix() * matrix;
			device.models_[D3DX_MODEL_TEAPOT_ID]->Matrix(matrix);
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
	device->SetTransform(D3DTS_VIEW,&matrix);
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
	device->SetTransform(D3DTS_VIEW,&matrix);

}