////////////////////////////////////////////////////////////////////////////////
// Filename: Model.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Model.h"


Model::Model()
{
	_vertexBuffer	= NULL;
	_indexBuffer	= NULL;

	_name			= L"";

	_indexCount		= 0;
	_vertexCount	= 0;
	_scaleFactor	= 1;

	_center			= XMFLOAT3(0.f, 0.f, 0.f);
	_rotate			= XMFLOAT3(0.f, 0.f, 0.f);
	_rotVelocity	= XMFLOAT3(0.f, 0.f, 0.f);
	_size			= XMFLOAT3(1.f, 1.f, 1.f);

	_accVel			= XMVectorZero();
	_isNormalize	= false;



	_shader = SHADERMANAGER->addResource(L"shader\\color");
	_primitiveType	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
}



Model::~Model()
{
	Release();
}

//모델 생성
bool Model::CreateModel(ID3D11Device* device, vector<Vertex> vertices, vector<unsigned long> indices)
{
	bool result;

	if (vertices.empty()) { return false; }
	
	result = InitVertextBuffers(device, vertices);
	if(!result)	{	
		_vertexCount = 0;
		return false;	
	}

	if (!indices.empty()) {

		if (!InitIndexBuffers(device, indices)) {
			_indexCount = 0;
		}
	}

	return true;
}

void Model::Render(ID3D11DeviceContext* deviceContext, PWindowRender pWR)
{
	
	//모델에 대한 정보구성
	RenderBuffers(deviceContext);
									
	vector<Node*>::iterator iter = _nodeList.begin();
	for (; iter != _nodeList.end(); iter++) {
		RenderNode(deviceContext, pWR, *iter);
	}
	
	return;
}

void Model::Release()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexBuffer);
	

	vector<Material*>::iterator matIter = _matList.begin();
	for (; matIter != _matList.end(); matIter++) {
		SAFE_DELETE(*matIter);
	}
	vector<Material*>().swap(_matList);


	vector<Node*>::iterator nodeIter = _nodeList.begin();
	for (; nodeIter != _nodeList.end(); nodeIter++) {
		SAFE_DELETE(*nodeIter);
	}
	vector<Node*>().swap(_nodeList);
}


int Model::GetIndexCount()
{
	return _indexCount;
}

int Model::GetVertexCount()
{
	return _vertexCount;
}

void Model::SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY type)
{
	_primitiveType = type;
}

void Model::SetRotate(XMFLOAT3 rotate)
{
	_rotate = rotate;
}

void Model::SetCenter(XMFLOAT3 center)
{
	_center = center;
}

void Model::SetSize(XMFLOAT3 size)
{
	_size = size;
}

void Model::SetRotVelocity(XMFLOAT3 rot)
{
	_rotVelocity = rot;
}

void Model::SetShader(wstring path)
{
	_shader = SHADERMANAGER->addResource(path);
}

void Model::SetName(wstring name)
{
	_name = name;
}

void Model::SetScaleFactor(float scaleFactor)
{
	_scaleFactor = scaleFactor;
}

void Model::SetNormalize(bool isNormalize)
{
	_isNormalize = isNormalize;
}

bool Model::IsNormalize()
{
	return _isNormalize;
}

void Model::InsertNode(Node* node)
{
	if (!node) { return; }

	Node* parrentNode = node->parrentNode;

	//최상위 노드면 노드리스트에 추가.
	if (!parrentNode) {
		_nodeList.push_back(node);
	}
	//부모 노드가 있으면 부모의 자식 노드에 추가
	else {
		parrentNode->childNode.push_back(node);
	}

	
}

void Model::InsertMaterial(Material * mat)
{
	_matList.push_back(mat);
}


float Model::GetScaleFactor()
{
	return _scaleFactor;
}

wstring Model::GetName()
{
	return _name;
}

XMFLOAT3 Model::GetRotate()
{
	return _rotate;
}

XMFLOAT3 Model::GetCenter()
{
	return _center;
}

XMFLOAT3 Model::GetRotVelocity()
{
	return _rotVelocity;
}

vector<Node*>& Model::GetNodeList()
{
	return _nodeList;
}

vector<Material*>& Model::GetMaterialList()
{
	return _matList;
}


XMFLOAT3 Model::GetSize()
{
	return _size;
}

//버텍스 버퍼 만들기
bool Model::InitVertextBuffers(ID3D11Device * device, vector<Vertex> vertices)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	HRESULT result;

	_vertexCount = vertices.size();


	//버퍼정보 구성(사용 유형, 크기, 기타)
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;						
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * _vertexCount;		
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Set Vertex Data
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result)) { return false; }

	
	return true;
}

//인덱스 버퍼 만들기
bool Model::InitIndexBuffers(ID3D11Device * device, vector<unsigned long> indices)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;
	_indexCount = indices.size();

	//버퍼정보 구성(사용 유형, 크기, 기타)
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Set Index Data
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result)) { return false; }

	

	return true;
}



//모델(오브젝트) 그리기 위한 셋팅
void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//모델에 대한 인덱스 버텍스 버퍼 셋, 고맙게도 영어로 주석이 달려있다.

	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	if (_indexBuffer) {
		deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}
	

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(_primitiveType);

	return;
}

void Model::RenderNode(ID3D11DeviceContext* deviceContext, PWindowRender pWR, Node * node)
{
	Mesh* meshData = node->nodeData;

	ID3D11ShaderResourceView* tex = NULL;

	if (!meshData->isDraw) { return; }



	vector<Node*>::iterator iter = node->childNode.begin();
	for (; iter != node->childNode.end(); iter++) {
		RenderNode(deviceContext, pWR, *iter);
	}


	XMMATRIX modelMatrix, worldMatrix, projectionMatrix, viewMatrix;

	_accVel += XMLoadFloat3(&_rotVelocity);

	XMFLOAT3 resRot;
	XMStoreFloat3(&resRot, _accVel + XMLoadFloat3(&_rotate));


	//크기
	if (_isNormalize) {
		modelMatrix = XMMatrixScaling(_size.x * _scaleFactor, _size.y * _scaleFactor, _size.z * _scaleFactor);
	}
	else {
		modelMatrix = XMMatrixScaling(_size.x, _size.y, _size.z);
	}


	//회전
	modelMatrix = XMMatrixMultiply(modelMatrix, XMMatrixRotationX(resRot.x));
	modelMatrix = XMMatrixMultiply(modelMatrix, XMMatrixRotationY(resRot.y));
	modelMatrix = XMMatrixMultiply(modelMatrix, XMMatrixRotationZ(resRot.z));

	//이동
	modelMatrix = XMMatrixMultiply(modelMatrix, XMMatrixTranslation(_center.x, _center.y, _center.z));

	viewMatrix = pWR->viewMatrix;
	projectionMatrix = pWR->projectionMatrix;
	worldMatrix = pWR->worldMatrix;

	if (_indexCount != 0) {
		_shader->IndexRender(deviceContext, meshData->count, meshData->start, modelMatrix, worldMatrix, viewMatrix, projectionMatrix, tex);
	}
	else {
		_shader->VertexRender(deviceContext, meshData->count, meshData->start, modelMatrix, worldMatrix, viewMatrix, projectionMatrix, tex);
	}
}
