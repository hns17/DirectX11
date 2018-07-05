#pragma once
#include "singletonBase.h"
#include "..\include\tinyxml\tinyxml.h"
#include "..\util\FontModel.h"


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


class GameFont : public singletonBase <GameFont>
{

	struct FontInfo{
		unsigned int		useLength;
		unsigned int		maxLength;

		Vertex				vertices;
		FontModel*			fontModel;
	};

private:
	std::map<unsigned int, CharFont>	_charators;		//문자 정보
	

	ID3D11ShaderResourceView*			_fontTex;		//문자 텍스쳐
	
	bool								_isModifyStFont = false;
	FontInfo							_stFont;
	FontInfo							_dyFont;


public:
	GameFont(void);
	~GameFont(void);
	HRESULT Init();
	

	void AddStaticText(wchar_t * str, int x, int y, XMFLOAT3 color, int fontSize);
	void AddDynamicText(wchar_t * str, int x, int y, XMFLOAT3 color, int fontSize);
	void Render();
	void Release();



private:
	bool LoadTexture(std::string fileName);
	bool InitFontInfo(FontInfo& ftInfo, int maxLength, bool isDynamic = false);
	bool UpdateBufferData(FontInfo& ftInfo);

	void GenerateTextData(FontInfo& ftInfo, wchar_t * str, int x, int y, XMFLOAT3 color, int fontSize);

	void StaticFontRender();
	void DynamicFontRender();
};

