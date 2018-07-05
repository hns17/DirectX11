//====================================================================================
//		## BasicModel ## (정점의 위치정보와 정점 색상 정보만 렌더링)
//====================================================================================
#pragma once
#include "BaseModel.h"

class BasicModel : public BaseModel
{
public:
	BasicModel();
	~BasicModel() {};

	virtual void Render(ID3D11DeviceContext* dc);
};
