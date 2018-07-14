#include "stdafx.h"
#include "RMModel.h"


//모델을 로드한다.
SkinModel * RMModel::loadResource(wstring fileName, void * param)
{
	unsigned int flag = 0;

	//파라미터가 없을 때 기본 플래그
	if (!param) {
		flag =  aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
	}
	else {
		flag =  *(unsigned int*)param;
	}
	

	//모델로드
	ModelLoader load;
	return load.LoadModel(fileName.c_str(), flag);
}

//모델 삭제
void RMModel::releaseResource(SkinModel * data)
{
	SAFE_RELEASE(data);
}
