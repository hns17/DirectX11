//====================================================================
//			## TextureDlg ## (Texture Management Dlg)
//====================================================================
#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "model\SimpleTextureModel.h"
#include "model\SkinModel.h"


class TextureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TextureDlg)

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TEXTURE };
#endif
/*************************************** 멤버 함수 *************************************/
public:
	TextureDlg(CWnd* pParent = NULL);  
	virtual ~TextureDlg();

protected:
	//다이얼로그 가상함수
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//다이얼로그 메시지 함수
	afx_msg void OnPaint();
	afx_msg void OnInsertInserttexture();
	afx_msg void OnDeleteDeletetexture();

	//텍스쳐 리스트 컨트롤 메시지 이벤트
	afx_msg void OnNMClickTexdlgTextureListctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTexdlgTextureListctrl(NMHDR *pNMHDR, LRESULT *pResult);
	
	//모델 콤보박스 드롭다운 메시지 이벤트
	afx_msg void OnCbnDropdownTexdlgComboModellist();
	afx_msg void OnCbnSelchangeTexdlgComboModellist();

	//OBJ TREE CTRL 이벤트
	afx_msg void OnNMClickTexdlgTreeObjlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTexdlgTreeObjlist(NMHDR *pNMHDR, LRESULT *pResult);

	//텍스쳐 콤보 박스 이벤트
	afx_msg void OnCbnDropdownTexdlgComboDiffuse();
	afx_msg void OnCbnDropdownTexdlgComboAlpha();
	afx_msg void OnCbnDropdownTexdlgComboBump();
	afx_msg void OnCbnSelchangeTexdlgComboDiffuse();
	afx_msg void OnCbnSelchangeTexdlgComboAlpha();
	afx_msg void OnCbnSelchangeTexdlgComboBump();
	

	DECLARE_MESSAGE_MAP()

private:
	void UpdateTextureList();						//텍스쳐 리스트 업데이트 이벤트
	void UpdateSelectTexItem();						//텍스쳐 선택 이벤트
	void UpdateObjList();							//모델의 오브젝트 정보 업데이트
	void UpdateObjInfo();							//선택된 오브젝트 정보를 업데이트

	//콤보박스 드롭다운 시 리스트 업데이트
	void UpdateCmbDropDownEvent(CComboBox* pCmb,	vector<CString> itmList);		



/************************************** 멤버 변수 ***************************************/
private:
	PWindowRender				_pWindow;			//다이얼로그 윈도우에서 사용될 DX11 렌더 정보
	XMFLOAT3					_bgColor;			//다이얼로그 배경색상
	SimpleTextureModel*			_textureImage;		//현재 선택된 텍스쳐를 표시하기위한 Square 모델
	SkinModel*					_currentModel;
	HierarchyMesh*				_currentMesh;			

	//컨트롤 윈도우 변수							
	CListCtrl					_lstTexInfo;		//텍스쳐 리스트를 보여주는 리스트 컨트롤과 연결된 변수
	CCustomTreeCtrl				_treeObjInfo;		//오브젝트 정보를 보여주는 트리 컨트롤과 연결된 변수
	CComboBox					_cmbModelList;		//모델 리스트 선택하는 콤보박스와 연결된 변수
													
	CComboBox					_cmbDiffuseMap;		//디퓨즈 텍스쳐 콤보박스 연결 변수
	CComboBox					_cmbAlphaMap;		//알파 텍스쳐 콤보박스 연결 변수
	CComboBox					_cmbBumpMap;		//노말 텍스쳐 콤보박스 연결 변수

	ID3D11DeviceContext*		_dc;
};

