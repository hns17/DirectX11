#pragma once

#include "SimpleTextureModel.h"

class BillboardModel : public SimpleTextureModel
{
public:
	BillboardModel() {};
	~BillboardModel() {};

	void Update();
	void Render(ID3D11DeviceContext* dc);
	
};

