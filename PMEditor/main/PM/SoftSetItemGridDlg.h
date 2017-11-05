#pragma once


// CSoftSetItemGridDlg 对话框
namespace SoftInfo{
class CSoftSetItemGridDlg : public CDialog
{
	DECLARE_DYNAMIC(CSoftSetItemGridDlg)
public:
	BOOL m_bCheck0;
	BOOL m_bCheck1;
	BOOL m_bCheck2;
	BOOL m_bCheck3;
	BOOL m_bCheck4;
	BOOL m_bCheck5;
	BOOL m_bCheck6;
	BOOL m_bCheck7;
	BOOL m_bCheck8;
	BOOL m_bCheck9;
	BOOL m_bCheck10;
	BOOL m_bCheck11;
	BOOL m_bCheck12;
	BOOL m_bCheck13;
	BOOL m_bCheck14;
	BOOL m_bCheck15;
	BOOL m_bCheck16;
	BOOL m_bCheck17;
	BOOL m_bCheck18;
	BOOL m_bCheck19;
	BOOL m_bCheck20;
	BOOL m_bCheck21;
	BOOL m_bCheck22;			// Modbus485
	BOOL m_bCheck30;
	BOOL m_bCheckPaste;			// 粘贴是否弹提示框
	BOOL m_bCheckItemIn;		// 导入是否弹提示框
	BOOL m_bCheckHex;			// 十六进制显示数值

	UINT m_uiFloatWidth;		// 保留小数点数
	BOOL m_bPrintWaterMark;		// 是否打印水印

public:
	CSoftSetItemGridDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoftSetItemGridDlg();

// 对话框数据
	enum { IDD = IDD_SOFT_SET_ITEMGRID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	bool ChangeValue(bool& bSrc, BOOL bShow, UINT& count);	// 将bShow赋值给bSrc，如果bShow，count自增，如果相同返回true

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

protected:
	virtual void OnOK();
};
}