#include "StdAfx.h"
#include "AboutDlg.h"
BEGIN_MESSAGE_MAP(Dialog::CAboutDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

HBRUSH Dialog::CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		if(pWnd == &m_txtContent || pWnd == &m_txtWebsite)
			pDC->SetTextColor(RGB(0, 0, 0));
		else if(pWnd != &m_txtURL && pWnd != &m_txtEmail)
			pDC->SetTextColor(RGB(255, 255, 255));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}

void Dialog::CAboutDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	OnOK();
}