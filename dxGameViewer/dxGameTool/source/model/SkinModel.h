//====================================================================================
//		## SkinModel ## (하드웨어 스키닝 모델 클레스)
//====================================================================================
#pragma once

#include "BaseModel.h"
#include "Animation.h"


class SkinModel : public BaseModel
{
public:
	SkinModel();
	virtual ~SkinModel();

	void					Release();
	void					Render(ID3D11DeviceContext* dc);
	
	//Animation Function
	void					PlayAni(int idx);
	void					StopAni()			{	_aniList[_playAniIdx].Stop();	}
	void					PauseAni()			{	_aniList[_playAniIdx].Pause();	}


	//마테리얼 단위 메쉬정보 업데이트
	void					UpdateMeshByMaterial();


	//Get Function
	vector<HierarchyMesh*>&	GetMeshList()		{	return _meshList;				}
	vector<NodeInfo*>&		GetNodeList()		{	return _nodeList;				}

	vector<Material>&		GetMaterialList()	{	return _materialList;			}
	vector<Animation>&		GetAnimationList()	{	return _aniList;				}

protected:
	void UpdateNodeTM();

protected:
	Shader*					_texShader		= NULL;
	Shader*					_skinShader		= NULL;
	int						_playAniIdx		= -1;

	vector<Material>		_materialList;
	vector<Animation>		_aniList;
	vector<NodeInfo*>		_nodeList;
	vector<HierarchyMesh*>	_meshList;

	//마테리얼 단위 렌더링을 위한 변수, 편의상 메쉬 정보와 분리 해 둠.
	vector<vector<HierarchyMesh*>> _meshByMaterial;
};
