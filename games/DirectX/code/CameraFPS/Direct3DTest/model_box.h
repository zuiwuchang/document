#pragma once
#include "d3dx_model_base.h"

#define D3DX_BOX_TEXTURE_0		0
#define D3DX_BOX_TEXTURE_1		1
#define D3DX_BOX_TEXTURE_2		2

#define D3DX_BOX_ATTRIB_COUNT	3

class ModelBox :
	public d3dx::ModelBase
{
public:
	ModelBox(void);
	~ModelBox(void);

protected:
	LPD3DXMESH				mesh_ptr_;

public:
	virtual void Create(IDirect3DDevice9* device_ptr);
	virtual void DrawModel();
};

