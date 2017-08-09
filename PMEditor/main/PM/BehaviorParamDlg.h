#pragma once
#include "DTreeCtrl.h"
//#include "MainFrm.h"
#include "DevMgr.h"
#include "PropertyParam.h"

#define CPropertyGridDlgBase CXTResizeDialog
// CBehaviorParamDlg 对话框
namespace MVC{namespace Device{class CDeviceOne;}}
namespace XmlInfo{class CXmlBehavior;}
namespace Dialog{
	class CBehaviorParamDlg : public CPropertyGridDlgBase, public Tool::IDTreeOwner, public MVC::Device::tagBehaviorRequest
{
	DECLARE_DYNAMIC(CBehaviorParamDlg)
private:
	UINT m_uiDevID;								//!< 设备的ID号
	boost::shared_ptr<MVC::Device::CDeviceOne> m_ShowDev;
	CStatic m_GridHolder;
	CXTPPropertyGrid m_PropertyGrid;
	Tool::CDTreeCtrl m_TreeCtrl;
	HTREEITEM m_CulItem;
	CString m_strGroupName;
	CComboBox m_GroupList;
	MVC::Device::CPropertyParam m_Object;		//!< 要显示的内容

	bool m_bLock;								//!< 当前是否是锁定状态
	int m_nWaitToCount;							//!< 需要等待回复的数量
	CString m_strOperate;						//!< 当前的操作类型"读","写","执行"
	CString m_strOut;							//!< 输出的信息

public:
	CBehaviorParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBehaviorParamDlg();

// 对话框数据
	enum { IDD = IDD_BEHAVIOR_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void FreshData();							//!< 刷新数据
	bool IsDevDwon();							//!< 设备突然没有了
	void EnabledEdit(BOOL enabl = FALSE);		//!< 禁止操作
	void SetCommTimer();						//!< 通信前设置超时时间

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnDataReady();					//!< 数据已经刷新好了
	virtual void OnDeviceChange();				//!< 设备拓扑改变了
	virtual void OnParaFresh(bool bReadOnly);	//!< 刷新参数，参数表示是否是只读
	virtual void OnTreeDblClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeLClick(CTreeCtrl*, HTREEITEM);
	virtual void OnTreeRClick(CTreeCtrl*, HTREEITEM,CPoint);
	virtual void OnTreeKeyDown(CTreeCtrl*, HTREEITEM,UINT);
	virtual INT_PTR DoModal(UINT devID);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnGroupChange();
	afx_msg void OnBnClickedBehaviorIn();
	afx_msg void OnBnClickedBehaviorOut();
	afx_msg void OnBnClickedBehaviorUpload();
	afx_msg void OnBnClickedBehaviorDownload();
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	};
}