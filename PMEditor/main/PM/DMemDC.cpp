#include "StdAfx.h"
#include "DMemDC.h"

using namespace Tool;
CDMemDC::CDMemDC(CDC* pDC, const CRect& rect, COLORREF clrColor)
: CXTMemDC(pDC, rect, clrColor)
{
	m_colorbk = clrColor;
	m_uiZoomXNum = m_uiZoomXDNum = m_uiZoomYNum = m_uiZoomYDNum = 1;
}

CDMemDC::~CDMemDC(void)
{
	if(!IsValid())	return;
	CPoint point = GetViewportOrg();
	SetViewportOrg(0,0);
	point.x = int(int(point.x * m_uiZoomXNum) / (int)m_uiZoomXDNum);
	point.y = int(int(point.y * m_uiZoomXNum) / (int)m_uiZoomXDNum);
	UINT width = (m_rc.Width() * m_uiZoomXNum) / m_uiZoomXDNum;
	UINT heigh = (m_rc.Height() * m_uiZoomYNum) / m_uiZoomYDNum;
	StretchBlt(m_rc.left, m_rc.top, width, heigh, this, 0, 0, m_rc.Width(), m_rc.Height(), SRCCOPY);
	DrawWords(this, point);

	if (IsValid())
	{
		m_pDC->BitBlt(m_rc.left, m_rc.top, m_rc.Width() + 200, m_rc.Height() + 200, this, 0, 0, SRCCOPY);
	}

	// Select the original bitmap back in
	if (m_hOldBitmap != NULL)
	{
		SelectObject(m_hOldBitmap);
		m_bitmap.DeleteObject();
	}
	DeleteDC();
}

void CDMemDC::SetZoomStyle(UINT xNum, UINT xDNum, UINT yNum, UINT yDNum)
{
	if(xNum == 0 || xDNum == 0 || yNum == 0 || yDNum == 0)		return;
	m_uiZoomXNum = xNum;
	m_uiZoomXDNum = xDNum;
	m_uiZoomYNum = yNum;
	m_uiZoomYDNum = yDNum;
}

BOOL CDMemDC::TextOut(LONG x, LONG y, CString str, LOGFONT logfont)
{
	CPoint point = CPoint(x, y);
	std::shared_ptr<CDMemDCWord> word = std::shared_ptr<CDMemDCWord>(new CDMemDCWord(point, str, logfont));
	m_ltWords.push_back(word);
	return TRUE;
}

void CDMemDC::DrawWords(CDC* pDC, CPoint point)
{
	if(!pDC)		return;
	std::shared_ptr<CDMemDCWord> words;
	CFont* pOldFont = NULL;
	long a = -16;
	long b = long(a * (long)m_uiZoomXNum);
	long c = long(b / (long)m_uiZoomXDNum);
	for (auto words : m_ltWords)
	{
		words->m_FontInfo.lfHeight = LONG(LONG(words->m_FontInfo.lfHeight * (LONG)m_uiZoomXNum) / (LONG)m_uiZoomXDNum);
		words->m_srcPoint.x = (words->m_srcPoint.x * m_uiZoomXNum) / m_uiZoomXDNum;
		words->m_srcPoint.y = (words->m_srcPoint.y * m_uiZoomXNum) / m_uiZoomXDNum;

		CFont font;
		font.CreateFontIndirect(&words->m_FontInfo);
		if(!pOldFont)	pOldFont = pDC->SelectObject(&font);
		else			pDC->SelectObject(&font);
		pDC->TextOut(words->m_srcPoint.x + point.x, words->m_srcPoint.y + point.y, words->m_strWords);
	}
//	if(pOldFont)		pDC->SelectObject(pOldFont);
}