#include "StdAfx.h"
#include "model_teapot.h"


ModelTeapot::ModelTeapot(void)
{
	mesh_ptr_	=	NULL;
}


ModelTeapot::~ModelTeapot(void)
{
	mesh_ptr_
		&& mesh_ptr_->Release();
}

void ModelTeapot::Create(IDirect3DDevice9* device_ptr)
{
	if(!device_ptr)
	{
		return;
	}

	device_ptr_	=	device_ptr;
	
	D3DXCreateTeapot(device_ptr_
		,&mesh_ptr_
		,0);

	materials_[D3DX_MODEL_MATERIAL_DEFAULT]	=	d3dx::white_material;

	textures_[D3DX_MODEL_TEXTURE_DEFAULT]	=	CreateTexture(L"..\\source\\dx5_logo.bmp");
	
}
void ModelTeapot::DrawModel()
{
	if(!device_ptr_
		|| !mesh_ptr_
		)
	{
		return;
	}

	//設置 紋理
	device_ptr_->SetTexture(0,textures_[D3DX_MODEL_TEXTURE_DEFAULT].get());

	//設置 位置 大小
	device_ptr_->SetTransform(D3DTS_WORLD,&matrix_);
		
	//設置材質
	device_ptr_->SetMaterial(&materials_[D3DX_MODEL_MATERIAL_DEFAULT]);

	mesh_ptr_->DrawSubset(0);
}

void ModelTeapot::DrawModel(D3DXMATRIX& matrix,d3dx::MapMaterial_t& materials,d3dx::MapTextures_t& textures)
{
	if(!device_ptr_
		|| !mesh_ptr_
		)
	{
		return;
	}

	//設置 紋理
	BOOST_AUTO(find_texture,textures.find(D3DX_MODEL_TEXTURE_DEFAULT));
	if(textures.end() == find_texture)
	{
		device_ptr_->SetTexture(0,0);
	}
	else
	{
		device_ptr_->SetTexture(0,find_texture->second.get());
	}

	//設置 位置 大小
	device_ptr_->SetTransform(D3DTS_WORLD,&matrix);

	//設置材質
	BOOST_AUTO(find_material,materials.find(D3DX_MODEL_TEXTURE_DEFAULT));
	if(materials.end() != find_material)
	{
		device_ptr_->SetMaterial(&(find_material->second));
	}

	mesh_ptr_->DrawSubset(0);
}