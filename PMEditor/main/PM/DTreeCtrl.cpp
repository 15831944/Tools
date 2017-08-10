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

// CDTreeCtrl ��Ϣ�������
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

//!< Ҫ��Enter���̼�������ȡ����Ϊ�ڻ��OnKeyDown����Ϣ���Ҳ���Enter�¼����Ķ��ˡ�
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

//!< ɾ������Ӧ�ڵ�
void CDTreeCtrl::DCCEDeleteItem(HTREEITEM item)
{
	if(item)		DeleteItem(item);		//!< MFC��bug���� item ���� NULL ʱ��������������ˣ��������������
}

//!< ɾ����������Ӧ���ӽڵ�
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

//!< Ѱ�����ֵ��� text �Ľڵ�
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

//!< �õ����±ߵĽڵ㣬���ܸýڵ����ĸ�������,findchild��ʾ�Ƿ���Ҫ�����ӽڵ�
HTREEITEM CDTreeCtrl::GetNextAny(HTREEITEM item,bool findchild /* =true */ )
{
	if(item==NULL)		return GetRootItem();	//!< ʲô��û������õ����ĸ�
	HTREEITEM next = NULL;
	if(findchild){
		next=GetNextItem(item,TVGN_CHILD);
		if(next!=NULL)	return next;			//!< �к��ӽڵ㣬���غ��ӽڵ�
	}
	next=GetNextItem(item,TVGN_NEXT);
	if(next!=NULL)		return next;			//!< û�к��ӽڵ㣬������һ���ڵ�
	item=GetParentItem(item);
	if(item==NULL)	return item;				//!< ��һ���ڵ㶼û�У��õ��Լ��ĸ��ڵ㣬�����ݹ����
	return GetNextAny(item,false);				//!< ǰ����֤���ӽڵ�û���ˣ����еݹ�ʱ����Ҫ�����ӽڵ���
}

//!< ��õ�level�����һ���ڵ㣬level��0��ʼ
HTREEITEM CDTreeCtrl::GetNextByLevel(HTREEITEM item, int level)
{
	if(GetRootItem() == NULL)		return NULL;
	HTREEITEM next = NULL;
	if(GetLevel(item) < level)		next = GetNextAny(item);				//!< �����ǰ�Ľڵ㼶�𲻵����Һ���
	else							next = GetNextAny(item, false);			//!< �����ǰ�Ľڵ㼶���ˣ����ֵ�
	if(!next)						return next;
	if(GetLevel(next) == level)		return next;
	return GetNextByLevel(next ,level);
}

//!< ��øýڵ����ڵڼ��㣬��0��ʼ
int CDTreeCtrl::GetLevel(HTREEITEM item)
{
	HTREEITEM parent = GetParentItem(item);
	return (parent == NULL) ? (0) : (GetLevel(parent) + 1);
}
