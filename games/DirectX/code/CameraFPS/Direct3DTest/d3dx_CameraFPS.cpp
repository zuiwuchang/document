#include "StdAfx.h"
#include "d3dx_CameraFPS.h"


d3dx::CameraFPS::CameraFPS(void)
{
	position_	=	D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	up_			=	D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	right_		=	D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	front_		=	D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}


d3dx::CameraFPS::~CameraFPS(void)
{
}

void d3dx::CameraFPS::ComputeViewMatrix(D3DXMATRIX* matrix_ptr)
{
	//規範化 坐標系
	D3DXVec3Normalize(&front_, &front_);

	D3DXVec3Cross(&up_, &front_, &right_);
	D3DXVec3Normalize(&up_, &up_);

	D3DXVec3Cross(&right_, &up_, &front_);
	D3DXVec3Normalize(&right_, &right_);

	//構造變換矩陣
	float x	=	-D3DXVec3Dot(&right_,&position_);
	float y	=	-D3DXVec3Dot(&up_,&position_);
	float z	=	-D3DXVec3Dot(&front_,&position_);

	(*matrix_ptr)(0,0)	=	right_.x;
	(*matrix_ptr)(0,1)	=	up_.x;
	(*matrix_ptr)(0,2)	=	front_.x;
	(*matrix_ptr)(0,3)	=	0;

	(*matrix_ptr)(1,0)	=	right_.y;
	(*matrix_ptr)(1,1)	=	up_.y;
	(*matrix_ptr)(1,2)	=	front_.y;
	(*matrix_ptr)(1,3)	=	0;

	(*matrix_ptr)(2,0)	=	right_.z;
	(*matrix_ptr)(2,1)	=	up_.z;
	(*matrix_ptr)(2,2)	=	front_.z;
	(*matrix_ptr)(2,3)	=	0;

	(*matrix_ptr)(3,0)	=	x;
	(*matrix_ptr)(3,1)	=	y;
	(*matrix_ptr)(3,2)	=	z;
	(*matrix_ptr)(3,3)	=	1;

}

void d3dx::CameraFPS::RotateUp(const float angle)
{
	//對於fps視角 可能發生了 俯/仰 
	//但依然應該 沿y軸 旋轉 而非 up_軸
	D3DXMATRIX matrix;
	D3DXMatrixRotationY(&matrix,angle);

	//計算 另外兩個向量 
	D3DXVec3TransformCoord(&right_,&right_, &matrix);
	D3DXVec3TransformCoord(&front_,&front_, &matrix);
}

void d3dx::CameraFPS::RotateRight(float angle)
{
	D3DXMATRIX matrix;
	D3DXMatrixRotationAxis(&matrix,&right_,angle);

	//計算 另外兩個向量 
	D3DXVec3TransformCoord(&up_,&up_, &matrix);
	D3DXVec3TransformCoord(&front_,&front_, &matrix);
}

void d3dx::CameraFPS::RotateFront(const float angle)
{
	D3DXMATRIX matrix;
	D3DXMatrixRotationAxis(&matrix,&front_,angle);

	//計算 另外兩個向量 
	D3DXVec3TransformCoord(&right_,&right_, &matrix);
	D3DXVec3TransformCoord(&front_,&front_, &matrix);

}