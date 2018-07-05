//=============================================================
//	## RMModel ## (로드된 모델링 파일 정보를 관리한다.)
//=============================================================
#pragma once

#include "ResourceManager.h"
#include "model/SkinModel.h"
#include "util/loader/ModelLoader.h"

class RMModel : public ResourceManager<SkinModel*, RMModel>
{
public:
	RMModel() {};
	~RMModel() {};

	//모델 로드
	SkinModel* loadResource(wstring fileName, void* param = NULL);

	//모델 삭제
	void releaseResource(SkinModel* data);
};
