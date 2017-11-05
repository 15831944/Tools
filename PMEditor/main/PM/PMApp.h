// PMApp.h : DXPEditor Ӧ�ó������ͷ�ļ�
//
#pragma once
#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif
#include "resource.h"       // ������

// CPMApp:
// �йش����ʵ�֣������ DXPEditor.cpp
//
class CProjectMgr;
namespace MVC{namespace Start{class CPMHtmlDialog;}}
namespace XmlInfo{class CXmlMgr;}
class CPMApp : public CWinApp
{
public:
	CMultiDocTemplate* m_pStartDocMgr;
	CMultiDocTemplate* m_pItemDocMgr;
	MVC::Start::CPMHtmlDialog* m_StartPage;				/**< ��ǰ����ʼҳDialogָ�� */

	std::shared_ptr<XmlInfo::CXmlMgr> m_XmlMgr;

public:
	std::shared_ptr<XmlInfo::CXmlMgr> GetXmlMgr(){return m_XmlMgr;}
	MVC::Start::CPMHtmlDialog* GetStartPage(){return m_StartPage;}

	void SetStartPage(MVC::Start::CPMHtmlDialog* page){m_StartPage = page;}				/**< ��ǰ����ʼҳDialogָ�� */
	CRecentFileList* GetRecentFileList(){return m_pRecentFileList;}

public:
	CPMApp();
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

extern CPMApp g_App;
