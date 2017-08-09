#pragma once
#include "DTreeCtrl.h"
//#include "MainFrm.h"
#include "DevMgr.h"
#include "PropertyParam.h"

#define CPropertyGridDlgBase CXTResizeDialog
// CBehaviorParamDlg �Ի���
namespace MVC{namespace Device{class CDeviceOne;}}
namespace XmlInfo{class CXmlBehavior;}
namespace Dialog{
	class CBehaviorParamDlg : public CPropertyGridDlgBase, public Tool::IDTreeOwner, public MVC::Device::tagBehaviorRequest
{
	DECLARE_DYNAMIC(CBehaviorParamDlg)
private:
	UINT m_uiDevID;								//!< �豸��ID��
	boost::shared_ptr<MVC::Device::CDeviceOne> m_ShowDev;
	CStatic m_GridHolder;
	CXTPPropertyGrid m_PropertyGrid;
	Tool::CDTreeCtrl m_TreeCtrl;
	HTREEITEM m_CulItem;
	CString m_strGroupName;
	CComboBox m_GroupList;
	MVC::Device::CPropertyParam m_Object;		//!< Ҫ��ʾ������

	bool m_bLock;								//!< ��ǰ�Ƿ�������״̬
	int m_nWaitToCount;							//!< ��Ҫ�ȴ��ظ�������
	CString m_strOperate;						//!< ��ǰ�Ĳ�������"��","д","ִ��"
	CString m_strOut;							//!< �������Ϣ

public:
	CBehaviorParamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBehaviorParamDlg();

// �Ի�������
	enum { IDD = IDD_BEHAVIOR_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void FreshData();							//!< ˢ������
	bool IsDevDwon();							//!< �豸ͻȻû����
	void EnabledEdit(BOOL enabl = FALSE);		//!< ��ֹ����
	void SetCommTimer();						//!< ͨ��ǰ���ó�ʱʱ��

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnDataReady();					//!< �����Ѿ�ˢ�º���
	virtual void OnDeviceChange();				//!< �豸���˸ı���
	virtual void OnParaFresh(bool bReadOnly);	//!< ˢ�²�����������ʾ�Ƿ���ֻ��
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