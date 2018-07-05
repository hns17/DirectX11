#include "stdafx.h"
#include "BaseModel.h"


//변수 초기화
BaseModel::BaseModel()
{
	_vertexCount = 0;
	_indexCount = 0;

	_indexBuffer = NULL;

	_isNormalize = false;
	_name = L"";

	_primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	_rotate		=	XMFLOAT3(0.f, 0.f, 0.f);									
	_center		=	XMFLOAT3(0.f, 0.f, 0.f);
	_size		=	XMFLOAT3(1.f, 1.f, 1.f);

	
	_shader = NULL; ;
	_scaleFactor = 1.f;								
	
}

//버퍼 메모리 해제
void BaseModel::Release()
{
	SAFE_RELEASE(_indexBuffer);
	
	SAFE_RELEASE(_vertexDataBuffer.positionBuffer);
	SAFE_RELEASE(_vertexDataBuffer.colorBuffer);
	SAFE_RELEASE(_vertexDataBuffer.texcoordBuffer);
	SAFE_RELEASE(_vertexDataBuffer.normalBuffer);
	SAFE_RELEASE(_vertexDataBuffer.tangentBuffer);
	SAFE_RELEASE(_vertexDataBuffer.bitangentBuffer);
}

bool BaseModel::CreateModel(ID3D11Device* device, Vertex& vertices, vector<unsigned long>& indices,	D3D11_PRIMITIVE_TOPOLOGY type)
{	

	//정점 정보 없으면 리턴
	if (vertices.position.empty()) { return false; }

	_primitiveType = type;
	float maxSize = 0.f;


	//Scale Factor 구하기
	//정점 위치 정보에서 가장 큰 값을 가져온다.
	for (auto vertex : vertices.position) {
		maxSize = XMMax(maxSize, abs(vertex.x));
		maxSize = XMMax(maxSize, abs(vertex.y));
		maxSize = XMMax(maxSize, abs(vertex.z));
	}

	_scaleFactor = 1.f / maxSize;

	//버텍스 버퍼 생성
	if (!InitVertextBuffers(device, vertices)) {
		_vertexCount = 0;
		return false;
	}

	//인덱스 정보가 있으면 인덱스 버퍼 생성
	if (!indices.empty()) {
		if (!InitIndexBuffers(device, indices)) {
			_indexCount = 0;
		}
	}

	

	return true;
}

//정점 버퍼 구성
bool BaseModel::InitVertextBuffers(ID3D11Device* device, Vertex &vertices, bool dynamicBuffer)
{
	_vertexCount = (UINT)vertices.position.size();

	//정점의 위치 정보가 없으면 반환
	if (vertices.position.empty()) {
		return false;
	}

	//위치 버퍼 생성
	if (!CreateBuffer(device, &_vertexDataBuffer.positionBuffer, vertices.position, dynamicBuffer)) {
		_vertexCount = 0;
		return false;
	}

	//색상 버퍼 생성
	CreateBuffer(device, &_vertexDataBuffer.colorBuffer, vertices.color, dynamicBuffer);			
	//UV 버퍼 생성
	CreateBuffer(device, &_vertexDataBuffer.texcoordBuffer, vertices.uv, dynamicBuffer);
	//법선 버퍼 생성
	CreateBuffer(device, &_vertexDataBuffer.normalBuffer, vertices.normal, dynamicBuffer);
	//탄젠트 버퍼 생성
	CreateBuffer(device, &_vertexDataBuffer.tangentBuffer, vertices.tangent, dynamicBuffer);
	//바이 탄젠트 버퍼 생성
	CreateBuffer(device, &_vertexDataBuffer.bitangentBuffer, vertices.bitangent, dynamicBuffer);
	//본 인덱스 버퍼 생성
	CreateBuffer(device, &_vertexDataBuffer.boneIdBuffer, vertices.boneidx, dynamicBuffer);
	//가중치 버퍼 생성
	CreateBuffer(device, &_vertexDataBuffer.weightBuffer, vertices.weight, dynamicBuffer);


	return true;
}

bool BaseModel::InitIndexBuffers(ID3D11Device* device, vector<unsigned long>& indices)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;
	_indexCount = (UINT)indices.size();

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

//정점 및 인덱스 버퍼 셋팅
void BaseModel::SetRenderBuffers(ID3D11DeviceContext* dc)
{
	unsigned int offset = 0;

	//정점 버퍼 셋팅
	for (unsigned int i = 0; i < _vertexBuffer.size(); i++) {
		dc->IASetVertexBuffers(i, 1, _vertexBuffer[i], &_vertexStride[i], &offset);
	}

	//인덱스 정보가 있으면 인덱스 버퍼 셋팅
	if (_indexBuffer) {
		dc->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	//모델의 렌더링 타입 설정
	dc->IASetPrimitiveTopology(_primitiveType);
}


//모델 행렬 재구성
XMMATRIX BaseModel::UpdateModelMatrix()
{
	//크기
	if (_isNormalize) {
		_tmModel = XMMatrixScaling(_size.x * _scaleFactor, _size.y * _scaleFactor, _size.z * _scaleFactor);
	}
	else {
		_tmModel = XMMatrixScaling(_size.x, _size.y, _size.z);
	}
	
	//회전
	_tmModel = XMMatrixMultiply(_tmModel, XMMatrixRotationX(_rotate.x));
	_tmModel = XMMatrixMultiply(_tmModel, XMMatrixRotationY(_rotate.y));
	_tmModel = XMMatrixMultiply(_tmModel, XMMatrixRotationZ(_rotate.z));

	//이동
	_tmModel = XMMatrixMultiply(_tmModel, XMMatrixTranslation(_center.x, _center.y, _center.z));
	
	return _tmModel;
}

//모델을 렌더링할 셰이더 셋팅
//셰이더 변경시 셰이더의 정점 레이아웃 정보를 바탕으로
//모델에서 사용될 정점 버퍼 정보를 재구성한다.
void BaseModel::SetShader(Shader * shader, bool buildLayoutInfo)
{	
	//셰이더 변경
	_shader = shader;

	if (!shader || !buildLayoutInfo) return;

	//셰이더의 정점 입력 정보를 가져온다.
	auto desc = shader->GetVertexLayoutDesc();

	//버퍼 초기화
	_vertexBuffer.clear();	//렌더링에 사용될 버텍스 버퍼
	_vertexStride.clear();	//각 버퍼에 담기는 데이터 크기
	
	//정점 레이아웃 정보를 바탕으로 정점 버퍼 재구성
	for (auto vertexItem : desc) {
		if (_stricmp(vertexItem.SemanticName, "POSITION") == 0) {
			_vertexBuffer.push_back(&_vertexDataBuffer.positionBuffer);
			_vertexStride.push_back(sizeof(XMFLOAT3));
		}
		else if (_stricmp(vertexItem.SemanticName, "COLOR") == 0) {
			_vertexBuffer.push_back(&_vertexDataBuffer.colorBuffer);
			_vertexStride.push_back(sizeof(XMFLOAT4));
		}
		else if (_stricmp(vertexItem.SemanticName, "TEXCOORD") == 0) {
			_vertexBuffer.push_back(&_vertexDataBuffer.texcoordBuffer);
			_vertexStride.push_back(sizeof(XMFLOAT2));
		}
		else if (_stricmp(vertexItem.SemanticName, "NORMAL") == 0) {
			_vertexBuffer.push_back(&_vertexDataBuffer.normalBuffer);
			_vertexStride.push_back(sizeof(XMFLOAT3));
		}
		else if (_stricmp(vertexItem.SemanticName, "TANGENT") == 0) {
			_vertexBuffer.push_back(&_vertexDataBuffer.tangentBuffer);
			_vertexStride.push_back(sizeof(XMFLOAT3));
		}
		else if (_stricmp(vertexItem.SemanticName, "BITANGENT") == 0) {
			_vertexBuffer.push_back(&_vertexDataBuffer.bitangentBuffer);
			_vertexStride.push_back(sizeof(XMFLOAT3));
		}
		else if (_stricmp(vertexItem.SemanticName, "BONEID") == 0) {
			_vertexBuffer.push_back(&_vertexDataBuffer.boneIdBuffer);
			_vertexStride.push_back(sizeof(XMUINT4));
		}
		else if (_stricmp(vertexItem.SemanticName, "WEIGHT") == 0) {
			_vertexBuffer.push_back(&_vertexDataBuffer.weightBuffer);
			_vertexStride.push_back(sizeof(XMFLOAT4));
		}
	}
}

//버퍼 생성
template<typename T>
bool BaseModel::CreateBuffer(ID3D11Device* device, ID3D11Buffer** buffer, vector<T>& data, bool dynamicBuffer)
{
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA bufferData;

	if (data.empty()) {
		return false;
	}

	//버퍼정보 구성(사용 유형, 크기, 기타)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = (UINT)(sizeof(T) * data.size());
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	if (dynamicBuffer) {
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}


	//Set Vertex Data
	bufferData.pSysMem = &data[0];
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;
	
	if (FAILED(device->CreateBuffer(&bufferDesc, &bufferData, buffer))) {
		return false;
	};
	

	return true;
}
