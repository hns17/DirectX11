//==================================================================
//		## Shader ## (Shader 관련 클래스)
//==================================================================
#pragma once
#include <d3dx11async.h>
#include <fstream>
using namespace std;

using LAYOUT_INFO = pair<ComPtr<ID3D11InputLayout>, vector<D3D11_INPUT_ELEMENT_DESC>> ;

class Shader
{
public:
	Shader();
	~Shader();

	//셰이더 추가
	bool InsertShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	//정점 렌더
	void VertexRender(ID3D11DeviceContext*, int, int);			
	//인덱스 렌더
	void IndexRender(ID3D11DeviceContext*, int, int) ;			
	
	
	//현재 셰이더의 정점 레이아웃 구성 정보를 반환
	vector<D3D11_INPUT_ELEMENT_DESC>&	GetVertexLayoutDesc()		{ return _layoutList[_layoutIdx].second;	}

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);		//셰이더 초기화
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);		//셰이더 에러 출력

	//셰이더의 정점 레이아웃 정보 구성
	void BuildLayoutDesc(char* vsPath, vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc);

	//레이아웃 탐색
	int FindLayout(vector<D3D11_INPUT_ELEMENT_DESC>& refLayoutDesc);
	
	//셰이더 렌더
	void RenderShader(ID3D11DeviceContext*);

private:
	ComPtr<ID3D11VertexShader>			_vertexShader;		//정점 셰이더 
	ComPtr<ID3D11PixelShader>			_pixelShader;		//픽셀 셰이더
	
	int									_layoutIdx;			//셰이더 정점 입력 레이아웃 index


	//로드된 모든 셰이더의 레이아웃 정보 리스트
	//정적 멤버 변수로 모든 셰이더가 공유한다.
	//셰이더가 삭제되더라도 레이아웃 정보는 계속 유지한다.
	static vector<LAYOUT_INFO>			_layoutList;	
};
