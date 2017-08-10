#include "stdafx.h"
#include "PMApp.h"
#include "DTreeCtrl.h"

// CDTreeCtrl
using namespace Tool;
IMPLEMENT_DYNAMIC(CDTreeCtrl, CXTTreeCtrl)
CDTreeCtrl::CDTreeCtrl()
{
	m_Owner=NULL;
	m_ImgList.Create(16, 16, ILC_MASK | ILC_COLOR24, 0, 1);
	CBitmap bmp;
	bmp.LoadBitmap(IDB_FILETREE);
	m_ImgList.Add(&bmp, RGB(0, 128, 128));
}

CDTreeCtrl::~CDTreeCtrl()
{}

BEGIN_MESSAGE_MAP(CDTreeCtrl, CXTTreeCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(NM_RCLICK, &CDTreeCtrl::OnNMRclick)
END_MESSAGE_MAP()

// CDTreeCtrl 消息处理程序
int CDTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)		return -1;
	InitImg();
	return 0;
}

void CDTreeCtrl::InitImg()
{
	if(!GetSafeHwnd())		return;
	SetImageList(&m_ImgList, TVSIL_NORMAL);
}

//!< 要对Enter键盘键单独截取，因为在获得OnKeyDown的消息中找不到Enter事件，磕儿了。
BOOL CDTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN){
		if(pMsg->wParam == VK_RETURN)
			OnKeyDown(VK_RETURN, 0, 0);
	}

	return CXTTreeCtrl::PreTranslateMessage(pMsg);
}

void CDTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	HTREEITEM item=HitTest(point);
	if(!item)	return;
	if (!(m_nMode & IDTreeOwner::MODE_CLICK_EMPTY))
	SelectDropTarget(item);
	if(m_Owner)	m_Owner->OnTreeDblClick(this, item);
	CXTTreeCtrl::OnLButtonDblClk(nFlags, point);
}

void CDTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM item = HitTest(point);
	if(!item)	return;
	if (!(m_nMode & IDTreeOwner::MODE_CLICK_EMPTY))
	SelectDropTarget(item);
	if(m_Owner)	m_Owner->OnTreeLClick(this, item);
	CXTTreeCtrl::OnLButtonUp(nFlags, point);
}

void CDTreeCtrl::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	HTREEITEM item = HitTest(point);
	if(item && m_Owner){
		if (!(m_nMode & IDTreeOwner::MODE_CLICK_EMPTY))
		SelectDropTarget(item);
		m_Owner->OnTreeRClick(this, item, point);
	}
}

void CDTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	HTREEITEM item=GetSelectedItem();
	if(!item)	return;
	if (!(m_nMode & IDTreeOwner::MODE_CLICK_EMPTY))
	if(m_Owner)	m_Owner->OnTreeKeyDown(this, item, nChar);
	CXTTreeCtrl::OnKeyDown(nChar,nRepCnt,nFlags);
}

//!< 删除掉相应节点
void CDTreeCtrl::DCCEDeleteItem(HTREEITEM item)
{
	if(item)		DeleteItem(item);		//!< MFC的bug，当 item 等于 NULL 时就清空所有内容了，在这里把它改了
}

//!< 删除掉所有相应的子节点
void CDTreeCtrl::DCCEDeleteItems(HTREEITEM item)
{
	HTREEITEM pChileItem = GetNextItem(item,TVGN_CHILD);
	TVITEM Item;
	while(pChileItem!=NULL){
		GetItem(&Item);
		DeleteItem(pChileItem);
		pChileItem=GetNextItem(pChileItem,TVGN_NEXT);
	}
}

//!< 寻找名字等于 text 的节点
HTREEITEM CDTreeCtrl::GetItemFromName(CString text)
{
	HTREEITEM hItem = NULL;
	while(true)
	{
		hItem = GetNextAny(hItem);
		if(!hItem)						break;
		if(GetItemText(hItem) == text)	return hItem;
	}
	return NULL;
}

//!< 得到它下边的节点，不管该节点在哪个级别上,findchild表示是否需要查找子节点
HTREEITEM CDTreeCtrl::GetNextAny(HTREEITEM item,bool findchild /* =true */ )
{
	if(item==NULL)		return GetRootItem();	//!< 什么都没传，会得到树的根
	HTREEITEM next = NULL;
	if(findchild){
		next=GetNextItem(item,TVGN_CHILD);
		if(next!=NULL)	return next;			//!< 有孩子节点，返回孩子节点
	}
	next=GetNextItem(item,TVGN_NEXT);
	if(next!=NULL)		return next;			//!< 没有孩子节点，返回下一个节点
	item=GetParentItem(item);
	if(item==NULL)	return item;				//!< 下一个节点都没有，得到自己的父节点，继续递归查找
	return GetNextAny(item,false);				//!< 前边以证明子节点没有了，所有递归时不需要再找子节点了
}

//!< 获得第level层的下一个节点，level从0开始
HTREEITEM CDTreeCtrl::GetNextByLevel(HTREEITEM item, int level)
{
	if(GetRootItem() == NULL)		return NULL;
	HTREEITEM next = NULL;
	if(GetLevel(item) < level)		next = GetNextAny(item);				//!< 如果当前的节点级别不到，找孩子
	else							next = GetNextAny(item, false);			//!< 如果当前的节点级别到了，找兄弟
	if(!next)						return next;
	if(GetLevel(next) == level)		return next;
	return GetNextByLevel(next ,level);
}

//!< 获得该节点所在第几层，从0开始
int CDTreeCtrl::GetLevel(HTREEITEM item)
{
	HTREEITEM parent = GetParentItem(item);
	return (parent == NULL) ? (0) : (GetLevel(parent) + 1);
}
