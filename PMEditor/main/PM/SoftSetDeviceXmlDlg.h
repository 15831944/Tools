#pragma once
#include "afxcmn.h"
#include "DTreeCtrl.h"
#include "XmlDevice.h"

// CSoftSetDeviceXmlDlg �Ի���
namespace XmlInfo{class CXmlDevice;}
namespace SoftInfo{
class CSoftSetDeviceXmlDlg : public CDialog, public Tool::IDTreeOwner
{
	DECLARE_DYNAMIC(CSoftSetDeviceXmlDlg)
private:
	CString m_strDeviceName;		//!< �豸�ļ������ƣ�������չ��
	CString m_strDevicePath;		//!< �豸�ļ���·��������·��
	std::shared_ptr<XmlInfo::CXmlDevice> m_XmlDevice;

public:
	Tool::CDTreeCtrl m_TreeCtrl;
	CString m_strParent;
	CListCtrl m_IconList;			//!< ͼ���б�ؼ�
	CImageList m_ImgList;			//!< ͼƬ�б�
	CStatic m_Icon;					//!< ͼ��
//	CString m_strDeviceType;

public:
	CSoftSetDeviceXmlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSoftSetDeviceXmlDlg();

// �Ի�������
	enum { IDD = IDD_SOFT_SET_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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