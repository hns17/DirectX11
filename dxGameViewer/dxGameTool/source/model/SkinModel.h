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
	
	void					InsertMaterial(Material& newMat);

	void					PlayAni(int idx);

	void					StopAni()			{	_aniList[_playAniIdx].Stop();	}
	void					PauseAni()			{	_aniList[_playAniIdx].Pause();	}


	vector<HierarchyMesh*>&	GetMeshList()		{	return _meshList;				}
	vector<NodeInfo*>&		GetNodeList()		{	return _nodeList;				}

	vector<Material>&		GetMaterialList()	{	return _materialList;			}
	vector<Animation>&		GetAnimationList()	{	return _aniList;				}

protected:
	void UpdateNodeTM();

protected:
	vector<Material>		_materialList;
	vector<NodeInfo*>		_nodeList;
	vector<Animation>		_aniList;
	vector<HierarchyMesh*>	_meshList;

	Shader*					_texShader		= NULL;
	Shader*					_skinShader		= NULL;
	int						_playAniIdx		= -1;


};
