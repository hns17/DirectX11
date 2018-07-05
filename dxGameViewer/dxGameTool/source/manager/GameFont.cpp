#include "stdafx.h"
#include "GameFont.h"


GameFont::GameFont(void)
{
}


GameFont::~GameFont(void)
{
}

//폰트의 텍스쳐 정보를 로드한다. 
HRESULT GameFont::Init()
{
	
	//Set Font Info
	char fontPath[50] = "res\\fonts\\";
	char fontImagePath[50] = "res\\fonts\\";
	
	strcat_s(fontPath, "candy.fnt");

	//Font Load
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


	InitBuffer(_maxLength);

	return S_OK;
}



//렌더에 사용될 버퍼 초기화
bool GameFont::InitBuffer(int maxLength)
{
	int vertCnt = maxLength * 4;
	int idxCnt = maxLength * 6;

	if (maxLength <= 0) {
		return false;
	}

	
	_vertices.position.resize(vertCnt);
	_vertices.uv.resize(vertCnt);
	_vertices.color.resize(vertCnt);


	vector<unsigned long> indices;
	MYUTIL::GenerateIndexList(indices, idxCnt);

	
	ID3D11Device *device= DEVICEMANAGER.GetDevice();
	_fontModel = make_unique<SimpleTextureModel>();
	_fontModel->InitVertextBuffers(device, _vertices, true);
	_fontModel->InitIndexBuffers(device, indices);
	_fontModel->SetShader(RM_SHADER.AddResource(L"source\\shader\\font"));
	_fontModel->SetTexture(_fontTex.Get());

	return true;
}

//텍스트 추가
void GameFont::Print(wchar_t * str, int x, int y, int fontSize, XMFLOAT3 color)
{
	//최대 표시 문자수를 넘으면 더 이상 문자를 생성하지 않는다.
	unsigned int fontLength =lstrlen(str) + _nowLength;

	if (_maxLength <= fontLength) {
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
		
		int fontWidth = charData.width * (fontSize / 10);
		int fontHeight = charData.height * (fontSize / 10);
		int beforeXOffset = charData.xoffset;
		
		float mulNum = 1.f / 1024.f;
		float tu = (float)charData.x, tv = (float)charData.y;

		if (tu > 0) {		tu *=  mulNum;	}
		if (tv > 0) {		tv *= mulNum;	}

		float maxTu = (charData.x + charData.width) * mulNum;
		float maxTv = (charData.y + charData.height) * mulNum;


		int indexCnt = _nowLength * 4;

		//좌상단
		_vertices.position[indexCnt] = (XMFLOAT3(position.x, position.y + fontHeight, 0.f));
		_vertices.color[indexCnt] = (colorData);
		_vertices.uv[indexCnt] = (XMFLOAT2(tu, tv));

		//우상단
		indexCnt++;
		_vertices.position[indexCnt] = (XMFLOAT3(position.x + fontWidth, position.y + fontHeight, 0.f));
		_vertices.color[indexCnt] = (colorData);
		_vertices.uv[indexCnt] = (XMFLOAT2(maxTu, tv));

		//좌하단
		indexCnt++;
		_vertices.position[indexCnt] = (XMFLOAT3(position.x, position.y, 0.f));
		_vertices.color[indexCnt] = (colorData);
		_vertices.uv[indexCnt] = (XMFLOAT2(tu, maxTv));

		//우하단
		indexCnt++;
		_vertices.position[indexCnt] = (XMFLOAT3(position.x + fontWidth, position.y, 0.f));
		_vertices.color[indexCnt] = (colorData);
		_vertices.uv[indexCnt] = (XMFLOAT2(maxTu, maxTv));


		position.x += fontWidth + beforeXOffset;	//글씨가 써지면 뒤로 물려나게 함
		str++;
		_nowLength++;
	}
			
}


//텍스트 그리기
void GameFont::Render(ID3D11DeviceContext * dc)
{
	unsigned int vertCnt = _nowLength*4;

	if (vertCnt <= 0) { 
		return;
	}

	//정점 버퍼 업데이트
	unsigned int idxCnt = (_nowLength) * 6;
	VertexDataBuffer* vertexBuffer = _fontModel->GetVertexBuffer();
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	
	ID3D11Buffer* buffer = vertexBuffer->positionBuffer;
	if (FAILED(dc->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return;
	}
	auto posPtr = (XMFLOAT3*)mappedResource.pData;
	memcpy(posPtr, (void*)&_vertices.position[0], (sizeof(XMFLOAT3) * vertCnt));
	dc->Unmap(buffer, 0);


	buffer = vertexBuffer->colorBuffer;
	if (FAILED(dc->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return;
	}
	auto colorPtr = (XMFLOAT4*)mappedResource.pData;
	memcpy(colorPtr, (void*)&_vertices.color[0], (sizeof(XMFLOAT4) * vertCnt));
	dc->Unmap(buffer, 0);

	buffer = vertexBuffer->texcoordBuffer;
	if (FAILED(dc->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return;
	}
	auto uvPtr = (XMFLOAT2*)mappedResource.pData;
	memcpy(uvPtr, (void*)&_vertices.uv[0], (sizeof(XMFLOAT2) * vertCnt));
	dc->Unmap(buffer, 0);


	////여기서 렌더하자
	_fontModel->SetIndexCnt(idxCnt);

	DEVICEMANAGER.TurnZBufferOff();
	DEVICEMANAGER.TurnOnAlphaBlending();
	
	_fontModel->Render(dc);

	DEVICEMANAGER.TurnOffAlphaBlending();
	DEVICEMANAGER.TurnZBufferOn();
	_nowLength = 0;
}


//텍스쳐 로드
bool GameFont::LoadTexture(std::string fileName)
{
	//폰트 텍스쳐 로딩
	wstring filePath = CA2CT(fileName.c_str());

	_fontTex = RM_TEXTURE.LoadTexture(filePath);
	
	if (!_fontTex) {
		return false;
	}

	return true;
}


