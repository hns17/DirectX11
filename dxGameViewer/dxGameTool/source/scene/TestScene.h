#pragma once
#include "gameNode.h"
#include "model\GeoEntity.h"

class TestScene : public GameNode
{	

private:
	unique_ptr<BaseModel>		_gridLine;

public:
	HRESULT Init(void);
	void Update(void);
	void Render(ID3D11DeviceContext* dc);
	
	TestScene() {}
	~TestScene() {}
};

