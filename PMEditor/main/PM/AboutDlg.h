// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
#if !defined(__ABOUTDLG_H__)
#define __ABOUTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (_MSC_VER > 1310) // VS2005
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include "StdAfx.h"
#include "DXPEditor.h"
#include "Gbl.h"
//#include "Power.h"
 
namespace Dialog{
class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* pParent = NULL)
		: CDialog(IDD_ABOUTBOX, pParent)
	{
		NONCLIENTMETRICS ncm;
		::ZeroMemory(&ncm, sizeof(NONCLIENTMETRICS));
		ncm.cbSize = sizeof(NONCLIENTMETRICS);

		VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
			sizeof(NONCLIENTMETRICS), &ncm, 0));

		ncm.lfMenuFont.lfWeight = FW_BOLD;
		m_fontBold.CreateFontIndirect(&ncm.lfMenuFont);
	}

	CStatic m_txtPackageVersion;
	CStatic m_txtSvnVersion;
	CStatic m_txtCopyrightInfo1;
	CStatic m_txtCopyrightInfo2;
	CStatic m_txtPower;
	CStatic m_txtAppName;
	CStatic m_txtContent;
	CStatic m_txtWebsite;
	CXTButton m_btnOk;
	CXTHyperLink m_txtURL;
	CXTHyperLink m_txtEmail;
	CFont m_fontBold;

protected:
	virtual void DoDataExchange(CDataExchange* pDX)    // DDX/DDV 支持
	{
		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CAboutDlg)
		DDX_Control(pDX, IDC_TXT_TITLE, m_txtPackageVersion);
		DDX_Control(pDX, IDC_TXT_SVN, m_txtSvnVersion);
		DDX_Control(pDX, IDC_TXT_COPYRIGHT1, m_txtCopyrightInfo1);
		DDX_Control(pDX, IDC_TXT_COPYRIGHT2, m_txtCopyrightInfo2);
		DDX_Control(pDX, IDC_TXT_POWER, m_txtPower);
		DDX_Control(pDX, IDC_TXT_APPNAME, m_txtAppName);
		DDX_Control(pDX, IDC_STATIC_CONTACT, m_txtContent);
		DDX_Control(pDX, IDC_STATIC_WEBSITE, m_txtWebsite);
		DDX_Control(pDX, IDOK, m_btnOk);
		DDX_Control(pDX, IDC_TXT_URL, m_txtURL);
		DDX_Control(pDX, IDC_TXT_EMAIL, m_txtEmail);
		//}}AFX_DATA_MAP
	}

	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();

		// get a pointer to CWinApp.
		CWinApp* pApp = AfxGetApp();
		ASSERT(pApp != NULL);

		// set the sample title.
		CString csSampleTitle;
		csSampleTitle.Format(_T("%s Application"), pApp->m_pszAppName);
		m_txtAppName.SetWindowText(csSampleTitle);

		// set the package version.
		CString csPackageVersion;
//		csPackageVersion.Format( _T( "%s v%d.%d.%d" ),
//			_XTP_PACKAGE_NAME, _XTPLIB_VERSION_MAJOR, _XTPLIB_VERSION_MINOR, _XTPLIB_VERSION_REVISION);
//		csPackageVersion.Format("Tina Pro V1.0.0");
		csPackageVersion = _T("DView ") + CGbl::GetMe().getVersionStr();
		m_txtPackageVersion.SetWindowText(csPackageVersion);
		m_txtSvnVersion.SetWindowText(CGbl::GetMe().getSVNStr() + _T("(S)"));

		// set the about caption.
		CString csAboutCaption;
		csAboutCaption.Format(_T("About %s"), pApp->m_pszAppName);
		SetWindowText( csAboutCaption );

		// set the copyright info.
		CString csCopyrightInfo1, csCopyrightInfo2;
		csCopyrightInfo1.Format(_T("(c)2009-%d DCCE Software" ), COleDateTime::GetCurrentTime().GetYear());
		csCopyrightInfo2 = _T("All Rights Reserved");
		m_txtCopyrightInfo1.SetWindowText(csCopyrightInfo1);
		m_txtCopyrightInfo2.SetWindowText(csCopyrightInfo2);

		// set the power of user
		CString csPower = _T("");	// 当前用户: 注册用户");
		m_txtPower.SetWindowText(csPower);

		// set the title text to bold font.
		m_txtPackageVersion.SetFont(&m_fontBold);

		// define the url for our hyperlinks.
		m_txtURL.SetURL(_T("http://www.dcce.cn"));
		m_txtURL.SetUnderline(false);

//		m_txtEmail.SetURL(_T("mailto:a_tlas@163.com"));
		m_txtEmail.SetURL(_T("mailto:service@dcce.cn"));
		m_txtEmail.SetUnderline(false);

		// set OK button style.
		m_btnOk.SetXButtonStyle(BS_XT_SEMIFLAT | BS_XT_HILITEPRESSED | BS_XT_WINXP_COMPAT);

		return TRUE;  // return TRUE unless you set the focus to a control
	}

public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifdef ENABLE_MANIFESTEDITOR


#if !defined(_UNICODE) && (_MSC_VER < 1400)

#if _MSC_VER > 1200 //MFC 7.0
#include <..\src\mfc\afximpl.h> // MFC Global data
#else
#include <..\src\afximpl.h>     // MFC Global data
#endif

#endif

AFX_INLINE void EnableManifestEditor()
{
#if !defined(_UNICODE) && (_MSC_VER < 1400)
	OSVERSIONINFO ovi = {sizeof(OSVERSIONINFO)};
	::GetVersionEx(&ovi);

	if ((ovi.dwPlatformId >= VER_PLATFORM_WIN32_NT) && (ovi.dwMajorVersion >= 5))
	{
#if (_MSC_VER >= 1200)
		afxData.bWin95 = TRUE;
#else
		afxData.bWin32s = TRUE;
#endif
	}
#endif
}

#else

AFX_INLINE void EnableManifestEditor()
{
}

#endif


AFX_INLINE void ShowSampleHelpPopup(CWnd* pParentWnd, UINT nIDResource)
{
	CXTPPopupControl* pPopup = new CXTPPopupControl();
	pPopup->SetTransparency(200);
	pPopup->SetTheme(xtpPopupThemeOffice2003);
	pPopup->AllowMove(TRUE);
	pPopup->SetAnimateDelay(500);
	pPopup->SetPopupAnimation();
	pPopup->SetShowDelay(5000);
	pPopup->SetAutoDelete(TRUE);

	CXTPPopupItem* pItemText = (CXTPPopupItem*)pPopup->AddItem(new CXTPPopupItem(CRect(8, 12, 500, 130)));
	pItemText->SetRTFText(nIDResource);
	pItemText->FitToContent();
	CSize sz(pItemText->GetRect().Size());

	// close icon.
	CXTPPopupItem* pItemIcon = (CXTPPopupItem*)pPopup->AddItem(
		new CXTPPopupItem(CRect(sz.cx + 2, 10, sz.cx + 2 + 16, 10 + 16)));

	pPopup->GetImageManager()->SetIcon(IDI_POPUP_CLOSE, IDI_POPUP_CLOSE);
	pItemIcon->SetIconIndex(IDI_POPUP_CLOSE);

	pItemIcon->SetButton(TRUE);
	pItemIcon->SetID(XTP_ID_POPUP_CLOSE);

	pPopup->SetPopupSize(CSize(sz.cx + 20, sz.cy + 20));
	pPopup->Show(pParentWnd);
}
}

#endif // !defined(__ABOUTDLG_H__)