//=============================================================
//	## Texture Manager ## (로드된 텍스쳐 정보를 관리한다.)
//=============================================================
#pragma once
#include "ResourceManager.h"

//Texture Info
struct Texture {
	wstring name					= L"";
	ID3D11ShaderResourceView* data	= NULL;

	~Texture() {
		SAFE_RELEASE(data);
	}
};

class RMTexture : public ResourceManager<Texture*, RMTexture>
{
public:
	RMTexture() {};
	~RMTexture() {};

	//Load Texture
	ID3D11ShaderResourceView* LoadTexture(wstring fileName);

protected:
	//Override
	Texture * loadResource(wstring fileName, void* param = NULL);
	void releaseResource(Texture* data);
};
