//d3dx_model_base.cpp

#include"stdafx.h"
#include"d3dx_model_base.h"

d3dx::ModelBase::ModelBase()
{
	device_ptr_			=	NULL;
	vertexs_buffer_ptr_	=	NULL;
	indexs_buffer_ptr_	=	NULL;

	D3DXMatrixTranslation(&matrix_,0,0,0);
}

d3dx::ModelBase::~ModelBase()
{
	vertexs_buffer_ptr_ 
		&& vertexs_buffer_ptr_->Release();

	indexs_buffer_ptr_
		&& indexs_buffer_ptr_->Release();

}

d3dx::ID3DXTexturePtr_t d3dx::ModelBase::CreateTexture(LPCWSTR file_path)
{
	d3dx::ID3DXTexturePtr_t rs;
	
	if(device_ptr_)
	{
		IDirect3DTexture9* texture_ptr	=	NULL;
		D3DXCreateTextureFromFile(device_ptr_,file_path,&texture_ptr);

		if(texture_ptr)
		{
			rs	=	d3dx::ID3DXTexturePtr_t(texture_ptr,d3dx::SharedPtrRelease<IDirect3DTexture9>);
		}
	}

	return rs;
}