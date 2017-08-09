#include "StdAfx.h"
#include "Gbl.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "DevMgr.h"
#include "ScanSetInfo.h"
#include "DSerial.h"

//const UINT IP192_168_1_0 = 3232235776;		// 倒序
//const UINT IP192_168_1_255 = 3232236031;		// 倒序
const UINT IP192_168_1_0 = 108736;				// 正序
const UINT IP192_168_1_255 = 4278298816;		// 正序
const CString SCAN_EXPAND_NAME = _T("ScanInfo.xml"); 

const CString SCAN_INFORMATION = _T("Information");
const CString SCAN_COMPILED = _T("Compiled");
const CString SCAN_LEVEL = _T("Level");
const CString SCAN_SYNC = _T("Sync");

const CString SCAN_ETHERNET = _T("Ethernet");
const CString SCAN_USEETHERNET = _T("Active");
const CString SCAN_IPSTART = _T("IPStart");
const CString SCAN_IPEND = _T("IPEnd");

const CString SCAN_SERIAL = _T("Serial");
const CString SCAN_USESERIAL = _T("Active");
const CString SCAN_SERIALNO = _T("SerialNo");
const CString SCAN_SERIALSTART = _T("SerialStart");
const CString SCAN_SERIALEND = _T("SerialEnd");
const CString SCAN_COM = _T("Com");
const CString SCAN_BAUD = _T("Baud");
const CString SCAN_PARITY = _T("Parity");
const CString SCAN_DATABIT = _T("DataBit");
const CString SCAN_STOPBIT = _T("StopBit");

using namespace MVC;
using namespace Device;
CScanSetInfo::CScanSetInfo(void)
:m_bScanCompiled(false)			//!< 是否已经经过了编译
,m_nScanLevel(2)			//!< 扫描级数
,m_bUseEthernet(true)		//!< 是否启用以太网扫描
,m_bUseSerial(false)		//!< 是否启用串口扫描
,m_bSync(true)				//!< 是否将扫描配置同步到工程师站配置中
,m_uiIPStart(IP192_168_1_0)	//!< 扫描起始IP地址,192.168.1.0的倒序
,m_uiIPEnd(IP192_168_1_255)	//!< 扫描终止IP地址,192.168.1.255的倒序
,m_nSerialStart(1)			//!< 起始模块地址
,m_nSerialEnd(16)			//!< 终止模块地址
,m_strCom(_T("COM1"))		//!< 串口
,m_strBaud(_T("9600"))		//!< 波特率
,m_strParity(_T("无"))		//!< 校验方式
,m_strDataBits(_T("8"))		//!< 数据位
,m_strStopBits(_T("1"))		//!< 停止位
{
}

CScanSetInfo::~CScanSetInfo(void)
{
}

//!< 数据初始化
void CScanSetInfo::Init()
{
	SetUnCompiled();				//!< 还没经过编译
// 	m_nScanLevel = 2;				//!< 扫描级数
// 	m_bUseEthernet = true;			//!< 是否启用以太网扫描
// 	m_bUseSerial = false;			//!< 是否启用串口扫描
// 	m_uiIPStart = IP192_168_1_0;	//!< 扫描起始IP地址,192.168.1.0的倒序
// 	m_uiIPEnd = IP192_168_1_255;	//!< 扫描终止IP地址,192.168.1.255的倒序
// 	m_nSerialStart = 1;				//!< 起始模块地址
// 	m_nSerialEnd = 16;				//!< 终止模块地址
// 	m_strCom = _T("COM1");			//!< 串口
// 	m_strBaud = _T("9600");			//!< 波特率
// 	m_strParity = _T("无");			//!< 校验方式
// 	m_strDataBits = _T("8");		//!< 数据位
// 	m_strStopBits = _T("1");		//!< 停止位
}


//!< 扫描级数
void CScanSetInfo::SetScanLevel(int level)
{
	if(level < 1)					level = 1;
	if(level > 5)					level = 5;
	if(level == m_nScanLevel)		return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_nScanLevel = level;
}

//!< 是否将扫描配置同步到工程师站配置中
void CScanSetInfo::SetSync(bool bSync)
{
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_bSync = bSync;
}

//!< 是否启用以太网扫描
void CScanSetInfo::SetUseEthernet(bool b)
{
	if(b == m_bUseEthernet)			return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_bUseEthernet = b;
}

//!< 是否启用串口扫描
void CScanSetInfo::SetUseSerial(bool b)
{
	if(b == m_bUseSerial)			return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_bUseSerial = b;
}

//!< 扫描起始IP地址
void CScanSetInfo::SetIPStart(DWORD start)
{
	if(start == m_uiIPStart)		return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_uiIPStart = start;
}

//!< 扫描终止IP地址
void CScanSetInfo::SetIPEnd(DWORD end)
{
	if(end == m_uiIPEnd)			return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_uiIPEnd = end;
}

//!< 起始模块地址
void CScanSetInfo::SetSerialStart(UINT start)
{
	if(start == m_nSerialStart)		return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_nSerialStart = start;
}

//!< 终止模块地址
void CScanSetInfo::SetSerialEnd(UINT end)
{
	if(end == m_nSerialEnd)			return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_nSerialEnd = end;
}

//!< 串口
void CScanSetInfo::SetCom(CString com)
{
	if(com == m_strCom)				return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_strCom = com;
}

//!< 波特率
void CScanSetInfo::SetBaud(CString baud)
{
	if(baud == m_strBaud)			return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_strBaud = baud;
}

//!< 校验方式
void CScanSetInfo::SetParity(CString parity)
{
	if(parity == m_strParity)		return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_strParity = parity;
}

//!< 数据位
void CScanSetInfo::SetDataBit(CString databit)
{
	if(databit == m_strDataBits)	return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_strDataBits = databit;
}

//!< 停止位
void CScanSetInfo::SetStopBit(CString stopbit)
{
	if(stopbit == m_strStopBits)	return;
	CDevMgr::GetMe().SetModify();
	SetUnCompiled();
	m_strStopBits = stopbit;
}

void CScanSetInfo::SaveFile()
{
	boost::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	CString pathAll = proj->GetPath() + SCAN_EXPAND_NAME;
	TiXmlDocument pTiXml(pathAll);
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< 起始声明
	pTiXml.LinkEndChild(decl);
	pTiXml.AddComment(_T("请勿手工编辑此文档"));							//!< 注释信息
	TiXmlElement *pRoot = new TiXmlElement("DocumentRoot");					//!< 写入根节点
	SerializeXml(pRoot, false);												//!< 添加工程节点,写操作
	pTiXml.LinkEndChild(pRoot);
	pTiXml.SaveFile();
}

void CScanSetInfo::LoadFile()
{
	boost::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	CString pathAll = proj->GetPath() + SCAN_EXPAND_NAME;
	TiXmlDocument pTiXml((LPCTSTR)pathAll);
	if(pTiXml.LoadFile())
		SerializeXml(pTiXml.RootElement(), true);	//!< 开始解析
}

void CScanSetInfo::SerializeXml(TiXmlElement* node, bool bRead)
{
	if(bRead)
	{
		CString text, strValue;
		CComVariant cv;
		TiXmlElement* element = node->FirstChildElement();
		bool bCompled = false;
		while(element)
		{
			text = element->ValueStr().c_str();
			if(SCAN_INFORMATION == text)
			{
				TiXmlAttribute* pAttr = element->FirstAttribute();
				while(pAttr)
				{
					text = pAttr->Name();
					strValue = pAttr->Value();
					cv = strValue;

					if(text == SCAN_COMPILED)			{bCompled = strValue == _T("1") ? true : false;}
					else if(text == SCAN_LEVEL)			{cv.ChangeType(VT_I4);	SetScanLevel(cv.intVal);}
					else if(text == SCAN_SYNC)			{SetSync(strValue == _T("1") ? true : false);}
					pAttr = pAttr->Next();
				}
			}
			else if(SCAN_ETHERNET == text)
			{
				TiXmlAttribute* pAttr = element->FirstAttribute();
				while(pAttr)
				{
					text = pAttr->Name();
					strValue = pAttr->Value();
					cv = strValue;

					if(text == SCAN_USEETHERNET)		{SetUseEthernet(strValue == _T("1") ? true : false);}
					else if(text == SCAN_IPSTART)		{cv.ChangeType(VT_I4);	SetIPStart(UINT(cv.intVal));}
					else if(text == SCAN_IPEND)			{cv.ChangeType(VT_I4);	SetIPEnd(UINT(cv.intVal));}
					pAttr = pAttr->Next();
				}
			}
			else if(SCAN_SERIAL == text)
			{
				TiXmlAttribute* pAttr = element->FirstAttribute();
				while(pAttr)
				{
					text = pAttr->Name();
					strValue = pAttr->Value();
					cv = strValue;

					if(text == SCAN_USESERIAL)			{SetUseSerial(strValue == _T("1") ? true : false);}
					else if(text == SCAN_SERIALNO)		{SetCom(cv);}
					else if(text == SCAN_SERIALSTART)	{cv.ChangeType(VT_I4);	SetSerialStart(UINT(cv.intVal));}
					else if(text == SCAN_SERIALEND)		{cv.ChangeType(VT_I4);	SetSerialEnd(UINT(cv.intVal));}
					else if(text == SCAN_BAUD)			{SetBaud(cv);}
					else if(text == SCAN_PARITY)		{SetParity(cv);}
					else if(text == SCAN_DATABIT)		{SetDataBit(cv);}
					else if(text == SCAN_STOPBIT)		{SetStopBit(cv);}
					pAttr = pAttr->Next();
				}
			}
			element = element->NextSiblingElement();
		}
		m_bScanCompiled = false;//bCompled;
	}
	else
	{
		TiXmlElement* element = node->AddTiXmlChild((LPCTSTR)SCAN_INFORMATION);
		element->SetAttribute(SCAN_COMPILED, IsCompiled() ? 1 : 0);
		element->SetAttribute(SCAN_LEVEL, GetScanLevel());
		element->SetAttribute(SCAN_SYNC, IsSync() ? 1 : 0);

		element = node->AddTiXmlChild((LPCTSTR)SCAN_ETHERNET);
		element->SetAttribute(SCAN_USEETHERNET, IsUseEthernet() ? 1 : 0);
		element->SetAttribute(SCAN_IPSTART, (int)GetIPStart());
		element->SetAttribute(SCAN_IPEND, (int)GetIPEnd());

		element = node->AddTiXmlChild((LPCTSTR)SCAN_SERIAL);
		element->SetAttribute(SCAN_USESERIAL, IsUseSerial() ? 1 : 0);
		element->SetAttribute(SCAN_SERIALNO, GetCom());
		element->SetAttribute(SCAN_SERIALSTART, GetSerialStart());
		element->SetAttribute(SCAN_SERIALEND, GetSerialEnd());
		element->SetAttribute(SCAN_BAUD, GetBaud());
		element->SetAttribute(SCAN_PARITY, GetParity());
		element->SetAttribute(SCAN_DATABIT, GetDataBit());
		element->SetAttribute(SCAN_STOPBIT, GetStopBit());
	}
}


//!< 同步到工程师站配置
void CScanSetInfo::OnSync()
{
	//!< 配置的都是串口的东西
	MVC::Device::CDevMgr* devMgr = &MVC::Device::CDevMgr::GetMe();
	boost::shared_ptr<MVC::Device::InterfaceSet::CDSerial> serial;
	CGbl* gbl = &CGbl::GetMe();

	foreach(serial, devMgr->m_ltSerial)
	{
		if(serial->getName() != GetCom())		continue;
		serial->setBaud(gbl->getBaudIndex(GetBaud()));
		serial->setCheckStyle(gbl->getCheckIndex(GetParity()));
		serial->setDataBitNum(gbl->getDataIndex(GetDataBit()));
		serial->setStopBitNum(gbl->getStopIndex(GetStopBit()));
		devMgr->SetModify();
	}
}
