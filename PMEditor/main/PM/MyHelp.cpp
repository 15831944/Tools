#include "StdAfx.h"
#include "DXPEditor.h"
#include "Gbl.h"
#include "MyHelp.h"

const CString HELP_FILE_NAME = _T("DViewʹ��˵��.chm");
const CString HELP_CONFIG = _T("DView Help.cfg");
const CString HELP_DEFAULT = _T("Ŀ¼");

std::map<CString, CString> MP_HELP;		//!< �����ֵ�
std::vector<CString> VT_TITLE;			//!< �����б�
std::vector<CString> VT_URL;			//!< ��ַ�б�

using namespace SoftInfo;
CMyHelp::CMyHelp(void)
{
	m_strHelpPath = CGbl::GetMe().getDocPath() + HELP_FILE_NAME;
}

CMyHelp::~CMyHelp(void)
{
}

//!< ��ʼ������
void CMyHelp::InitInfo()
{
	TiXmlDocument pTiXml(CGbl::GetMe().getDataPath() + HELP_CONFIG);
	if(!pTiXml.LoadFile())		return;
	VT_TITLE.clear();			VT_URL.clear();
	//!< ��ʼ����
	SerializeXml(pTiXml.RootElement());
}

//!< ���ػ򱣴�����
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

			ASSERT(MP_HELP[title] == _T(""));		// ȷ�����title�����������
			MP_HELP[title] = url;

			VT_TITLE.push_back(title);
			VT_URL.push_back(url);
		}
		pChild = pChild->NextSiblingElement();
	}
}

//!< ��ʾ������Ϣ
void CMyHelp::ShowHelp(CString strTitle)
{
	//��ȡ���ܿ���Ϣ��Ȼ���ҵ���ӦchmĿ¼
	CString strUrl = GetHelpUrl(strTitle);
	if(strUrl != _T(""))
		::HtmlHelp(g_App.GetMainWnd()->GetSafeHwnd(), strUrl, HH_DISPLAY_TOPIC, 0);
}

//!< ������������ð����ĵ�������
CString CMyHelp::GetHelpUrl(CString strTitle)
{
	if(MP_HELP[strTitle] == _T("")){						// ���û�ҵ�
		if(strTitle != HELP_DEFAULT)						// ���������Ĭ��
			return GetHelpUrl(HELP_DEFAULT);				// �Ǿ���Ĭ�ϵ���
		return _T("");										// Ĭ�ϵ�Ҳû�ҵ�,���ؿհ�
	}
	return GetHelpPath() + _T("::/") + MP_HELP[strTitle];	// ����ҵ���
}
