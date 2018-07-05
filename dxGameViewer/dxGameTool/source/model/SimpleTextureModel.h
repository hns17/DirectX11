//====================================================================================
//		## SimpleTextureModel ## (단일 텍스쳐를 렌더링하는 Square Model)
//====================================================================================
#pragma once
#include "BasicModel.h"

class SimpleTextureModel : public BasicModel
{
public:
	SimpleTextureModel();
	~SimpleTextureModel() {};

	virtual void Render(ID3D11DeviceContext* dc);
	
	void SetTexture(ID3D11ShaderResourceView* texture);
	ID3D11ShaderResourceView* GetTexture();

protected:
	ID3D11ShaderResourceView* _tex = NULL;
};
