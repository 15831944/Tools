#include "StdAfx.h"
#include "PMApp.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "SoftInfo.h"
#include "Gbl.h"
#include "MainFrm.h"
#include "ItemMgr.h"
#include "ItemGrid.h"

using namespace SoftInfo;

CSoftInfo::CSoftInfo(void)
{
	m_vtColInfo.resize(MVC::Item::CItemGrid::COL_REV_DB + 1);
	m_vtColInfo[MVC::Item::CItemGrid::COL_NAME].Init(_T("名称"), true, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_TAG].Init(_T("标签"), true, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_ID].Init(_T("编号"), true, 60);
	m_vtColInfo[MVC::Item::CItemGrid::COL_SRC].Init(_T("数据源"), true, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_ACCESS].Init(_T("访问权限"), true, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_TYPE].Init(_T("工程值类型"), true, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_IOTYPE].Init(_T("IO值类型"), true, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_VALUE].Init(_T("工程值"), true, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_IOVALUE].Init(_T("原始值"), true, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_CONVERT].Init(_T("转换"), false, 150);
	m_vtColInfo[MVC::Item::CItemGrid::COL_GROUP].Init(_T("变量组"), false, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_DESCRIPT].Init(_T("备注"), false, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_SCRIPT].Init(_T("脚本"), false, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_FRESHTIME].Init(_T("刷新时间"), true, 60);
	m_vtColInfo[MVC::Item::CItemGrid::COL_BIT].Init(_T("位报警"), true, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_LOLO].Init(_T("下下限"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_LOW].Init(_T("下限"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_HIGH].Init(_T("上限"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_HIHI].Init(_T("上上限"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_AIM].Init(_T("目标"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_SHIFT].Init(_T("变化率"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_REV_DB].Init(_T("保留历史"), true, 40);

	m_bTooltip = true;														//!< 悬停提示，要不要有
	m_uiFreshDataTime = 500;												//!< 刷新数据的默认周期，默认500毫秒
	m_bIsOutPut = true;														//!< 是否启动输出框进行输出
	m_bIsOutWarning = false;												//!< 是否输出警告信息
	m_uiZoomSize = 100;														//!< 缩放比例，X%
	m_uiCloneNumber = 1;													//!< 克隆变量名称整数间隔数
	m_uiCloneFloat = 16;													//!< 克隆变量名称浮点进位数
	m_uiItemFloatWidth = 6;													//!< 变量值小数点保留位数
	m_bShowHex = false;														//!< 是否按十六进制显示
	m_bPrintItemWaterMark = false;											//!< 是否打印水印
	m_bCplXml = false;
	m_nCmplOverTime = 600000;												//!< 编译超时时间,默认10分钟
	m_strItemNabled = _T("Yes");											//!< 变量名称是否能修改
	std::shared_ptr<CFont> tuopuFont= std::shared_ptr<CFont>(new CFont);
	tuopuFont->CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));
	tuopuFont->GetLogFont(&m_lgTuopuFontInfo);								//!< 拓扑图字体信息
}

CSoftInfo::~CSoftInfo(void)
{
}

CSoftInfo& SoftInfo::CSoftInfo::GetMe()
{
	static CSoftInfo one;
	return one;
}

void CSoftInfo::SaveFile()
{
	TiXmlDocument pTiXml(CGbl::GetMe().getDataPath() + _T("DEditor.cfg"));
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< 起始声明
	pTiXml.LinkEndChild(decl);
	TiXmlComment * comment = new TiXmlComment();							//!< 注释信息
	comment->SetValue(_T("请勿手工编辑此文档"));
	pTiXml.LinkEndChild(comment);
	TiXmlElement *pRoot=new TiXmlElement("DocumentRoot");					//!< 写入根节点
	SerializeXml(pRoot, false);												//!< 添加工程节点,写操作
	pTiXml.LinkEndChild(pRoot);
	if(!pTiXml.SaveFile()){
		MessageBox(NULL, _T("保存失败！"), _T("错误"), MB_ICONEXCLAMATION);
		return ;
	}
}

void CSoftInfo::LoadFile()
{
	TiXmlDocument pTiXml(CGbl::GetMe().getDataPath() + _T("DEditor.cfg"));
	if(!pTiXml.LoadFile())		return;
	//!< 开始解析
	SerializeXml(pTiXml.RootElement(), true);
}

void CSoftInfo::SerializeXml(TiXmlElement *pNode, bool bRead)
{
	if(bRead)
	{
		TiXmlElement* pChild = pNode->FirstChildElement();
		while(pChild)
		{
			CString text = pChild->ValueStr().c_str();
			if(text == _T("ItemGrid"))				SerializeItem(pChild, bRead);
			else if(text == _T("TuoPu"))			SerializeTuoPu(pChild, bRead);
			else if(text == _T("Server"))			SerializeServer(pChild, bRead);
			else if(text == _T("Compile"))			SerializeCompile(pChild, bRead);
			else if(text == _T("Pawer"))			SerializePower(pChild, bRead);
			pChild = pChild->NextSiblingElement();
		}
	}
	else
	{
		SerializeItem(pNode->AddTiXmlChild(_T("ItemGrid")), bRead);
		SerializeTuoPu(pNode->AddTiXmlChild(_T("TuoPu")), bRead);
		SerializeServer(pNode->AddTiXmlChild(_T("Server")), bRead);
		SerializeCompile(pNode->AddTiXmlChild(_T("Compile")), bRead);
		SerializePower(pNode->AddTiXmlChild(_T("Pawer")), bRead);
	}
}

void CSoftInfo::SerializeItem(TiXmlElement *pNode, bool bRead)
{
	if(bRead)
	{
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			CString name = pAttr->Name();
			CString value = pAttr->Value();
			if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_NAME].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_NAME].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_TAG].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_TAG].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_ID].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_ID].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_SRC].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_SRC].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_ACCESS].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_ACCESS].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_TYPE].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_TYPE].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_IOTYPE].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_IOTYPE].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_VALUE].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_VALUE].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_IOVALUE].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_IOVALUE].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_GROUP].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_GROUP].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_CONVERT].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_CONVERT].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_DESCRIPT].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_DESCRIPT].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_SCRIPT].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_SCRIPT].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_FRESHTIME].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_FRESHTIME].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_BIT].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_BIT].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_LOLO].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_LOLO].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_LOW].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_LOW].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_HIGH].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_HIGH].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_HIHI].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_HIHI].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_AIM].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_AIM].bShow = (value == _T("1")?true:false);
			else if(name == m_vtColInfo[MVC::Item::CItemGrid::COL_SHIFT].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_SHIFT].bShow = (value == _T("1")?true:false);
			else if (name == m_vtColInfo[MVC::Item::CItemGrid::COL_REV_DB].name)
				m_vtColInfo[MVC::Item::CItemGrid::COL_REV_DB].bShow = (value == _T("1") ? true : false);
			else if(name == _T("CloneNumber"))		m_uiCloneNumber = (UINT)atoi(value);
			else if(name == _T("CloneFloat"))		m_uiCloneFloat = (UINT)atoi(value);
			else if(name == _T("ItemNabled"))		m_strItemNabled = value;
			else if(name == _T("ShowHex"))			m_bShowHex = (value == _T("1")?true:false);
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_NAME].name, m_vtColInfo[MVC::Item::CItemGrid::COL_NAME].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_TAG].name, m_vtColInfo[MVC::Item::CItemGrid::COL_TAG].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_ID].name, m_vtColInfo[MVC::Item::CItemGrid::COL_ID].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_SRC].name, m_vtColInfo[MVC::Item::CItemGrid::COL_SRC].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_ACCESS].name, m_vtColInfo[MVC::Item::CItemGrid::COL_ACCESS].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_TYPE].name, m_vtColInfo[MVC::Item::CItemGrid::COL_TYPE].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_IOTYPE].name, m_vtColInfo[MVC::Item::CItemGrid::COL_IOTYPE].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_VALUE].name, m_vtColInfo[MVC::Item::CItemGrid::COL_VALUE].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_IOVALUE].name, m_vtColInfo[MVC::Item::CItemGrid::COL_IOVALUE].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_CONVERT].name, m_vtColInfo[MVC::Item::CItemGrid::COL_CONVERT].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_GROUP].name, m_vtColInfo[MVC::Item::CItemGrid::COL_GROUP].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_DESCRIPT].name, m_vtColInfo[MVC::Item::CItemGrid::COL_DESCRIPT].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_SCRIPT].name, m_vtColInfo[MVC::Item::CItemGrid::COL_SCRIPT].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_FRESHTIME].name, m_vtColInfo[MVC::Item::CItemGrid::COL_FRESHTIME].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_BIT].name, m_vtColInfo[MVC::Item::CItemGrid::COL_BIT].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_LOLO].name, m_vtColInfo[MVC::Item::CItemGrid::COL_LOLO].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_LOW].name, m_vtColInfo[MVC::Item::CItemGrid::COL_LOW].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_HIGH].name, m_vtColInfo[MVC::Item::CItemGrid::COL_HIGH].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_HIHI].name, m_vtColInfo[MVC::Item::CItemGrid::COL_HIHI].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_AIM].name, m_vtColInfo[MVC::Item::CItemGrid::COL_AIM].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_SHIFT].name, m_vtColInfo[MVC::Item::CItemGrid::COL_SHIFT].bShow?1:0);
		pNode->SetAttribute(m_vtColInfo[MVC::Item::CItemGrid::COL_REV_DB].name, m_vtColInfo[MVC::Item::CItemGrid::COL_REV_DB].bShow ? 1 : 0);
		pNode->SetAttribute(_T("CloneNumber"), m_uiCloneNumber);
		pNode->SetAttribute(_T("CloneFloat"), m_uiCloneFloat);
		pNode->SetAttribute(_T("ItemNabled"), m_strItemNabled);
		pNode->SetAttribute(_T("ShowHex"), m_bShowHex?1:0);
	}
}

void CSoftInfo::SerializeTuoPu(TiXmlElement *pNode, bool bRead)
{
	if(bRead)
	{
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			CString name = pAttr->Name();
			CString value = pAttr->Value();
			 if(name == _T("ToolTip"))			m_bTooltip = (value == _T("1")?true:false);
			else if(name == _T("ZoomSize"))		m_uiZoomSize = (UINT)atoi(value.Trim());
			else if(name == _T("Font"))			SetFont(m_lgTuopuFontInfo, value.Trim());
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(_T("ToolTip"), m_bTooltip?1:0);
		pNode->SetAttribute(_T("ZoomSize"), m_uiZoomSize);
		pNode->SetAttribute(_T("Font"), GetFontText(m_lgTuopuFontInfo));
	}
}

void CSoftInfo::SerializeServer(TiXmlElement *pNode, bool bRead)
{
	if(bRead)
	{
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			CString name = pAttr->Name();
			CString val = pAttr->Value();
			if(name == _T("FreshTime"))				m_uiFreshDataTime = atoi(val);
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(_T("FreshTime"), m_uiFreshDataTime);
	}
}

void CSoftInfo::SerializeCompile(TiXmlElement *pNode, bool bRead)
{
	if(bRead)
	{
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			CString name = pAttr->Name();
			CString val = pAttr->Value();
			if(name == _T("IsOutPut"))				m_bIsOutPut = val == _T("0") ? false : true;
			else if(name == _T("IsOutWarning"))		m_bIsOutWarning = val == _T("0") ? false : true;
			else if(name == _T("CmpXml"))			m_bCplXml = val == _T("0") ? false : true;
			else if(name == _T("CmplOverTime"))		m_nCmplOverTime = atoi(val);
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(_T("IsOutPut"), m_bIsOutPut?1:0);
		pNode->SetAttribute(_T("IsOutWarning"), m_bIsOutWarning?1:0);
		pNode->SetAttribute(_T("CmpXml"), m_bCplXml?1:0);
		pNode->SetAttribute(_T("CmplOverTime"), m_nCmplOverTime);
	}
}

void CSoftInfo::SerializePower(TiXmlElement *pNode, bool bRead)
{
	if(bRead)
	{
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		while(pAttr)
		{
			CString name = pAttr->Name();
			CString val = pAttr->Value();
			if(name == _T("SerialNo"))			m_strSerialNo = val;
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(_T("SerialNo"), m_strSerialNo);
	}
}

void CSoftInfo::SetFreshDataTime(UINT fdt)
{
	m_uiFreshDataTime = fdt;
	CMainFrame* mf = (CMainFrame *)g_App.GetMainWnd();
	if(mf && CProjectMgr::GetMe().IsWatch()){
		mf->InitCommerTime();
		mf->KillTimer(1);
		mf->SetTimer(1, getFreshDataTime(), NULL);
	}
}

//!< 将本字体信息转成字符串，用于保存
CString CSoftInfo::GetFontText(LOGFONT& lf)
{
	CString part, strRet;
	part.Format("%d", lf.lfHeight);			strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfWidth);			strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfEscapement);		strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfOrientation);	strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfWeight);			strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfItalic);			strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfUnderline);		strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfStrikeOut);		strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfCharSet);		strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfOutPrecision);	strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfClipPrecision);	strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfQuality);		strRet = strRet + part + _T("~");
	part.Format("%d", lf.lfPitchAndFamily);	strRet = strRet + part + _T("~");
	part = lf.lfFaceName;					strRet = strRet + part + _T("~");
	return strRet.Left(strRet.GetLength() - 1);
}

//!< 将字符串解析成字体信息
void CSoftInfo::SetFont(LOGFONT&lf, CString strFont)
{
	std::vector<CString> vtStr;
	CGbl::SpliteBy(strFont, _T("~"), vtStr);
	if(vtStr.size() != 14)		return;		//!< 一共14项数据
	lf.lfHeight = (LONG)atoi(vtStr[0]);
	lf.lfWidth = (LONG)atoi(vtStr[1]);
	lf.lfEscapement = (LONG)atoi(vtStr[2]);
	lf.lfOrientation = (LONG)atoi(vtStr[3]);
	lf.lfWeight = (LONG)atoi(vtStr[4]);
	lf.lfItalic = (BYTE)atoi(vtStr[5]);
	lf.lfUnderline = (BYTE)atoi(vtStr[6]);
	lf.lfStrikeOut = (BYTE)atoi(vtStr[7]);
	lf.lfCharSet = (BYTE)atoi(vtStr[8]);
	lf.lfOutPrecision = (BYTE)atoi(vtStr[9]);
	lf.lfClipPrecision = (BYTE)atoi(vtStr[10]);
	lf.lfQuality = (BYTE)atoi(vtStr[11]);
	lf.lfPitchAndFamily = (BYTE)atoi(vtStr[12]);
	memset(&lf.lfFaceName[0], 0, LF_FACESIZE);
	strcpy(&lf.lfFaceName[0], vtStr[13].GetBuffer());
}

//!< 设置编译文件是否保存描述
void CSoftInfo::setCmpXml(bool b)
{
	if(m_bCplXml == b)		return;
	m_bCplXml = b;
	if(CProjectMgr::GetMe().GetProj())
		CProjectMgr::GetMe().GetProj()->SetModify(true);
}

//!< 设置显示十六进制数值
void CSoftInfo::setShowHex(bool b)
{
	if (b == m_bShowHex)	return;
	m_bShowHex = b;
	MVC::Item::CItemMgr::GetMe().FreshAllGrid();
}