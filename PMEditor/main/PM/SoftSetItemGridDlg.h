#pragma once


// CSoftSetItemGridDlg �Ի���
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
	BOOL m_bCheckPaste;			// ճ���Ƿ���ʾ��
	BOOL m_bCheckItemIn;		// �����Ƿ���ʾ��
	BOOL m_bCheckHex;			// ʮ��������ʾ��ֵ

	UINT m_uiFloatWidth;		// ����С������
	BOOL m_bPrintWaterMark;		// �Ƿ��ӡˮӡ

public:
	CSoftSetItemGridDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSoftSetItemGridDlg();

// �Ի�������
	enum { IDD = IDD_SOFT_SET_ITEMGRID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	bool ChangeValue(bool& bSrc, BOOL bShow, UINT& count);	// ��bShow��ֵ��bSrc�����bShow��count�����������ͬ����true

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

protected:
	virtual void OnOK();
};
}