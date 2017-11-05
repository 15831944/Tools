#pragma once

#define MESSAGE_OK 11111

namespace SoftInfo{

typedef struct tagColInfo{
	CString name;				// 字段名
	bool bShow;					// 是否显示
	UINT width;					// 宽度
	void Init(CString n, bool show, UINT w){name = n; bShow = show; width = w;}
}SColInfo;

class CSoftInfo
{
public:
	std::vector<SColInfo> m_vtColInfo;	// 变量表可以显示的列

private:
	// 软件配置
	UINT m_uiLanguage;					// 当前语言，0中文简体，1英文……
	bool m_bTooltip;					// 悬停提示，要不要有
	UINT m_uiZoomSize;					// 缩放比例，X%
	CString m_strTPFontName;			// 拓扑图中的文字的字体
	LOGFONT m_lgTuopuFontInfo;			// 拓扑图中的文字信息，全局只能配置它的字体、字号

	UINT m_uiFreshDataTime;				// 刷新数据的默认周期
	bool m_bIsOutPut;					// 是否启动输出框进行输出
	bool m_bIsOutWarning;				// 是否输出警告信息
	UINT m_uiCloneNumber;				// 克隆变量名称数字间隔基数
	UINT m_uiCloneFloat ;				// 克隆变量名称浮点进位数
	UINT m_uiItemFloatWidth;			// 变量值小数点保留位数
	bool m_bPrintItemWaterMark;			// 是否打印水印
	bool m_bShowHex;					// 是否按十六进制显示

	bool m_bCplXml;						// 编译是否存描述，true存，false不存
	int m_nCmplOverTime;				// 编译超时时间
	CString m_strItemNabled;			// 变量名称是否能修改

	CString m_strSerialNo;				// 序列号

private:
	CSoftInfo(void);
	~CSoftInfo(void);

private:
	CString GetFontText(LOGFONT& lf);								// 将本字体信息转成字符串，用于保存
	void SetFont(LOGFONT&lf, CString strFont);						// 将字符串解析成字体信息

public:
	const CString getSerialNo(){return m_strSerialNo;}				// 获得已注册的序列号
	const bool getTooltip(){return m_bTooltip;}						// 获得拓扑编辑时是否要有鼠标悬停的提示框
	const bool IsCplXml(){return m_bCplXml;}						// 获得编译是否存描述，true存，false不存
	const UINT getZoomSize(){return m_uiZoomSize;}					// 获得缩放比例，单位%
	const UINT getFreshDataTime(){return m_uiFreshDataTime;}		// 获得刷新数据的默认周期
	const bool isShowHex(){return m_bShowHex;}						// 是否按十六进制显示
	const bool isOutPut(){return m_bIsOutPut;}						// 获得是否启动输出栏
	const bool isOutWarning(){return m_bIsOutWarning;}				// 获得是否输出警告信息
	const void getTuopuFontInfo(LOGFONT& lgFont)
	{memcpy(&lgFont, &m_lgTuopuFontInfo, sizeof(LOGFONT));}			// 获得拓扑图字体信息
	const UINT getCloneNumber(){return m_uiCloneNumber;}
	const UINT getCloneFloat(){return m_uiCloneFloat;}
	const UINT getItemFloatWidth(){return m_uiItemFloatWidth;}
	const bool isItemWaterMark(){return m_bPrintItemWaterMark;}		// 打印变量表时是否打印水印
	const bool isItemNabled(){return m_strItemNabled == _T("Yes");}	// 变量名称是否能修改
	const int getCompileOverTime(){return m_nCmplOverTime;}			// 编译超时时间

	void setSerialNo(CString strNo){m_strSerialNo = strNo;}			// 设置序列号
	void setTooltip(bool h){m_bTooltip = h;}						// 设置拓扑编辑时是否要有鼠标悬停的提示框
	void setZoomSize(UINT size){if(size >= 50 && size <= 500)		m_uiZoomSize = size;}				// 设置缩放比例，单位%
	void setOutPut(bool output){m_bIsOutPut = output;}				// 设置是否启动输出栏
	void setOutWarning(bool bOut){m_bIsOutWarning = bOut;}			// 设置是否输出警告信息
	void setTuopuFont(LOGFONT lgFont)
	{memcpy(&m_lgTuopuFontInfo, &lgFont, sizeof(LOGFONT));}			// 设置拓扑图字体信息
	void setCloneNumber(UINT number){m_uiCloneNumber = number;}		
	void setCloneFloat(UINT f){m_uiCloneFloat = f;}
	void setItemFloatWidth(UINT width){m_uiItemFloatWidth = width;}
	void setItemWaterMark(bool b){m_bPrintItemWaterMark = b;}		// 设置打印变量表时是否打印水印
	void setCmpXml(bool b);											// 设置编译文件是否保存描述

	void SerializeXml(TiXmlElement *pNode, bool bRead);
	void SerializeItem(TiXmlElement *pNode, bool bRead);
	void SerializeTuoPu(TiXmlElement *pNode, bool bRead);
	void SerializeServer(TiXmlElement *pNode, bool bRead);
	void SerializeCompile(TiXmlElement *pNode, bool bRead);
	void SerializePower(TiXmlElement *pNode, bool bRead);

public:
	static CSoftInfo& GetMe();
	void LoadFile();
	void SaveFile();

	void SetFreshDataTime(UINT fdt);
	void setShowHex(bool b);										// 设置显示十六进制数值
};
}