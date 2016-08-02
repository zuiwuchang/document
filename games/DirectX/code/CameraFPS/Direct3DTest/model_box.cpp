#include "StdAfx.h"
#include "model_box.h"


ModelBox::ModelBox(void)
{
	mesh_ptr_	=	NULL;
}


ModelBox::~ModelBox(void)
{
	mesh_ptr_
		&& mesh_ptr_->Release();
}

void ModelBox::Create(IDirect3DDevice9* device_ptr)
{
	if(!device_ptr)
	{
		return;
	}
	
	device_ptr_	=	device_ptr;

	//創建 網格
	D3DXCreateMeshFVF(2 * 6		//12個面
		,4 * 6					//24個點
		,D3DXMESH_MANAGED
		,d3dx::Vertex::FVF
		,device_ptr
		,&mesh_ptr_
		);
	
	//寫入點數據
	d3dx::Vertex* vertexs_ptr;
	mesh_ptr_->LockVertexBuffer(0,(void**)&vertexs_ptr);

	vertexs_ptr[0]	=	d3dx::Vertex(D3DXVECTOR3(0,1,-1),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,0));
	vertexs_ptr[1]	=	d3dx::Vertex(D3DXVECTOR3(0,0,-1),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,1));
	vertexs_ptr[2]	=	d3dx::Vertex(D3DXVECTOR3(1,0,-1),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,1));
	vertexs_ptr[3]	=	d3dx::Vertex(D3DXVECTOR3(1,1,-1),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,0));

	vertexs_ptr[4]	=	d3dx::Vertex(D3DXVECTOR3(1,1,-1),D3DXVECTOR3(1,0,0),D3DXVECTOR2(0,0));
	vertexs_ptr[5]	=	d3dx::Vertex(D3DXVECTOR3(1,0,-1),D3DXVECTOR3(1,0,0),D3DXVECTOR2(0,1));
	vertexs_ptr[6]	=	d3dx::Vertex(D3DXVECTOR3(1,0,0),D3DXVECTOR3(1,0,0),D3DXVECTOR2(1,1));
	vertexs_ptr[7]	=	d3dx::Vertex(D3DXVECTOR3(1,1,0),D3DXVECTOR3(1,0,0),D3DXVECTOR2(1,0));

	
	vertexs_ptr[8]	=	d3dx::Vertex(D3DXVECTOR3(0,1,0),D3DXVECTOR3(-1,0,0),D3DXVECTOR2(0,0));
	vertexs_ptr[9]	=	d3dx::Vertex(D3DXVECTOR3(0,0,0),D3DXVECTOR3(-1,0,0),D3DXVECTOR2(0,1));
	vertexs_ptr[10]	=	d3dx::Vertex(D3DXVECTOR3(0,0,-1),D3DXVECTOR3(-1,0,0),D3DXVECTOR2(1,1));
	vertexs_ptr[11]	=	d3dx::Vertex(D3DXVECTOR3(0,1,-1),D3DXVECTOR3(-1,0,0),D3DXVECTOR2(1,0));

	vertexs_ptr[12]	=	d3dx::Vertex(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,-1,0),D3DXVECTOR2(0,0));
	vertexs_ptr[13]	=	d3dx::Vertex(D3DXVECTOR3(0,0,-1),D3DXVECTOR3(0,-1,0),D3DXVECTOR2(0,1));
	vertexs_ptr[14]	=	d3dx::Vertex(D3DXVECTOR3(1,0,-1),D3DXVECTOR3(0,-1,0),D3DXVECTOR2(1,1));
	vertexs_ptr[15]	=	d3dx::Vertex(D3DXVECTOR3(1,0,0),D3DXVECTOR3(0,-1,0),D3DXVECTOR2(1,0));


	vertexs_ptr[16]	=	d3dx::Vertex(D3DXVECTOR3(0,1,0),D3DXVECTOR3(0,1,0),D3DXVECTOR2(0,0));
	vertexs_ptr[17]	=	d3dx::Vertex(D3DXVECTOR3(0,1,-1),D3DXVECTOR3(0,1,0),D3DXVECTOR2(0,1));
	vertexs_ptr[18]	=	d3dx::Vertex(D3DXVECTOR3(1,1,-1),D3DXVECTOR3(0,1,0),D3DXVECTOR2(1,1));
	vertexs_ptr[19]	=	d3dx::Vertex(D3DXVECTOR3(1,1,0),D3DXVECTOR3(0,1,0),D3DXVECTOR2(1,0));

	vertexs_ptr[20]	=	d3dx::Vertex(D3DXVECTOR3(0,1,0),D3DXVECTOR3(0,0,1),D3DXVECTOR2(0,0));
	vertexs_ptr[21]	=	d3dx::Vertex(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,0,1),D3DXVECTOR2(0,1));
	vertexs_ptr[22]	=	d3dx::Vertex(D3DXVECTOR3(1,0,0),D3DXVECTOR3(0,0,1),D3DXVECTOR2(1,1));
	vertexs_ptr[23]	=	d3dx::Vertex(D3DXVECTOR3(1,1,0),D3DXVECTOR3(0,0,1),D3DXVECTOR2(1,0));

	mesh_ptr_->UnlockVertexBuffer();

	//寫入索引數據
	WORD* index_ptr;
	mesh_ptr_->LockIndexBuffer(0,(void**)&index_ptr);
	
	index_ptr[0]	=	2;
	index_ptr[1]	=	1;
	index_ptr[2]	=	0;
	index_ptr[3]	=	3;
	index_ptr[4]	=	2;
	index_ptr[5]	=	0;

	index_ptr[6]	=	6;
	index_ptr[7]	=	5;
	index_ptr[8]	=	4;
	index_ptr[9]	=	7;
	index_ptr[10]	=	6;
	index_ptr[11]	=	4;


	index_ptr[12]	=	10;
	index_ptr[13]	=	9;
	index_ptr[14]	=	8;
	index_ptr[15]	=	11;
	index_ptr[16]	=	10;
	index_ptr[17]	=	8;

	index_ptr[18]	=	12;
	index_ptr[19]	=	13;
	index_ptr[20]	=	14;
	index_ptr[21]	=	12;
	index_ptr[22]	=	14;
	index_ptr[23]	=	15;


	index_ptr[24]	=	18;
	index_ptr[25]	=	17;
	index_ptr[26]	=	16;
	index_ptr[27]	=	19;
	index_ptr[28]	=	18;
	index_ptr[29]	=	16;

	index_ptr[30]	=	20;
	index_ptr[31]	=	21;
	index_ptr[32]	=	22;
	index_ptr[33]	=	20;
	index_ptr[34]	=	22;
	index_ptr[35]	=	23;

	mesh_ptr_->UnlockIndexBuffer();

	//寫入子集屬性id
	DWORD* ids_ptr;
	mesh_ptr_->LockAttributeBuffer(0,&ids_ptr);
	int i=0;
	for(;i<4;++i)
	{
		ids_ptr[i]	=	0;
	}
	for(i=0;i<4;++i)
	{
		ids_ptr[i + 4]	=	1;
	}
	for(i=0;i<4;++i)
	{
		ids_ptr[i + 4 * 2]	=	2;
	}

	mesh_ptr_->UnlockAttributeBuffer();

	//創建紋理
	textures_[D3DX_BOX_TEXTURE_0]	=	CreateTexture(L"..\\source\\checker.jpg");
	textures_[D3DX_BOX_TEXTURE_1]	=	CreateTexture(L"..\\source\\brick0.jpg");
	textures_[D3DX_BOX_TEXTURE_2]	=	CreateTexture(L"..\\source\\brick1.jpg");

	//設置材質
	materials_[D3DX_BOX_TEXTURE_0]	=	d3dx::white_material;
	
	//獲取 鄰接數據
	boost::scoped_array<DWORD> adjacency_ptr(new DWORD[mesh_ptr_->GetNumFaces() * 3]);
	mesh_ptr_->GenerateAdjacency(0.0f,adjacency_ptr.get());

	//優化網格
	mesh_ptr_->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE
		,adjacency_ptr.get()
		,NULL,NULL,NULL
		);
}

void ModelBox::DrawModel()
{
	if(!device_ptr_
		|| !mesh_ptr_
		)
	{
		return;
	}
	device_ptr_->SetMaterial(&materials_[D3DX_BOX_TEXTURE_0]);
	device_ptr_->SetTransform(D3DTS_WORLD,&matrix_);
	
	for(int i=0;i<D3DX_BOX_ATTRIB_COUNT;++i)
	{
		device_ptr_->SetTexture(0,textures_[i].get());
		mesh_ptr_->DrawSubset(i);
	}
}