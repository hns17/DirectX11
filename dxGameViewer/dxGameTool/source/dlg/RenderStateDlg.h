//====================================================================
//			## RenderStateDlg ## (DX11 Device Render Control)
//====================================================================
#pragma once

class RenderStateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RenderStateDlg)

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RENDER_STATE };
#endif

public:
	RenderStateDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~RenderStateDlg();



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	//메시지 이벤트
	afx_msg void OnBnClickedRenderdlgRendermodeSolidRadio();
	afx_msg void OnBnClickedRenderdlgRendermodeWireframeRadio();

	afx_msg void OnBnClickedRenderdlgCullmodeNoneRadio();
	afx_msg void OnBnClickedRenderdlgCullmodeFrontfaceRadio();
	afx_msg void OnBnClickedRenderdlgCullmodeBackfaceRadio();
	DECLARE_MESSAGE_MAP()
};
