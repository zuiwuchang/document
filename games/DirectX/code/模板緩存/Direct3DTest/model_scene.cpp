#include "StdAfx.h"
#include "model_scene.h"


ModelScene::ModelScene(void)
{
}


ModelScene::~ModelScene(void)
{
}

void ModelScene::Create(IDirect3DDevice9* device_ptr)
{
	if(!device_ptr)
	{
		return;
	}

	device_ptr_	=	device_ptr;

	//創建 頂點 緩存
	device_ptr_->CreateVertexBuffer(4 * sizeof(d3dx::Vertex)
		,D3DUSAGE_WRITEONLY
		,d3dx::Vertex::FVF
		,D3DPOOL_MANAGED
		,&vertexs_buffer_ptr_
		,0
		);
	
	//設置使用的點信息
	d3dx::Vertex* vertexs_ptr;
	vertexs_buffer_ptr_->Lock(0
		,0
		,(void**)&vertexs_ptr
		,0);
	D3DXVECTOR3 point0(-1,1,0)
		,point1(1,1,0)
		,point2(1,-1,0)
		,point3(-1,-1,0)
		,vector
		;
	//計算點 法線
	d3dx::ComputeNormal(&vector,point0,point1,point2);
	
	//計算紋理坐標 並設置點數據
	vertexs_ptr[0]	=	d3dx::Vertex(point0,vector,D3DXVECTOR2(0,0));
	vertexs_ptr[1]	=	d3dx::Vertex(point1,vector,D3DXVECTOR2(1,0));
	vertexs_ptr[2]	=	d3dx::Vertex(point2,vector,D3DXVECTOR2(1,1));
	vertexs_ptr[3]	=	d3dx::Vertex(point3,vector,D3DXVECTOR2(0,1));
	
	vertexs_buffer_ptr_->Unlock();


	//創建 索引 緩存
	device_ptr_->CreateIndexBuffer(2 * 3 * sizeof(WORD)
		,D3DUSAGE_WRITEONLY
		,D3DFMT_INDEX16
		,D3DPOOL_MANAGED
		,&indexs_buffer_ptr_
		,0
		);
	//寫入索引 建模
	WORD* indexs_ptr;
	indexs_buffer_ptr_->Lock(0
		,0
		,(void**)&indexs_ptr
		,0
		);

	indexs_ptr[0]	=	0;
	indexs_ptr[1]	=	1;
	indexs_ptr[2]	=	2;
	indexs_ptr[3]	=	0;
	indexs_ptr[4]	=	2;
	indexs_ptr[5]	=	3;

	indexs_buffer_ptr_->Unlock();

	//組建 模型
	//back
	D3DXMATRIX matrix;
	D3DXMatrixScaling(&matrixs_[D3DX_SCENE_MATRIX_BACK2],2,4,1);
	D3DXMatrixTranslation(&matrix,-4,0,3);	
	matrixs_[D3DX_SCENE_MATRIX_BACK]	=	matrixs_[D3DX_SCENE_MATRIX_BACK2] * matrix;
	
	textures_[D3DX_SCENE_TEXTURE_BACK]	=	CreateTexture(L"..\\source\\brick0.jpg");

	D3DXMatrixTranslation(&matrix,4,0,3);
	D3DXMatrixScaling(&matrixs_[D3DX_SCENE_MATRIX_BACK2],2,4,1);
	matrixs_[D3DX_SCENE_MATRIX_BACK2]	*=	matrix;

	//mirror
	D3DXMatrixTranslation(&matrixs_[D3DX_SCENE_MATRIX_MIRROR],0,0,3);
	D3DXMatrixScaling(&matrix,2,4,1);
	matrixs_[D3DX_SCENE_MATRIX_MIRROR]	*=	matrix;

	textures_[D3DX_SCENE_TEXTURE_MIRROR]	=	CreateTexture(L"..\\source\\ice.bmp");

	//bootom
	D3DXMatrixScaling(&matrixs_[D3DX_SCENE_MATRIX_BOTTOM],6,3,1);
	D3DXMatrixRotationX(&matrix,D3DX_PI/2);
	matrixs_[D3DX_SCENE_MATRIX_BOTTOM]	*=	matrix;
	D3DXMatrixTranslation(&matrix,0,-4,0);
	matrixs_[D3DX_SCENE_MATRIX_BOTTOM]	*=	matrix;

	textures_[D3DX_SCENE_TEXTURE_BOTTOM]	=	CreateTexture(L"..\\source\\checker.jpg");

	//材質
	materials_[D3DX_SCENE_MATERIAL_INDEX]	=	d3dx::white_material;
}
void ModelScene::DrawModel()
{
	if(!device_ptr_)
	{
		return;
	}
	//設置 數據源
	device_ptr_->SetStreamSource(0,vertexs_buffer_ptr_,0,sizeof(d3dx::Vertex));
	device_ptr_->SetIndices(indexs_buffer_ptr_);
	device_ptr_->SetFVF(d3dx::Vertex::FVF);


	device_ptr_->SetMaterial(&materials_[D3DX_SCENE_MATERIAL_INDEX]);

	D3DXMATRIX matrix;
	//back
	device_ptr_->SetTexture(0,textures_[D3DX_SCENE_TEXTURE_BACK].get());

	matrix	=	matrixs_[D3DX_SCENE_MATRIX_BACK] * matrix_;
	device_ptr_->SetTransform(D3DTS_WORLD,&matrix);
	device_ptr_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	matrix	=	matrixs_[D3DX_SCENE_MATRIX_BACK2] * matrix_;
	device_ptr_->SetTransform(D3DTS_WORLD,&matrix);
	device_ptr_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	//mirror
	matrix	=	matrixs_[D3DX_SCENE_MATRIX_MIRROR] * matrix_;
	device_ptr_->SetTransform(D3DTS_WORLD,&matrix);
	device_ptr_->SetTexture(0,textures_[D3DX_SCENE_TEXTURE_MIRROR].get());
	device_ptr_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	//bottom
	matrix	=	matrixs_[D3DX_SCENE_MATRIX_BOTTOM] * matrix_;
	device_ptr_->SetTransform(D3DTS_WORLD,&matrix);
	device_ptr_->SetTexture(0,textures_[D3DX_SCENE_TEXTURE_BOTTOM].get());
	device_ptr_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);
}

void ModelScene::DrawMirror(d3dx::ModelBase& model)
{
	//清空模板 緩存
	device_ptr_->Clear(0,0,D3DCLEAR_STENCIL,0,0,0);
	
	//開啟 融合
	device_ptr_->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//開啟 模板緩存
	device_ptr_->SetRenderState(D3DRS_STENCILENABLE,    TRUE);
	
	//設置 模板測試 始終為TRUE
	device_ptr_->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_ALWAYS);
	//設置模板參考值
	device_ptr_->SetRenderState(D3DRS_STENCILREF,       0x1);
	
	//設置 左 右 掩碼
	device_ptr_->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
	device_ptr_->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	//失敗 保持模板值不變
	device_ptr_->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);	//深度測試失敗 說明鏡像的此像素 被其他 物品 遮蓋
	device_ptr_->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
	//成功 替換值 為 D3DRS_STENCILREF 設定值
	device_ptr_->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_REPLACE);

	/*********************************此後三行代碼 保證 深度緩存 後台緩存 不被修改****************************/
	//關閉 繪製 對深度緩存的 更改
	device_ptr_->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	//設置融合因子 使對後台緩存寫入無效
	device_ptr_->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ZERO);
	device_ptr_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//繪製鏡面 既標記模板緩存
	device_ptr_->SetStreamSource(0,vertexs_buffer_ptr_,0,sizeof(d3dx::Vertex));
	device_ptr_->SetIndices(indexs_buffer_ptr_);
	device_ptr_->SetFVF(d3dx::Vertex::FVF);

	D3DXMATRIX matrix;
	matrix	=	matrixs_[D3DX_SCENE_MATRIX_MIRROR] * matrix_;
	device_ptr_->SetTransform(D3DTS_WORLD,&matrix);
	device_ptr_->SetTexture(0,textures_[D3DX_SCENE_TEXTURE_MIRROR].get());
	device_ptr_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	//恢復 關閉的 深度緩存 
	device_ptr_->SetRenderState( D3DRS_ZWRITEENABLE,TRUE);

	//將模板測試設置為 比較 0x1 & mask_l = value & mask_r
	device_ptr_->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_EQUAL);
	
	//關閉 繪製 對深度緩存的 更改
	//鏡面 所在平面
	D3DXPLANE plane(0.0f, 0.0f, 1.0f, -3.0f);
	
	//計算 倒影矩陣 
	D3DXMATRIX	matrix_old,matrix_mirror;
	D3DXMatrixReflect(&matrix_mirror, &plane);
	matrix_old	=	model.Matrix();
	model.Matrix(matrix_old * matrix_mirror);
	
	//修改 消影 方式
	device_ptr_->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//禁用深度緩存 設置融合因子
	device_ptr_->SetRenderState(D3DRS_ZENABLE,FALSE);
	device_ptr_->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR);
    device_ptr_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	//繪製
	model.DrawModel();
	
	//恢復 改變的狀態
	model.Matrix(matrix_old);

	device_ptr_->SetRenderState(D3DRS_ZENABLE,TRUE);

	device_ptr_->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	device_ptr_->SetRenderState( D3DRS_STENCILENABLE,FALSE);

	device_ptr_->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
}

void ModelScene::DrawShadow(d3dx::ModelBase& model,D3DXVECTOR4& light)
{
	//清空模板 緩存
	device_ptr_->Clear(0,0,D3DCLEAR_STENCIL,0,0,0);
	
	//開啟 融合
	device_ptr_->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//開啟 模板緩存
	device_ptr_->SetRenderState(D3DRS_STENCILENABLE,    TRUE);
	
	//設置 模板測試 始終為TRUE
	device_ptr_->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_ALWAYS);
	//設置模板參考值
	device_ptr_->SetRenderState(D3DRS_STENCILREF,       0x1);
	
	//設置 左 右 掩碼
	device_ptr_->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
	device_ptr_->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	//失敗 保持模板值不變
	device_ptr_->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);	//深度測試失敗 說明鏡像的此像素 被其他 物品 遮蓋
	device_ptr_->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
	//成功 替換值 為 D3DRS_STENCILREF 設定值
	device_ptr_->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_REPLACE);

	/*********************************此後三行代碼 保證 深度緩存 後台緩存 不被修改****************************/
	//關閉 繪製 對深度緩存的 更改
	device_ptr_->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	//設置融合因子 使對後台緩存寫入無效
	device_ptr_->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ZERO);
	device_ptr_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//繪製鏡面 既標記模板緩存
	device_ptr_->SetStreamSource(0,vertexs_buffer_ptr_,0,sizeof(d3dx::Vertex));
	device_ptr_->SetIndices(indexs_buffer_ptr_);
	device_ptr_->SetFVF(d3dx::Vertex::FVF);

	D3DXMATRIX matrix;
	matrix	=	matrixs_[D3DX_SCENE_MATRIX_BOTTOM] * matrix_;
	device_ptr_->SetTransform(D3DTS_WORLD,&matrix);
	device_ptr_->SetTexture(0,textures_[D3DX_SCENE_TEXTURE_MIRROR].get());
	device_ptr_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	//恢復 關閉的 深度緩存 
	device_ptr_->SetRenderState( D3DRS_ZWRITEENABLE,TRUE);

	//將模板測試設置為 比較 0x1 & mask_l = value & mask_r
	device_ptr_->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_EQUAL);
	//若模板測試成功 模板值 +1
	device_ptr_->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_INCR);

	//關閉 繪製 對深度緩存的 更改
	//鏡面 所在平面
	D3DXPLANE plane(0.0f, 1.0f, 0.0f, 3.0f);
	
	//計算 倒影矩陣 
	D3DXMATRIX	matrix_old,matrix_shadow;
	matrix_shadow	=	matrix_old	=	model.Matrix();
	D3DXMatrixShadow(&matrix_shadow,&light,&plane);
	matrix_shadow	=	matrix_old * matrix_shadow;
	
	//禁用深度緩存 設置融合因子
	device_ptr_->SetRenderState(D3DRS_ZENABLE,FALSE);
	device_ptr_->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR);
    device_ptr_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	//繪製
	d3dx::MapMaterial_t materials;
	materials[D3DX_MODEL_TEXTURE_DEFAULT]	=	d3dx::shadow_material;
	model.DrawModel(matrix_shadow,materials);
	
	//恢復 改變的狀態
	device_ptr_->SetRenderState(D3DRS_ZENABLE,TRUE);
		
	device_ptr_->SetRenderState( D3DRS_STENCILENABLE,FALSE);

	device_ptr_->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);

}