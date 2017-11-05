#pragma once

namespace Tool{
struct IDTreeOwner
{
public:
	enum{
		MODE_CLICK_EMPTY = 1,		//  点击是否父类处理
	};

public:
	virtual void OnTreeDblClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem){;}
	virtual void OnTreeLClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem){;}
	virtual void OnTreeRClick(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, CPoint point){;}
	virtual void OnTreeKeyDown(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, UINT nChar){;}
};

// CDTreeCtrl
class CDTreeCtrl : public CXTTreeCtrl
{
	DECLARE_DYNAMIC(CDTreeCtrl)
private:
	IDTreeOwner* m_Owner;
	CImageList m_ImgList;
	int m_nMode;

public:
	void SetOwner(IDTreeOwner* owner){m_Owner=owner;}
	void SetMode(int nFlag){m_nMode = nFlag;}

public:
	CDTreeCtrl();
	virtual ~CDTreeCtrl();

	virtual void InitImg();
	virtual void PMDeleteItem(HTREEITEM item);					// 删除掉相应节点
	virtual void PMDeleteItems(HTREEITEM item);					// 删除掉所有相应的子节点
	virtual HTREEITEM GetItemFromName(CString text);				// 寻找名字等于 text 的节点
	virtual HTREEITEM GetNextAny(HTREEITEM item,bool findchild=true);	// 得到它下边的节点，不管该节点在哪个级别上,findchild表示是否需要查找子节点
	virtual HTREEITEM GetNextByLevel(HTREEITEM item, int level);	// 获得第level层的下一个节点，level从0开始
	virtual int GetLevel(HTREEITEM item);							// 获得该节点所在第几层，从0开始

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
};
}