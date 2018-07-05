#include "stdafx.h"
#include "CustomTreeCtrl.h"


CCustomTreeCtrl::CCustomTreeCtrl()
{
}

CCustomTreeCtrl::~CCustomTreeCtrl()
{
}

//노드 추가
HTREEITEM CCustomTreeCtrl::CreateItem(HTREEITEM hItem, CString name, DWORD_PTR data)
{
	HTREEITEM subItem = this->InsertItem(name, hItem, TVI_LAST);

	if(data)
		this->SetItemData(subItem, data);

	return subItem;
}


//노드 열기
void CCustomTreeCtrl::NodeExpand(HTREEITEM hitem)
{
	//노드 확장
	while (hitem)
	{
		this->Expand(hitem, TVE_EXPAND);

		if (this->ItemHasChildren(hitem))
			NodeExpand(this->GetChildItem(hitem));

		hitem = this->GetNextItem(hitem, TVGN_NEXT);
	}

}

//노드 탐색 후 인덱스 반환
int CCustomTreeCtrl::GetNodeIndex(HTREEITEM hitem)
{
	int idx = 0;
	//부모노드를 구한다.
	HTREEITEM finditem = this->GetParentItem(hitem);
	
	//NULL면 최상위 노드 중 가장 첫번째 노드를 선택
	//NULL 아니면, 자식노드 중 가장 첫번째 노드를 선택
	if (finditem == NULL)
		finditem = this->GetRootItem();
	else
		finditem = this->GetChildItem(finditem);

	//노드 탐색.
	while (finditem) {
		if (finditem == hitem)
			return idx;

		finditem = this->GetNextSiblingItem(finditem);
		idx++;
	}
	return -1;
}

//노드 탐색 후 인덱스 반환
int CCustomTreeCtrl::GetNodeIndex(HTREEITEM findNode, HTREEITEM findTree)
{
	int Cnt = 0;
	while (findTree) {
		if (findNode == findTree)
			return Cnt;
		
		findTree = this->GetNextItem(findTree, TVGN_NEXT);
		Cnt++;
	}

	return -1;
}

//루트노드 탐색
HTREEITEM CCustomTreeCtrl::GetRootNode(HTREEITEM hitem) {

	HTREEITEM find = hitem;

	while (this->GetParentItem(find)) {
		find = this->GetParentItem(find);
	}

	return find;
}


//컨트롤 클릭 이벤트
HTREEITEM CCustomTreeCtrl::HitEvent()
{
	CPoint pt;
	// 화면상에서 마우스의 위치를 얻는다.
	GetCursorPos(&pt);
	

	// 현재 마우스 좌표가 위치한 항목 정보를 얻는다.
	::ScreenToClient(this->m_hWnd, &pt);
	HTREEITEM hitem = HitTest(pt);
	
	return hitem;
}

//체크박스 영역 클릭 이벤트
HTREEITEM CCustomTreeCtrl::CheckBoxHitEvent()
{

	CPoint	point;
	UINT	nFlags = 0;

	GetCursorPos(&point);
	::ScreenToClient(this->m_hWnd, &point);

	HTREEITEM	hItem = HitTest(point, &nFlags);

	// 아이템이 존재하고 체크박스에 이벤트가 발생했다면
	if (hItem != NULL && (nFlags & TVHT_ONITEMSTATEICON) != 0) {
		return hItem;
	}
	return NULL;
}


//자식노드 체크시 부모노드 전체 체크
void CCustomTreeCtrl::CheckParent(HTREEITEM hItem)
{
	//하위노드의 활성화 상태
	HTREEITEM hParent = this->GetParentItem(hItem);
	while (hParent) {
		this->SetCheck(hParent, TRUE);
		hParent = this->GetParentItem(hParent);
	}
}

//부노드 체크시 자식노드 전체 체크
void CCustomTreeCtrl::CheckChild(HTREEITEM hItem, BOOL check)
{
	//하위노드의 활성화 상태
	HTREEITEM hChild = this->GetChildItem(hItem);
	while (hChild) {
		this->SetCheck(hChild, check);
		hChild = this->GetNextItem(hChild, TVGN_NEXT);
	}
}

