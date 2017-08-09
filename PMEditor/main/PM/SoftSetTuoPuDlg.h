#pragma once


// CSoftSetTuoPuDlg 对话框
namespace SoftInfo{
class CSoftSetTuoPuDlg : public CDialog
{
	DECLARE_DYNAMIC(CSoftSetTuoPuDlg)
public:
	CComboBox m_comHV;
	BOOL m_bTooltip;
	BOOL m_bReadBehaviorFirst;
	BOOL m_bShowAddr;
	CSpinButtonCtrl m_ZoomCtrl;
	LOGFONT m_lgFont;
	//UINT m_uiZoomSize;

public:
	CSoftSetTuoPuDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoftSetTuoPuDlg();

// 对话框数据
	enum { IDD = IDD_SOFT_SET_TUOPU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedBtFont();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

protected:
	virtual void OnOK();
};
}