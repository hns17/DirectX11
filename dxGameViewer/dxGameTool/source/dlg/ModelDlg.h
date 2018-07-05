//====================================================================
//			## ModelDlg ## (Model Management Dlg)
//====================================================================
#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "CustomTreeCtrl.h"


// ModelDlg 대화 상자입니다.

class ModelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ModelDlg)

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MODEL };
#endif

/***************************************** 멤버 함수 ******************************************/
public:
	ModelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ModelDlg();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	//popup menu event
	afx_msg void OnLoadModel();
	afx_msg void OnLoadAnimation();
	afx_msg void OnDeleteModel();
	
	//tree & list ctrl event
	afx_msg void OnNMClickModelDlgModelListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickModeldlgModelListctrl(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMClickModeldlgAniListctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickModeldlgAniListctrl(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMClickModeldlgObjectTree(NMHDR *pNMHDR, LRESULT *pResult);

	//spin ctrl event
	afx_msg void OnDeltaposModeldlgPosxSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposModeldlgPosySpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposModeldlgPoszSpin(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnDeltaposModeldlgRotxSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposModeldlgRotySpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposModeldlgRotzSpin(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnDeltaposModeldlgSizexSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposModeldlgSizeySpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposModeldlgSizezSpin(NMHDR *pNMHDR, LRESULT *pResult);

	
	
	//check box event
	afx_msg void OnBnClickedModeldlgModelNormalizeCheck();
	afx_msg void OnBnClickedModeldlgModelAnirepeatCheck();

	//reset btn event
	afx_msg void OnBnClickedButton1();

	//애니메이션 BTN Event
	afx_msg void OnBnClickedModeldlgAniplayBtn();
	afx_msg void OnBnClickedModeldlgAnipauseBtn();
	afx_msg void OnBnClickedModeldlgAnistopBtn();
	
	DECLARE_MESSAGE_MAP()

private:
	void InsertModel(CString name, CString path, SkinModel* model);		//리스트 컨트롤에 모델을 추가한다.
	void UpdateControl();												//다이얼로그에 배치된 컨트롤 윈도우를 업데이트 한다.
	void UpdateTreeControl();											//트리 컨트롤에 선택된 모델의 구조를 보여준다.
	void UpdateTreeControlCheckEvent(HTREEITEM hItem);					//트리 컨트롤 아이템의 체크 박스 클릭시 발생하는 이벤트를 처리한다.
	void UpdateAnimationListCtrl();										//애니메이션 컨트롤 업데이트

/***************************************** 멤버 변수 ******************************************/
private:
	//현재 선택된 모델
	SkinModel*								_currentModel;

	//컨트롤 변수
	CButton									_checkModelNormalize;
	CButton									_checkAniRepeat;
	
	CListCtrl								_lstModelList;	
	CListCtrl								_lstAniList;
	
	CCustomTreeCtrl							_treeObjList;
	
	CEdit									_editModelPositionX;
	CEdit									_editModelPositionY;
	CEdit									_editModelPositionZ;
	CEdit									_editModelRotationX;
	CEdit									_editModelRotationY;
	CEdit									_editModelRotationZ;
	CEdit									_editModelSizeX;
	CEdit									_editModelSizeY;
	CEdit									_editModelSizeZ;
};
