#pragma once
#include "SingletonBase.h"
#include <wrl.h>

//====================================================================
//			## DeviceManager ## (DX11 Divece Init&Control)
//====================================================================

typedef struct tagWindowRender {
	IDXGISwapChain*				swapChain;
	ID3D11RenderTargetView*		renderTargetView;
	ID3D11DepthStencilView*		depthStencilView;
	ID3D11Texture2D*			depthStencilBuffer;

	D3D11_VIEWPORT				viewport;

	XMMATRIX					projectionMatrix;
	XMMATRIX					worldMatrix;
	XMMATRIX					orthoMatrix;
	XMMATRIX					viewMatrix;


	~tagWindowRender() {
		swapChain->Release();
		renderTargetView->Release();
		depthStencilView->Release();
		depthStencilBuffer->Release();
	}
} WindowRender, *PWindowRender;

using namespace Microsoft::WRL;

//디바이스 매니저
class DeviceManager : public SingletonBase <DeviceManager>
{
	
//*********************************************** 멤버 함수 **********************************************************//
public:
	//dx11 device 생성
	HRESULT						CreateDevice(int screenWidth, int screenHeight);
	//렌더링 뷰 정보 초기화
	PWindowRender				InitRenderScreen(HWND hWnd, int screenWidth, int screenHeight, float screenNear, float screenDepth);
	
	//씬 정리
	void						BeginScene(PWindowRender pWR, float, float, float, float);
	void						EndScene(PWindowRender pWR);

	//뷰 정보 변경
	XMFLOAT3					ConvertScreenCoordinate(PWindowRender pWR, POINT ptWinPos, XMVECTOR cameraPos, bool isDepthBase = true);

	//set fill mode
	void						SetFillMode(D3D11_FILL_MODE mode);
	
	//set face cull
	void						SetCullMode(D3D11_CULL_MODE mode);
	
	//texture mode
	bool						SetSamplerAddrees(D3D11_TEXTURE_ADDRESS_MODE addMode);

	//Alpha blending On/Off
	void						TurnOnAlphaBlending();
	void						TurnOffAlphaBlending();

	//Vsync On/Off
	void						TurnVSync()				{ _vsyncEnabled = !_vsyncEnabled;												}
	
	//Zbuffer On/Off
	void						TurnZBufferOn()			{ _deviceContext->OMSetDepthStencilState(_depthEnabledStencilState.Get(), 1);	}
	void						TurnZBufferOff()		{ _deviceContext->OMSetDepthStencilState(_depthDisabledStencilState.Get(), 1);	}

	//check vsync
	bool						IsVSync()				{ return _vsyncEnabled;															}
	
	
	//Get func
	ID3D11DeviceContext*		GetDeviceContext()		{ return _deviceContext.Get();													}
	ID3D11Device*				GetDevice()				{ return _device.Get();															}

	D3D11_FILL_MODE				GetFillMode()			{ return _rasterDesc.FillMode;													}
	D3D11_CULL_MODE				GetCullMode()			{ return _rasterDesc.CullMode;													}

	DeviceManager();
	~DeviceManager() {}


private:
	bool						InitDepthState();
	bool						InitDepthBuffer(PWindowRender pWR, int screenWidth, int screenHeight);
	
	bool						InitAlphaBlend();
	bool						InitRasterize();
	bool						setRasterState();
	bool						InitSamplerState();


//*********************************************** 멤버 변수 **********************************************************//
private:
	bool								_vsyncEnabled;
	unsigned int						_numerator, _denominator;

	ComPtr<ID3D11Device>				_device;
	ComPtr<ID3D11DeviceContext>			_deviceContext;
	D3D_FEATURE_LEVEL					_featureLevel;

	ComPtr<ID3D11DepthStencilState>		_depthEnabledStencilState;
	ComPtr<ID3D11DepthStencilState>		_depthDisabledStencilState;

	ComPtr<ID3D11BlendState>			_alphaEnableBlendingState;
	ComPtr<ID3D11BlendState>			_alphaDisableBlendingState;
	
	ComPtr<ID3D11RasterizerState>		_rasterState;
	D3D11_RASTERIZER_DESC				_rasterDesc;

	ComPtr<ID3D11SamplerState>			_sampleState;
	D3D11_SAMPLER_DESC					_sampleDesc;

	int									_videoCardMemory;
	char								_videoCardDescription[128];
};

