//d3dx.h	一些d3dx編程的複製函數
#ifndef _DARK_D3DX_
#define _DARK_D3DX_

namespace d3dx
{
	bool Create_d3dx_window(IDirect3DDevice9** ppDevice
		,HINSTANCE hApplication
		,WNDPROC wProc
		,int nWidth = 800
		,int nHeight = 640
		,bool bWindow = true
		,D3DDEVTYPE devType= D3DDEVTYPE_HAL
		);

	
	void Initialization(IDirect3DDevice9* pDevice,int nWidth,int nHeight);
	void Create(IDirect3DDevice9* pDevice);
	void Release();
	void Draw_scene(IDirect3DDevice9* pDevice,float time);
};

#endif