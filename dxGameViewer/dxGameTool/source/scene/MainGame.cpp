#include "stdafx.h"
#include "MainGame.h"


PWindowRender GameNode::_sceneInfo = NULL;

//==================================================================
//		## 초기화 ## Init(void)
//==================================================================
HRESULT MainGame::Init(ID3D11DeviceContext* dc)
{
	_sceneInfo = DEVICEMANAGER.InitRenderScreen(_hWnd, dc, WINSIZEX, WINSIZEY, 0.1f, 1000.f);
	InitScene();

	return S_OK;
}


//==================================================================
//		## Scene 설정 ## InitScene(void)
//==================================================================
HRESULT MainGame::InitScene()
{
	//씬 추가 후 셋팅
	SCENEMANAGER.AddScene("test", new TestScene);
	SCENEMANAGER.ChangeScene("test");

	return S_OK;
}

//==================================================================
//		## 업데이트 ## Update(void)
//==================================================================
void MainGame::Update(void)
{
	TIMEMANAGER.Update(0.0f);
	SCENEMANAGER.Update();
	
	UpdateMainGameFont();

	_sceneInfo->viewMatrix = _mainCam.Update(TIMEMANAGER.GetElapedTime());
}


//==================================================================
//		## 렌더 ## render(void)
//==================================================================
void MainGame::Render(ID3D11DeviceContext* dc)
{	
	//Set Main DC State
	DEVICEMANAGER.SetRasterState(dc);
	DEVICEMANAGER.SetSamplerState(dc);

	DEVICEMANAGER.BeginScene(_sceneInfo, dc, 0, 0, 0, 1);
	
	//투영 및 뷰 행렬 설정
	RM_SHADER.SetShaderParameters(dc, _sceneInfo->viewMatrix, _sceneInfo->projectionMatrix);
	SCENEMANAGER.Render(dc);

	//투영행렬 : 직교행렬로 설정
	RM_SHADER.SetShaderParameters(dc, XMMatrixTranslation(0, 0, 1), _sceneInfo->orthoMatrix);
	FONTMANAGER.Render(dc);
	
	//command Excute
	DEVICEMANAGER.ExcuteCommand(dc);

	//SwapChain
	DEVICEMANAGER.EndScene(_sceneInfo);
}

void MainGame::UpdateMainGameFont()
{
	TCHAR buff[255];
	wsprintf(buff, L"프레임 비율 : %d", TIMEMANAGER.GetFrameRate());
	FONTMANAGER.Print(buff, 30, 80, 20, XMFLOAT3(1, 1, 0));


	if (DEVICEMANAGER.IsVSync()) {
		FONTMANAGER.Print(L"수직동기화(F3) : On", 30, 180);
	}
	else {
		FONTMANAGER.Print(L"수직동기화(F3) : Off", 30, 180);
	}
}

