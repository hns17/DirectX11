//====================================================================
//			## CCustomTreeCtrl ## (MFC TreeCtrl 기능 확장)
//====================================================================

#pragma once
class CCustomTreeCtrl : public CTreeCtrl
{
public:
//********************************************* 멤버 함수 *******************************************//
	CCustomTreeCtrl();
	~CCustomTreeCtrl();

	//노드 추가
	HTREEITEM		CreateItem(HTREEITEM hItem, CString name = L"none", DWORD_PTR data = NULL);	
	
	//노드 확장
	void			NodeExpand(HTREEITEM hitem);							
	
	//GetFunction
	int				GetNodeIndex(HTREEITEM hitem);							//노드를 찾는다, 루트 부터 탐색
	int				GetNodeIndex(HTREEITEM findNode, HTREEITEM findTree);	//노드를 찾는다. 입력된 위치부터 탐색
	HTREEITEM		GetRootNode(HTREEITEM hitem);							//해당 노드의 루트 반환

	//트리 컨트롤 히트 이벤트
	HTREEITEM		HitEvent();												
	
	HTREEITEM		CheckBoxHitEvent();										//체크 박스 클릭 이벤트
	void			CheckParent(HTREEITEM hItem);							//부모 노드 체크
	void			CheckChild(HTREEITEM hItem, BOOL check);				//자식 노드 전부 체크


//*********************************************** 멤버 변수 *****************************************************//
private:
	//Select Item
	HTREEITEM		m_treeSelectItem;															
};
