#include "stdafx.h"
#include "Shader.h"

//정적변수 초기화
vector<LAYOUT_INFO> Shader::_layoutList = {};

Shader::Shader()
{
}

Shader::~Shader()
{

}


//정점을 이용해 그린다.
void Shader::VertexRender(ID3D11DeviceContext *deviceContext, int vertexCount, int startVertex)
{
	RenderShader(deviceContext);
	deviceContext->Draw(vertexCount, startVertex);
}

//인덱스를 이용해 그린다.
void Shader::IndexRender(ID3D11DeviceContext *deviceContext, int indexCount, int startIndex)
{
	RenderShader(deviceContext);
	deviceContext->DrawIndexed(indexCount, startIndex, 0);
}


//셰이더를 추가한다.
bool Shader::InsertShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	//셰이더 컴파일 및 초기화
	if (!InitializeShader(device, hwnd, vsFilename, psFilename)) {
		return false;
	}

	return true;
}

//셰이더를 읽어 컴파일 후 생성
bool Shader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	ComPtr<ID3D10Blob> errorMessage;
	ComPtr<ID3D10Blob> vertexShaderBuffer;
	ComPtr<ID3D10Blob> pixelShaderBuffer;
	

    // Compile the vertex shader code.
	if(FAILED(D3DX11CompileFromFile(vsFilename, NULL, NULL, "vsMain", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL))) {
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage) {
			OutputShaderErrorMessage(errorMessage.Get(), hwnd, vsFilename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else {
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    // Compile the pixel shader code.
	if(FAILED(D3DX11CompileFromFile(psFilename, NULL, NULL, "psMain", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL))) {
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage) {
			OutputShaderErrorMessage(errorMessage.Get(), hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else {
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    // Create the vertex shader from the buffer.
	if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), 
											vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader))) {
		return false;
	}
	

    // Create the pixel shader from the buffer.
	if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), 
											pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader))) {
		return false;
	}
	
	//레이아웃 정보 구성
	vector<D3D11_INPUT_ELEMENT_DESC> layoutDescInfo;

	//컴파일된 셰이더의 정점 정보를 바탕으로 정점입력정보를 구성한다.
	BuildLayoutDesc(CT2CA(vsFilename), layoutDescInfo);

	//정점 정보가 없으면 반환, Vertex Shader를 사용하지 않는 경우를 고려 하지 않았다.
	if (layoutDescInfo.empty()) 
		return false;

	//생성된 레이아웃 리스트에서 동일한 형태의 구성이 있으면 가져온다.
	//없는 경우 추가한다.
	if ((_layoutIdx = FindLayout(layoutDescInfo)) == -1) {
		ComPtr<ID3D11InputLayout> layout;
		//레이아웃 생성
		if (FAILED(device->CreateInputLayout(&layoutDescInfo[0], (UINT)layoutDescInfo.size(), vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(), &layout))) {
			return false;
		}

		//현재 셰이더에서 사용할 레이아웃 정보 인덱스
		_layoutIdx = (int)_layoutList.size();

		//생성된 레이아웃 정적변수로 추가하여 공유
		_layoutList.emplace_back(LAYOUT_INFO{ layout, layoutDescInfo });
	}


	return true;
}


//셰이더 오류 출력
void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	auto bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(auto i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

//정점 입력 레이아웃 정보 구성
void Shader::BuildLayoutDesc(char * vsPath, vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc)
{
	//버텍스 셰이더 파일을 연다.
	FILE* vsShader;
	if (fopen_s(&vsShader, vsPath, "r") != 0)
		return;
	

	//파싱 시작
	char line[512];
	while (NULL != fgets(line, sizeof(line), vsShader)) {
		char word[256] = { 0, };
		sscanf_s(line, "%s", word, 256);

		//struct 키워드 탐색
		if (_stricmp(word, "struct") == 0) {
			char type[128] = { 0, };
			sscanf_s(line, "%s%s", word, 256, word, 256);

			//버텍스 타입 키워드 인지 확인
			if (_stricmp(word, "VertexInputType") == 0) {
				while (NULL != fgets(line, sizeof(line), vsShader)) {
					if (_stricmp(line, "};\n") == 0) {
						break;
					}
					//시멘틱 가져오기
					char* context = NULL;
					char* tok = strtok_s(line, " ", &context);
					
					while (tok) {
						UINT semanticIdx = (UINT)layoutDesc.size();

						if (_stricmp(tok, "POSITION;\n") == 0) {
							layoutDesc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, semanticIdx, 0, D3D11_INPUT_PER_VERTEX_DATA,	0 });
						}
						else if (_stricmp(tok, "COLOR;\n") == 0) {
							layoutDesc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, semanticIdx, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0 });
						}
						else if (_stricmp(tok, "TEXCOORD;\n") == 0) {
							layoutDesc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, semanticIdx, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0 });
						}
						else if (_stricmp(tok, "NORMAL;\n") == 0) {
							layoutDesc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, semanticIdx, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0 });
						}
						else if (_stricmp(tok, "TANGENT;\n") == 0) {
							layoutDesc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, semanticIdx, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0 });
						}
						else if (_stricmp(tok, "BITANGENT;\n") == 0) {
							layoutDesc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, semanticIdx, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0 });
						}
						else if (_stricmp(tok, "BONEID;\n") == 0) {
							layoutDesc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "BONEID", 0, DXGI_FORMAT_R32G32B32A32_UINT, semanticIdx, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0 });
						}
						else if (_stricmp(tok, "WEIGHT;\n") == 0) {
							layoutDesc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, semanticIdx, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0 });
						}
						tok = strtok_s(NULL, " ", &context);
					}
				}
				break;
			}
		}
	}

	fclose(vsShader);
}


//사용할 레이아웃, 정점, 픽셀 셰이더를 등록한다.
void Shader::RenderShader(ID3D11DeviceContext* deviceContext)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(_layoutList[_layoutIdx].first.Get());
	
	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(_vertexShader.Get(), NULL, 0);
	deviceContext->PSSetShader(_pixelShader.Get(), NULL, 0);
	
	return;
}

//레이아웃 탐색
int Shader::FindLayout(vector<D3D11_INPUT_ELEMENT_DESC>& refLayoutDesc)
{

	//layoutList에 동일한 정점 레이아웃이 있는지 찾는다.
	for (int i = 0; i < _layoutList.size(); i++) {
		auto desc = _layoutList[i].second;

		//1. 레이아웃 정보의 수가 일치하는지 체크
		if (desc.size() != refLayoutDesc.size()) 
			continue;

		//모든 정보가 일치하는지 체크
		unsigned int cnt = 0;
		for (cnt = 0; cnt < desc.size(); cnt++) {
			if (desc[cnt].SemanticName != refLayoutDesc[cnt].SemanticName) {
				break;
			}
		}

		//모든 정보가 일치하면 위치 값을 반환
		if (cnt >= desc.size()) {
			return i;
		}
	}
	return -1;
}
