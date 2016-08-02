#pragma once
#include "d3dx_model_base.h"
class ModelX :
	public d3dx::ModelBase
{
public:
	ModelX(void);
	~ModelX(void);

protected:
	LPD3DXMESH	mesh_ptr_;
	DWORD		ids_count_;
public:
	virtual void Create(IDirect3DDevice9* device_ptr);
	virtual void DrawModel();
};

