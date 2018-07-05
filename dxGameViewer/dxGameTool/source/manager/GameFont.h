//====================================================================
//			## GameFont ## (Font 그리기)
//====================================================================
#pragma once
#include "singletonBase.h"
#include "..\include\tinyxml\tinyxml.h"
#include "model\SimpleTextureModel.h"


//폰트 구조체
struct CharFont
{
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;
};


class GameFont : public SingletonBase <GameFont>
{
private:
	std::map<unsigned int, CharFont>	_charators;			//문자 정보
	ComPtr<ID3D11ShaderResourceView>	_fontTex;			//문자 텍스쳐
	
	Vertex								_vertices;			//그려질 문자의 정점 데이터
	unique_ptr<SimpleTextureModel>		_fontModel;			//렌더링 될 모델

	unsigned int						_nowLength = 0;		//화면에 그려지는 전체 글자 길이
	unsigned int						_maxLength = 200;	//전체 글자 최대 길이

	
public:
	GameFont(void);
	~GameFont(void);
	HRESULT Init();
	
	//Text 추가
	void Print(wchar_t * str, int x, int y, int fontSize = 25, XMFLOAT3 color = XMFLOAT3(1.f,1.f,1.f));
	//Text Render
	void Render(ID3D11DeviceContext* dc);


private:
	//폰트 텍스쳐 로드
	bool LoadTexture(std::string fileName);
	//버퍼 초기화
	bool InitBuffer(int maxLength);
};

