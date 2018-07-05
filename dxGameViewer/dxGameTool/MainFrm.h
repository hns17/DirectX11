//====================================================================
//			## MainFrm.h ## (CMainFrame 클래스)
//====================================================================

#pragma once
#include "scene\mainGame.h"

#include "dlg\ModelDlg.h"
#include "dlg\TextureDlg.h"
#include "dlg\RenderStateDlg.h"


class CMainFrame : public CFrameWnd
{
	//msg event
	DECLARE_DYNAMIC(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
	
public:
	//Default Func
	CMainFrame();
	virtual ~CMainFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	//Msg Process 
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);


public:
	//System Init
	HRESULT Init();

	//System Main Event
	void MainLoop();

	//System Release
	void Release();

private:
	//Camera Update
	void UpdateMainCamera();

	//Key Interrupt
	void UpdateMainFrameKeyEvent();

	//Init Singltone Class
	HRESULT	InitManager();

private:
	//MainGame 클래스			
	unique_ptr<MainGame>	_mg = NULL;

	//DC
	ID3D11DeviceContext*	_dc = NULL;

	//다이얼로그 클래스
	ModelDlg				_modelDlg;				
	TextureDlg				_textureDlg;
	RenderStateDlg			_renderStateDlg;
	
	//mouse Flag
	BOOL					_mouseEvent;
};


