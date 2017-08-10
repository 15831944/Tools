#pragma once
#include "afxcmn.h"
#include "DTreeCtrl.h"
#include "XmlDevice.h"

// CSoftSetDeviceXmlDlg 对话框
namespace XmlInfo{class CXmlDevice;}
namespace SoftInfo{
class CSoftSetDeviceXmlDlg : public CDialog, public Tool::IDTreeOwner
{
	DECLARE_DYNAMIC(CSoftSetDeviceXmlDlg)
private:
	CString m_strDeviceName;		//!< 设备文件的名称，包括扩展名
	CString m_strDevicePath;		//!< 设备文件的路径，绝对路径
	std::shared_ptr<XmlInfo::CXmlDevice> m_XmlDevice;

public:
	Tool::CDTreeCtrl m_TreeCtrl;
	CString m_strParent;
	CListCtrl m_IconList;			//!< 图标列表控件
	CImageList m_ImgList;			//!< 图片列表
	CStatic m_Icon;					//!< 图标
//	CString m_strDeviceType;

public:
	CSoftSetDeviceXmlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoftSetDeviceXmlDlg();

// 对话框数据
	enum { IDD = IDD_SOFT_SET_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void FreshTree();

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

	afx_msg void OnBnClickedBtDevin();
	afx_msg void OnBnClickedBtDevinfo();
	afx_msg void OnBnClickedBtAdddev();
	afx_msg void OnBnClickedBtDeldev();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}