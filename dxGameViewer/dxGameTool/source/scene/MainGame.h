//==================================================================
//		## MainGame ## (Process Game)
//==================================================================

#pragma once
#include "GameNode.h"
#include "TestScene.h"


class MainGame : public GameNode
{
public:
	HRESULT Init();
	HRESULT	InitScene();

	void Update();
	void Render(ID3D11DeviceContext* dc);


	MainGame() {}
	~MainGame() {}

};
