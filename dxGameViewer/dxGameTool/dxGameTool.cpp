#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "dxGameTool.h"


CdxGameToolApp theApp;

CdxGameToolApp::CdxGameToolApp()
{
	
}

CdxGameToolApp::~CdxGameToolApp()
{
}

BOOL CdxGameToolApp::InitInstance()
{
	CWinAppEx::InitInstance();
	EnableTaskbarInteraction(FALSE);

	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	_pFrame = new CMainFrame;
	m_pMainWnd = _pFrame;

	if (!_pFrame) {	return FALSE;	}
		
	if (!_pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL)) {
		return FALSE;
	}
		
	
	_pFrame->ShowWindow(SW_SHOW);
	_pFrame->UpdateWindow();


	return TRUE;
}

int CdxGameToolApp::ExitInstance()
{
	return CWinAppEx::ExitInstance();
}



BOOL CdxGameToolApp::OnIdle(LONG lCount)
{
	_pFrame->MainLoop();
	return CWinAppEx::OnIdle(lCount);
}


int CdxGameToolApp::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			OnIdle(NULL);
		}
	}


	return CWinAppEx::Run();
}
