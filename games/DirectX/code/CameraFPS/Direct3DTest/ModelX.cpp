#include "StdAfx.h"
#include "ModelX.h"


ModelX::ModelX(void)
{
	mesh_ptr_	=	NULL;
}


ModelX::~ModelX(void)
{
	mesh_ptr_ 
		&&	mesh_ptr_->Release();
}

void ModelX::Create(IDirect3DDevice9* device_ptr)
{
	if(!device_ptr)
	{
		return;
	}
	device_ptr_	=	device_ptr;
	
	//創建網格
	LPD3DXBUFFER  materials_ptr;
	D3DXLoadMeshFromX(L"..\\source\\bigship1.x"
		,D3DXMESH_MANAGED
		,device_ptr
		,NULL
		,&materials_ptr
		,NULL
		,&ids_count_
		,&mesh_ptr_
		);
	if(!mesh_ptr_)
	{
		return;
	}

	//加載紋理 材質
	LPD3DXMATERIAL	material_arrays	=	(LPD3DXMATERIAL)materials_ptr->GetBufferPointer();
	for(DWORD i=0;i<ids_count_;++i)
	{        
		materials_[i]	=	material_arrays[i].MatD3D;
		textures_[i]	=	CreateTexture(material_arrays[i].pTextureFilename);
	}
	materials_ptr->Release();
}

void ModelX::DrawModel()
{
	if(!device_ptr_
		|| !mesh_ptr_
		)
	{
		return;
	}

	device_ptr_->SetTransform(D3DTS_WORLD,&matrix_);
	for(UINT i=0;i<ids_count_;++i)
	{
		device_ptr_->SetMaterial(&materials_[i]);
		if(textures_[i])
		{
			device_ptr_->SetTexture(0,textures_[i].get());
		}
		else
		{
			device_ptr_->SetTexture(0,0);
		}
		mesh_ptr_->DrawSubset(i);
	}
}