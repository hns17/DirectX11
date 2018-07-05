
#include "stdafx.h"
#include "dxGameTool.h"
#include "LoadModelStateDlg.h"
#include "afxdialogex.h"


// LoadModelStateDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(LoadModelStateDlg, CDialogEx)

LoadModelStateDlg::LoadModelStateDlg(BOOL isAniInfo, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOADMODEL_STATE, pParent), _isAniInfo(isAniInfo)
{

}

LoadModelStateDlg::~LoadModelStateDlg()
{
}


void LoadModelStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LMS_HIERARCHY_CHECK, _checkHierarchy);
	DDX_Control(pDX, IDC_LMS_FLIPUV_CHECK, _checkFlipUV);
	DDX_Control(pDX, IDC_LMS_FLIPWIND_CHECK, _checkFlipWind);
	DDX_Control(pDX, IDC_LMS_LEFTHAND_CHECK, _checkConvertLeftHand);
	DDX_Control(pDX, IDC_LMS_GENNORMAL_CHECK, _checkGenNormal);
}


BEGIN_MESSAGE_MAP(LoadModelStateDlg, CDialogEx)
	ON_BN_CLICKED(IDC_LMS_OK, &LoadModelStateDlg::OnBnClickedLmsOk)
END_MESSAGE_MAP()

BOOL LoadModelStateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	_aiFlag = 0;

	//컨트롤 초기화
	_checkHierarchy.SetCheck(FALSE);
	_checkFlipUV.SetCheck(TRUE);
	_checkConvertLeftHand.SetCheck(TRUE);
	_checkGenNormal.SetCheck(TRUE);
	_checkFlipWind.SetCheck(TRUE);

	//애니메이션 정보만 로드하는 경우
	if (_isAniInfo) {
		_checkFlipUV.SetCheck(FALSE);
		_checkFlipWind.SetCheck(FALSE);
		_checkGenNormal.EnableWindow(FALSE);
		_checkHierarchy.EnableWindow(FALSE);
	}


	_aiFlag = aiProcess_Triangulate | aiProcess_OptimizeMeshes;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void LoadModelStateDlg::OnBnClickedLmsOk()
{
	//메쉬 정보를 제외한 계층&애니 정보제거
	if (_checkHierarchy.GetCheck()) {
		_aiFlag |= aiProcess_PreTransformVertices;
	}
	//texture inverse y
	if (_checkFlipUV.GetCheck()) {
		_aiFlag |= aiProcess_FlipUVs;
	}
	//inveres winding, default : ccw
	if (_checkFlipWind.GetCheck()) {
		_aiFlag |= aiProcess_FlipWindingOrder;
	}
	//convert left hand
	if (_checkConvertLeftHand.GetCheck()) {
		_aiFlag |= aiProcess_MakeLeftHanded;
	}
	//create normal
	if (_checkGenNormal.GetCheck()) {
		_aiFlag |= aiProcess_GenNormals | aiProcess_CalcTangentSpace;
	}

	CDialog::OnOK();
}
