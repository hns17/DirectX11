//====================================================================
//			## LoadModelStateDlg ## (Set Load Model Option)
//====================================================================
#pragma once
#include "afxwin.h"


// LoadModelStateDlg 대화 상자입니다.
class LoadModelStateDlg : public CDialogEx
{

	DECLARE_DYNAMIC(LoadModelStateDlg)

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LOADMODEL_STATE };
#endif

/******************************************** 멤버 함수 *****************************************/
public:
	LoadModelStateDlg(BOOL isAniInfo = FALSE, CWnd* pParent = NULL);	// 표준 생성자입니다.
	virtual ~LoadModelStateDlg();

	//상태에 맞게 assimp 상태 flag 반환
	UINT	GetAiState()	{	return _aiFlag;	 };

protected:
	//다이얼로그 이벤트 함수
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	

	afx_msg void OnBnClickedLmsOk();
	DECLARE_MESSAGE_MAP()

/******************************************** 멤버 변수 *****************************************/
private:
	//컨트롤 변수
	CButton _checkHierarchy;
	CButton _checkFlipUV;
	CButton _checkFlipWind;
	CButton _checkConvertLeftHand;
	CButton _checkGenNormal;

	UINT	_aiFlag;		//assimp model load flag
	BOOL	_isAniInfo;		//only animation load

};
