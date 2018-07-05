#pragma once
#include "SingletonBase.h"
#include <chrono>
//==================================================================
//		## timeManager ## (타임 매니져, 가져옴. 크로노로 변경할수도 있음.)
//==================================================================

class TimeManager : public SingletonBase <TimeManager>
{
private:
	float _timeScale;				//시간 경과량
	float _timeElapsed;				//한프레임당 경과량
	__int64 _curTime;				//현재시간
	__int64 _lastTime;				//마지막시간
	__int64 _periodFrequency;		//시간주기 (1초에 몇번이나 카운트되는지)

	unsigned long _frameRate;		//FPS
	unsigned long _FPSFrameCount;	//FPS 카운트
	float _FPSTimeElapsed;			//FPS 경과량
	float _worldTime;				//전체 경과시간

public:
	HRESULT Init(void);
	void Release(void);
	void Update(float lockFPS);

	//한프레임당 경과시간 가져오기
	float GetElapedTime(void) { return _timeElapsed; }
	//전체 경과시간 가져오기
	float GetWorldTime(void) { return _worldTime; }
	int		GetFrameRate() { return _frameRate; }

	TimeManager() {}
	~TimeManager() {}
};