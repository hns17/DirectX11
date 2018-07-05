#pragma once
#include "SingletonBase.h"
//=============================================================
//	## resourceManager ## (리소스 매니져)
//=============================================================
template<typename T, typename A>
class ResourceManager : public SingletonBase <A>
{
protected:
	typedef map<wstring, T> mapResource;
	mapResource _resourceList;	// 맵안에 담긴 리소스 목록들

public:
	// 리소스 추가
	T AddResource(wstring fileName, void* param = NULL);
	// 리소스 삭제
	void RemoveResource(wstring fileName);
	void RemoveResource(T);

	// 모든 리소스 삭제
	void ClearResource(void);

	//리소스 리스트 반환
	mapResource GetResource() {
		return _resourceList;
	}

	//인덱스에 해당하는 리소스 데이터 반환
	T GetResourceData(int i) {
		mapResource::iterator it = _resourceList.begin();
		std::advance(it, i);
		return it->second;
	}

	//인덱스에 해당하는 키 반환
	const wstring* GetResourceKey(int i) {
		mapResource::iterator it = _resourceList.begin();
		std::advance(it, i);
		return &it->first;
	}

	//=============================================================
	//	## 리소스 로드 해제관련 순수가상함수 ##
	//=============================================================
	virtual T loadResource(wstring fileName, void* param = NULL) PURE;
	virtual void releaseResource(T data) PURE;

	ResourceManager() {}
	~ResourceManager() {}
};



// 리소스 추가
template<typename T, typename A>
inline T ResourceManager<T, A>::AddResource(wstring fileName, void * param)
{
	mapResource::iterator find;
	// 이미 리소스가 추가가 되었는지 확인
	find = _resourceList.find(fileName);

	// 해당 리소스가 맵에 추가되어 있지 않다면 새로 만들어서 추가시키기
	if (find == _resourceList.end())
	{
		// 로드리소스 함수로 새로운 리소스 추가
		T newResource = this->loadResource(fileName, param);
		// 리소스 추가가 실패했다면 0리턴
		if (newResource == NULL) return NULL;
		// 리소스목록에 추가하기
		_resourceList.insert(make_pair(fileName, newResource));

		return newResource;
	}

	return find->second;
}

// 리소스 삭제
template<typename T, typename A>
inline void ResourceManager<T, A>::RemoveResource(wstring fileName)
{
	mapResource::iterator find;
	// 이미 리소스가 추가가 되었는지 확인
	find = _resourceList.find(fileName);

	// 리소스가 있다면 지우기
	if (find != _resourceList.end())
	{
		// 찾은 리소스 해제
		this->releaseResource(find->second);
		// 해당하는 값을 삭제
		_resourceList.erase(find);
	}
}

template<typename T, typename A>
inline void ResourceManager<T, A>::RemoveResource(T data)
{
	mapResource::iterator iter;
	for (iter = _resourceList.begin(); iter != _resourceList.end(); ++iter)
	{
		if (iter->second == data) {
			this->releaseResource(iter->second);
			_resourceList.erase(iter);
			break;
		}
	}
}

// 모든 리소스 삭제
template<typename T, typename A>
inline void ResourceManager<T, A>::ClearResource(void)
{
	mapResource::iterator iter;
	for (iter = _resourceList.begin(); iter != _resourceList.end(); ++iter)
	{
		this->releaseResource(iter->second);
	}
	// 전체삭제
	_resourceList.clear();
}

