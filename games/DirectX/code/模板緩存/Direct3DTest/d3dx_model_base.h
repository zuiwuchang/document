//d3dx_model_base.h 模型基類
#ifndef _DARK_D3DX_MODEL_BASE_
#define _DARK_D3DX_MODEL_BASE_

#include"d3dx_utility.h"

#define D3DX_MODEL_TEXTURE_DEFAULT	0
#define D3DX_MODEL_MATERIAL_DEFAULT	0

namespace d3dx
{
	typedef boost::unordered_map<unsigned int,ID3DXTexturePtr_t>	MapTextures_t;
	typedef boost::unordered_map<unsigned int,D3DMATERIAL9>			MapMaterial_t;

	class ModelBase
	{
	protected:
		IDirect3DDevice9*		device_ptr_;
		IDirect3DVertexBuffer9*	vertexs_buffer_ptr_;
		IDirect3DIndexBuffer9*	indexs_buffer_ptr_;


	public:
		ModelBase();
		virtual ~ModelBase();
		virtual void Create(IDirect3DDevice9* device_ptr)=0;
		virtual void DrawModel()	=	0;
		virtual void DrawModel(D3DXMATRIX& matrix,MapMaterial_t& materials = MapMaterial_t(),MapTextures_t& textures = MapTextures_t()){};
		virtual void DrawMirror(ModelBase& model){}
		virtual void DrawShadow(ModelBase& model,D3DXVECTOR4& light){}
	public:
		ID3DXTexturePtr_t CreateTexture(LPCWSTR file_path);

		/*********************變換矩陣(位置 大小)*************************************/
	protected:
		D3DXMATRIX matrix_;
	public:
		inline void Matrix(const D3DXMATRIX& matrix)
		{
			matrix_	=	matrix;
		}
		inline D3DXMATRIX Matrix() const
		{
			return matrix_;
		}

		/***************************紋理*******************************/
	protected:
		MapTextures_t textures_;
	public:
		inline void Texture(unsigned int what,ID3DXTexturePtr_t texture_ptr)
		{
			textures_[what]	=	texture_ptr;
		}
		inline ID3DXTexturePtr_t Texture(unsigned int what)
		{
			return textures_[what];
		}

		/***************************材質*******************************/
	protected:
		boost::unordered_map<unsigned int,D3DMATERIAL9> materials_;
	public:
		inline void Material(unsigned int what,const D3DMATERIAL9& material)
		{
			materials_[what]	=	material;
		}
		inline D3DMATERIAL9 Material(unsigned int what)
		{
			return materials_[what];
		}

	};

	typedef boost::shared_ptr<ModelBase> ModlePtr_t;
};

#endif	//_DARK_D3DX_MODEL_BASE_
