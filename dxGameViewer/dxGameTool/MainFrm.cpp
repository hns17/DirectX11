
// MainFrm.cpp: CMainFrame 클래스의 구현

#include "stdafx.h"
#include "MainFrm.h"


const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
	Release();
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
	
	DEVICEMANAGER.SetDeferredContext(&_mainDC);

	//create mainGame Object
	_mg = make_unique<MainGame>();
	_mg->Init(_mainDC);
	
	//set camera State
	_mainCam.SetPosition(0.0f, 1.0f, -10.0f);
	_mouseEvent = false;


	//Dlg를 독립적으로 처리할 UI 쓰레드 생성
	_dlgUIThread = AfxBeginThread(RUNTIME_CLASS(DlgThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	_hUIThread = _dlgUIThread->m_hThread;


	_dlgUIThread->ResumeThread();

	
	return S_OK;
}

//==================================================================
//		## system main loop ## MainLoop()
//==================================================================
void CMainFrame::MainLoop()
{
	if (!_mg) {	return;	}

	//camera move update
	UpdateMainCamera();	
	UpdateSystemKeyEvent();

	_mg->Update();
	_mg->Render(_mainDC);

	
}


//==================================================================
//		## release system  ## Release()
//==================================================================
void CMainFrame::Release()
{
	//UI 쓰레드를 종료한다.
	_dlgUIThread->PostThreadMessageW(WM_QUIT,0,0);

	//안전하게 종료하기 위해 완전히 종료될때 까지 기다린다.
	::WaitForSingleObject(_hUIThread, INFINITE);

	SAFE_RELEASE(_mainDC);

	//Manager Release
	SCENEMANAGER.Release();
	RM_SHADER.ClearResource();
	RM_MODEL.ClearResource();
	RM_TEXTURE.ClearResource();
	
}


//==================================================================
//		##  MainCamera Event  ##
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
//		##  SystemKey Event								##
//		##	MainWnd에 포커스가 없는 상태에도 처리한다.	##
//==================================================================
void CMainFrame::UpdateSystemKeyEvent()
{
	if (KEYMANAGER.IsOnceKeyDown(VK_F2)) {
		_dlgUIThread->PostThreadMessageW(WM_VISIBLE_DLG, 0, 0);
	}
	else if (KEYMANAGER.IsOnceKeyDown(VK_F3)) {
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
	if (message == WM_UPDATE_COMMAND) {
		DEVICEMANAGER.ExcuteCommand((ID3D11DeviceContext*)lParam);
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
