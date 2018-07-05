//==================================================================
//		## sceneManager ## (씬 매니져)
//==================================================================
#pragma once
#include "SingletonBase.h"


//게임노드 클래스 전방선언
class GameNode;

class SceneManager : public SingletonBase <SceneManager>
{
private:
	typedef map<string, GameNode*> mSceneList;
	typedef map<string, GameNode*>::iterator miSceneList;

private:
	mSceneList _mSceneList;		//각각의 씬(화면)들을 담아둘 맵
	GameNode* _currentScene;	//현재씬
public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);
	void Render(ID3D11DeviceContext* dc);


	//씬추가
	GameNode* AddScene(string sceneName, GameNode* scene);

	//씬변경
	HRESULT ChangeScene(string sceneName);

	SceneManager() : _currentScene(NULL) {}
	~SceneManager() {}
};