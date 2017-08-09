#pragma once
#include "XTooltip.h"
// CDeviceMapView 视图

namespace MVC{
namespace Device{
class CDeviceMapDoc;
class CDeviceOne;
class CDeviceMapView : public CScrollView
{
	DECLARE_DYNCREATE(CDeviceMapView)
private:
	CString m_strInWord;			//!< 记录键盘的输入键，做彩蛋之用
	CPen m_Pen;
	CFont m_Font;
	bool m_bHostLineSelect;			//!< 目前是否被高亮了
	COleDropTarget m_DropTarget;	//!< 为了激活view的Drop功能
	bool m_bDropNewIn;				//!< 当前是否处于drag状态，添加新设备，是否要画Drop图片
	UINT m_uiDropDevType;			//!< 当前处于drag状态下的设备类型
	bool m_bDropOldIn;				//!< 移动设备的标示，只在最初时有效，移动以后就要看m_DropDevice了
	Tool::CXTooltip m_ToolTip;		//!< 悬浮提示窗口

	CString m_strHeader;			//!< 悬浮标题
	CString m_strBody;				//!< 悬浮内容
	CString m_strFooter;			//!< 底部文本
	CString m_strHtml;				//!< 超链接文本

protected:
	CDeviceMapView();				//!< 动态创建所使用的受保护的构造函数
	virtual ~CDeviceMapView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);			//!< 重写以绘制该视图
	virtual void OnDrawHostLine(CDC* pDC);	//!< 画母线
	virtual void OnInitialUpdate();			//!< 构造后的第一次
	virtual void OnDrawInit(CDC* pDC);		//!< 画初始化状态
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	bool IsInHostLineRect(CPoint point);
	void ClearSelect();						//!< 取消所有设备的选中状态
	bool IfDevCanLink();					//!< 看看当前被拖拽的设备与被选中的设备是否能连接
	boost::shared_ptr<CDeviceOne> GetSelectDev(bool bProj = true);	//!< 找到被选中的设备，参数表示是否要工程设备
	boost::shared_ptr<CDeviceOne> GetLTDev();			//!< 获得左顶点那个设备
	virtual void OnSelectUp();				//!< 选择上面的设备
	virtual void OnSelectDown();			//!< 选择下面的设备
	virtual void OnSelectLeft();			//!< 选择左面的设备
	virtual void OnSelectRight();			//!< 选择右面的设备

public:
	void FreshDeviceMap();					//!< 重新画一边设备拓扑
	void OnAddDev();						//!< 添加设备，如果没有被选中的则添加根设备，否则添加从设备
	void OnAddMainDev(UINT devID);			//!< 添加主设备，这里已经制定了哪个设备
	void OnAddSlave(UINT devID);			//!< 添加从设备，这里已经制定了哪个设备
	void OnAddScan(UINT devID);				//!< 将其置为工程设备
	void ShowDeviceAt(UINT devID);			//!< 选中这个设备，并显示这个设备
	void ShowPoint(CPoint pt);				//!< 显示这个点

	DECLARE_MESSAGE_MAP()
public:
	CDeviceMapDoc* GetDocument();
	afx_msg void OnPaint();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnAddMainDev();
	afx_msg void OnAddSlave();
	afx_msg void OnDevinterfaceSet();
	afx_msg void OnDevDelete();
	afx_msg void OnDevParainfo();
	afx_msg void OnDevOut();
	afx_msg void OnDevIn();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCopyWithChild();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnHyperlinkClicked(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnInterfaceSet();
	afx_msg void OnZoom(UINT index);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnAddScan();
	afx_msg void OnScanSet();
	afx_msg void OnScanStart();
	afx_msg void OnScanStop();
	afx_msg void OnH();
	afx_msg void OnV();
	afx_msg void OnAddScanInf2Proj();
	afx_msg void OnFreshState();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

	afx_msg void OnUpdateAddMainDev(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAddSlave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDevInfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInterfaceSet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateParainfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDevOut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDevIn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopyWithChild(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnProgram();
public:
	afx_msg void OnUpdateProgram(CCmdUI *pCmdUI);
};
}
}
