// DCCE_SplashDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "DCCE_SplashDlg.h"


// CDCCE_SplashDlg 对话框
const int SPLASH_HEIGHT = 400;
const int SPLASH_WIDTH  = 576;

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED		0x80000
#define LWA_ALPHA			0x02
#endif

#define AW_BLEND			0x00080000
#define AW_HIDE				0x00010000

using namespace Dialog;

IMPLEMENT_DYNAMIC(CDCCE_SplashDlg, CDialog)

CDCCE_SplashDlg::CDCCE_SplashDlg(int MaxInitNO,BOOL bSilentMode)
	: CDialog(CDCCE_SplashDlg::IDD, NULL)
{
	m_nPos=0;
	m_nMax=MaxInitNO;

//	m_strVersionNo.Format("Version:%s.%s.%s(Build %s,SVN %s)",
//		g_Marjor,g_Minor,g_Update,g_Build,g_SVNRevision);

	m_strPreString="正在进行初始化:";

	m_hUser32=NULL;
	m_pfnAnimateWindow=NULL;
	//! create
	Create(IDD);

}

CDCCE_SplashDlg::~CDCCE_SplashDlg()
{
	if (m_hUser32!=NULL) {
		FreeLibrary(m_hUser32);
	}
}

void CDCCE_SplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDCCE_SplashDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CDCCE_SplashDlg 消息处理程序

void CDCCE_SplashDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	DoPaint( &dc );
}

BOOL CDCCE_SplashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CClientDC dcScreen( this );
	m_bmSplash.LoadBitmap(IDB_WELCOME_DDS);

	m_bmBuffer.CreateCompatibleBitmap( &dcScreen, SPLASH_WIDTH, SPLASH_HEIGHT );
	m_dcBuffer1.CreateCompatibleDC( &dcScreen );
	m_dcBuffer2.CreateCompatibleDC( &dcScreen );


	SetWindowPos( NULL, 0, 0, SPLASH_WIDTH, SPLASH_HEIGHT, SWP_NOMOVE );
	CenterWindow();

	if (( m_hUser32 = LoadLibrary( _T("User32.dll") ) ) != 0)
	{
		(FARPROC&)m_pfnAnimateWindow = GetProcAddress( m_hUser32, "AnimateWindow" );

		if ( m_pfnAnimateWindow != NULL )
		{
			(*m_pfnAnimateWindow)( GetSafeHwnd(), 250, AW_BLEND );
		}
	}

	SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
	UpdateWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDCCE_SplashDlg::DrawFragment(CDC* pDC, CRect* prcBar, DWORD nTotal, DWORD nOffset, DWORD nLength, COLORREF crFill, BOOL b3D)
{
	CRect rcArea;


	rcArea.left		= prcBar->left + (int)( (double)( prcBar->Width() + 1 ) / (double)nTotal * (double)nOffset );
	rcArea.right	= prcBar->left + (int)( (double)( prcBar->Width() + 1 ) / (double)nTotal * (double)( nOffset + nLength ) );

	rcArea.top		= prcBar->top;
	rcArea.bottom	= prcBar->bottom;

	rcArea.left		= max( rcArea.left, prcBar->left );
	rcArea.right	= min( rcArea.right, prcBar->right );

	if ( rcArea.right <= rcArea.left ) return;

	if ( b3D && rcArea.Width() > 2 )
	{
		pDC->Draw3dRect( &rcArea, CalculateColour( crFill, RGB(255,255,255), 75 ), CalculateColour( crFill, RGB(0,0,0), 75 ) );

		rcArea.DeflateRect( 1, 1 );
		pDC->FillSolidRect( &rcArea, crFill );
		rcArea.InflateRect( 1, 1 );
	}
	else
	{
		pDC->FillSolidRect( &rcArea, crFill );
	}

	pDC->ExcludeClipRect( &rcArea );
}

COLORREF CDCCE_SplashDlg::CalculateColour(COLORREF crFore, COLORREF crBack, int nAlpha)
{
	int nRed	= GetRValue( crFore ) * ( 255 - nAlpha ) / 255 + GetRValue( crBack ) * nAlpha / 255;
	int nGreen	= GetGValue( crFore ) * ( 255 - nAlpha ) / 255 + GetGValue( crBack ) * nAlpha / 255;
	int nBlue	= GetBValue( crFore ) * ( 255 - nAlpha ) / 255 + GetBValue( crBack ) * nAlpha / 255;

	return RGB( nRed, nGreen, nBlue );
}

void CDCCE_SplashDlg::DoPaint(CDC* pDC)
{
	CBitmap* pOld1 = (CBitmap*)m_dcBuffer1.SelectObject( &m_bmSplash );
	CBitmap* pOld2 = (CBitmap*)m_dcBuffer2.SelectObject( &m_bmBuffer );

	m_dcBuffer2.BitBlt( 0, 0, SPLASH_WIDTH, SPLASH_HEIGHT, &m_dcBuffer1, 0, 0, SRCCOPY );

	m_dcBuffer2.SetBkMode( TRANSPARENT );

	CRect rc(10, 160, 350, SPLASH_HEIGHT);
	DrawHollowText(&m_dcBuffer2,rc,RGB(0x00, 0x50, 0x9C),RGB(255,255,255),m_strShowInfor);

	rc.SetRect(50, 290, 350, SPLASH_HEIGHT);
//	DrawHollowText(&m_dcBuffer2,rc,RGB(0xff, 0x00, 0x00),RGB(255,255,255), CGbl::GetMe().getVersionStr());

//	rc.SetRect( 0, 0, 0, 0 );
//	m_dcBuffer2.Draw3dRect( &rc, RGB( 0xFF, 0xFF, 0xFF ), RGB( 0xFF, 0xFF, 0xFF ) );
//	rc.DeflateRect( 1, 1 );
//	m_dcBuffer2.FillSolidRect( &rc, RGB( 0xFF, 0xFF, 0xFF ) );

//	DrawFragment( &m_dcBuffer2, &rc, m_nMax, 0, min( m_nPos, m_nMax ),RGB( 0x00, 0x50, 0x9C ), TRUE );
//	m_dcBuffer2.SelectClipRgn( NULL );

	pDC->BitBlt( 0, 0, SPLASH_WIDTH, SPLASH_HEIGHT, &m_dcBuffer2, 0, 0, SRCCOPY );

	m_dcBuffer2.SelectObject( pOld2 );
	m_dcBuffer1.SelectObject( pOld1 );
}

void CDCCE_SplashDlg::StepOne(const CString& showText)
{
	m_nPos ++;
	m_strPosString.Format("......%d %%",(m_nPos*100)/m_nMax);
	m_strShowInfor.Format("%s%s%s",m_strPreString,showText,m_strPosString);

	CClientDC dc( this );
	DoPaint( &dc );
}

void CDCCE_SplashDlg::TopMost()
{
	if ( IsWindowVisible() )
	{
		SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
	}
}

void CDCCE_SplashDlg::Hide()
{
	m_strShowInfor = _T("启动完成!");
	Invalidate();

	if ( m_pfnAnimateWindow != NULL )
	{
		HWND shWnd;
		shWnd=GetSafeHwnd();
		(*m_pfnAnimateWindow)( shWnd, 250, AW_HIDE|AW_BLEND );
	}

	OnDestroy();
	delete this;
}

void CDCCE_SplashDlg::DrawHollowText(CDC *pDC,CRect& posRC,COLORREF crEdge,COLORREF crFill,const CString& strText)
{
	pDC->SetTextColor(crEdge);

	UINT nFormat = DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;

	posRC.OffsetRect( -1, 0 );
	pDC->DrawText( strText, &posRC, nFormat );
	posRC.OffsetRect( 2, 0 );
	pDC->DrawText( strText, &posRC, nFormat );
	posRC.OffsetRect( -1, -1 );
	pDC->DrawText( strText, &posRC, nFormat );
	posRC.OffsetRect( 0, 2 );
	pDC->DrawText( strText, &posRC, nFormat );
	posRC.OffsetRect( 0, -1 );

	pDC->SetTextColor(crFill);
	pDC->DrawText( strText, &posRC, nFormat );
}

BOOL Dialog::CDCCE_SplashDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("目录"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
