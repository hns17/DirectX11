#include "stdafx.h"
#include "dxGameTool.h"
#include "TextureDlg.h"
#include "afxdialogex.h"


// TextureDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(TextureDlg, CDialogEx)

TextureDlg::TextureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_TEXTURE, pParent)
{

	Create(IDD_DLG_TEXTURE);
}



TextureDlg::~TextureDlg()
{
}

void TextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//컨트롤 윈도우와 변수 연결
	DDX_Control(pDX, IDC_TEXDLG_TEXTURE_LISTCTRL, _lstTexInfo);
	DDX_Control(pDX, IDC_TEXDLG_COMBO_MODELLIST, _cmbModelList);
	DDX_Control(pDX, IDC_TEXDLG_TREE_OBJLIST, _treeObjInfo);
	DDX_Control(pDX, IDC_TEXDLG_COMBO_DIFFUSE, _cmbDiffuseMap);
	DDX_Control(pDX, IDC_TEXDLG_COMBO_ALPHA, _cmbAlphaMap);
	DDX_Control(pDX, IDC_TEXDLG_COMBO_BUMP, _cmbBumpMap);
}


BEGIN_MESSAGE_MAP(TextureDlg, CDialogEx)
	ON_WM_PAINT()
	ON_COMMAND(ID_INSERT_TEXTURE, &TextureDlg::OnInsertInserttexture)
	ON_COMMAND(ID_DELETE_TEXTURE, &TextureDlg::OnDeleteDeletetexture)
	ON_CBN_DROPDOWN(IDC_TEXDLG_COMBO_MODELLIST, &TextureDlg::OnCbnDropdownTexdlgComboModellist)
	ON_NOTIFY(NM_CLICK, IDC_TEXDLG_TEXTURE_LISTCTRL, &TextureDlg::OnNMClickTexdlgTextureListctrl)
	ON_NOTIFY(NM_RCLICK, IDC_TEXDLG_TEXTURE_LISTCTRL, &TextureDlg::OnNMRClickTexdlgTextureListctrl)
	ON_CBN_SELCHANGE(IDC_TEXDLG_COMBO_MODELLIST, &TextureDlg::OnCbnSelchangeTexdlgComboModellist)
	ON_NOTIFY(NM_CLICK, IDC_TEXDLG_TREE_OBJLIST, &TextureDlg::OnNMClickTexdlgTreeObjlist)
	ON_CBN_DROPDOWN(IDC_TEXDLG_COMBO_DIFFUSE, &TextureDlg::OnCbnDropdownTexdlgComboDiffuse)
	ON_CBN_DROPDOWN(IDC_TEXDLG_COMBO_ALPHA, &TextureDlg::OnCbnDropdownTexdlgComboAlpha)
	ON_CBN_DROPDOWN(IDC_TEXDLG_COMBO_BUMP, &TextureDlg::OnCbnDropdownTexdlgComboBump)
	ON_CBN_SELCHANGE(IDC_TEXDLG_COMBO_DIFFUSE, &TextureDlg::OnCbnSelchangeTexdlgComboDiffuse)
	ON_CBN_SELCHANGE(IDC_TEXDLG_COMBO_ALPHA, &TextureDlg::OnCbnSelchangeTexdlgComboAlpha)
	ON_CBN_SELCHANGE(IDC_TEXDLG_COMBO_BUMP, &TextureDlg::OnCbnSelchangeTexdlgComboBump)
	ON_NOTIFY(NM_DBLCLK, IDC_TEXDLG_TREE_OBJLIST, &TextureDlg::OnNMDblclkTexdlgTreeObjlist)
END_MESSAGE_MAP()


// TextureDlg 메시지 처리기입니다.



BOOL TextureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect winSize;
	this->GetWindowRect(&winSize);
	
	//텍스쳐 표시할 그룹 컨트롤 크기
	CRect rc;
	this->GetDlgItem(IDC_TEX_IMGFRAME_GROUP)->GetWindowRect(&rc);
	ScreenToClient(&rc);

	DEVICEMANAGER.SetDeferredContext(_defContext.GetAddressOf());

	//DX로 렌더링 할 윈도우와 스크린 설정
	_pWindow = DEVICEMANAGER.InitRenderScreen(m_hWnd, _defContext.Get(), winSize.Width(), winSize.Height(), 0.1f, 10.f);
	
	_pWindow->viewport.TopLeftX = (float)rc.left + 12;		_pWindow->viewport.TopLeftY = (float)rc.top + 14;
	_pWindow->viewport.Width = (float)rc.Width() - 2;		_pWindow->viewport.Height = (float)rc.Height() - 2;

	

	//컨트롤 윈도우 초기화
	_lstTexInfo.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	_lstTexInfo.InsertColumn(0, L"Name", LVCFMT_LEFT, 120);

	//다이얼로그 배경색 정보 초기화
	COLORREF color = GetSysColor(COLOR_3DFACE);		
	_bgColor = XMFLOAT3{ (float)GetRValue(color) / 255.f,	(float)GetGValue(color) / 255.f,	(float)GetBValue(color) / 255.f};
	

	_textureImage = new SimpleTextureModel;
	 
	//콤보박스 상태 초기화.
	_cmbDiffuseMap.EnableWindow(false);
	_cmbAlphaMap.EnableWindow(false);
	_cmbBumpMap.EnableWindow(false);

	UpdateTextureList();

	return TRUE;  			  
}


//새로 그릴 영역이 있을때만 호출된다. 
void TextureDlg::OnPaint()
{
	CPaintDC dc(this);

	auto defContext = _defContext.Get();
	
	//선택된 텍스쳐가 있으면 화면에 그려준다.
	DEVICEMANAGER.BeginScene(_pWindow, defContext, _bgColor.x, _bgColor.y, _bgColor.z, 1);

	//proj, view 매트릭스를 직교에 맞게 변환한다.
	XMMATRIX matIdentity = XMMatrixIdentity();
	RM_SHADER.SetShaderParameters(defContext, matIdentity, matIdentity);

	//알파블렌딩 활성화, 깊이 버퍼 해제
	DEVICEMANAGER.TurnZBufferOff(defContext);
	DEVICEMANAGER.TurnOnAlphaBlending(defContext);

	//렌더
	_textureImage->Render(defContext);
	
	DEVICEMANAGER.TurnOffAlphaBlending(defContext);
	DEVICEMANAGER.TurnZBufferOn(defContext);

	::SendMessage(_hWnd, WM_UPDATE_COMMAND, NULL, (LPARAM)defContext);
	DEVICEMANAGER.EndScene(_pWindow);
}


//모델 리스트 오른쪽 마우스 버튼 클릭 이벤트
//텍스쳐를 추가하거나, 삭제한다.
void TextureDlg::OnNMRClickTexdlgTextureListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	NM_LISTVIEW* pNMView = (NM_LISTVIEW*)pNMHDR;
	int index = pNMView->iItem;

	CPoint pt;
	GetCursorPos(&pt);

	CMenu menu, *pmenu;
	menu.LoadMenu(IDR_TEXDLG_TEXLIST_POPUP);
	pmenu = menu.GetSubMenu(0);

	//선택된 아이템이 있는 경우 삭제 메뉴를, 없는 경우 추가 메뉴를 Setting 한다.
	if (index < 0) {
		pmenu->EnableMenuItem(ID_DELETE_TEXTURE, MF_DISABLED | MF_GRAYED);
	}
	else {
		pmenu->EnableMenuItem(ID_DELETE_TEXTURE, MF_ENABLED);
		
	}

	//마우스 위치에 메뉴 생성
	pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	*pResult = 0;
}


//텍스쳐 추가 메뉴 선택 이벤트
void TextureDlg::OnInsertInserttexture()
{
	

	//파일 열기 공용 다이얼로그 호출
	CFileDialog dlg(TRUE); 
	if (dlg.DoModal() == IDOK) {

		if (!RM_TEXTURE.AddResource((wstring)dlg.GetPathName())) {
			//텍스쳐 로드 실패하면 메시지 박스 호출
			AfxMessageBox(L"Load Failed");		
		} else {			
			UpdateTextureList();
		}
	}
}


//텍스쳐 정보 변경시 정보를 업데이트 한다.
void TextureDlg::UpdateTextureList()
{
	//현재 아이템 전부 해제
	_lstTexInfo.DeleteAllItems();

	//텍스쳐 매니저에 등록된 리스트 가져와서 정보를 재구성한다.
	if (!RM_TEXTURE.GetResource().empty()) {
		map<wstring, Texture*> _texItemList = RM_TEXTURE.GetResource();
		map<wstring, Texture*>::iterator iterBegin = _texItemList.begin();
		map<wstring, Texture*>::iterator iterEnd = _texItemList.end();

		int index = 0;
		for (iterBegin; iterBegin != iterEnd; iterBegin++) {
			CString name = MYUTIL::getFileName(iterBegin->first.c_str());
			_lstTexInfo.InsertItem(index, name);
			index++;
		}
	}
	
	//선택된 텍스쳐 정보도 업데이트 한다.
	UpdateSelectTexItem();

}





//텍스쳐 리스트 선택 이벤트
void TextureDlg::OnNMClickTexdlgTextureListctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	//최근 선택한 텍스쳐 정보 갱신
	UpdateSelectTexItem();

	*pResult = 0;
}


//텍스쳐 삭제 메뉴 이벤트
//선택된 텍스쳐를 삭제한다.
void TextureDlg::OnDeleteDeletetexture()
{
	//삭제할 텍스쳐 정보를 가져온다.
	int index = _lstTexInfo.GetNextItem(-1, LVNI_SELECTED);
	Texture* tex = RM_TEXTURE.GetResourceData(index);

	//삭제되는 텍스쳐를 가진  모든 모델 탐색 후 null 초기화
	auto modelList = RM_MODEL.GetResource();

	for (auto data : modelList) {
		auto model = data.second;
		
		if (model == NULL) continue;
		
		for (auto& mData : model->GetMaterialList()) {
			if (tex == mData.diffuseMap) {
				mData.diffuseMap = NULL;
			}
			if (tex == mData.alphaMap) {
				mData.alphaMap = NULL;
			}
			if (tex == mData.normalMap) {
				mData.normalMap = NULL;
			}
		}
	}

	//텍스쳐 정보 삭제
	RM_TEXTURE.RemoveResource(RM_TEXTURE.GetResourceKey(index)->c_str());
	//텍스쳐 정보 업데이트
	UpdateTextureList();
	
}

//Texture Item Select Event
void TextureDlg::UpdateSelectTexItem()
{
	int index = _lstTexInfo.GetNextItem(-1, LVNI_SELECTED);

	//선택한 텍스쳐 목록이 없는 경우
	if (index < 0) { 
		_textureImage->SetTexture(NULL);
		SetDlgItemText(IDC_TEXDLG_PATH_EDIT, L"");
	}
	//텍스쳐 리스트에서 선택한 경우
	else {
		_textureImage->SetTexture(RM_TEXTURE.GetResourceData(index)->data);
		SetDlgItemText(IDC_TEXDLG_PATH_EDIT, RM_TEXTURE.GetResourceKey(index)->c_str());
	}

	Invalidate(FALSE); 
}

//오브젝트 트리 컨트롤 업데이트
void TextureDlg::UpdateObjList()
{
	_treeObjInfo.DeleteAllItems();

	if (!_currentModel) { return; }

	//선택된 모델 이름을 루트 노드로 등록
	HTREEITEM  hitem = _treeObjInfo.InsertItem(_currentModel->GetName().c_str(), TVI_ROOT, TVI_LAST);
	_treeObjInfo.SetCheck(hitem, true);

	for (auto& mesh : _currentModel->GetMeshList())
		_treeObjInfo.CreateItem(hitem, mesh->name.c_str(), (DWORD_PTR)mesh);
}


//오브젝트 정보 표시 컨트롤 업데이트
void TextureDlg::UpdateObjInfo()
{
	_cmbDiffuseMap.ResetContent();
	_cmbAlphaMap.ResetContent();
	_cmbBumpMap.ResetContent();

	_cmbDiffuseMap.EnableWindow(false);
	_cmbAlphaMap.EnableWindow(false);
	_cmbBumpMap.EnableWindow(false);
	
	if (!_currentMesh)		{	return; 	}
	if (_currentMesh->matIdx == -1) {	return;		}

	_cmbDiffuseMap.EnableWindow(true);
	/*_cmbAlphaMap.EnableWindow(true);
	_cmbBumpMap.EnableWindow(true);*/

	_cmbDiffuseMap.AddString(L"None");
	_cmbAlphaMap.AddString(L"None");
	_cmbBumpMap.AddString(L"None");

	_cmbDiffuseMap.SetCurSel(0);
	_cmbAlphaMap.SetCurSel(0);
	_cmbBumpMap.SetCurSel(0);

	bool flag[3] = { false, };
	int texCnt = _lstTexInfo.GetItemCount();
	
	for (int i = 0; i < texCnt; i++) {
		Texture* pTex = RM_TEXTURE.GetResourceData(i);
		CString texName = pTex->name.c_str();

		auto materialList = _currentModel->GetMaterialList();

		if (materialList.empty()) 	continue;
	

		auto materialInfo = materialList[_currentMesh->matIdx];

		if (materialInfo.diffuseMap == pTex) {
			_cmbDiffuseMap.AddString(texName);
			_cmbDiffuseMap.SetCurSel(1);
			flag[0] = true;
		}
		if (materialInfo.alphaMap == pTex) {
			_cmbAlphaMap.AddString(texName);
			_cmbAlphaMap.SetCurSel(1);
			flag[1] = true;
		}
		if (materialInfo.normalMap == pTex) {
			_cmbBumpMap.AddString(texName);
			_cmbBumpMap.SetCurSel(1);
			flag[2] = true;
		}

		if (flag[0] && flag[1] && flag[2]) {
			break;
		}
	}
}

//콤보박스 이벤트
void TextureDlg::UpdateCmbDropDownEvent(CComboBox * pCmb, vector<CString> itmList)
{
	CString selStr;
	int selIdx = pCmb->GetCurSel();

	if (selIdx >= 0) {
		pCmb->GetLBText(pCmb->GetCurSel(), selStr);
	}
	
	pCmb->ResetContent();

	for (int i = 0; i < itmList.size(); i++) {
		CString insertStr = itmList[i];
		pCmb->AddString(insertStr);

		if (selStr == insertStr) {
			pCmb->SetCurSel(i);
		}
	}

}



//트리 컨트롤에 표시된 아이템 선택 이벤트
void TextureDlg::OnNMClickTexdlgTreeObjlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem = _treeObjInfo.HitEvent();
	if (!hItem) {
		_currentMesh = NULL;
	}
	else {
		_treeObjInfo.SelectItem(hItem);
		_currentMesh = (HierarchyMesh*)_treeObjInfo.GetItemData(hItem);
	}

	UpdateObjInfo();
	Invalidate(FALSE);

	*pResult = 0;
}

//ModelList Combox Drop Event
void TextureDlg::OnCbnDropdownTexdlgComboModellist()
{
	vector<CString> itmList;
	vector<DWORD_PTR> modelData;

	int itmCnt = (int)RM_MODEL.GetResource().size();
	itmList.reserve(itmCnt + 1);
	modelData.reserve(itmCnt + 1);

	itmList.push_back(L"None");
	modelData.push_back(NULL);

	for (int i = 0; i < itmCnt; i++) {
		auto model = RM_MODEL.GetResourceData(i);
		itmList.push_back(model->GetName().c_str());
		modelData.push_back((DWORD_PTR)model);
	}

	UpdateCmbDropDownEvent(&_cmbModelList, itmList);

	for (int i = 0; i < itmCnt+1; i++) {
		_cmbModelList.SetItemData(i, modelData[i]);
	}
}

//텍스쳐 콤보 박스 드롭다운 이벤트
void TextureDlg::OnCbnDropdownTexdlgComboDiffuse()
{
	vector<CString> itmList;

	int itmCnt = _lstTexInfo.GetItemCount();
	itmList.reserve(itmCnt + 1);
	itmList.push_back(L"None");

	for (int i = 0; i < itmCnt; i++) {
		itmList.push_back(_lstTexInfo.GetItemText(i, 0));
	}

	UpdateCmbDropDownEvent(&_cmbDiffuseMap, itmList);
}

//알파맵 콤보박스 이벤트
void TextureDlg::OnCbnDropdownTexdlgComboAlpha()
{
	vector<CString> itmList;

	int itmCnt = _lstTexInfo.GetItemCount();
	itmList.reserve(itmCnt + 1);
	itmList.push_back(L"None");

	for (int i = 0; i < itmCnt; i++) {
		itmList.push_back(_lstTexInfo.GetItemText(i, 0));
	}
	
	UpdateCmbDropDownEvent(&_cmbAlphaMap, itmList);
}

//노말맵 콤보박스 이벤트
void TextureDlg::OnCbnDropdownTexdlgComboBump()
{
	vector<CString> itmList;

	int itmCnt = _lstTexInfo.GetItemCount();
	itmList.reserve(itmCnt + 1);
	itmList.push_back(L"None");

	for (int i = 0; i < itmCnt; i++) {
		itmList.push_back(_lstTexInfo.GetItemText(i, 0));
	}

	UpdateCmbDropDownEvent(&_cmbBumpMap, itmList);
}



//ModelList Combox Change Event
void TextureDlg::OnCbnSelchangeTexdlgComboModellist()
{
	int selIdx = _cmbModelList.GetCurSel();

	if (selIdx < 0) {
		_currentModel = NULL;
	}
	else {

		_currentModel = (SkinModel*)_cmbModelList.GetItemData(selIdx);
	}
	
	UpdateObjList();
	Invalidate(FALSE);
}


//디퓨즈맵 콤보박스 변경 이벤트
void TextureDlg::OnCbnSelchangeTexdlgComboDiffuse()
{
	int matIdx = _currentMesh->matIdx;

	if (matIdx < 0) return;

	int selIdx = _cmbDiffuseMap.GetCurSel() - 1;
	if (selIdx >= 0) {
		_currentModel->GetMaterialList()[matIdx].diffuseMap = RM_TEXTURE.GetResourceData(selIdx);
	}
	else {
		_currentModel->GetMaterialList()[matIdx].diffuseMap = NULL;
	}

	
}

//알파맵 콤보박스 변경 이벤트
void TextureDlg::OnCbnSelchangeTexdlgComboAlpha()
{
	int matIdx = _currentMesh->matIdx;

	if (matIdx < 0) return;

	int selIdx = _cmbAlphaMap.GetCurSel() - 1;
	if (selIdx >= 0) {
		_currentModel->GetMaterialList()[matIdx].alphaMap = RM_TEXTURE.GetResourceData(selIdx);
	}
	else {
		_currentModel->GetMaterialList()[matIdx].alphaMap = NULL;
	}
}

//노말맵 콤보박스 변경 이벤트
void TextureDlg::OnCbnSelchangeTexdlgComboBump()
{
	int matIdx = _currentMesh->matIdx;

	if (matIdx < 0) return;

	int selIdx = _cmbBumpMap.GetCurSel() - 1;
	if (selIdx >= 0) {
		_currentModel->GetMaterialList()[matIdx].normalMap = RM_TEXTURE.GetResourceData(selIdx);
	}
	else {
		_currentModel->GetMaterialList()[matIdx].normalMap = NULL;
	}
}



//더블 클릭으로 트리 확장시 윈도우 컨트롤 업데이트
void TextureDlg::OnNMDblclkTexdlgTreeObjlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	Invalidate(FALSE);
	*pResult = 0;
}


//메시지 처리 함수
BOOL TextureDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{

	//모델이 추가되면 텍스쳐 정보 업데이트
	if (message == WM_INSERT_MODEL) {
		UpdateTextureList();
	}
	//모델이 삭제되면
	else if (message == WM_DELETE_MODEL) {
		_currentModel = NULL;
		_currentMesh = NULL;

		_cmbModelList.SetCurSel(-1);
		UpdateObjList();
		UpdateObjInfo();

		Invalidate(FALSE);
	}

	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}