#include "stdafx.h"
#include "dxGameTool.h"
#include "dlg\RenderStateDlg.h"
#include "afxdialogex.h"


// RenderStateDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(RenderStateDlg, CDialogEx)

RenderStateDlg::RenderStateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_RENDER_STATE, pParent)
{
	Create(IDD_DLG_RENDER_STATE);
}

RenderStateDlg::~RenderStateDlg()
{
}

void RenderStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RenderStateDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RENDERDLG_RENDERMODE_SOLID_RADIO, &RenderStateDlg::OnBnClickedRenderdlgRendermodeSolidRadio)
	ON_BN_CLICKED(IDC_RENDERDLG_RENDERMODE_WIREFRAME_RADIO, &RenderStateDlg::OnBnClickedRenderdlgRendermodeWireframeRadio)
	ON_BN_CLICKED(IDC_RENDERDLG_CULLMODE_NONE_RADIO, &RenderStateDlg::OnBnClickedRenderdlgCullmodeNoneRadio)
	ON_BN_CLICKED(IDC_RENDERDLG_CULLMODE_FRONTFACE_RADIO, &RenderStateDlg::OnBnClickedRenderdlgCullmodeFrontfaceRadio)
	ON_BN_CLICKED(IDC_RENDERDLG_CULLMODE_BACKFACE_RADIO, &RenderStateDlg::OnBnClickedRenderdlgCullmodeBackfaceRadio)
END_MESSAGE_MAP()

BOOL RenderStateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	((CButton*)GetDlgItem(IDC_RENDERDLG_CULLMODE_BACKFACE_RADIO))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RENDERDLG_RENDERMODE_SOLID_RADIO))->SetCheck(true);

	return TRUE;  
}

//Set Solid Mode Render
void RenderStateDlg::OnBnClickedRenderdlgRendermodeSolidRadio()
{
	DEVICEMANAGER.ChangeFillMode(D3D11_FILL_SOLID);
}

//Set Wire Mode Render
void RenderStateDlg::OnBnClickedRenderdlgRendermodeWireframeRadio()
{
	DEVICEMANAGER.ChangeFillMode(D3D11_FILL_WIREFRAME);
}


//Set Off Face Culling
void RenderStateDlg::OnBnClickedRenderdlgCullmodeNoneRadio()
{
	DEVICEMANAGER.ChangeCullMode(D3D11_CULL_NONE);
}

//Set Front Face Culling
void RenderStateDlg::OnBnClickedRenderdlgCullmodeFrontfaceRadio()
{
	DEVICEMANAGER.ChangeCullMode(D3D11_CULL_FRONT);
}

//Set Back Face Culling
void RenderStateDlg::OnBnClickedRenderdlgCullmodeBackfaceRadio()
{
	DEVICEMANAGER.ChangeCullMode(D3D11_CULL_BACK);
}

