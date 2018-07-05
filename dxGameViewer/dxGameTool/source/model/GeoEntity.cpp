#include "stdafx.h"
#include "GeoEntity.h"

//해당 모델 만들어서 반환해주는 함수들...
BaseModel* GeoEntity::CreateTriangle(ID3D11Device* device, BaseModel* model)
{

	Vertex					vertices;
	vector<unsigned long>	indices;

	vertices.position.emplace_back(XMFLOAT3(-1.0f, -1.0f, 0.0f));
	vertices.position.emplace_back(XMFLOAT3(0.0f, 1.0f, 0.0f));
	vertices.position.emplace_back(XMFLOAT3(1.0f, -1.0f, 0.0f));

	vertices.color.emplace_back(XMFLOAT4(1.f, 0.f, 0.f, 1.f));
	vertices.color.emplace_back(XMFLOAT4(1.f, 0.f, 0.f, 1.f));
	vertices.color.emplace_back(XMFLOAT4(1.f, 0.f, 0.f, 1.f));

	vertices.uv.emplace_back(XMFLOAT2(0.f, 1.f));
	vertices.uv.emplace_back(XMFLOAT2(0.5f, 0.f));
	vertices.uv.emplace_back(XMFLOAT2(1.f, 1.f));


	// Load the index array with data.
	indices.emplace_back(unsigned long(0));
	indices.emplace_back(unsigned long(1));
	indices.emplace_back(unsigned long(2));

	BaseModel* pModel = model;

	if (!pModel) {
		pModel = new BasicModel;
	}


	if (!pModel->CreateModel(device, vertices, indices)) {
		SAFE_DELETE(pModel);
	}

	return pModel;
}

BaseModel* GeoEntity::CreateSquare(ID3D11Device* device, BaseModel* model)
{

	Vertex					vertices;
	vector<unsigned long>	indices;


	vertices.position.emplace_back(XMFLOAT3(-1.0f, -1.0f, 0.0f));
	vertices.position.emplace_back(XMFLOAT3(-1.0f, 1.0f, 0.0f));
	vertices.position.emplace_back(XMFLOAT3(1.0f, 1.0f, 0.0f));
	vertices.position.emplace_back(XMFLOAT3(1.0f, -1.0f, 0.0f));
	
	vertices.color.emplace_back(XMFLOAT4(1.f, 0.f, 0.f, 1.f));
	vertices.color.emplace_back(XMFLOAT4(0.f, 1.f, 0.f, 1.f));
	vertices.color.emplace_back(XMFLOAT4(0.f, 0.f, 1.f, 1.f));
	vertices.color.emplace_back(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f));

	vertices.uv.emplace_back(XMFLOAT2(0.f, 1.f));
	vertices.uv.emplace_back(XMFLOAT2(0.f, 0.f));
	vertices.uv.emplace_back(XMFLOAT2(1.f, 0.f));
	vertices.uv.emplace_back(XMFLOAT2(1.f, 1.f));

	// Load the index array with data.
	indices.push_back(unsigned long(0));
	indices.push_back(unsigned long(1));
	indices.push_back(unsigned long(2));

	indices.push_back(unsigned long(2));
	indices.push_back(unsigned long(3));
	indices.push_back(unsigned long(0));

	BaseModel* pModel = model;

	if (!pModel) {
		pModel = new BasicModel;
	}


	if (!pModel->CreateModel(device, vertices, indices)) {
		SAFE_DELETE(pModel);
	}

	return pModel;
}

BaseModel* GeoEntity::CreateCube(ID3D11Device* device, BaseModel* model)
{
	Vertex						vertices;
	vector<unsigned long>		indices;

	vertices.position.emplace_back(XMFLOAT3(-1.0, 1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, 1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, -1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, -1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, 1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, -1.0, -1.0));

	vertices.uv.emplace_back(XMFLOAT2(0.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 1.0));


	vertices.position.emplace_back(XMFLOAT3(1.0, 1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, 1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, -1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, -1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, 1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, -1.0, 1.0));

	vertices.uv.emplace_back(XMFLOAT2(0.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 1.0));


	vertices.position.emplace_back(XMFLOAT3(1.0, 1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, 1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, -1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, -1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, 1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, -1.0, 1.0));

	vertices.uv.emplace_back(XMFLOAT2(0.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 1.0));


	vertices.position.emplace_back(XMFLOAT3(-1.0, 1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, 1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, -1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, -1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, 1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, -1.0, -1.0));

	vertices.uv.emplace_back(XMFLOAT2(0.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 1.0));



	vertices.position.emplace_back(XMFLOAT3(-1.0, 1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, 1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, 1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, 1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, 1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, 1.0, -1.0));

	vertices.uv.emplace_back(XMFLOAT2(0.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 1.0));


	vertices.position.emplace_back(XMFLOAT3(-1.0, -1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, -1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, -1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(-1.0, -1.0, 1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, -1.0, -1.0));
	vertices.position.emplace_back(XMFLOAT3(1.0, -1.0, 1.0));

	vertices.uv.emplace_back(XMFLOAT2(0.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(0.0, 1.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 0.0));
	vertices.uv.emplace_back(XMFLOAT2(1.0, 1.0));


	BaseModel* pModel = model;
	if (!pModel) {
		pModel = new BasicModel;
	}


	if (!pModel->CreateModel(device, vertices, indices)) {
		SAFE_DELETE(pModel);
	}

	return pModel;
}



BaseModel* GeoEntity::CreateGridLine(ID3D11Device* device, float radius, BaseModel* model)
{
	Vertex						vertices;
	vector<unsigned long>		indices;

	//x, y, z Arrow
	//X Arrow, Vertex Cnt : 6
	vertices.position.emplace_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	vertices.position.emplace_back(XMFLOAT3(-1.0f, 0.0f, 0.0f));
	vertices.position.emplace_back(XMFLOAT3(-1.0f, 0.1f, 0.0f));
	vertices.position.emplace_back(XMFLOAT3(-1.1f, -0.1f, 0.0f));
	vertices.position.emplace_back(XMFLOAT3(-1.1f, 0.1f, 0.0f));
	vertices.position.emplace_back(XMFLOAT3(-1.0f, -0.1f, 0.0f));


	for (unsigned int i = 0; i < 6; i++) {
		vertices.color.emplace_back((XMFLOAT4(1.f, 0.f, 0.f, 1.f)));
	}

	//Y Arrow, Vertex Cnt : 8
	vertices.position.emplace_back(XMFLOAT3(0.0f, 0.0f, 0.f));
	vertices.position.emplace_back(XMFLOAT3(0.0f, 1.0f, 0.f));
	vertices.position.emplace_back(XMFLOAT3(-0.05f, 1.25f, 0.f));
	vertices.position.emplace_back(XMFLOAT3(0.0f, 1.15f, 0.f));
	vertices.position.emplace_back(XMFLOAT3(0.05f, 1.25f, 0.f));
	vertices.position.emplace_back(XMFLOAT3(0.0f, 1.15f, 0.f));
	vertices.position.emplace_back(XMFLOAT3(0.0f, 1.15f, 0.f));
	vertices.position.emplace_back(XMFLOAT3(0.0f, 1.05f, 0.f));

	for (unsigned int i = 0; i < 8; i++) {
		vertices.color.emplace_back((XMFLOAT4(0.f, 1.f, 0.f, 1.f)));
	}

	//Z Arrow, Vertex Cnt : 8
	vertices.position.emplace_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	vertices.position.emplace_back(XMFLOAT3(0.0f, 0.0f, 1.0f));
	vertices.position.emplace_back(XMFLOAT3(-0.05f, 0.1f, 1.05f));
	vertices.position.emplace_back(XMFLOAT3(0.05f, 0.1f, 1.05f));
	vertices.position.emplace_back(XMFLOAT3(0.05f, 0.1f, 1.05f));
	vertices.position.emplace_back(XMFLOAT3(-0.05f, -0.1f, 1.05f));
	vertices.position.emplace_back(XMFLOAT3(-0.05f, -0.1f, 1.05f));
	vertices.position.emplace_back(XMFLOAT3(0.05f, -0.1f, 1.05f));	

	for (unsigned int i = 0; i < 8; i++) {
		vertices.color.emplace_back((XMFLOAT4(0.f, 0.f, 1.f, 1.f)));
	}

	//GridLine Data
	for (float i = -radius; i <= radius; i += 1.0f) {
		vertices.position.emplace_back(XMFLOAT3(i, 0.0f, -radius));
		vertices.position.emplace_back(XMFLOAT3(i, 0.0f, radius));
		vertices.position.emplace_back(XMFLOAT3(-radius, 0.0f, i));
		vertices.position.emplace_back(XMFLOAT3(radius, 0.0f, i));

		vertices.color.emplace_back((XMFLOAT4(1.f, 1.f, 1.f, 1.f)));
		vertices.color.emplace_back((XMFLOAT4(1.f, 1.f, 1.f, 1.f)));
		vertices.color.emplace_back((XMFLOAT4(1.f, 1.f, 1.f, 1.f)));
		vertices.color.emplace_back((XMFLOAT4(1.f, 1.f, 1.f, 1.f)));
	}


	BaseModel* pModel = model;

	if (!pModel) {
		pModel = new BasicModel;
	}


	if (!pModel->CreateModel(device, vertices, indices)) {
		SAFE_DELETE(pModel);
	}

	pModel->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	return pModel;
}

BaseModel* GeoEntity::CreateIcoHedronSphere(ID3D11Device* device,  int Lod, BaseModel* model)
{
	Vertex					vertices;
	vector<unsigned long>	indices;
	
	GenerateVertexSphere(vertices, indices, Lod);
		
	BaseModel* pModel = model;
	
	if (!pModel) {
		pModel = new BasicModel;
	}


	if (!pModel->CreateModel(device, vertices, indices)) {
		SAFE_DELETE(pModel);
	}
	
	return pModel;
}

BaseModel * GeoEntity::CreateCapsule(ID3D11Device * device, XMFLOAT4 color, BaseModel * model)
{
	Vertex						vertices;
	vector<unsigned long>		indices;


	GenerateVertexCapsule(vertices, indices, color);


	BaseModel* pModel = model;
	if (!pModel) {
		pModel = new BasicModel;
	}


	if (!pModel->CreateModel(device, vertices, indices)) {
		SAFE_DELETE(pModel);
	}

	//pModel->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return pModel;
}







void GeoEntity::GenerateVertexCube(Vertex & vertices, vector<unsigned long>& indices, XMFLOAT3 center, XMFLOAT3 size, XMFLOAT4 color)
{
	UINT offset = (UINT)vertices.position.size();

	XMFLOAT3 radius = XMFLOAT3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);

	//X Axis
	vertices.position.emplace_back(XMFLOAT3(center.x - radius.x, center.y + radius.y, center.z + radius.z));
	vertices.position.emplace_back(XMFLOAT3(center.x + radius.x, center.y + radius.y, center.z + radius.z));
	vertices.position.emplace_back(XMFLOAT3(center.x + radius.x, center.y - radius.y, center.z + radius.z));
	vertices.position.emplace_back(XMFLOAT3(center.x - radius.x, center.y - radius.y, center.z + radius.z));

	vertices.position.emplace_back(XMFLOAT3(center.x - radius.x, center.y + radius.y, center.z - radius.z));
	vertices.position.emplace_back(XMFLOAT3(center.x + radius.x, center.y + radius.y, center.z - radius.z));
	vertices.position.emplace_back(XMFLOAT3(center.x + radius.x, center.y - radius.y, center.z - radius.z));
	vertices.position.emplace_back(XMFLOAT3(center.x - radius.x, center.y - radius.y, center.z - radius.z));


	for (int i = 0; i < 8; i++) {
		vertices.color.push_back(color);
	}


	//BACK
	indices.emplace_back(0 + offset);	indices.emplace_back(1 + offset);	indices.emplace_back(3 + offset);
	indices.emplace_back(1 + offset);	indices.emplace_back(2 + offset);	indices.emplace_back(3 + offset);

	//FRONT				 
	indices.emplace_back(4 + offset);	indices.emplace_back(5 + offset);	indices.emplace_back(7 + offset);
	indices.emplace_back(5 + offset);	indices.emplace_back(6 + offset);	indices.emplace_back(7 + offset);

	//TOP				 
	indices.emplace_back(0 + offset);	indices.emplace_back(1 + offset);	indices.emplace_back(4 + offset);
	indices.emplace_back(1 + offset);	indices.emplace_back(5 + offset);	indices.emplace_back(4 + offset);

	//BOTTOM			 
	indices.emplace_back(3 + offset);	indices.emplace_back(2 + offset);	indices.emplace_back(7 + offset);
	indices.emplace_back(2 + offset);	indices.emplace_back(6 + offset);	indices.emplace_back(7 + offset);

	//LEFT				 
	indices.emplace_back(0 + offset);	indices.emplace_back(4 + offset);	indices.emplace_back(3 + offset);
	indices.emplace_back(4 + offset);	indices.emplace_back(7 + offset);	indices.emplace_back(3 + offset);

	//RIGHT				 
	indices.emplace_back(1 + offset);	indices.emplace_back(5 + offset);	indices.emplace_back(2 + offset);
	indices.emplace_back(5 + offset);	indices.emplace_back(6 + offset);	indices.emplace_back(2 + offset);
}

void GeoEntity::GenerateVertexCone(Vertex & vertices, vector<unsigned long>& indices, XMFLOAT3 center, XMFLOAT3 degree, XMFLOAT3 size, XMFLOAT4 color)
{
	UINT offset = (UINT)vertices.position.size();

	vertices.position.emplace_back(XMFLOAT3(-1.f, 0.f, 1.f));
	vertices.position.emplace_back(XMFLOAT3(1.f, 0.f, 1.f));
	vertices.position.emplace_back(XMFLOAT3(-1.f, 0.f, -1.f));
	vertices.position.emplace_back(XMFLOAT3(1.f, 0.f, -1.f));
	vertices.position.emplace_back(XMFLOAT3(0.f, 1.f, 0.f));


	//크기
	XMMATRIX tm = XMMatrixScaling(size.x, size.y, size.z);

	//회전
	tm = XMMatrixMultiply(tm, XMMatrixRotationX(XMConvertToRadians(degree.x)));
	tm = XMMatrixMultiply(tm, XMMatrixRotationY(XMConvertToRadians(degree.y)));
	tm = XMMatrixMultiply(tm, XMMatrixRotationZ(XMConvertToRadians(degree.z)));

	//이동
	tm = XMMatrixMultiply(tm, XMMatrixTranslation(center.x, center.y, center.z));

	for (UINT i = offset; i < offset + 5; i++) {
		XMVECTOR vert = XMLoadFloat3(&vertices.position[i]);
		vert = XMVector3Transform(vert, tm);

		XMStoreFloat3(&vertices.position[i], vert);
	}


	for (int i = 0; i < 4; i++) {
		vertices.color.emplace_back(XMFLOAT4(color.x * 0.2f, color.y* 0.2f, color.z* 0.2f, 1.f));
	}
	vertices.color.emplace_back(color);

	indices.emplace_back(0 + offset);	indices.emplace_back(1 + offset);	indices.emplace_back(2 + offset);
	indices.emplace_back(1 + offset);	indices.emplace_back(3 + offset);	indices.emplace_back(2 + offset);

	indices.emplace_back(4 + offset);	indices.emplace_back(0 + offset);	indices.emplace_back(1 + offset);
	indices.emplace_back(4 + offset);	indices.emplace_back(1 + offset);	indices.emplace_back(3 + offset);

	indices.emplace_back(4 + offset);	indices.emplace_back(3 + offset);	indices.emplace_back(2 + offset);
	indices.emplace_back(4 + offset);	indices.emplace_back(2 + offset);	indices.emplace_back(0 + offset);
}

void GeoEntity::GenerateVertexCircle(Vertex & vertices, XMFLOAT4 & color, XMFLOAT3 degree)
{
	int segment = 36;
	float intervalAngle = (3.141592f * 2.0f) / (float)segment;

	vertices.position.reserve(segment);
	vertices.color.reserve(segment);

	XMFLOAT3 rotAngle(XMConvertToRadians(degree.x), XMConvertToRadians(degree.y), XMConvertToRadians(degree.z));

	for (int i = 0; i <= 36; i++)
	{
		float angle = i * intervalAngle;
		vertices.position.emplace_back(XMFLOAT3(cosf(angle), sinf(angle), 0.0f));

		XMVECTOR rot = XMQuaternionRotationRollPitchYaw(rotAngle.x, rotAngle.y, rotAngle.z);
		XMVECTOR vertex = XMVector3Rotate(XMLoadFloat3(&vertices.position[i]), rot);
		XMStoreFloat3(&vertices.position[i], XMVector3Normalize(vertex));

		vertices.color.push_back(color);
	}
}

void GeoEntity::GenerateVertexCapsule(Vertex & vertices, vector<unsigned long>& indices, XMFLOAT4 color)
{
	UINT N = 32; // Mesh resolution
	float radius = 1;
	float height = 2;

	UINT index = 0;
	float theta, phi;
	
	unsigned int size = (N + 1)*(N / 2 + 2);
	vertices.position.resize(size);
	vertices.normal.resize(size);
	vertices.color.resize(size);
	vertices.uv.resize(size);


	for (UINT j = 0; j <= N / 4; j++) { // top cap
		for (UINT i = 0; i <= N; i++) {
			theta = i * XM_2PI / N;
			phi = -XM_PIDIV2 + XM_PI * j / (N / 2);
			vertices.position[index].x = radius * cos(phi) * cos(theta);
			vertices.position[index].y = radius * cos(phi) * sin(theta);
			vertices.position[index].z = radius * sin(phi);
			vertices.normal[index].x = vertices.position[index].x;
			vertices.normal[index].y = vertices.position[index].y;
			vertices.normal[index].z = vertices.position[index].z;
			vertices.position[index].z -= height / 2;

			index++;
		}
	}


	for (UINT j = N / 4; j <= N / 2; j++) { // bottom cap
		for (UINT i = 0; i <= N; i++) {
			theta = i * XM_2PI / N;
			phi = -XM_PIDIV2 +XM_PI * j / (N / 2);
			vertices.position[index].x = radius * cos(phi) * cos(theta);
			vertices.position[index].y = radius * cos(phi) * sin(theta);
			vertices.position[index].z = radius * sin(phi);
			vertices.normal[index].x = vertices.position[index].x;
			vertices.normal[index].y = vertices.position[index].y;
			vertices.normal[index].z = vertices.position[index].z;
			vertices.position[index].z += height * 0.5f;
			index++;
		}
	}

	// Calculate texture coordinates
	for (UINT i = 0; i<index; i++) {
		vertices.uv[i].x = atan2(vertices.position[i].y, vertices.position[i].x) / XM_2PI;
		if (vertices.uv[i].x < 0)
			vertices.uv[i].x = 1 + vertices.uv[i].x;
		vertices.uv[i].y = 0.5f + atan2(vertices.position[i].z, sqrt(vertices.position[i].x*vertices.position[i].x + vertices.position[i].y*vertices.position[i].y)) / XM_PI;
	}

	for (UINT j = 0; j <= N / 2; j++) {
		for (UINT i = 0; i< N; i++) {
			int i1 = j * (N + 1) + i + 1;
			int i2 = j * (N + 1) + (i + 1) + 1;
			int i3 = (j + 1) * (N + 1) + (i + 1) + 1;
			int i4 = (j + 1) * (N + 1) + i + 1;

			indices.push_back(i1);			indices.push_back(i2);			indices.push_back(i3);
			indices.push_back(i1);			indices.push_back(i3);			indices.push_back(i4);
		}
	}

	//color
	for (UINT i = 0; i < size; i++) {
		vertices.color[i] = color;
	}

	//rotation
	for (UINT i = 0; i < size; i++) {
		XMVECTOR rot = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(90), 0.f, 0.f);
		XMVECTOR vertex = XMVector3Rotate(XMLoadFloat3(&vertices.position[i]), rot);
		XMStoreFloat3(&vertices.position[i], XMVector3Normalize(vertex));
	}
}

void GeoEntity::GenerateVertexSphere(Vertex & vertices, vector<unsigned long>& indices, int LOD)
{
	vector<XMFLOAT3>		vertData;

	IcoSphere sphere;
	sphere.generateIcosphereMesh(LOD, indices, vertData);

	for (int i = 0; i < vertData.size(); i++) {
		vertices.position.emplace_back(vertData[i]);
		vertices.uv.emplace_back(XMFLOAT2(0.f, (vertData[i].y + 1.0f) / 2.0f));
		vertices.color.emplace_back(XMFLOAT4(1, 0, 0, 1));
	}
}
