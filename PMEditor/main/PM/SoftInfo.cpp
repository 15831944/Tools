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
	m_vtColInfo[MVC::Item::CItemGrid::COL_NAME].Init(_T("����"), true, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_TAG].Init(_T("��ǩ"), true, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_ID].Init(_T("���"), true, 60);
	m_vtColInfo[MVC::Item::CItemGrid::COL_SRC].Init(_T("����Դ"), true, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_ACCESS].Init(_T("����Ȩ��"), true, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_TYPE].Init(_T("����ֵ����"), true, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_IOTYPE].Init(_T("IOֵ����"), true, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_VALUE].Init(_T("����ֵ"), true, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_IOVALUE].Init(_T("ԭʼֵ"), true, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_CONVERT].Init(_T("ת��"), false, 150);
	m_vtColInfo[MVC::Item::CItemGrid::COL_GROUP].Init(_T("������"), false, 80);
	m_vtColInfo[MVC::Item::CItemGrid::COL_DESCRIPT].Init(_T("��ע"), false, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_SCRIPT].Init(_T("�ű�"), false, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_FRESHTIME].Init(_T("ˢ��ʱ��"), true, 60);
	m_vtColInfo[MVC::Item::CItemGrid::COL_BIT].Init(_T("λ����"), true, 100);
	m_vtColInfo[MVC::Item::CItemGrid::COL_LOLO].Init(_T("������"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_LOW].Init(_T("����"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_HIGH].Init(_T("����"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_HIHI].Init(_T("������"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_AIM].Init(_T("Ŀ��"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_SHIFT].Init(_T("�仯��"), true, 50);
	m_vtColInfo[MVC::Item::CItemGrid::COL_REV_DB].Init(_T("������ʷ"), true, 40);

	m_bTooltip = true;														//!< ��ͣ��ʾ��Ҫ��Ҫ��
	m_uiFreshDataTime = 500;												//!< ˢ�����ݵ�Ĭ�����ڣ�Ĭ��500����
	m_bIsOutPut = true;														//!< �Ƿ����������������
	m_bIsOutWarning = false;												//!< �Ƿ����������Ϣ
	m_uiZoomSize = 100;														//!< ���ű�����X%
	m_uiCloneNumber = 1;													//!< ��¡�����������������
	m_uiCloneFloat = 16;													//!< ��¡�������Ƹ����λ��
	m_uiItemFloatWidth = 6;													//!< ����ֵС���㱣��λ��
	m_bShowHex = false;														//!< �Ƿ�ʮ��������ʾ
	m_bPrintItemWaterMark = false;											//!< �Ƿ��ӡˮӡ
	m_bCplXml = false;
	m_nCmplOverTime = 600000;												//!< ���볬ʱʱ��,Ĭ��10����
	m_strItemNabled = _T("Yes");											//!< ���������Ƿ����޸�
	std::shared_ptr<CFont> tuopuFont= std::shared_ptr<CFont>(new CFont);
	tuopuFont->CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("����"));
	tuopuFont->GetLogFont(&m_lgTuopuFontInfo);								//!< ����ͼ������Ϣ
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
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< ��ʼ����
	pTiXml.LinkEndChild(decl);
	TiXmlComment * comment = new TiXmlComment();							//!< ע����Ϣ
	comment->SetValue(_T("�����ֹ��༭���ĵ�"));
	pTiXml.LinkEndChild(comment);
	TiXmlElement *pRoot=new TiXmlElement("DocumentRoot");					//!< д����ڵ�
	SerializeXml(pRoot, false);												//!< ��ӹ��̽ڵ�,д����
	pTiXml.LinkEndChild(pRoot);
	if(!pTiXml.SaveFile()){
		MessageBox(NULL, _T("����ʧ�ܣ�"), _T("����"), MB_ICONEXCLAMATION);
		return ;
	}
}

void CSoftInfo::LoadFile()
{
	TiXmlDocument pTiXml(CGbl::GetMe().getDataPath() + _T("DEditor.cfg"));
	if(!pTiXml.LoadFile())		return;
	//!< ��ʼ����
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

//!< ����������Ϣת���ַ��������ڱ���
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

//!< ���ַ���������������Ϣ
void CSoftInfo::SetFont(LOGFONT&lf, CString strFont)
{
	std::vector<CString> vtStr;
	CGbl::SpliteBy(strFont, _T("~"), vtStr);
	if(vtStr.size() != 14)		return;		//!< һ��14������
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

//!< ���ñ����ļ��Ƿ񱣴�����
void CSoftInfo::setCmpXml(bool b)
{
	if(m_bCplXml == b)		return;
	m_bCplXml = b;
	if(CProjectMgr::GetMe().GetProj())
		CProjectMgr::GetMe().GetProj()->SetModify(true);
}

//!< ������ʾʮ��������ֵ
void CSoftInfo::setShowHex(bool b)
{
	if (b == m_bShowHex)	return;
	m_bShowHex = b;
	MVC::Item::CItemMgr::GetMe().FreshAllGrid();
}