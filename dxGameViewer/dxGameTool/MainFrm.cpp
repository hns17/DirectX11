
// MainFrm.cpp: CMainFrame 클래스의 구현

#include "stdafx.h"
#include "MainFrm.h"


const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (FAILED(Init()))
		return -1;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	
	cs.x = 80;		cs.y = 20;
	cs.cx = WINSIZEX;	cs.cy = WINSIZEY;

	
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;


	return TRUE;
}


//==================================================================
//		## system init ## Init()
//==================================================================
HRESULT CMainFrame::Init()
{
	_hWnd = this->m_hWnd;

	if(FAILED(InitManager()))
		return E_FAIL;

	_dc = DEVICEMANAGER.GetDeviceContext();

	//create mainGame Object
	_mg = make_unique<MainGame>();
	_mg->Init();
	
	//create Dlg
	_modelDlg.Create(IDD_DLG_MODEL, this);
	_modelDlg.ShowWindow(SW_SHOW);

	_renderStateDlg.Create(IDD_DLG_RENDER_STATE, this);
	_renderStateDlg.ShowWindow(SW_SHOW);

	_textureDlg.Create(IDD_DLG_TEXTURE, this);
	_textureDlg.ShowWindow(SW_SHOW);

	//set camera State
	_mainCam.SetPosition(0.0f, 1.0f, -10.0f);
	_mouseEvent = false;

	return S_OK;
}

//==================================================================
//		## system main loop ## MainLoop()
//==================================================================
void CMainFrame::MainLoop()
{
	if (!_mg) {	return;	}

	//camera update
	UpdateMainCamera();	

	//key event
	UpdateMainFrameKeyEvent();

	//print fps title
	CString frame;
	frame.Format(L"%d", TIMEMANAGER.GetFrameRate());
	SetWindowText(frame);

	_mg->Update();
	_mg->Render(_dc);

}


//==================================================================
//		## release system  ## Release()
//==================================================================
void CMainFrame::Release()
{

	//Manager Release
	SCENEMANAGER.Release();
	RM_SHADER.ClearResource();
	RM_MODEL.ClearResource();
	RM_TEXTURE.ClearResource();
	
	//Destroy Dlg
	_modelDlg.DestroyWindow();
	_renderStateDlg.DestroyWindow();
	_textureDlg.DestroyWindow();

}


//==================================================================
//		##  MainCamera Event  ## UpdateMainCamera()
//==================================================================
void CMainFrame::UpdateMainCamera()
{
	float duration = TIMEMANAGER.GetElapedTime();
	_mainCam.KeyboardInput(duration);

	if (_mouseEvent) {
		_mainCam.MouseInput(duration);
	}
}


//==================================================================
//		##  Key Event  ## UpdateMainFrameKeyEvent()
//==================================================================
void CMainFrame::UpdateMainFrameKeyEvent()
{
	if (KEYMANAGER.IsOnceKeyDown(VK_F2)) {
		if (!_modelDlg.IsWindowVisible()) {
			_modelDlg.ShowWindow(SW_SHOW);
			_textureDlg.ShowWindow(SW_SHOW);
			_renderStateDlg.ShowWindow(SW_SHOW);
		}
		else {
			_modelDlg.ShowWindow(SW_HIDE);
			_textureDlg.ShowWindow(SW_HIDE);
			_renderStateDlg.ShowWindow(SW_HIDE);
		}
	}

	if (KEYMANAGER.IsOnceKeyDown(VK_F3)) {
		DEVICEMANAGER.TurnVSync();
	}

}


//==================================================================
//		## Create Manager ## InitManager()
//==================================================================
HRESULT CMainFrame::InitManager()
{
	if (FAILED(DEVICEMANAGER.CreateDevice(WINSIZEX, WINSIZEY))) {
		AfxMessageBox(L"Failed Init Device!");
		return E_FAIL;
	}

	if (FAILED(KEYMANAGER.Init())) {
		AfxMessageBox(L"Failed Init KEY_MANAGER!");
		return E_FAIL;
	}

	if (FAILED(TIMEMANAGER.Init())) {
		AfxMessageBox(L"Failed Init TIME_MANAGER!");
		return E_FAIL;
	}

	if (FAILED(SCENEMANAGER.Init())) {
		AfxMessageBox(L"Failed Init SCENE_MANAGER!");
		return E_FAIL;
	}


	ID3D11Device* device = DEVICEMANAGER.GetDevice();

	if (FAILED(RM_SHADER.Init(device))) {
		AfxMessageBox(L"Failed Init SCENE_MANAGER!");
		return E_FAIL;
	}

	if (FAILED(FONTMANAGER.Init())) {
		AfxMessageBox(L"Failed Init FONT_MANAGER!");
		return E_FAIL;
	}
	
	return S_OK;
}



//==================================================================
//		## Process Message ## OnWndMsg()
//==================================================================
BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	//Load Model
	if (message == WM_INSERT_MODEL) {
		_textureDlg.SendMessage(WM_INSERT_MODEL, NULL, NULL);
	} 
	//Delete Model
	else if (message == WM_DELETE_MODEL) {
		_textureDlg.SendMessage(WM_DELETE_MODEL, wParam, NULL);
	} 

	return CFrameWnd::OnWndMsg(message, wParam, lParam, pResult);
}


void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point)
{
	_mouseEvent = true;
	CFrameWnd::OnRButtonDown(nFlags, point);
}


void CMainFrame::OnRButtonUp(UINT nFlags, CPoint point)
{
	_mouseEvent = false;
	CFrameWnd::OnRButtonUp(nFlags, point);
}
