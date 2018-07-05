//==================================================================
//		## GeoEntity ## (지오메트리 모델 생성)
//==================================================================
#pragma once
#include "icosphereGenerator.h"
#include "BasicModel.h"

namespace GeoEntity {

	//지오메트리 모델 생성
	BaseModel* CreateTriangle(ID3D11Device* device, BaseModel* model = NULL);
	BaseModel* CreateSquare(ID3D11Device* device, BaseModel* model = NULL);
	BaseModel* CreateCube(ID3D11Device* device, BaseModel* model = NULL);
	
	BaseModel* CreateGridLine(ID3D11Device* device, float radius, BaseModel* model = NULL);
	BaseModel* CreateIcoHedronSphere(ID3D11Device* device, int Lod, BaseModel* model = NULL);
	BaseModel* CreateCapsule(ID3D11Device* device, XMFLOAT4 color, BaseModel* model = NULL);


	// 모델 데이터 생성
	void GenerateVertexCube(Vertex& vertices, vector<unsigned long>& indices, XMFLOAT3 center, XMFLOAT3 size, XMFLOAT4 color);
	void GenerateVertexCone(Vertex& vertices, vector<unsigned long>& indices, XMFLOAT3 center, XMFLOAT3 degree, XMFLOAT3 size, XMFLOAT4 color);
	void GenerateVertexCircle(Vertex & vertices, XMFLOAT4& color, XMFLOAT3 degree);
	void GenerateVertexCapsule(Vertex& vertices, vector<unsigned long>& indices, XMFLOAT4 color);
	void GenerateVertexSphere(Vertex& vertices, vector<unsigned long>& indices, int LOD);
}