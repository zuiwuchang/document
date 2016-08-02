//d3dx_utility.h	一些簡化操作的 函數 和 定義
#ifndef _DARK_D3DX_UTILITY_
#define _DARK_D3DX_UTILITY_

#include<boost\smart_ptr.hpp>

#define	D3DX_COLOR_WHITE	D3DCOLOR_XRGB(255, 255, 255)
#define	D3DX_COLOR_BLACK	D3DCOLOR_XRGB(0,0,0)
#define	D3DX_COLOR_RED		D3DCOLOR_XRGB(255,0,0)
#define	D3DX_COLOR_GREEN	D3DCOLOR_XRGB(0,255,0)
#define	D3DX_COLOR_BLUE		D3DCOLOR_XRGB(0,0,255)
#define	D3DX_COLOR_YELLOW	D3DCOLOR_XRGB(255,255,0)
#define	D3DX_COLOR_CYAN		D3DCOLOR_XRGB(0,255,255)	//青
#define D3DX_COLOR_MAGENTA	D3DCOLOR_XRGB(255,0,255)	//品紅

namespace d3dx
{
	//COM接口 shared_ptr 刪除器
	template<typename T>
	void SharedPtrRelease(T* t)
	{
		t->Release();
		t	=	NULL;
	}

	/************************************	光照	**********************************************/
	//創建方向光
	boost::shared_ptr<D3DLIGHT9> CreateDirectionLight(D3DXVECTOR3& direction, D3DXCOLOR color);


	/************************************	材質	**********************************************/
	D3DMATERIAL9 CreateMaterial(D3DXCOLOR ambient,D3DXCOLOR diffuse,D3DXCOLOR specular,D3DXCOLOR emissive,float power);
	
	//白 紅 綠 藍 黃 材質
const D3DMATERIAL9 white_material	=	CreateMaterial(D3DX_COLOR_WHITE,D3DX_COLOR_WHITE,D3DX_COLOR_WHITE,D3DX_COLOR_BLACK,2.0f);
const D3DMATERIAL9 red_material		=	CreateMaterial(D3DX_COLOR_RED,D3DX_COLOR_RED,D3DX_COLOR_RED,D3DX_COLOR_BLACK,2.0f);
const D3DMATERIAL9 green_material	=	CreateMaterial(D3DX_COLOR_GREEN,D3DX_COLOR_GREEN,D3DX_COLOR_GREEN,D3DX_COLOR_BLACK,2.0f);
const D3DMATERIAL9 blue_material	=	CreateMaterial(D3DX_COLOR_BLUE,D3DX_COLOR_BLUE,D3DX_COLOR_BLUE,D3DX_COLOR_BLACK,2.0f);
const D3DMATERIAL9 yellow_material	=	CreateMaterial(D3DX_COLOR_YELLOW,D3DX_COLOR_YELLOW,D3DX_COLOR_YELLOW,D3DX_COLOR_BLACK,2.0f);

	/************************************	算法	**********************************************/
	//返回三角區域的 頂點法線
	void ComputeNormal(D3DXVECTOR3* vector_ptr,D3DXVECTOR3& p0,D3DXVECTOR3& p1,D3DXVECTOR3& p2);
};



#endif //_DARK_D3DX_UTILITY_