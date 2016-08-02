//d3dx.h	一些d3dx編程的複製函數
#ifndef _DARK_D3DX_
#define _DARK_D3DX_

#include<boost\noncopyable.hpp>
#include<boost\tuple\tuple.hpp>
#include<boost\smart_ptr.hpp>
#include<boost\typeof\typeof.hpp>
#include<boost\unordered_map.hpp>

#define D3DX_INDEX_MODEL	0
#define D3DX_INDEX_MATRIX	1
#define D3DX_INDEX_MATERIAL 2

namespace d3dx
{
	//D3DX 模型 智能指針
	typedef boost::shared_ptr<ID3DXMesh>			ID3DXMeshPtr;
	//紋理 智能指針
	typedef boost::shared_ptr<IDirect3DTexture9>	ID3DXTexturePtr;
	
	//定義點結構
	struct Vertex
	{
		//點坐標
		D3DXVECTOR3	pos_;
	
		//點法線
		D3DXVECTOR3 vector_;

		//紋理坐標
		D3DXVECTOR2 texture_;
	
		static const DWORD FVF;
		Vertex()
		{
		}
		Vertex(const D3DXVECTOR3& pos,const D3DXVECTOR3& vector,const D3DXVECTOR2& texture)
		{
			pos_		=	pos;
			vector_		=	vector;
			texture_	=	texture;
		}
	};

	//設備 class
	class Device
		: boost::noncopyable
	{
	private:
		IDirect3DDevice9*		device_ptr_;
		IDirect3DVertexBuffer9*	vertexs_buffer_ptr_;
		IDirect3DIndexBuffer9*	indexs_buffer_ptr_;

		int width_;
		int height_;
	public:
		Device();
		~Device();

		//創建窗口 IDirect3DDevice9 接口 必須第一個被調用的初始化函數
		bool CreateDeviceWindow(HINSTANCE hInstance		//應用程序實例
			,WNDPROC window_fun							//窗口函數
			,int width = 800							//窗口寬
			,int height = 640							//窗口高
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
		
		//保存模型 點 位置
		class ModelIndex
		{
		public:
			D3DPRIMITIVETYPE primitive_type;
			INT		base_point_;
			UINT	point_count_;
			UINT	primitive_count_;

			ModelIndex()
			{
				primitive_type		=	D3DPT_TRIANGLELIST;
				base_point_			=	0;
				point_count_		=	0;
				primitive_count_	=	0;
			}
		};
		//保存 點 模型
		boost::unordered_map<unsigned int,boost::tuple<ModelIndex,D3DXMATRIX,D3DMATERIAL9>> models_;
		

		//保存 D3DX 提供的 D3DXCreate* 模型
		boost::unordered_map<unsigned int,boost::tuple<ID3DXMeshPtr,D3DXMATRIX,D3DMATERIAL9>> models_d3dx_;
	
	public:
		//提供方便的 ID3DXMeshPtr 創建
		ID3DXMeshPtr CreateTeapot(__out  LPD3DXBUFFER *ppAdjacency = NULL);

	private:
		//保存 光源
		boost::unordered_map<unsigned int,boost::shared_ptr<D3DLIGHT9>> lights_;
		//保存 紋理
		boost::unordered_map<unsigned int,ID3DXTexturePtr> textures_;

	public:
		//創建紋理
		ID3DXTexturePtr CreateTexture(LPCWSTR file_path);
	};
	
	//創建 簡單光照
	//創建方向光
	boost::shared_ptr<D3DLIGHT9> CreateDirectionLight(D3DXVECTOR3& direction, D3DXCOLOR color);


	//shared_ptr 刪除器
	template<typename T>
	void SharedPtrRelease(T* t)
	{
		t->Release();
		t	=	NULL;
	}

	//返回三角區域的 頂點法線
	void ComputeNormal(D3DXVECTOR3* out,D3DXVECTOR3& p0,D3DXVECTOR3& p1,D3DXVECTOR3& p2);

	
};

#endif