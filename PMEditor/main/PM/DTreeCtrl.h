#pragma once

namespace Tool{
struct IDTreeOwner
{
public:
	enum{
		MODE_CLICK_EMPTY = 1,		//  ����Ƿ��ദ��
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
	virtual void PMDeleteItem(HTREEITEM item);					// ɾ������Ӧ�ڵ�
	virtual void PMDeleteItems(HTREEITEM item);					// ɾ����������Ӧ���ӽڵ�
	virtual HTREEITEM GetItemFromName(CString text);				// Ѱ�����ֵ��� text �Ľڵ�
	virtual HTREEITEM GetNextAny(HTREEITEM item,bool findchild=true);	// �õ����±ߵĽڵ㣬���ܸýڵ����ĸ�������,findchild��ʾ�Ƿ���Ҫ�����ӽڵ�
	virtual HTREEITEM GetNextByLevel(HTREEITEM item, int level);	// ��õ�level�����һ���ڵ㣬level��0��ʼ
	virtual int GetLevel(HTREEITEM item);							// ��øýڵ����ڵڼ��㣬��0��ʼ

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