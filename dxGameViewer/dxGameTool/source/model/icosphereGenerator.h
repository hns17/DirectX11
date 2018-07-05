#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>


class Vec3KeyFuncs {
public:
	size_t operator()(const XMFLOAT3& k)const {
		return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
	}

	bool operator()(const XMFLOAT3& a, const XMFLOAT3& b)const {
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

class IcoSphere
{
public:
	IcoSphere();
	~IcoSphere();

	
	void generateIcosphereMesh(size_t lod, std::vector<unsigned long>& indices, std::vector<XMFLOAT3>& positions);
	inline XMFLOAT3 findMidpoint(XMFLOAT3 vertex1, XMFLOAT3 vertex2);
private:

};
