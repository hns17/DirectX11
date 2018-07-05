#include "stdafx.h"
#include "TestScene.h"


HRESULT TestScene::Init(void)
{
	//그리드 모델 추가
	ID3D11Device* device = DEVICEMANAGER.GetDevice();
	_gridLine.reset(GeoEntity::CreateGridLine(device, 50.f));
	 

	return S_OK;
}

void TestScene::Update(void)
{

}

void TestScene::Render(ID3D11DeviceContext* dc)
{	
	_gridLine->Render(dc);
	
	for (auto data : RM_MODEL.GetResource()) {
		data.second->Render(dc);
	}

}



