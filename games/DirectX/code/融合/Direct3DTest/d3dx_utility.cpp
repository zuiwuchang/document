//d3dx_utility.cpp

#include"stdafx.h"
#include"d3dx_utility.h"

boost::shared_ptr<D3DLIGHT9> d3dx::CreateDirectionLight(D3DXVECTOR3& direction, D3DXCOLOR color)
{
	boost::shared_ptr<D3DLIGHT9> light_ptr	=	boost::make_shared<D3DLIGHT9>();
	memset(light_ptr.get(),0,sizeof(D3DLIGHT9));
		
	light_ptr->Type			=	D3DLIGHT_DIRECTIONAL;
	light_ptr->Ambient		=	color * 0.6f;
	light_ptr->Diffuse		=	color;
	light_ptr->Specular		=	color * 0.6f;
	light_ptr->Direction	=	direction;

	return light_ptr;
}

D3DMATERIAL9 d3dx::CreateMaterial(D3DXCOLOR ambient,D3DXCOLOR diffuse,D3DXCOLOR specular,D3DXCOLOR emissive,float power)
{
	D3DMATERIAL9 material;

	material.Ambient	=	ambient;
	material.Diffuse	=	diffuse;
	material.Specular	=	specular;
	material.Emissive	=	emissive;
	material.Power		=	power;

	return material;
}

void d3dx::ComputeNormal(D3DXVECTOR3* vector_ptr,D3DXVECTOR3& p0,D3DXVECTOR3& p1,D3DXVECTOR3& p2)
{
	D3DXVECTOR3 u = p1 - p0;
	D3DXVECTOR3 v = p2 - p0;

	D3DXVec3Cross(vector_ptr,&u,&v);
	D3DXVec3Normalize(vector_ptr,vector_ptr);
}