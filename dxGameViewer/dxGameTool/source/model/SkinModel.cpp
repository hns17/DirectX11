#include "stdafx.h"
#include "SkinModel.h"

SkinModel::SkinModel()
{
	_texShader	= RM_SHADER.AddResource(L"source\\shader\\texture");
	_skinShader = RM_SHADER.AddResource(L"source\\shader\\skin");
	SetShader(_skinShader);
}

SkinModel::~SkinModel()
{
}

void SkinModel::Release()
{
	for (auto item : _meshList)
		SAFE_DELETE(item);

	for (auto item : _nodeList)
		SAFE_DELETE(item);

	vector<NodeInfo*>().swap(_nodeList);
	vector<HierarchyMesh*>().swap(_meshList);


	BaseModel::Release();


}


void SkinModel::Render(ID3D11DeviceContext * dc)
{
	//메쉬 정보가 없으면 리턴
	if (_meshList.empty()) 	return;

	//렌더링을 위한 버퍼셋팅
	SetRenderBuffers(dc);
		
	UpdateModelMatrix();
	UpdateNodeTM();

	for (auto meshData : _meshList) {
		if (!meshData->isDraw) continue;

		auto hiMesh = (HierarchyMesh*)meshData;
		int matIdx = hiMesh->matIdx;
		
		
		//본 정보 셋팅
		if (!hiMesh->boneList.empty()) {
			SetShader(_skinShader, false);
			vector<XMMATRIX> calcBoneList;
			for (auto boneInfo : hiMesh->boneList)
				calcBoneList.emplace_back(XMMatrixTranspose(boneInfo.matOffset * boneInfo.linkNode->worldTM));

			RM_SHADER.SetShaderParameters(dc, calcBoneList);
		}
		else {
			SetShader(_texShader, false);

			if (hiMesh->linkNode)
				_tmModel = hiMesh->linkNode->worldTM * _tmModel;
		}

		RM_SHADER.SetShaderParameters(dc, _tmModel);

		//마테리얼 셋팅
		if (matIdx > -1) {
			auto texData = _materialList[matIdx].diffuseMap;
		
			if (texData)
				dc->PSSetShaderResources(0, 1, &texData->data);
		}
		else {
			dc->PSSetShaderResources(0, 1, NULL);
		}

		//렌더
		GetShader()->IndexRender(dc, meshData->count, meshData->start);
	}
}

void SkinModel::InsertMaterial(Material& newMat)
{
	_materialList.emplace_back(newMat);
}

void SkinModel::PlayAni(int idx)
{
	if (_playAniIdx != idx) {
		_aniList[idx].Stop();
		_playAniIdx = idx;
	}

	_aniList[idx].Play();
}


void SkinModel::UpdateNodeTM()
{
	if (_aniList.empty())
		return;


	if(_playAniIdx >= 0)
		_aniList[_playAniIdx].UpdateAnimation(TIMEMANAGER.GetElapedTime());

	for (auto& nodeItem : _nodeList) {
		XMMATRIX tm = nodeItem->localTM;
		
		if (_playAniIdx >= 0)
			_aniList[_playAniIdx].GetAniTM(nodeItem->name, tm);

		nodeItem->worldTM = tm;
		if (nodeItem->parent)
			nodeItem->worldTM = tm * nodeItem->parent->worldTM;
	}
}
