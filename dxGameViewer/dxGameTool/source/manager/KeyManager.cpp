#include "stdafx.h"
#include "KeyManager.h"

HRESULT KeyManager::Init()
{
	//키값을 전부 눌려 있지 않은 상태로 초기화 한다
	for (int i = 0; i < KEYMAX; i++)
	{
		_keyUp[i] = false;
		_keyDown[i] = false;
	}

	return S_OK;
}

void KeyManager::release(void)
{
}

bool KeyManager::IsOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x0001)
	{
		if (!_keyDown[key])
		{
			_keyDown[key] = true;
			return true;
		}
	}
	else
	{
		_keyDown[key] = false;
	}

	return false;
}

bool KeyManager::IsOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		_keyUp[key] = true;
	}
	else
	{
		if (_keyUp[key])
		{
			_keyUp[key] = false;
			return true;
		}
	}

	return false;
}

bool KeyManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

bool KeyManager::IsToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}
