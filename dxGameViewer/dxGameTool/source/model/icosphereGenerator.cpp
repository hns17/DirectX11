#include "stdafx.h"
#include "icosphereGenerator.h"


const static float GOLDEN_RATIO = 1.61803398875f;

const static int NUM_ICOSOHEDRON_VERTICES = 12;
const static XMFLOAT3 ICOSOHEDRON_VERTICES[12] = {
	XMFLOAT3(-1.0f, GOLDEN_RATIO, 0.0f),
	XMFLOAT3(1.0f, GOLDEN_RATIO, 0.0f),
	XMFLOAT3(-1.0f, -GOLDEN_RATIO, 0.0f),
	XMFLOAT3(1.0f, -GOLDEN_RATIO, 0.0f),

	XMFLOAT3(0.0f, -1.0f, GOLDEN_RATIO),
	XMFLOAT3(0.0f, 1.0f, GOLDEN_RATIO),
	XMFLOAT3(0.0f, -1.0, -GOLDEN_RATIO),
	XMFLOAT3(0.0f, 1.0f, -GOLDEN_RATIO),

	XMFLOAT3(GOLDEN_RATIO, 0.0f, -1.0f),
	XMFLOAT3(GOLDEN_RATIO, 0.0f, 1.0f),
	XMFLOAT3(-GOLDEN_RATIO, 0.0f, -1.0f),
	XMFLOAT3(-GOLDEN_RATIO, 0.0, 1.0f)
};

const static int NUM_ICOSOHEDRON_INDICES = 60;
const static unsigned int ICOSOHEDRON_INDICES[60] = {
	0, 11, 5,
	0, 5, 1,
	0, 1, 7,
	0, 7, 10,
	0, 10, 11,

	1, 5, 9,
	5, 11, 4,
	11, 10, 2,
	10, 7, 6,
	7, 1, 8,

	3, 9, 4,
	3, 4, 2,
	3, 2, 6,
	3, 6, 8,
	3, 8, 9,

	4, 9, 5,
	2, 4, 11,
	6, 2, 10,
	8, 6, 7,
	9, 8, 1
};


IcoSphere::IcoSphere()
{
}

IcoSphere::~IcoSphere()
{
}
inline XMFLOAT3 IcoSphere::findMidpoint(XMFLOAT3 vertex1, XMFLOAT3 vertex2) {
	XMFLOAT3 res((vertex1.x + vertex2.x) / 2.0f, (vertex1.y + vertex2.y) / 2.0f, (vertex1.z + vertex2.z) / 2.0f);
	XMVECTOR vec = XMVector3Normalize(XMLoadFloat3(&res));
	XMStoreFloat3(&res, vec);

	return res;
}

void IcoSphere::generateIcosphereMesh(size_t lod, std::vector<unsigned long>& indices, std::vector<XMFLOAT3>& positions)
{
	std::vector<unsigned long> newIndices;
	newIndices.reserve(256);

	std::unordered_map<XMFLOAT3, int, Vec3KeyFuncs, Vec3KeyFuncs> vertexLookup;

	indices.resize(NUM_ICOSOHEDRON_INDICES);
	for (unsigned int i = 0; i < NUM_ICOSOHEDRON_INDICES; i++) {
		indices[i] = ICOSOHEDRON_INDICES[i];
	}
	positions.resize(NUM_ICOSOHEDRON_VERTICES);
	for (unsigned int i = 0; i < NUM_ICOSOHEDRON_VERTICES; i++) {

		XMVECTOR vertVec = XMVector3Normalize(XMLoadFloat3(&ICOSOHEDRON_VERTICES[i]));

		XMStoreFloat3(&positions[i], vertVec);
		vertexLookup[positions[i]] = i;
	}

	for (size_t i = 0; i < (size_t)lod; i++) {
		for (size_t j = 0; j < indices.size(); j += 3) {
			/*
			j
			mp12   mp13
			j+1    mp23   j+2
			*/
			// Defined in counter clockwise order
			XMFLOAT3 vertex1 = positions[indices[j + 0]];
			XMFLOAT3 vertex2 = positions[indices[j + 1]];
			XMFLOAT3 vertex3 = positions[indices[j + 2]];

			XMFLOAT3 midPoint12 = findMidpoint(vertex1, vertex2);
			XMFLOAT3 midPoint23 = findMidpoint(vertex2, vertex3);
			XMFLOAT3 midPoint13 = findMidpoint(vertex1, vertex3);

			unsigned int mp12Index;
			unsigned int mp23Index;
			unsigned int mp13Index;

			auto iter = vertexLookup.find(midPoint12);
			if (iter != vertexLookup.end()) { // It is in the map
				mp12Index = iter->second;
			}
			else { // Not in the map
				mp12Index = (unsigned int)positions.size();
				positions.push_back(midPoint12);
				vertexLookup[midPoint12] = mp12Index;
			}

			iter = vertexLookup.find(midPoint23);
			if (iter != vertexLookup.end()) { // It is in the map
				mp23Index = iter->second;
			}
			else { // Not in the map
				mp23Index = (unsigned int)positions.size();
				positions.push_back(midPoint23);
				vertexLookup[midPoint23] = mp23Index;
			}

			iter = vertexLookup.find(midPoint13);
			if (iter != vertexLookup.end()) { // It is in the map
				mp13Index = iter->second;
			}
			else { // Not in the map
				mp13Index = (unsigned int)positions.size();
				positions.push_back(midPoint13);
				vertexLookup[midPoint13] = mp13Index;
			}

			newIndices.push_back(indices[j]);
			newIndices.push_back(mp12Index);
			newIndices.push_back(mp13Index);

			newIndices.push_back(mp12Index);
			newIndices.push_back(indices[j + 1]);
			newIndices.push_back(mp23Index);

			newIndices.push_back(mp13Index);
			newIndices.push_back(mp23Index);
			newIndices.push_back(indices[j + 2]);

			newIndices.push_back(mp12Index);
			newIndices.push_back(mp23Index);
			newIndices.push_back(mp13Index);
		}
		indices.swap(newIndices);
		newIndices.clear();
	}
}