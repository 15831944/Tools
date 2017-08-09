#include "StdAfx.h"
#include "DXPEditor.h"
#include "Gbl.h"
#include "MyHelp.h"

const CString HELP_FILE_NAME = _T("DView使用说明.chm");
const CString HELP_CONFIG = _T("DView Help.cfg");
const CString HELP_DEFAULT = _T("目录");

std::map<CString, CString> MP_HELP;		//!< 帮助字典
std::vector<CString> VT_TITLE;			//!< 名称列表
std::vector<CString> VT_URL;			//!< 地址列表

using namespace SoftInfo;
CMyHelp::CMyHelp(void)
{
	m_strHelpPath = CGbl::GetMe().getDocPath() + HELP_FILE_NAME;
}

CMyHelp::~CMyHelp(void)
{
}

//!< 初始化数据
void CMyHelp::InitInfo()
{
	TiXmlDocument pTiXml(CGbl::GetMe().getDataPath() + HELP_CONFIG);
	if(!pTiXml.LoadFile())		return;
	VT_TITLE.clear();			VT_URL.clear();
	//!< 开始解析
	SerializeXml(pTiXml.RootElement());
}

//!< 加载或保存配置
void CMyHelp::SerializeXml(TiXmlElement* pNode)
{
	TiXmlElement* pChild = pNode->FirstChildElement();
	while(pChild)
	{
		CString text = pChild->ValueStr().c_str();
		if(text == _T("Item"))
		{
			TiXmlAttribute* pAttr = pChild->FirstAttribute(_T("title"));
			ASSERT(pAttr);
			CString title = pAttr->Value();

			pAttr = pChild->FirstAttribute(_T("url"));
			ASSERT(pAttr);
			CString url = pAttr->Value();

			ASSERT(MP_HELP[title] == _T(""));		// 确保这个title不会出现两次
			MP_HELP[title] = url;

			VT_TITLE.push_back(title);
			VT_URL.push_back(url);
		}
		pChild = pChild->NextSiblingElement();
	}
}

//!< 显示帮助信息
void CMyHelp::ShowHelp(CString strTitle)
{
	//提取功能块信息，然后找到对应chm目录
	CString strUrl = GetHelpUrl(strTitle);
	if(strUrl != _T(""))
		::HtmlHelp(g_App.GetMainWnd()->GetSafeHwnd(), strUrl, HH_DISPLAY_TOPIC, 0);
}

//!< 根据索引名获得帮助文档的名称
CString CMyHelp::GetHelpUrl(CString strTitle)
{
	if(MP_HELP[strTitle] == _T("")){						// 如果没找到
		if(strTitle != HELP_DEFAULT)						// 如果还不是默认
			return GetHelpUrl(HELP_DEFAULT);				// 那就用默认的找
		return _T("");										// 默认的也没找到,返回空吧
	}
	return GetHelpPath() + _T("::/") + MP_HELP[strTitle];	// 如果找到了
}
