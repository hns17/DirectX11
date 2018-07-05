#include "stdafx.h"
#include "GameFont.h"


GameFont::GameFont(void)
{
}


GameFont::~GameFont(void)
{
}

HRESULT GameFont::Init()
{
	

	char fontPath[50] = "res\\fonts\\";
	char fontImagePath[50] = "res\\fonts\\";
	
	strcat_s(fontPath, "candy.fnt");

	TiXmlDocument fontXml;
	if (fontXml.LoadFile(fontPath, TIXML_ENCODING_UTF8) == false)	//xml 로드 실패
	{
		MessageBox(NULL, L"Can't find font file", L"Error", MB_OK);
		return E_FAIL;
	}
	else
	{
		TiXmlNode *pNode;
		//이미지 파일의 이름을 받아서 해당 이미지 로딩
		pNode = fontXml.FirstChild("font")->FirstChild("pages")->FirstChild("page");
		const char *fontImage = pNode->ToElement()->Attribute("file");
		strcat_s(fontImagePath, fontImage);
		
		if (!LoadTexture(fontImagePath)) {
			MessageBox(NULL, L"Can't find font texture file", L"Error", MB_OK);
			return E_FAIL;
		}

		pNode = fontXml.FirstChild("font")->FirstChild("chars");
		pNode = pNode->FirstChild("char");

		int id = 0;
		while (pNode != NULL)
		{
			CharFont charFont;
			pNode->ToElement()->Attribute("id", &id);

			pNode->ToElement()->Attribute("x", &charFont.x);
			pNode->ToElement()->Attribute("y", &charFont.y);
			pNode->ToElement()->Attribute("width", &charFont.width);
			pNode->ToElement()->Attribute("height", &charFont.height);
			pNode->ToElement()->Attribute("xoffset", &charFont.xoffset);
			pNode->ToElement()->Attribute("yoffset", &charFont.yoffset);
			pNode->ToElement()->Attribute("xadvance", &charFont.xadvance);
			
			_charators[id] = charFont;

			pNode = pNode->NextSibling();
		}
		fontXml.Clear();
	}


	InitFontInfo(_stFont, 200);
	InitFontInfo(_dyFont, 200, true);

	return S_OK;
}

bool GameFont::InitFontInfo(FontInfo& ftInfo, int maxLength, bool isDynamic)
{
	if (maxLength <= 0) {
		return false;
	}


	int vertCnt = maxLength * 4;
	int idxCnt = maxLength * 6;

	

	ftInfo.maxLength = maxLength;
	ftInfo.useLength = 0;
	
	auto& vertices = ftInfo.vertices;

	vertices.position.resize(vertCnt);
	vertices.uv.resize(vertCnt);
	vertices.color.resize(vertCnt);


	vector<unsigned long> indices;
	MYUTIL::GenerateIndexList(indices, idxCnt);

	
	ID3D11Device *device= DEVICEMANAGER->GetDevice();
	ftInfo.fontModel = new FontModel;
	ftInfo.fontModel->InitVertextBuffers(device, vertices, isDynamic);
	ftInfo.fontModel->InitIndexBuffers(device, indices);

	ftInfo.fontModel->SetTexture(_fontTex);


	return true;
}


bool GameFont::UpdateBufferData(FontInfo& ftInfo)
{
	if (ftInfo.useLength <= 0) 
		return false;

	unsigned int vertCnt = ftInfo.useLength * 4;
	unsigned int idxCnt = ftInfo.useLength * 6;


	ID3D11DeviceContext*	dc				= DEVICEMANAGER->GetDeviceContext();
	VertexDataBuffer*		vertexBuffer	= ftInfo.fontModel->GetVertexBuffer();
	ID3D11Buffer*			buffer			= vertexBuffer->positionBuffer;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	
	auto vertice = ftInfo.vertices;
	if (FAILED(dc->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}
	auto posPtr = (XMFLOAT3*)mappedResource.pData;
	memcpy(posPtr, (void*)&vertice.position[0], (sizeof(XMFLOAT3) * vertCnt));
	dc->Unmap(buffer, 0);


	buffer = vertexBuffer->colorBuffer;
	if (FAILED(dc->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}
	auto colorPtr = (XMFLOAT4*)mappedResource.pData;
	memcpy(colorPtr, (void*)&vertice.color[0], (sizeof(XMFLOAT4) * vertCnt));
	dc->Unmap(buffer, 0);

	buffer = vertexBuffer->texcoordBuffer;
	if (FAILED(dc->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}
	auto uvPtr = (XMFLOAT2*)mappedResource.pData;
	memcpy(uvPtr, (void*)&vertice.uv[0], (sizeof(XMFLOAT2) * vertCnt));
	dc->Unmap(buffer, 0);

	return true;
}

void GameFont::GenerateTextData(FontInfo & ftInfo, wchar_t * str, int x, int y, XMFLOAT3 color, int fontSize)
{
	//최대 표시 문자수를 넘으면 더 이상 문자를 생성하지 않는다.
	int fontLength = lstrlen(str) + ftInfo.useLength;

	if (ftInfo.maxLength <= fontLength) {
		return;
	}

	float left = (float)((WINSIZEX / 2) * -1) + (float)x;
	float top = (float)(WINSIZEY / 2) - (float)y;

	XMFLOAT3 position = XMFLOAT3(left, top, 0.f);
	XMFLOAT4 colorData = XMFLOAT4(color.x, color.y, color.z, 1.f);

	//끝날때까지 루프
	while (*str != NULL) {
		int charNum = *str;	//유니코드값을 받음

							//한칸 띄우는 경우 그리지 말고 11픽셀만큼 뒤로 옮겨서 그리게 함
		if (charNum == 32) {
			position.x += 11;
			str++;
			continue;
		}

		auto charData = _charators.find(charNum)->second;

		int fontWidth = charData.width * (fontSize * 0.1f);
		int fontHeight = charData.height * (fontSize * 0.1f);
		int beforeXOffset = charData.xoffset;

		float mulNum = 1.f / 1024.f;
		float tu = charData.x, tv = charData.y;

		if (tu > 0) { tu *= mulNum; }
		if (tv > 0) { tv *= mulNum; }

		float maxTu = (charData.x + charData.width) * mulNum;
		float maxTv = (charData.y + charData.height) * mulNum;


		int indexCnt = ftInfo.useLength * 4;
		auto& vertices = ftInfo.vertices;
		//좌상단
		vertices.position[indexCnt] = (XMFLOAT3(position.x, position.y + fontHeight, 0.f));
		vertices.color[indexCnt] = (colorData);
		vertices.uv[indexCnt] = (XMFLOAT2(tu, tv));

		//우상단
		indexCnt++;
		vertices.position[indexCnt] = (XMFLOAT3(position.x + fontWidth, position.y + fontHeight, 0.f));
		vertices.color[indexCnt] = (colorData);
		vertices.uv[indexCnt] = (XMFLOAT2(maxTu, tv));

		//좌하단
		indexCnt++;
		vertices.position[indexCnt] = (XMFLOAT3(position.x, position.y, 0.f));
		vertices.color[indexCnt] = (colorData);
		vertices.uv[indexCnt] = (XMFLOAT2(tu, maxTv));

		//우하단
		indexCnt++;
		vertices.position[indexCnt] = (XMFLOAT3(position.x + fontWidth, position.y, 0.f));
		vertices.color[indexCnt] = (colorData);
		vertices.uv[indexCnt] = (XMFLOAT2(maxTu, maxTv));


		position.x += fontWidth + beforeXOffset;	//글씨가 써지면 뒤로 물려나게 함
		str++;
		ftInfo.useLength++;
	}

}



void GameFont::AddStaticText(wchar_t * str, int x, int y, XMFLOAT3 color, int fontSize)
{
	_isModifyStFont = true;
	GenerateTextData(_stFont, str, x, y, color, fontSize);
}

void GameFont::AddDynamicText(wchar_t * str, int x, int y, XMFLOAT3 color, int fontSize)
{
	GenerateTextData(_dyFont, str, x, y, color, fontSize);	
}

void GameFont::Render()
{
	StaticFontRender();
	DynamicFontRender();
}


void GameFont::StaticFontRender()
{
	ID3D11DeviceContext * dc = DEVICEMANAGER->GetDeviceContext();

	if (_isModifyStFont) {
		if (!UpdateBufferData(_stFont))
			return;
	}

	unsigned int idxCnt = (_stFont.useLength) * 6;

	//여기서 렌더하자
	_stFont.fontModel->SetIndexCnt(idxCnt);
	_stFont.fontModel->Render(dc);

}

void GameFont::DynamicFontRender()
{
	ID3D11DeviceContext * dc = DEVICEMANAGER->GetDeviceContext();

	if (!UpdateBufferData(_dyFont))
		return;

	unsigned int idxCnt = (_dyFont.useLength) * 6;

	//여기서 렌더하자
	_dyFont.fontModel->SetIndexCnt(idxCnt);
	_dyFont.fontModel->Render(dc);

	_dyFont.useLength = 0;
}

bool GameFont::LoadTexture(std::string fileName)
{
	//폰트 텍스쳐 로딩
	wstring filePath = CA2CT(fileName.c_str());

	_fontTex = RM_TEXTURE->LoadTexture(filePath);
	
	if (!_fontTex) {
		return false;
	}

	return true;
}

void GameFont::Release()
{
	if (_fontTex) {
		SAFE_RELEASE(_fontTex);
	}
}