#include "stdafx.h"
#include "TimeManager.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
//timeGetTime함수를 사용하기 위해서
//윈도우가 시작되고 난 후 시간을 리턴시켜준다
//GetTickCount보다 성능이 쪼오끔~ 더 좋다

HRESULT TimeManager::Init(void)
{
	//고성능 타이머 지원여부 체크
	QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency);
	//초당 시간 계산(1초 몇번 카운팅 하냐)
	_timeScale = 1.0f / _periodFrequency;
	//초기화 시점의 시간을 마지막 시간으로
	QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);


	//변수들 초기화
	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0.0f;
	_worldTime = 0;

	return S_OK;
}

void TimeManager::Release(void)
{
}

void TimeManager::Update(float lockFPS)
{

	//현재시간 얻기
	QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);

	//한프레임 간격의 초 계산 (지난 시간과의 격차를 계산)
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	//프레임 스키핑
	if (lockFPS > 0.0f)
	{
		while (_timeElapsed < (1.0f / lockFPS))
		{
			//현재시간 얻기
			QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			
			//한프레임 간격의 초 계산
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	//지난 시간 갱신
	_lastTime = _curTime;
	//프레임 초 누적
	_FPSTimeElapsed += _timeElapsed;
	//총지난 시간 누적
	_worldTime += _timeElapsed;

	//프레임 초기화를 1초마다 진행하기
	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}

	_FPSFrameCount++;
}
