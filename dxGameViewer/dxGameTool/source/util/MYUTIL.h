// MYUTIL.h 
// 2017.12.20
#pragma once

#define EPSILON 0.0001f
#define ONE_RADIAN	 0.0174532925f

// float 값끼리 비교할 때 엡실론 이하의 오차는 같다 
// a = 10.00001f
// b = 10.00008f
// abs(a-b) < c => a = b 는 같다. 

#define FLOATEQUAL(a, b)	(abs((a) - (b)) < EPSILON)		// a 와 b 가 같은지?
#define FLOATZERO(a)		(abs(a) < EPSILON)				// a 의 값이 0과 같은지?
#define VECTORZERO(v)		(FLOATZERO((v).x) && FLOATZERO((v).y) && FLOATZERO((v).z))



//레이 구조체
typedef struct tagRay
{
	XMFLOAT3 origin;		//직선의 시작위치
	XMFLOAT3	direction;	//직선의 방향(정규화 되어 있어야 함)
}Ray, *LPRay;

namespace MYUTIL
{
	//=============================================================
	//	## 마우스 ##
	//=============================================================
	POINT getMousePos();						// 마우스 위치 얻기
	void setMousePos(int x, int y);				// 마우스 위치 셋팅

	CString getFileName(CString path);
	CString GetDirectoryPath(CString path);

	//=============================================================
	//	## 랜덤 함수 ##
	//=============================================================
	// 랜덤 시드 생성
	void initForRandomSeed();
	// min ~ max 사이의 랜덤 정수 생성
	int randomIntRange(int min, int max);
	// min ~ max 사이의 랜덤 실수 생성
	float randomFloatRange(float min, float max);
	wstring ConvertToWString(CString& str);

	float InverseFloat(float num);
	
	void GenerateIndexList(std::vector<unsigned long> &indices, int idxCnt);
}

