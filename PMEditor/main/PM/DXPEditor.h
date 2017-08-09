// DXPEditor.h : DXPEditor Ӧ�ó������ͷ�ļ�
//
#pragma once
#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif
#include "resource.h"       // ������

// CDXPEditorApp:
// �йش����ʵ�֣������ DXPEditor.cpp
//
class CProjectMgr;
namespace MVC{namespace Start{class CDCCE_HtmlDialog;}}
namespace XmlInfo{class CXmlMgr;}
class CDXPEditorApp : public CWinApp
{
public:
	CMultiDocTemplate* m_pStartDocMgr;
	CMultiDocTemplate* m_pItemDocMgr;
	CMultiDocTemplate* m_pDeviceDocMgr;
	CMultiDocTemplate* m_pCamDocMgr;
	MVC::Start::CDCCE_HtmlDialog* m_StartPage;				/**< ��ǰ����ʼҳDialogָ�� */

	boost::shared_ptr<XmlInfo::CXmlMgr> m_XmlMgr;

public:
	boost::shared_ptr<XmlInfo::CXmlMgr> GetXmlMgr(){return m_XmlMgr;}
	MVC::Start::CDCCE_HtmlDialog* GetStartPage(){return m_StartPage;}

	void SetStartPage(MVC::Start::CDCCE_HtmlDialog* page){m_StartPage = page;}				/**< ��ǰ����ʼҳDialogָ�� */
	CRecentFileList* GetRecentFileList(){return m_pRecentFileList;}

public:
	CDXPEditorApp();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);
	virtual void DeleteRecentFile(LPCTSTR lpszPathName);
	virtual void ShowStartPage();			//!< ����ʼҳ

	afx_msg void OnAppAbout();
	afx_msg void MRUFileHandler(UINT index);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void RegistSoftware();					//!< ���ע��
	void ReadCmdLine(CString cmdLine);		//!< ����������
};

extern CDXPEditorApp g_App;
