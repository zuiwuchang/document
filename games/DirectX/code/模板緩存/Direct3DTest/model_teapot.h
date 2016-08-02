#pragma once
#include "d3dx_model_base.h"
class ModelTeapot :
	public d3dx::ModelBase
{
public:
	ModelTeapot(void);
	~ModelTeapot(void);

	virtual void Create(IDirect3DDevice9* device_ptr);
	virtual void DrawModel();
	virtual void DrawModel(D3DXMATRIX& matrix,d3dx::MapMaterial_t& materials = d3dx::MapMaterial_t(),d3dx::MapTextures_t& textures = d3dx::MapTextures_t());
protected:
	LPD3DXMESH				mesh_ptr_;
	
};

