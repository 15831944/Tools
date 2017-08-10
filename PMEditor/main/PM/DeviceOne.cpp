#include "StdAfx.h"
#include "PMApp.h"
#include "MainFrm.h"
#include "ServerCommer.h"
#include "Gbl.h"
#include "SoftInfo.h"
#include "DMemDC.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "DeviceMapDoc.h"
#include "DeviceInterface.h"
#include "DeviceOne.h"
#include "DevMgr.h"
#include "DeviceParam.h"
#include "ConfigMgr.h"
#include "Config.h"

#include "XmlMgr.h"
#include "XmlDevice.h"
#include "XmlBehavior.h"
#include "XmlParaInfo.h"
#include "XmlInterface.h"

using namespace MVC;
using namespace Device;

const CString DEV_EXPAND_NAME = _T("dcfg");

const CString DEV_ROOT = _T("DevInfo");
const CString ID = _T("ID");
const CString TYPENAME = _T("DevTypeName");
const CString DEVTYPE = _T("DevType");
const CString NAME = _T("Name");
const CString TAG = _T("Tag");
const CString PATH = _T("Path");

const CString PARENT_ID = _T("ParentID");
const CString LEVEL = _T("Level");
const CString REDUNDANCY_FLAG = _T("RedundancyFlag");
const CString RELATE_DEVID = _T("RelateDevID");
const CString INTERFACE_SORT = _T("InterfaceSort");
const CString PARA2USER = _T("Para2User");

const CString INTERFACES = _T("Interface");

//////////////////////////////////////////////////////////////////////////
const CString DEV_PARA = _T("Para");

//////////////////////////////////////////////////////////////////////////
const int EXPAND_X = 44;				//!< 展开折叠图标的位置X
const int EXPAND_Y = 3;					//!< 展开折叠图标的位置Y
const int ONLINE_X = 44;				//!< 在离线图标的位置X
const int ONLINE_Y = 25;				//!< 在离线图标的位置Y

MVC::Device::CDeviceOne::CDeviceOne()
{
	m_uiID = 0;							//!< 设备ID号
	m_uiType = 0;						//!< 设备的类型号
	m_uiParentID = UINT(-1);			//!< 父设备编号，默认-1
	m_uiLevel = 0;						//!< 设备所在级别，从0开始，游离设备界别为-1
	m_bInterfaceSort = FALSE;			//!< 是否对设备接口的配置自动排序

	m_Point = CPoint(CDeviceMapDoc::DEV_EDGE, CDeviceMapDoc::DEV_EDGE);			//!< 自己的位置
	m_bModify = false;					//!< 是否参数被修改了
	m_bSelect = false;					//!< 是否被选中了
	m_bExpand = true;					//!< 表示设备是否处于伸展状态
	m_bShow = true;						//!< 是否画

	m_bLoadDcfg = false;				//!< 是否解析到设备属性配置文档*.dcfg
	m_bProj = true;						//!< 是否是工程设备，true是工程设备，false不是

	//!< 默认使用一个接口
	m_vtInterface.clear();
	std::shared_ptr<CDeviceInterface> inf = std::shared_ptr<CDeviceInterface>(new CDeviceInterface(this));
	inf->SetID(0);
	m_vtInterface.push_back(inf);
}

MVC::Device::CDeviceOne::~CDeviceOne(void)
{
}

//!< 作为新设备添加到拓扑中的事件
void MVC::Device::CDeviceOne::OnAddNew()
{
	m_bLoadDcfg = true;
	m_bModify = true;					//!< 是否参数被修改了
}

//!< 新增的设备要初始化，这个很重要
bool MVC::Device::CDeviceOne::InitDevType(UINT devType)
{
	if(!setDevType(devType))				return false;
	std::shared_ptr<CDeviceParam> param;
	std::shared_ptr<XmlInfo::CXmlParaInfo> xmlParam;

	//!< 创建所有参数
	int pCount = m_spXmlDevice->m_vtPara.size();
	m_vtParam.clear();
	m_vtParam.resize(pCount);
	for(int i = 0; i < pCount; ++i)
	{
		if(!m_spXmlDevice->m_vtPara[i])		continue;
		param = std::shared_ptr<CDeviceParam>(new CDeviceParam());
		param->InitParaType(m_spXmlDevice->m_vtPara[i],1);
		m_vtParam[i] = param;
	}

	//!< 遍历所有行为，为设置参数的数值数组
	std::shared_ptr<XmlInfo::CXmlBehavior> xmlBehavior;
	for (auto xmlBehavior : m_spXmlDevice->m_vtBehavior)
	{
		if(!xmlBehavior)					continue;
		if(!xmlBehavior->m_bShowUser)		continue;		//!< 不显示的行为我先不生成参数
		if(xmlBehavior->m_uiArrayNum < 1)	continue;		//!< 小于1不合法
		for (UINT id : xmlBehavior->m_ltParaID)
		{
			if(id >= pCount)				AfxMessageBox("参数错误");
			if(!m_vtParam[id])				continue;
			m_vtParam[id]->SetMaxSize(xmlBehavior->m_uiArrayNum);
		}
	}
	return true;
}

//!< 这个需要代码处理，计算一下存储对应的设备描述
bool MVC::Device::CDeviceOne::setDevType(UINT type)
{
	m_uiType = type;
	std::shared_ptr<XmlInfo::CXmlMgr> xmlMgr = ((CPMApp *)AfxGetApp())->m_XmlMgr;
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = xmlMgr->GetDevice(getDevType());
	if(!xmlDev)								return false;
	if(!xmlDev->OpenXml())					return false;	//!< 如果这个设备没有解析设备描述文件
	m_spXmlDevice = xmlDev;
	m_strType = m_spXmlDevice->getName();
	return true;
}

//!< 判断坐标是否在区域范围内,0表示没选中，1表示选中了，2表示选中了加减号
int MVC::Device::CDeviceOne::IsInMyRect(CPoint point)
{
	if(!IsShow())					return 0;
	point.x = (point.x * 100) / SoftInfo::CSoftInfo::GetMe().getZoomSize();
	point.y = (point.y * 100) / SoftInfo::CSoftInfo::GetMe().getZoomSize();
	int ret = 0;
	CRect rect(m_Point.x, m_Point.y,
		m_Point.x + CDeviceMapDoc::DEV_WIDTH, m_Point.y + CDeviceMapDoc::DEV_HEIGH);
	if(rect.PtInRect(point))	ret++;
	if(IsInExpandRect(point))	ret++;
	return ret;
}

//!< 递归设置一下设备的级别
void MVC::Device::CDeviceOne::OnSetLevel(UINT level)
{
	m_uiLevel = level;
	std::shared_ptr<CDeviceOne> device;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	for (UINT id : m_ltChildID){
		device = devMgr->GetDevice(id);
		ASSERT(device);
		device->OnSetLevel(level + 1);
	}
}

//!< 计算自己所在ZigBee结构中的层次
void MVC::Device::CDeviceOne::OnCountLevel()
{
	//!< 自己的层次得到了，该计算自己孩子的了
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> device;
	for (UINT id : m_ltChildID){
		device = devMgr->GetDevice(id);
		ASSERT(device);
		device->OnCountLevel();
	}
	if(SoftInfo::CSoftInfo::GetMe().getHDevMap())
		m_Point.y = m_uiLevel * CDeviceMapDoc::DEV_DIS_Y + 50;		//!< 20是头与Client边缘的间距，所有的都必须加上20
	else
		m_Point.x = m_uiLevel * CDeviceMapDoc::DEV_DIS_X + 50;
}

//!< 计算自己坐在的坐标
void MVC::Device::CDeviceOne::OnCountPoint()
{
	CPoint point=m_Point;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> device, empty;
	CDeviceMapDoc* pDoc = devMgr->m_DevMapDoc;
	if(!pDoc)		return;
	std::list<std::shared_ptr<CDeviceOne> > ltScanChild;

	if(!m_ltChildID.empty() && m_bExpand)	//!< 如果有下属设备，计算它们的坐标
	{
		for (UINT id : m_ltChildID){
			device = devMgr->GetDevice(id);
			ASSERT(device);
			if(device->IsProj())		device->OnCountPoint();
			else						ltScanChild.push_back(device);
		}
		for (auto device : ltScanChild)	device->OnCountPoint();		//!< 扫描设备要画到后头去
		device = empty;
		for (UINT id : m_ltChildID)
		{
			if(!devMgr->GetDevice(id)->IsProj())	continue;
			device = devMgr->GetDevice(id);
			break;
		}
		if(device == empty)
			device = devMgr->GetDevice(m_ltChildID.front());
		if(SoftInfo::CSoftInfo::GetMe().getHDevMap()){
			m_Point.x = device->m_Point.x;
			pDoc->OnMoveMaxX(m_uiLevel, m_Point.x);
		}
		else{
			m_Point.y = device->m_Point.y;
			pDoc->OnMoveMaxX(m_uiLevel, m_Point.y);
		}
	}
	else									//!< 如果没有了，自己就等于该最右侧坐标再往外走100像素
	{
		if(SoftInfo::CSoftInfo::GetMe().getHDevMap()){
			m_Point.x = pDoc->m_MaxX[m_uiLevel];
			pDoc->OnMoveMaxX(m_uiLevel, m_Point.x);
		}
		else{
			m_Point.y = pDoc->m_MaxY[m_uiLevel];
			pDoc->OnMoveMaxX(m_uiLevel, m_Point.y);
		}
	}
	if(point!=m_Point)	pDoc->UpdateAllViews(NULL);
}

//!< 画设备
void MVC::Device::CDeviceOne::OnDraw(CDC* pDC)
{
	if(!IsShow())					return;
	OnDrawFrame(pDC);				//!< 画边框
	OnDrawWord(pDC);				//!< 画名字和编号
	OnDrawToParent(pDC);			//!< 画设备与主设备的连接线
	OnDrawPic(pDC);					//!< 画设备图片
}

//!< 话边框的函数，区分选中与未选中
void MVC::Device::CDeviceOne::OnDrawFrame(CDC* pDC)
{
	CPen *pOldPen;
	CPen pen1, pen2, pen3, pen4, pen5 ,pen6, pen7, pen8;
	pen1.CreatePen(PS_SOLID,1,COLORREF(RGB(113, 111, 100)));	//!< 深灰
	pen2.CreatePen(PS_SOLID,1,COLORREF(RGB(172, 168, 153)));	//!< 浅灰
	pen3.CreatePen(PS_SOLID,1,COLORREF(RGB(193, 210, 238)));	//!< 淡蓝
	pen4.CreatePen(PS_SOLID,1,COLORREF(RGB(152, 181, 226)));	//!< 中蓝
	pen5.CreatePen(PS_SOLID,1,COLORREF(RGB(241, 239, 226)));	//!< 乳白
	pen6.CreatePen(PS_SOLID,1,COLORREF(RGB(255, 255, 255)));	//!< 白色
	pen7.CreatePen(PS_SOLID,1,COLORREF(RGB(172, 168, 153)));	//!< 分割线外
	pen8.CreatePen(PS_SOLID,1,COLORREF(RGB(244, 242, 232)));	//!< 分割线内
	pOldPen = pDC->SelectObject(&pen1);

	if(IsSelect()){												//!< 选中状态
		pDC->SelectObject(&pen1);
		pDC->MoveTo(m_Point.x + 10, m_Point.y + 1);
		pDC->LineTo(m_Point.x + 46, m_Point.y + 1);
		pDC->LineTo(m_Point.x + 46, m_Point.y + 36);
		pDC->LineTo(m_Point.x + 10, m_Point.y + 36);
		pDC->LineTo(m_Point.x + 10, m_Point.y + 1);
	}
	pDC->SelectObject(pOldPen);
}

//!< 画文字部分
void MVC::Device::CDeviceOne::OnDrawWord(CDC *pDC)
{
	LOGFONT font_info;
	SoftInfo::CSoftInfo::GetMe().getTuopuFontInfo(font_info);
	if(IsSelect())		font_info.lfWeight = FW_HEAVY;
	CString strName = m_strName;
	if(strName.GetLength() > 11)	strName = CGbl::StringFormat(strName, 9);
	CPoint point = CPoint(m_Point.x + 2, m_Point.y + 38);

	if(pDC->IsKindOf(RUNTIME_CLASS(Tool::CDMemDC)))
	{
		((Tool::CDMemDC *)pDC)->TextOut(point.x, point.y, strName, font_info);
		if(SoftInfo::CSoftInfo::GetMe().getShowAddr())
		{	// 画地址
			int i = 0;
			for (std::shared_ptr<CDeviceInterface> inf : m_vtInterface)
			{
				if(!inf)					continue;
				CString strInterface =inf->GetName();
				if(strInterface.GetLength() > 20) strInterface = CGbl::StringFormat(strInterface, 18);
				//((Tool::CDMemDC *)pDC)->TextOut(point.x, point.y + 14 * (++i), inf->GetName(), font_info);
				((Tool::CDMemDC *)pDC)->TextOut(point.x, point.y + 14 * (++i), strInterface, font_info);
			}
		}
	}
	else
	{
		CFont font, *pOldFont;
		font.CreateFontIndirect(&font_info);
		pOldFont = pDC->SelectObject(&font);
		pDC->TextOut(point.x, point.y, strName);					//!< 名称
		if(SoftInfo::CSoftInfo::GetMe().getShowAddr())
		{	// 画地址
			int i = 0;
			for (std::shared_ptr<CDeviceInterface> inf : m_vtInterface)
			{
				if(!inf)					continue;
				CString strInterface =inf->GetName();
				if(strInterface.GetLength() > 20) strInterface = CGbl::StringFormat(strInterface, 18);
				//pDC->TextOut(point.x, point.y + 13 * (++i), inf->GetName());
				pDC->TextOut(point.x, point.y + 13 * (++i), strInterface);
			}
			if(IsSelect())		pDC->SelectObject(pOldFont);
		}
	}
}

//!< 画设备与主设备的连接线
void MVC::Device::CDeviceOne::OnDrawToParent(CDC* pDC)
{
	if(m_uiParentID == UINT(-1)){
		OnDrawToHostLine(pDC);
		return;
	}
	std::shared_ptr<CDeviceOne> device = CDevMgr::GetMe().GetDevice(m_uiParentID);
	ASSERT(device);
	if(!device)					return;
	CPen *pOldPen;
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, CDeviceInterface::INF_SLAVE);		//!< 正常情况
	pOldPen = pDC->SelectObject(&pen);								//!< 交换画笔
	CPoint ptBegin,ptEnd;
	if(SoftInfo::CSoftInfo::GetMe().getHDevMap()){
		int offsetLen = 0;		// 如果是要显示设备地址,那么这根线要连多长就跟父设备的借口数量有关了
		if(SoftInfo::CSoftInfo::GetMe().getShowAddr())
			offsetLen = device->m_vtInterface.size() * 10;
		ptBegin.x = device->m_Point.x + CDeviceMapDoc::DEV_WIDTH / 3;
		ptBegin.y = device->m_Point.y + CDeviceMapDoc::DEV_HEIGH + offsetLen;
		ptEnd.x = m_Point.x + CDeviceMapDoc::DEV_WIDTH / 3;
		ptEnd.y = m_Point.y;
		pDC->MoveTo(ptBegin.x, ptBegin.y);
		pDC->LineTo(ptBegin.x, (ptEnd.y + ptBegin.y) / 2);
		pDC->LineTo(ptEnd.x, (ptEnd.y + ptBegin.y) / 2);
		pDC->LineTo(ptEnd.x, ptEnd.y);
	}
	else{
		ptBegin.x = device->m_Point.x + CDeviceMapDoc::DEV_WIDTH;
		ptBegin.y = device->m_Point.y + CDeviceMapDoc::DEV_HEIGH / 3;
		ptEnd.x = m_Point.x;
		ptEnd.y = m_Point.y + CDeviceMapDoc::DEV_HEIGH / 3;
		pDC->MoveTo(ptBegin.x, ptBegin.y);
		pDC->LineTo((ptBegin.x + ptEnd.x) / 2, ptBegin.y);
		pDC->LineTo((ptBegin.x + ptEnd.x) / 2, ptEnd.y);
		pDC->LineTo(ptEnd.x, ptEnd.y);
	}
	pDC->SelectObject(pOldPen);				//!< 交换画笔
}

void MVC::Device::CDeviceOne::OnDrawToHostLine(CDC* pDC)
{
	CPen *pOldPen = NULL;
	std::shared_ptr<CDeviceInterface> inf;
	int index = 0;
	bool bWatch = CProjectMgr::GetMe().IsWatch();
	for (auto inf : m_vtInterface)
	{
		if(!inf)							continue;
		if(bWatch && inf->GetState() == 0)	continue;	//!< 监控状态下,不在线的接口不划线
		CPoint pt = m_Point;
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, inf->GetInfColor());
		pOldPen = pDC->SelectObject(&pen);

		if(SoftInfo::CSoftInfo::GetMe().getHDevMap()){
			pt.x = pt.x + index * 3;
			pDC->MoveTo(pt.x + CDeviceMapDoc::DEV_WIDTH / 3, pt.y);
			pDC->LineTo(pt.x + CDeviceMapDoc::DEV_WIDTH / 3, CDeviceMapDoc::DEV_HOSTLINE_Y + 5);
		}
		else{
			pt.y = pt.y + index * 3;
			pDC->MoveTo(pt.x, pt.y + CDeviceMapDoc::DEV_HEIGH / 3);
			pDC->LineTo(CDeviceMapDoc::DEV_HOSTLINE_X + 5, pt.y + CDeviceMapDoc::DEV_HEIGH / 3);
		}
		++index;
	}
	if(!pOldPen)		pDC->SelectObject(pOldPen);
}

void MVC::Device::CDeviceOne::OnDrawPic(CDC* pDC)
{
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BMP_DEVICELIST);
	memdc.SelectObject(&bitmap);
	BITMAP bm;
	bitmap.GetObject(sizeof(BITMAP),&bm);
	int iconIndex = m_spXmlDevice->getIcon();
	if(iconIndex >= (bm.bmWidth / 32))	m_spXmlDevice->m_uiDevIcon = 0;
	if(!IsProj())		iconIndex = (bm.bmWidth / 32) - 1;			//!< 为加载到工程中的设备使用这个图标
	pDC->BitBlt(m_Point.x + 12, m_Point.y + 3, 32, 32, &memdc, 32 * iconIndex, 0, SRCAND);

	//!< 如果有孩子，要画加减号
	if(!m_ltChildID.empty()){
		CDC memdc2;
		memdc2.CreateCompatibleDC(pDC);
		CBitmap bitmap2;
		bitmap2.LoadBitmap(IDB_BMP_EXPAND);
		memdc2.SelectObject(&bitmap2);
		bitmap2.GetObject(sizeof(BITMAP),&bm);
		pDC->BitBlt(m_Point.x + EXPAND_X + 10, m_Point.y + EXPAND_Y, 11, 11, &memdc2, 0, 0, SRCAND);
		if(!m_bExpand)
		{
			CPen p;
			p.CreatePen(PS_SOLID, 1, COLORREF(RGB(48,72,96)));
			pDC->SelectObject(p);
			pDC->MoveTo(m_Point.x + EXPAND_X + 15, m_Point.y + 6);
			pDC->LineTo(m_Point.x + EXPAND_X + 15, m_Point.y + 11);
		}
	}
}

//!< 显示或隐藏自己和孩子
void MVC::Device::CDeviceOne::ShowHideChild(bool show)
{
	SetShow(show);
	std::shared_ptr<CDeviceOne> device;
	CDevMgr* mgr = &CDevMgr::GetMe();
	if(!show)	//!< 不显示很好办，就是把他们都隐藏了
	{
		for (UINT id : m_ltChildID){
			device = mgr->GetDevice(id);
			ASSERT(device);
			device->ShowHideChild(show);
		}
	}
	else		//!< 显示的话，就要看自己是不是折叠的了
	{
		if(!m_bExpand)		return;
		for (UINT id : m_ltChildID){
			device = mgr->GetDevice(id);
			ASSERT(device);
			device->SetShow(show);
			device->ShowHideChild(show);
		}
	}
}

//!< 判断坐标是否在伸缩区域范围内
bool MVC::Device::CDeviceOne::IsInExpandRect(CPoint point)
{
	CRect rect(m_Point.x + EXPAND_X +7, m_Point.y + EXPAND_Y - 3, m_Point.x + EXPAND_X + 24, m_Point.y + EXPAND_Y + 14);
	if(rect.PtInRect(point))	return true;
	return false;
}

bool MVC::Device::CDeviceOne::SerializeXmlGeneral(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		CString name, strValue;
		CComVariant cv;
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		int iValue;
		m_uiParentID = UINT(-1);				//!< 初始化一下这个变量
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cv = strValue;
			if(ID == name)						{cv.ChangeType(VT_I4);		iValue = cv.intVal;		m_uiID = UINT(iValue);}
			else if(NAME == name)				{m_strName = cv;}
			else if(TYPENAME == name)			{m_strType = cv;}
			else if(DEVTYPE == name)
			{
				cv.ChangeType(VT_I4);
				iValue = cv.intVal;
				if(!InitDevType(UINT(iValue)))
				{
					MessageBox(NULL, _T("加载失败，存在不支持的设备类型 ") + m_strType, _T("错误"), MB_OK | MB_ICONEXCLAMATION);
					return false;
				}
			}
			else if(TAG == name)				{m_strTag = cv;}
			else if(PATH == name)				{m_strPathName = cv;}
			else if(PARENT_ID == name)			{cv.ChangeType(VT_I4);		iValue = cv.intVal;		m_uiParentID = UINT(iValue);}
			else if(LEVEL == name)				{cv.ChangeType(VT_I4);		iValue = cv.intVal;		m_uiLevel = UINT(iValue);}
			else if(INTERFACE_SORT == name)		{cv.ChangeType(VT_UI4);		m_bInterfaceSort = !!cv.uintVal;}

			pAttr = pAttr->Next();
		}
		m_vtInterface.clear();
		TiXmlElement* pChild = pNode->FirstChildElement();
		std::shared_ptr<CDeviceInterface> inf;
		while(pChild)
		{
			name = pChild->Value();
			if(INTERFACES == name){
				inf = std::shared_ptr<CDeviceInterface>(new CDeviceInterface(this));
				if(inf->SerializeXml(pChild, bRead))	m_vtInterface.push_back(inf);
			}
			for(int i = 0; i < (int)m_vtInterface.size(); ++i)
				m_vtInterface[i]->SetID(i);
			pChild = pChild->NextSiblingElement();
		}
	}
	else
	{
		pNode->SetAttribute(ID, (int)m_uiID);
		pNode->SetAttribute(NAME, m_strName);
		pNode->SetAttribute(TYPENAME, m_strType);
		pNode->SetAttribute(DEVTYPE, (int)getDevType());
		pNode->SetAttribute(TAG, m_strTag);
		pNode->SetAttribute(PATH, m_strPathName);

		pNode->SetAttribute(PARENT_ID, (int)m_uiParentID);
		pNode->SetAttribute(LEVEL, (int)m_uiLevel);
		pNode->SetAttribute(REDUNDANCY_FLAG, 0);
		pNode->SetAttribute(RELATE_DEVID, -1);
		pNode->SetAttribute(INTERFACE_SORT, m_bInterfaceSort?1:0);
		pNode->SetAttribute(PARA2USER, m_spXmlDevice->IsPara2User()?1:0);

		//std::shared_ptr<CDeviceInterface> inf;
		for (auto inf : m_vtInterface){
			if(!inf)				continue;
			if(!inf->IsProj())		continue;
			inf->SerializeXml(pNode->AddTiXmlChild((LPCTSTR)INTERFACES), bRead);
		}
	}
	return true;
}

//!< 解析xml文档
void MVC::Device::CDeviceOne::LoadXml()
{
	if(!m_bLoadDcfg)		//!< 如果存在并还没有打开，打开文件并加载
	{
		LoadXmlFile();
		m_bLoadDcfg = true;
	}
}

bool MVC::Device::CDeviceOne::LoadXmlFile()
{
#ifdef _DEBUG
	CGbl::PrintOut(m_strName + _T(" 解析XMl文件"));
#endif
	CString path = CProjectMgr::GetMe().GetProj()->GetPath();
	TiXmlDocument pTiXml(path + m_strPathName);
	if(!pTiXml.LoadFile()){
//		MessageBox(NULL, _T("设备拓扑文件格式错误！"), _T("错误"), MB_ICONEXCLAMATION);
		return false;
	}

	//!< 判断是否类型相同
	TiXmlElement* xmlPara = pTiXml.RootElement()->FirstChildElement(DEV_ROOT);
	if(!xmlPara)		return false;
	TiXmlAttribute* xmlAttr = xmlPara->FirstAttribute(DEVTYPE);
	if(!xmlAttr)		return false;
	UINT type = atoi(xmlAttr->Value());
	if(type != getDevType()){		//!< 类型不同，提示错误
		CString text = _T("设备类型与文件中记录的类型不同，如果继续加载，不匹配的信息将丢失!\r\n是否继续加载？");
		if(IDNO == MessageBox(NULL, text, _T("提示"), MB_YESNO | MB_ICONINFORMATION))
			return false;
	}

	//!< 开始解析
	return SerializeXml(pTiXml.RootElement(), true);
}

//!< 保存文件的地址是相对地址 + 工程目录的地址
bool MVC::Device::CDeviceOne::SaveXmlFile()
{
	if(!m_bLoadDcfg)				return true;								//!< 如果都没打开过,就不用保存了
	if(IsModify())																//!< 如果没做修改,就不用保存
	{
		std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
		CString pathAll = proj->GetPath() + m_strName + _T(".") + DEV_EXPAND_NAME;
		TiXmlDocument pTiXml(pathAll);
		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< 起始声明
		pTiXml.LinkEndChild(decl);
		TiXmlComment * comment = new TiXmlComment();							//!< 注释信息
		comment->SetValue(_T("请勿手工编辑此文档"));
		pTiXml.LinkEndChild(comment);
		TiXmlElement *pRoot = new TiXmlElement("DocumentRoot");					//!< 写入根节点
		SerializeXml(pRoot, false);												//!< 添加工程节点,写操作
		pTiXml.LinkEndChild(pRoot);
		pTiXml.SaveFile();
	}
	m_bModify = false;
	return true;
}

bool MVC::Device::CDeviceOne::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		//!< 软件版本，先不考虑
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		CString name, strValue;
		CComVariant cvr;
		//!< 子节点
		int num = pNode->GetChildSize();
		if(num < 0)		return false;
		CGbl::SetProgressRange(num);
		TiXmlElement* xmlPara = pNode->FirstChildElement();
		std::shared_ptr<CDeviceParam> param, realParam;
		std::list< std::shared_ptr<CDeviceParam> > ltParam;
		while(xmlPara)
		{
			CGbl::SetProgressStep(1);
			name = xmlPara->Value();
			if(DEV_ROOT == name){}
			else if(DEV_PARA == name){
				param = std::shared_ptr<CDeviceParam>(new CDeviceParam());
				param->SerializeXml(xmlPara, bRead, this);
				//!< 先把他们仍在链表里
				ltParam.push_back(param);
			}
			xmlPara = xmlPara->NextSiblingElement();
		}
		//!< 根据链表中参数的ID最大值，来设置参数数据的大小
		UINT maxID = 0;
		for (auto param : ltParam)
			maxID = max(maxID, param->getParaID());
		m_vtParam.resize(maxID + 1);

		//!< 最后将链表中的参数导进数组中来，这里只导入那些匹配的
		for (auto param : ltParam)
		{
			realParam = GetParam(param->getParaID());
			if(!realParam)												continue;
			if(realParam->getName() != param->getName())				continue;
			if(realParam->getType() != param->getType())				continue;
			if(realParam->m_vtValue.size() != param->m_vtValue.size())	continue;
			m_vtParam[param->getParaID()] = param;
		}
		CGbl::SetProgressEnd();
	}
	else
	{
		pNode->SetAttribute(_T("FileType"), _T("EditDevFile"));
		pNode->SetAttribute(_T("Version"), _T("1.0"));
		TiXmlElement* para;

		//!< 添加DevInfo
		para = pNode->AddTiXmlChild((LPCTSTR)DEV_ROOT);
		para->SetAttribute(TYPENAME, getStrType());
		para->SetAttribute(DEVTYPE, getDevType());

		//!< 添加参数
		//std::shared_ptr<CDeviceParam> param;
		for (auto param : m_vtParam){
			if(!param)		continue;
			para = pNode->AddTiXmlChild((LPCTSTR)DEV_PARA);
			param->SerializeXml(para, bRead, this);
		}
	}
	return true;
}

//!< 导出设备配置
bool MVC::Device::CDeviceOne::DevOut(CString path, CString name)
{
	LoadXml();		//!< 如果还没解析描述，就先解析吧
	CString oldName = m_strName;
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	CString oldPath = proj->GetPath();
	path = CGbl::GetPathFromFilePathName(path);
	proj->SetPath(path);
	setName(name);
	SaveXmlFile();
	proj->SetPath(oldPath);
	setName(oldName);
	return true;
}

//!< 导入设备配置
bool MVC::Device::CDeviceOne::DevIn(CString path, CString name)
{
	CString oldName = m_strName;
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
	CString oldPath = proj->GetPath();
	path = CGbl::GetPathFromFilePathName(path);
	proj->SetPath(path);
	setName(name);
	LoadXmlFile();
	proj->SetPath(oldPath);
	setName(oldName);
	return true;
}

//!< 删除这个孩子
void MVC::Device::CDeviceOne::DeleteChild(UINT id)
{
	m_ltChildID.remove(id);
}

//!< 添加孩子
void MVC::Device::CDeviceOne::AddChild(UINT id)
{
	for (UINT index : m_ltChildID)
		if(index == id)
			return;
	m_ltChildID.push_back(id);
}

//!< 被复制了
void MVC::Device::CDeviceOne::OnCopy()
{
	std::shared_ptr<CDeviceOne> newDev = std::shared_ptr<CDeviceOne>(new CDeviceOne);
	newDev->CopyFrom(*this);
	newDev->setID(m_uiID);
	newDev->setParentID(m_uiParentID);
	CDevMgr::GetMe().m_ltDevClipBoard.push_back(newDev);
}

//!< 被复制了，自己的所有孩子也被复制了
void MVC::Device::CDeviceOne::OnCopyWithChild()
{
	OnCopy();
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> device;
	for (UINT id : m_ltChildID)
	{
		device = devMgr->GetDevice(id);
		ASSERT(device);
		device->OnCopyWithChild();
	}
}

//!< 被剪切了，孩子也得被剪切
void MVC::Device::CDeviceOne::OnCut(CDeviceMapDoc* pDoc)
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> device = devMgr->GetDevice(m_uiID);
	ASSERT(device);
	devMgr->m_ltDevClipBoard.push_back(device);
	std::shared_ptr<SDevUndo> sdu = std::shared_ptr<SDevUndo>(new SDevUndo(CGbl::UNDO_TYPE_DEL, device));
	pDoc->AddUndoMember(sdu);
	for (UINT id : m_ltChildID)
	{
		device = devMgr->GetDevice(id);
		ASSERT(device);
		device->OnCut(pDoc);
	}
}

//!< 被粘贴，将自己放到拓扑中
void MVC::Device::CDeviceOne::OnPaste()
{
}

//!< 重新定义接口数组的元素数量
void MVC::Device::CDeviceOne::ResizeInterface(UINT num)
{
	if(num > 32)	return;
	m_vtInterface.resize(num);
	std::shared_ptr<CDeviceInterface> inf;
	for(int i = 0; i < num; ++i)
	{
		if(m_vtInterface[i])	continue;
		inf = std::shared_ptr<CDeviceInterface>(new CDeviceInterface(this));
		m_vtInterface[i] = inf;
		inf->SetID(i);
	}
}

//!< 获得设备类型名称
CString MVC::Device::CDeviceOne::GetTypeName()
{
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDevice = GetXmlInfo();
	if(!xmlDevice)	return _T("无");
	return xmlDevice->m_strName;
}

//!< 对设备右键
void MVC::Device::CDeviceOne::OnRButtonUp(UINT nFlags, CPoint point, CWnd* wnd)
{
	CMenu popMenu;
	popMenu.LoadMenu(IDR_RBUTTON_MENU);
	CDevMgr* devMgr = &CDevMgr::GetMe();
	CMenu* menu;
	if(m_bProj)
	{
		menu =popMenu.GetSubMenu(2);
		if(devMgr->m_ltDevClipBoard.empty())
		{
			menu->EnableMenuItem(ID_EDIT_PASTE, MF_DISABLED|MF_BYCOMMAND);
		}
		if(m_ltChildID.empty())
		{
			menu->EnableMenuItem(ID_COPY_WITH_CHILD, MF_DISABLED|MF_BYCOMMAND);
		}
		menu->EnableMenuItem(ID_DEV_CONFIG, MF_DISABLED|MF_BYCOMMAND);
		menu->EnableMenuItem(ID_DEV_OUT, MF_DISABLED|MF_BYCOMMAND);
		menu->EnableMenuItem(ID_FRESH_STATE, MF_DISABLED|MF_BYCOMMAND);
		if(CProjectMgr::GetMe().IsWatch())
		{
			menu->EnableMenuItem(ID_ADD_MAINDEV, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_ADD_SLAVE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_DELETE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_DEVINTERFACE_SET, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_EDIT_PASTE, MF_DISABLED|MF_BYCOMMAND);
			menu->EnableMenuItem(ID_EDIT_CUT, MF_DISABLED|MF_BYCOMMAND);
			if(GetOnLineInf() == -1)
				menu->EnableMenuItem(ID_FRESH_STATE, MF_ENABLED|MF_BYCOMMAND);
		}
		else
		{
			menu->DeleteMenu(ID_FRESH_STATE, MF_BYCOMMAND);
			//menu->EnableMenuItem(ID_FRESH_STATE, MF_REMOVE|MF_BYCOMMAND);
		}
		if(!GetXmlInfo()->IsProgrammable()){
			menu->EnableMenuItem(ID_PROGRAM, MF_DISABLED | MF_BYCOMMAND);
		}
		if (m_spXmlDevice->getMaxInf2Slave() <= 0){
			menu->EnableMenuItem(ID_ADD_SLAVE, MF_DISABLED | MF_BYCOMMAND);
		}
	}
	else
	{
		menu =popMenu.GetSubMenu(7);
	}
	CXTPCommandBars::TrackPopupMenu(menu, TPM_LEFTALIGN|TPM_NONOTIFY, point.x, point.y, wnd);
}

void MVC::Device::CDeviceOne::setName(CString name)
{
	m_strName = name;
	setPath(m_strName + _T(".") + DEV_EXPAND_NAME);
}

void MVC::Device::CDeviceOne::CopyFrom(CDeviceOne& device)
{
	setID(device.getID());
	setDevType(device.getDevType());
	setName(device.getName());
	setTag(device.getTag());
	setParentID(device.getParentID());
	setInterfaceSort(device.getInterfaceSort());
	setStrID(device.getStrID());

	std::shared_ptr<CDeviceInterface> inf, newInf;
	m_vtInterface.clear();
	m_vtInterface.resize(device.m_vtInterface.size());
	for (auto inf : device.m_vtInterface)
	{
		if(!inf)		continue;
		newInf = std::shared_ptr<CDeviceInterface>(new CDeviceInterface(this));
		*newInf = *inf;
		m_vtInterface[inf->GetID()] = newInf;
	}

	std::shared_ptr<CDeviceParam> para, newPara;
	m_vtParam.clear();
	m_vtParam.resize(device.m_vtParam.size());
	for (auto para : device.m_vtParam)
	{
		if(!para)		continue;
		newPara = std::shared_ptr<CDeviceParam>(new CDeviceParam);
		*newPara = *para;
		m_vtParam[para->getParaID()] = newPara;
	}
}

//!< 判断两个设备是否一样
bool MVC::Device::CDeviceOne::operator == (CDeviceOne& device) const
{
	if(m_uiType != device.getDevType())					return false;
	if(m_strName != device.getName())					return false;
	if(m_strName != device.getTag())					return false;
	if(m_bInterfaceSort != device.getInterfaceSort())	return false;

	int count = m_vtInterface.size();
	if(count != device.m_vtInterface.size())			return false;
	for(int i = 0; i < count; ++i){
		if(*m_vtInterface[i] != *device.m_vtInterface[i])	return false;
	}
	return true;
}

//!< 获得第id个接口
std::shared_ptr<CDeviceInterface> MVC::Device::CDeviceOne::GetInterface(UINT id)
{
	std::shared_ptr<CDeviceInterface> inf, empty;
	if(id >= (UINT)m_vtInterface.size())	return empty;
	return m_vtInterface[id];
}

//!< 根据接口的类型找到第一个属于这个类型的接口
std::shared_ptr<CDeviceInterface> MVC::Device::CDeviceOne::GetInterfaceFromType(UINT type)
{
	std::shared_ptr<CDeviceInterface> inf, empty;
	for (auto inf : m_vtInterface){
		if(!inf)		continue;
		if(inf->GetType() == type)
			return inf;
	}
	return empty;
}

//!< 找参数
std::shared_ptr<CDeviceParam> MVC::Device::CDeviceOne::GetParam(UINT id)
{
	std::shared_ptr<CDeviceParam> empty;
	if(id >= (UINT)m_vtParam.size())	return empty;
	return m_vtParam[id];
}

//!< 设备是否在线，返回在线的接口ID，不在返回-1
long MVC::Device::CDeviceOne::GetOnLineInf()
{
	for (std::shared_ptr<CDeviceInterface> inf : m_vtInterface){
		if(!inf)				continue;
		if(inf->IsOnline())		return inf->GetID();
	}
	return -1;
}

//!< 设置设备在线状态，参数是接口ID
void MVC::Device::CDeviceOne::SetOnLineInf(UINT infID, bool bOnLine)
{
	if(infID < (UINT)m_vtInterface.size() && m_vtInterface[infID])
		m_vtInterface[infID]->SetState(bOnLine?1:0);
}

//!< 直接设置下线
void MVC::Device::CDeviceOne::SetOffLine()
{
	for (std::shared_ptr<CDeviceInterface> inf : m_vtInterface)
	{
		if(!inf)		continue;
		inf->SetState(0);
	}
}

//!< 获得监控时的信息
void MVC::Device::CDeviceOne::GetWatchInfo(Tool::TooltipText& toolTip)
{
	toolTip.txtHead = getName();
	CString strInf = _T("");

	if(CProjectMgr::GetMe().IsWatch()){
		long nState = getState();
		if(nState == 0)			strInf = _T("通信状态: 离线");
		else if(nState == 2)	strInf = _T("通信状态: 在线未配置");
		else if(nState == 3)	strInf = _T("通信状态: 冲突");
		else if(nState == 1){
			long id = GetOnLineInf();
			strInf = _T("通信状态: 在线\r\n");
			strInf += _T("通信接口: ") + m_vtInterface[id]->GetName() + _T("\r\n");
			if(	m_vtInterface[id]->GetType() ==0)
			{
				CString port;
				port.Format("%d",m_vtInterface[id]->GetDevPort());
				strInf += _T("通信端口: ") + port +_T("\r\n");
			}			
		}
	}
	else{
		int count = m_vtInterface.size();
		CString strID;
		for(int i = 0; i < count; ++i)
		{
			strID.Format("接口%d: ", i);
			strInf = strInf + strID + m_vtInterface[i]->GetName() + _T("\r\n");
		}
	}
	toolTip.txtBody.Format("编号:%d\r\n", getID());
	toolTip.txtBody = toolTip.txtBody + _T("类型: ") + GetTypeName();
	if(getTag() != _T(""))			toolTip.txtBody = toolTip.txtBody + _T("\r\n标签: ") + getTag();
	toolTip.txtBody = toolTip.txtBody + _T("\r\n") + strInf;
	if(m_uiLevel == 0)
		toolTip.txtRemark = _T("<a msg=\"主设备\">获得帮助.</a>");
	else
		toolTip.txtRemark = _T("<a msg=\"从设备\">获得帮助.</a>");
}

//!< id设备是否是自己的上代
bool MVC::Device::CDeviceOne::IsMyParent(UINT id)
{
	if(id < 0)					return false;
	if(getParentID() == id)		return true;		//!< 如果是自己的父亲，返回正确
	std::shared_ptr<CDeviceOne> dev = CDevMgr::GetMe().GetDevice(getParentID());
	if(!dev)					return false;		//!< 如果自己没有父亲了，也返回正确
	return dev->IsMyParent(id);						//!< 如果有父亲，看看是不是父亲的上一代
}

//!< 判断是否能连从设备
bool MVC::Device::CDeviceOne::CanAddSlave(bool bSayWhy /* = true */)
{
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = GetXmlInfo();
	if(!xmlDev)							return false;
	UINT maxSlave = xmlDev->getMaxInf2Slave();
	UINT slaveNum = 0;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> dev;
	for (auto dev : devMgr->m_vtDevice)
	{
		if(!dev)	continue;
		if(dev->getParentID() == m_uiID)	++slaveNum;
	}
	if(slaveNum >= maxSlave && bSayWhy)
	{
		CString strError;
		strError.Format("从设备数量已经达到最大值%d个，无法添加从设备", maxSlave);
		CGbl::PrintOut(strError);
	}
	return slaveNum < maxSlave;
}

//!< 判断是否能连该类型的从设备
bool MVC::Device::CDeviceOne::CanAddSlaveType(UINT devType)
{
	std::shared_ptr<XmlInfo::CXmlDevice> xmlDev = GetXmlInfo();
	std::shared_ptr<XmlInfo::CXmlDevice> xmlSlave = g_App.m_XmlMgr->GetDevice(devType);
	if(!xmlDev || !xmlSlave)			return false;
	return xmlDev->getInf2Slave() & xmlSlave->getInf2Main();
}

//!< 上载一个行为的参数，和组索引
void MVC::Device::CDeviceOne::UpLoadBehavior(UINT id, CComVariant cvrIndex)
{
	CMainFrame * mf = (CMainFrame *)g_App.GetMainWnd();
	std::shared_ptr<XmlInfo::CXmlBehavior> xmlBehavior = m_spXmlDevice->getBehavior(id);
	if(!xmlBehavior)	return;
	std::shared_ptr<CDeviceParam> para;
	VARIANT cvr;

	UINT count = 1;
	cvr.vt = VT_ARRAY|VT_VARIANT;
	cvr.parray = ::SafeArrayCreateVector(VT_VARIANT, 0, count);
	mf->m_VarArray[0] = cvrIndex;
	void* pDest;
	AfxCheckError(::SafeArrayAccessData(cvr.parray, &pDest));
	memcpy(pDest, mf->m_VarArray, count * sizeof(VARIANT));
	{
		std::list<UINT> ltAddr;
		GetWholeAddr(ltAddr, true);
		if(ltAddr.empty())
		{
			AfxCheckError(::SafeArrayUnaccessData(cvr.parray));
			AfxCheckError(::SafeArrayDestroy(cvr.parray));
			return;
		}
		UINT infType = ltAddr.size() == getLevel() + 1 ? 0 : 1;		//!< 如果数据相同表示以太网，否则表示串口，因为串口多个串口号
		UINT i = 0;
		long* plAddr = new long[ltAddr.size()];
		for (auto index : ltAddr)
		{
			plAddr[i++] = index;
		}
		delete[] plAddr;
	}
	AfxCheckError(::SafeArrayUnaccessData(cvr.parray));
	AfxCheckError(::SafeArrayDestroy(cvr.parray));
}

//!< 下载一个行为的参数，和组索引
void MVC::Device::CDeviceOne::DownLoadBehavior(UINT id, CComVariant cvrIndex)
{
	CMainFrame * mf = (CMainFrame *)g_App.GetMainWnd();
	std::shared_ptr<XmlInfo::CXmlBehavior> xmlBehavior = m_spXmlDevice->getBehavior(id);
	if(!xmlBehavior)	return;
	std::shared_ptr<CDeviceParam> para;
	VARIANT cvr;

	UINT count = UINT(xmlBehavior->m_ltParaID.size() * 2 + 1);
	cvr.vt = VT_ARRAY|VT_VARIANT;
	cvr.parray = ::SafeArrayCreateVector(VT_VARIANT, 0, count);
	mf->m_VarArray[0] = cvrIndex;
	UINT index = 0, i = 0;
	for (auto paraID : xmlBehavior->m_ltParaID)
	{
		para = GetParam(paraID);
		ASSERT(para);
		mf->m_VarArray[i * 2 + 1] = para->getParaID();
		mf->m_VarArray[i * 2 + 2] = para->getValue((UINT)cvrIndex.intVal);
		++i;
	}
	void* pDest;
	AfxCheckError(::SafeArrayAccessData(cvr.parray/*psa*/, &pDest));
	memcpy(pDest, mf->m_VarArray, count * sizeof(VARIANT));
	AfxCheckError(::SafeArrayUnaccessData(cvr.parray/*psa*/));
	AfxCheckError(::SafeArrayDestroy(cvr.parray/*psa*/));
}

//!< 获得根设备的接口类型，bOnLine表示是否需要第一个在线的
void MVC::Device::CDeviceOne::GetWholeAddr(std::list<UINT>& ltAddr, bool bOnLine /* = true */)
{
	if(getParentID() != UINT(-1)){	//!< 如果有父设备，就问父设备的地址吧
		ltAddr.push_front(m_vtInterface[0]->GetHandle());
		CDevMgr::GetMe().GetDevice(getParentID())->GetWholeAddr(ltAddr, bOnLine);
	}
	else	//!< 如果没有父设备，就是自己的地址，这时候bOnLine就用得上了
	{
		std::shared_ptr<CDeviceInterface> inf;
		bool bFind = false;
		if(!bOnLine && !m_vtInterface.empty())					bFind = true;
		for (auto inf : m_vtInterface)
		{
			if(bOnLine && inf->GetState() == 0)					continue;
			if(inf->GetType() == 0)								//!< 如果是以太网IP
			{
				int temp;
			    if(CGbl::DomainToIp(	inf->GetDoMain(),temp))
					ltAddr.push_front(temp);
				else 
				ltAddr.push_front(0);	
			}
			else if(inf->GetType() == 1)						//!< 如果是串口
			{
				ltAddr.push_front(inf->GetHandle());
				ltAddr.push_front(inf->GetSerialNo());
			}
			else if(inf->GetType() == 0x20)						//!< 外网IP被动接
			{
				ltAddr.push_back(inf->GetSerialNo());
			}
			else	ASSERT(FALSE);
			bFind = true;
			break;
		}
		if(bOnLine && !bFind)									ltAddr.clear();
	}
}

//!< 查找本变量，要匹配的文本，全字符匹配，大小写匹配
bool MVC::Device::CDeviceOne::DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex /* = false */)
{
	CString strInfo;
	bool bMatch = false;
	int nMatchCount = 0;
	CComVariant cvr;
	strInfo.Format("%d", m_uiID);
	strInfo = _T("Information:设备") + strInfo + _T(":设备 ") + m_strName + _T("  ——  ");

	//!< 名称是否匹配
	cvr = m_strName;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("名称"), strInfo))	++nMatchCount;

	//!< 标签是否匹配
	cvr = m_strTag;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("标签"), strInfo))	++nMatchCount;

	//!< 类型是否匹配
	cvr = m_spXmlDevice->getName();
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("类型"), strInfo))	++nMatchCount;

	//!< 接口是否匹配
	for (auto inf : m_vtInterface)
	{
		if(!inf)		continue;
		cvr = inf->GetTypeName();
		if(CGbl::SearchT(cvr, str, false, bAllCase, bRegex, _T("接口类型"), strInfo)){++nMatchCount;	continue;}

		cvr = inf->GetName();
		if(CGbl::SearchT(cvr, str, false, bAllCase, bRegex, _T("接口"), strInfo))	++nMatchCount;
	}

	if(nMatchCount > 0){
		CGbl::PrintOut(strInfo);
		return true;
	}
	return false;
}

//!< 返回设备当前状态，0离线，1在线，2在线未配置，3在线冲突
long MVC::Device::CDeviceOne::getState()
{
	for (auto inf : m_vtInterface)
	{
		if(inf->GetState() != 0)	return inf->GetState();
	}
	return 0;
}

//!< 获得父设备的ID
CString MVC::Device::CDeviceOne::getStrParentID()
{
	UINT minLevel = UINT(-1);		//!< 起始5就够了
	CString strID;
	return strID;
}

//!< 获得可容纳的接口数量
int MVC::Device::CDeviceOne::GetInterfaceSize()
{
	return 2;
}

//!< 启动对其编程
void MVC::Device::CDeviceOne::OnProgram()
{
	Servers::Config::CConfigMgr::GetMe().AddConfig(getID());
}