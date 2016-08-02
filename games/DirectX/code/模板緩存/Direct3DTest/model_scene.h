#pragma once
#include "d3dx_model_base.h"
#include<boost\array.hpp>

#define D3DX_SCENE_MATRIX_COUNT		4
#define D3DX_SCENE_MATRIX_BACK		0
#define D3DX_SCENE_MATRIX_MIRROR	1
#define D3DX_SCENE_MATRIX_BACK2		2
#define D3DX_SCENE_MATRIX_BOTTOM	3


#define D3DX_SCENE_MATERIAL_COUNT	1
#define D3DX_SCENE_MATERIAL_INDEX	0

#define D3DX_SCENE_TEXTURE_COUNT	3
#define D3DX_SCENE_TEXTURE_BACK		0
#define D3DX_SCENE_TEXTURE_MIRROR	1
#define D3DX_SCENE_TEXTURE_BOTTOM	2

class ModelScene :
	public d3dx::ModelBase
{
public:
	ModelScene(void);
	~ModelScene(void);

	virtual void Create(IDirect3DDevice9* device_ptr);
	virtual void DrawModel();
	virtual void DrawMirror(d3dx::ModelBase& model);
	virtual void DrawShadow(ModelBase& model,D3DXVECTOR4& light);
private:
	boost::array<D3DXMATRIX,D3DX_SCENE_MATRIX_COUNT>				matrixs_;
};

