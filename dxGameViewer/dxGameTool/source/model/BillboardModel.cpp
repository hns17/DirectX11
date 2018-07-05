#include "stdafx.h"
#include "BillboardModel.h"

void BillboardModel::Update()
{
	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, _mainCam.GetPosition());

	
	float angle = atan2(_center.x - camPos.x, _center.z - camPos.z) * (180.f / XM_PI); 
	_rotate.y = angle * ONE_RADIAN;

}

void BillboardModel::Render(ID3D11DeviceContext * dc)
{
	Update();
	SimpleTextureModel::Render(dc);
}
