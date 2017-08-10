#pragma once


// CPMSplashDlg 对话框


namespace Dialog{
class CPMSplashDlg : public CDialog
{
	typedef BOOL (WINAPI *pfnAnimateWindow)(HWND, DWORD, DWORD);
	DECLARE_DYNAMIC(CPMSplashDlg)

public:
	CPMSplashDlg(int MaxInitNO=0,BOOL bSilentMode=FALSE);
	virtual ~CPMSplashDlg();

// 对话框数据
	enum { IDD = IDD_PMSPLASHDLG };

private:
	int m_nPos;
	int m_nMax;
	CString m_strPreString;
	CString m_strPosString;
	CString m_strShowInfor;
	
//	CString m_strVersionNo;

	CBitmap m_bmSplash;
	CBitmap m_bmBuffer;

	CDC m_dcBuffer1;
	CDC m_dcBuffer2;

	HINSTANCE m_hUser32;
	pfnAnimateWindow m_pfnAnimateWindow;

	void DrawFragment(CDC* pDC, CRect* prcBar, DWORD nTotal, DWORD nOffset, DWORD nLength, COLORREF crFill, BOOL b3D);
	COLORREF CalculateColour(COLORREF crFore, COLORREF crBack, int nAlpha);
	void DoPaint(CDC* pDC);
	void DrawHollowText(CDC *pDC,CRect& posRC,COLORREF crEdge,COLORREF crFill,const CString& strText);
public:
	void StepOne(const CString& showText);
	void TopMost();
	void Hide();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
}