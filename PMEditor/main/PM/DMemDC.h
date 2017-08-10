#pragma once

namespace Tool{
//!< �ڿ����ŵ�CDC��������ʾ�ı�����
class CDMemDCWord
{
public:
	CPoint m_srcPoint;				//!< ԭʼ����ϵ�µ�����
	CString m_strWords;				//!< ��ԭʼ����ϵ��Ҫ���������
	LOGFONT m_FontInfo;				//!< ԭʼ����ϵ�µ�������Ϣ

public:
	CDMemDCWord(CPoint srcPoint, CString str, LOGFONT logfont)
	{
		m_srcPoint = srcPoint;
		m_strWords = str;
		memcpy(&m_FontInfo, &logfont, sizeof(LOGFONT));
	}
	~CDMemDCWord(void){;}
};

//!< �����������ŵ�CDC
class CDMemDC :
	public CXTMemDC
{
private:
	COLORREF m_colorbk;		//!< ����ɫ
	UINT m_uiZoomXNum;		//!< �������
	UINT m_uiZoomXDNum;		//!< �������
	UINT m_uiZoomYNum;		//!< �������
	UINT m_uiZoomYDNum;		//!< �������

	std::list<std::shared_ptr<CDMemDCWord> > m_ltWords;	//!< ����ԭʼ����ϵ��������ı�

private:
	void DrawWords(CDC* pDC, CPoint point);					//!< ������Ļ�����Ѿ��������ˣ������־�����һ����ƫ��

public:
	CDMemDC(CDC* pDC, const CRect& rect, COLORREF clrColor = GetSysColor(COLOR_3DFACE));
	~CDMemDC(void);
	virtual void SetZoomStyle(UINT xNum, UINT xDNum, UINT yNum, UINT yDNum);
	virtual BOOL TextOut(LONG x, LONG y, CString str, LOGFONT logfont);
};
}