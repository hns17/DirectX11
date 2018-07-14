#include "stdafx.h"
#include "DeviceManager.h"


DeviceManager::DeviceManager()
{

}

bool DeviceManager::InitDepthState()
{

	D3D11_DEPTH_STENCIL_DESC depthEnabledStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthEnabledStencilDesc, sizeof(depthEnabledStencilDesc));

	// Set up the description of the stencil state.
	depthEnabledStencilDesc.DepthEnable = true;
	depthEnabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthEnabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthEnabledStencilDesc.StencilEnable = true;
	depthEnabledStencilDesc.StencilReadMask = 0xFF;
	depthEnabledStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthEnabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthEnabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthEnabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthEnabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthEnabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthEnabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthEnabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthEnabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	// Create the depth stencil state.
	bool result = _device->CreateDepthStencilState(&depthEnabledStencilDesc, _depthEnabledStencilState.GetAddressOf());
	if (FAILED(result)) { return false; }


	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = _device->CreateDepthStencilState(&depthDisabledStencilDesc, _depthDisabledStencilState.GetAddressOf());
	if (FAILED(result)) { return false; }

	return true;
}

bool DeviceManager::InitDepthBuffer(PWindowRender pWR, ID3D11DeviceContext* dc, int screenWidth, int screenHeight)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	//========== 깊이 버퍼 생성 =========//
	// Set the depth stencil state.

	auto depthEnabledStencilState = _depthEnabledStencilState.Get();
	dc->OMSetDepthStencilState(depthEnabledStencilState, 1);

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	bool result = _device->CreateTexture2D(&depthBufferDesc, NULL, &pWR->depthStencilBuffer);
	if (FAILED(result)) { return false; }


	//========== 깊이 뷰 생성 =========//
	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = _device->CreateDepthStencilView(pWR->depthStencilBuffer, &depthStencilViewDesc, &pWR->depthStencilView);
	if (FAILED(result)) { return false;	}


	return true;
}

bool DeviceManager::InitAlphaBlend()
{
	bool result;
	D3D11_BLEND_DESC blendStateDescription;

	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = _device->CreateBlendState(&blendStateDescription, _alphaEnableBlendingState.GetAddressOf());
	if (FAILED(result)) { return false; }

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.	
	result = _device->CreateBlendState(&blendStateDescription, _alphaDisableBlendingState.GetAddressOf());
	if (FAILED(result)) { return false; }

	return true;
}

bool DeviceManager::InitRasterize()
{

	// Setup the raster description which will determine how and what polygons will be drawn.
	_rasterDesc.AntialiasedLineEnable = false;
	_rasterDesc.DepthBias = 0;
	_rasterDesc.DepthBiasClamp = 0.0f;
	_rasterDesc.DepthClipEnable = true;
	_rasterDesc.CullMode = D3D11_CULL_BACK;
	_rasterDesc.FillMode = D3D11_FILL_SOLID;
	_rasterDesc.FrontCounterClockwise = false;
	_rasterDesc.MultisampleEnable = false;
	_rasterDesc.ScissorEnable = false;
	_rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(_device->CreateRasterizerState(&_rasterDesc, _rasterState.GetAddressOf()))) {
		return false;
	}

	return true;
}

void DeviceManager::SetRasterState(ID3D11DeviceContext* dc)
{
	dc->RSSetState(_rasterState.Get());
}

void DeviceManager::SetSamplerState(ID3D11DeviceContext * dc)
{
	dc->PSSetSamplers(0, 1, _sampleState.GetAddressOf());
}

bool DeviceManager::InitSamplerState()
{
	
	// Create a texture sampler state description.
	_sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	_sampleDesc.MipLODBias = 0.0f;
	_sampleDesc.MaxAnisotropy = 1;
	_sampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	_sampleDesc.BorderColor[0] = 0;
	_sampleDesc.BorderColor[1] = 0;
	_sampleDesc.BorderColor[2] = 0;
	_sampleDesc.BorderColor[3] = 0;
	_sampleDesc.MinLOD = 0;
	_sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (!ChangeSamplerAddrees(D3D11_TEXTURE_ADDRESS_WRAP)) {
		return false;
	}
		


	return true;
}



//디바이스 생성
HRESULT DeviceManager::CreateDevice(int screenWidth, int screenHeight)
{
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	size_t stringLength;
	_vsyncEnabled = true;

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory))) {
		return false;
	}

	if (FAILED(factory->EnumAdapters(0, &adapter))) {
		return false;
	}

	// Enumerate the primary adapter output (monitor)
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput))) {
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) {
		return false;
	}

	// Now fill the display mode list structures.
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList))) {
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				_numerator = displayModeList[i].RefreshRate.Numerator;
				_denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	if (FAILED(adapter->GetDesc(&adapterDesc))) {
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	
	error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, 
									D3D11_SDK_VERSION, _device.GetAddressOf(), &featureLevel, _deviceContext.GetAddressOf()))) {
		return false;
	}


	if (!InitDepthState()) {
		return false;
	}

	if (FAILED(InitAlphaBlend())) {
		return false;
	}


	if (FAILED(InitRasterize())) {
		return false;
	}

	if (FAILED(InitSamplerState())) {
		return false;
	}
	
	return S_OK;
}

PWindowRender DeviceManager::InitRenderScreen(HWND hWnd, ID3D11DeviceContext* dc, int screenWidth, int screenHeight, float screenNear, float screenDepth)
{

	PWindowRender pWR = new WindowRender;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ComPtr<ID3D11Texture2D> backBufferPtr = nullptr;

	

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (_vsyncEnabled) {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = _numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = _denominator;
	}
	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hWnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode, false면 전체화면
	swapChainDesc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;
	

	IDXGIFactory* factory;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory))) {
		return false;
	}
	

	if (FAILED(factory->CreateSwapChain(_device.Get(), &swapChainDesc, &pWR->swapChain))) {
		SAFE_DELETE(pWR);
		return pWR;
	}
	
	factory->Release();


	// Get the pointer to the back buffer.
	if (FAILED(pWR->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBufferPtr.GetAddressOf()))) {
		SAFE_DELETE(pWR);
		return pWR;
	}

	// Create the render target view with the back buffer pointer.
	if (FAILED(_device->CreateRenderTargetView(backBufferPtr.Get(), NULL, &pWR->renderTargetView))) {
		SAFE_DELETE(pWR);
		return pWR;
	}

	if (!InitDepthBuffer(pWR, dc, screenWidth, screenHeight)) {
		SAFE_DELETE(pWR);
		return pWR;
	}

	//뷰포트 정보 구성
	pWR->viewport.Width = (float)screenWidth;
	pWR->viewport.Height = (float)screenHeight;
	pWR->viewport.MinDepth = 0.0f;
	pWR->viewport.MaxDepth = 1.0f;
	pWR->viewport.TopLeftX = 0.0f;
	pWR->viewport.TopLeftY = 0.0f;

	
	//각 윈도우에 맞는 월드, 투영 정보 생성
	float fieldOfView, screenAspect;
	fieldOfView = (float)XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	pWR->projectionMatrix	= XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	pWR->worldMatrix		= XMMatrixIdentity();
	pWR->orthoMatrix		= XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);


	return pWR;
}

HRESULT DeviceManager::SetDeferredContext(ID3D11DeviceContext** defContext)
{
	return _device->CreateDeferredContext(0, defContext);
}





void DeviceManager::ChangeFillMode(D3D11_FILL_MODE mode)
{
	_rasterDesc.FillMode = mode;
	
	if (FAILED(_device->CreateRasterizerState(&_rasterDesc, _rasterState.GetAddressOf()))) {
		return;
	}

}

void DeviceManager::ChangeCullMode(D3D11_CULL_MODE mode)
{
	_rasterDesc.CullMode = mode;

	if (FAILED(_device->CreateRasterizerState(&_rasterDesc, _rasterState.GetAddressOf()))) {
		return;
	}

}

bool DeviceManager::ChangeSamplerAddrees(D3D11_TEXTURE_ADDRESS_MODE addMode)
{
	_sampleDesc.AddressU = addMode;
	_sampleDesc.AddressV = addMode;
	_sampleDesc.AddressW = addMode;

	
	if (FAILED(_device->CreateSamplerState(&_sampleDesc, _sampleState.GetAddressOf()))) {
		return false;
	}

	return true;
}



void DeviceManager::TurnOnAlphaBlending(ID3D11DeviceContext* dc)
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	dc->OMSetBlendState(_alphaEnableBlendingState.Get(), blendFactor, 0xffffffff);
}

void DeviceManager::TurnOffAlphaBlending(ID3D11DeviceContext* dc)
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	dc->OMSetBlendState(_alphaDisableBlendingState.Get(), blendFactor, 0xffffffff);
}

void DeviceManager::TurnZBufferOn(ID3D11DeviceContext * dc)
{
	dc->OMSetDepthStencilState(_depthEnabledStencilState.Get(), 1);
}

void DeviceManager::TurnZBufferOff(ID3D11DeviceContext * dc)
{
	dc->OMSetDepthStencilState(_depthDisabledStencilState.Get(), 1);
}


void DeviceManager::BeginScene(PWindowRender pWR, ID3D11DeviceContext* dc, float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	
	dc->RSSetViewports(1, &pWR->viewport);
	// Bind the render target view and depth stencil buffer to the output render pipeline.

 	dc->OMSetRenderTargets(1, &pWR->renderTargetView, pWR->depthStencilView);

	// Clear the back buffer.
	dc->ClearRenderTargetView(pWR->renderTargetView, color);
										   
	// Clear the depth buffer.			   
	dc->ClearDepthStencilView(pWR->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void DeviceManager::EndScene(PWindowRender pWR)
{
	// Present the back buffer to the screen since rendering is complete.
	if (_vsyncEnabled)
	{
		// Lock to screen refresh rate.
		pWR->swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		pWR->swapChain->Present(0, 0);
	}
}


void DeviceManager::ExcuteCommand(ID3D11DeviceContext * defContext)
{
	if (!defContext)
		return;

	ID3D11CommandList* cmdList = nullptr;
	if (SUCCEEDED(defContext->FinishCommandList(false, &cmdList))) {
		_deviceContext.Get()->ExecuteCommandList(cmdList, false);
		SAFE_RELEASE(cmdList);
	}
}


XMFLOAT3 DeviceManager::ConvertScreenCoordinate(PWindowRender pWR, POINT ptWinPos, XMVECTOR cameraPos, bool isDepthBase)
{
	
	////원근좌표 변환
	//XMFLOAT3 coord = glm::unProject(glm::vec3(win_pos.x, win_pos.y, 0.f), view, proj, viewport);


	XMVECTOR coord = XMVector3Unproject(XMLoadFloat3(&XMFLOAT3((float)ptWinPos.x, (float)ptWinPos.y, 0)), 0, 0, WINSIZEX, WINSIZEY, 0, 1, pWR->projectionMatrix, pWR->viewMatrix, pWR->worldMatrix);

	//카메라시점과 마우스 위치를 이용해 중심점 위치 계산
	XMVECTOR view_dir = coord - cameraPos;

	
	float t = 1.f;
	if (isDepthBase)	//깊이 기반으로 그린다.
		t = (-XMVectorGetY(cameraPos)) / XMVectorGetY(view_dir);
	else				//높이 기반으로 그린다.
		t = (-XMVectorGetZ(cameraPos)) / XMVectorGetZ(view_dir);
	
	XMFLOAT3 res;
	XMVECTOR test = (view_dir * t);
	test += cameraPos;
	XMStoreFloat3(&res, (view_dir * t) + cameraPos);

	return res;
}

