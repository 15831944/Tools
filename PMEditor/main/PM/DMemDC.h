#pragma once

namespace Tool{
//!< 在可缩放的CDC，用于显示文本的类
class CDMemDCWord
{
public:
	CPoint m_srcPoint;				//!< 原始坐标系下的坐标
	CString m_strWords;				//!< 在原始坐标系下要输出的文字
	LOGFONT m_FontInfo;				//!< 原始坐标系下的字体信息

public:
	CDMemDCWord(CPoint srcPoint, CString str, LOGFONT logfont)
	{
		m_srcPoint = srcPoint;
		m_strWords = str;
		memcpy(&m_FontInfo, &logfont, sizeof(LOGFONT));
	}
	~CDMemDCWord(void){;}
};

//!< 可以用于缩放的CDC
class CDMemDC :
	public CXTMemDC
{
private:
	COLORREF m_colorbk;		//!< 背景色
	UINT m_uiZoomXNum;		//!< 横向乘数
	UINT m_uiZoomXDNum;		//!< 横向除数
	UINT m_uiZoomYNum;		//!< 纵向乘数
	UINT m_uiZoomYDNum;		//!< 纵向除数

	std::list<std::shared_ptr<CDMemDCWord> > m_ltWords;	//!< 想在原始坐标系下输出的文本

private:
	void DrawWords(CDC* pDC, CPoint point);					//!< 由于屏幕可能已经被滚动了，所以字具有了一定的偏移

public:
	CDMemDC(CDC* pDC, const CRect& rect, COLORREF clrColor = GetSysColor(COLOR_3DFACE));
	~CDMemDC(void);
	virtual void SetZoomStyle(UINT xNum, UINT xDNum, UINT yNum, UINT yDNum);
	virtual BOOL TextOut(LONG x, LONG y, CString str, LOGFONT logfont);
};
}