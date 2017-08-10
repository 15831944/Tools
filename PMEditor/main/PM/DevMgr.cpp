#include "StdAfx.h"
#include "DXPEditor.h"
#include "ProjectMgr.h"
#include "Gbl.h"
#include "Project.h"
#include "DevMgr.h"
#include "DeviceOne.h"
#include "DSerial.h"
#include "DeviceMapDoc.h"
#include "DeviceMapView.h"
#include "DeviceMapFrm.h"
#include "DeviceInterface.h"
#include "DeviceParam.h"
#include "XmlDevice.h"
#include "XmlBehavior.h"

#include "ScanSetDlg.h"
#include "ScanMgr.h"
#include "ScanSetInfo.h"

using namespace MVC;
using namespace Device;

#include "SerialSetDlg.h"
#include "EtherNetSetDlg.h"
#include "DEthernet.h"

const CString DEVMGR_EXPAND_NAME = _T("dtp");

const CString INFO_HEAD = _T("Infomation");
const CString DEVINFO = _T("DevInfo");

const CString ETHERNET_INFO = _T("EthernetInfo");
const CString SERIAL_INFO = _T("SerialInfo");

CDevMgr::CDevMgr(void)
{
	m_DevMapDoc = NULL;
	m_Ethernet = std::shared_ptr<InterfaceSet::CDEthernet>(new InterfaceSet::CDEthernet());
	m_ScanMgr = &CScanMgr::GetMe();
	m_ScanSetInfo = std::shared_ptr<CScanSetInfo>(new CScanSetInfo);
	m_ScanSetInfo->Init();
}

CDevMgr::~CDevMgr(void)
{
	m_ltBRequest.clear();
}

CDevMgr& CDevMgr::GetMe()
{
	static CDevMgr one;
	return one;
}

CString CDevMgr::GetEditTime()
{
	if(IsModify())		::GetLocalTime(&m_EditTime);
	return CGbl::GetMe().GetSystemTime2Str(m_EditTime);
}

void CDevMgr::OpenDoc()
{
	if(!m_DevMapDoc){
		m_DevMapDoc = (MVC::Device::CDeviceMapDoc*)((CDXPEditorApp *)AfxGetApp())->m_pDeviceDocMgr->OpenDocumentFile(NULL);
		if(CProjectMgr::GetMe().IsWatch())	m_DevMapDoc->SetTitle(_T("设备拓扑图（监控状态）"));
		else			m_DevMapDoc->SetTitle(_T("设备拓扑图"));
		m_DevMapDoc->OnCountChildPoint();
	}
	else{
		POSITION pos = m_DevMapDoc->GetFirstViewPosition();
		CView* view = m_DevMapDoc->GetNextView(pos);
		ASSERT(view);
		CFrameWnd* frm = view->GetParentFrame();
		ASSERT(frm);
		if(frm->IsKindOf(RUNTIME_CLASS(CDeviceMapFrm)))
			frm->ActivateFrame();
	}
}

std::shared_ptr<CDeviceOne> CDevMgr::GetDevice(UINT id)
{
	std::shared_ptr<CDeviceOne> empty;
	if(id >= m_vtDevice.size())		return empty;
	else							return m_vtDevice[id];
}

std::shared_ptr<CDeviceOne> CDevMgr::GetDevice(CString name)
{
	std::shared_ptr<CDeviceOne> empty;
	for (std::shared_ptr<CDeviceOne> device : m_vtDevice)
	{
		if(!device)					continue;
		if(device->getName() == name)
			return device;
	}
	return empty;
}

std::shared_ptr<CDeviceOne> CDevMgr::GetDevice(UINT devType, UINT infType, UINT level, long* plAddr)
{
	std::shared_ptr<CDeviceOne> empty, device;
	int count = level + 1;
	if(infType == 1)	++count;				//!< 串口的地址会多一个
	std::list<UINT> ltAddr;
	for (auto device : m_vtDevice)
	{
		if(!device)								continue;
		if(device->getDevType() != devType)		continue;	//!< 类型不同，不是一个设备
		if(device->getLevel() != level)			continue;	//!< 级别不同，不是一个设备
		ltAddr.clear();
		device->GetWholeAddr(ltAddr);
		if(ltAddr.size() != count)				continue;	//!< 接口数不同，不是一个设备
		int i = 0;
		bool bFind = true;
		for(std::list<UINT>::iterator iter = ltAddr.begin(); iter != ltAddr.end(); ++iter, ++i)
		{
			if(*iter == (UINT)plAddr[i])		continue;
			bFind = false;
			break;
		}
		//!< 这个设备就是我要的设备了
		if(bFind)								return device;
	}
	return empty;
}

//!< 获得所有工程设备的名称列表，返回defIndex号的设备
std::shared_ptr<CDeviceOne> CDevMgr::GetDeviceNameList(std::list<CString>& strList, int& defIndex)
{
	std::shared_ptr<MVC::Device::CDeviceOne> device, empty;
	int index = -1;
	for (auto device : m_vtDevice)
	{
		if(!device)					continue;
		if(!device->IsProj())		continue;
		if(device->getID() == defIndex){
			index = (int)strList.size();
			empty = device;
		}
		strList.push_back(device->getName());
	}
	defIndex = index;
	return empty;
}

//!< 这里加的设备一定要是新增的设备，因为我要在这里进行初始化
std::shared_ptr<CDeviceOne> CDevMgr::AddNewDevice(std::shared_ptr<CDeviceOne> device)
{
	if(!device)						return device;
	if(!device->GetXmlInfo())		return device;
	for(UINT i = 0; i < m_vtDevice.size(); ++i)
	{
		if(m_vtDevice[i])			continue;
		device->setID(i);									//!< 找到了一个空的位置，把device的编号附上
		m_vtDevice[i] = device;
		device->OnAddNew();			// 激活这个事件
		if(device->IsProj())		AwayFromSameInf(device);
		return device;
	}
	//!< 如果前边没有空的位置，则从最后添加
	if(m_vtDevice.size() < CGbl::GetMe().getMaxDevNum())	//!< 数量不能超过权限值
	{
		device->setID((UINT)m_vtDevice.size());				//!< 设置变量ID号
		m_vtDevice.push_back(device);
		device->OnAddNew();			// 激活这个事件
		if(device->IsProj())		AwayFromSameInf(device);
		return device;
	}
	//!< 超限了，不能添了
	MessageBox(NULL, _T("数量超过了限制，无法再添加设备！"), _T("警告"), MB_OK);
	std::shared_ptr<CDeviceOne> empty;
	return empty;
}

//!< 保证设备拥有与别人不同的接口
void CDevMgr::AwayFromSameInf(std::shared_ptr<CDeviceOne> device)
{
	std::vector<CString> vtStr;
	CString str, strID;
	strID.Format("%d", device->getID());
	int count = (int)device->m_vtInterface.size();
	vtStr.resize(count);
	for(int i = 0; i < count; ++i)		
		vtStr[i] = device->m_vtInterface[i]->GetName();
	MakeSureDfrtInf(device);
	for(int i = 0; i < count; ++i)
	{
		str = device->m_vtInterface[i]->GetName();
		if(vtStr[i] == str)				continue;

		CGbl::PrintOut(_T("information:设备") + strID + _T(":为保证通信接口不重叠，现已将设备 \"") + device->getName() + \
			_T("\" 的接口 \"") + vtStr[i] + _T("\" 自动改成 \"") + str + _T("\""));
	}
}

//!< 保证设备拥有与别人不同的接口，如果相同则重新设置
void CDevMgr::MakeSureDfrtInf(std::shared_ptr<CDeviceOne> device)
{
	for (auto other : m_vtDevice)
	{
		if(!other)											continue;
		if(!other->IsProj())								continue;		//!< 扫描上来的设备当然就不管理
		if(other->getParentID() != device->getParentID())	continue;
		for (auto otherInf : other->m_vtInterface)
		{
			for (auto inf : device->m_vtInterface)
			{
				if(otherInf == inf)							continue;
				if(otherInf->GetName() != inf->GetName())	continue;
				//if(!CGbl::RegexIP(otherInf->GetName()))						//!<域名重复判断端口是否相同
				{
					if(otherInf->GetDevPort()!=inf->GetDevPort())
						continue;
				}
				//if(inf->GetType() == 0)						inf->SetIP(CGbl::IPAddNumber(inf->GetIP(), 1));
				if(inf->GetType() == 0)			
				{
					if(CGbl::RegexIP(otherInf->GetDoMain()))	 
					{
						UINT temp=CGbl::GetNumberFromIP(otherInf->GetDoMain());
						inf->SetDoMain(CGbl::GetIPFromNumber(CGbl::IPAddNumber(temp,1)));
					}
					else if(CGbl::RegexDomain(otherInf->GetDoMain()))			
					{
						inf->SetDoMain(otherInf->GetDoMain()+"1");
					}
				}
				else										inf->SetHandle(otherInf->GetHandle() + 1);
				MakeSureDfrtInf(device);					//!< 如果改了还得再次递归检查，以免再次重复
				return;
			}
		}
	}
}

//!< 获得设备的真实数量
UINT CDevMgr::GetDeviceSize()
{
	UINT size = 0;
	for (auto device : m_vtDevice)
		if(device && device->IsProj())
			++size;
	return size;
}

//!< 删除设备，包括其子设备
void CDevMgr::DeleteDevice(UINT id)
{
	std::shared_ptr<CDeviceOne> device, child, parent, empty;
	device = GetDevice(id);
	if(!device)		return;
	if(device->m_ltChildID.size() > 0)
	{
		for (UINT childID : device->m_ltChildID)
			OnDeleteDeviceChild(childID);
	}
	parent = GetDevice(device->getParentID());
	if(parent)		parent->DeleteChild(id);
	m_vtDevice[id] = empty;

	//!< 通知所有监听者，设备拓扑改变了
	for (auto br : m_ltBRequest)	br->OnDeviceChange();
}

//!< 删除该设备的所有孩子，包括孩子的孩子，等等
void CDevMgr::OnDeleteDeviceChild(UINT id)
{
	std::shared_ptr<CDeviceOne> device, child, empty;
	device = GetDevice(id);
	if(!device)		return;
	if(device->m_ltChildID.size() > 0)
	{
		for (UINT childID : device->m_ltChildID)
			OnDeleteDeviceChild(childID);
	}
	m_vtDevice[id] = empty;
}

//!< 修改设备，但要记录撤销信息
void CDevMgr::UndoUpdDevice(UINT id)
{
	if(!m_DevMapDoc)		return;
	std::shared_ptr<CDeviceOne> device, undoDev;
	device = GetDevice(id);
	if(!device)				return;
	undoDev = std::shared_ptr<CDeviceOne>(new CDeviceOne);
	undoDev->CopyFrom(*device);
	std::shared_ptr<SDevUndo> sdu = std::shared_ptr<SDevUndo>(new SDevUndo(CGbl::UNDO_TYPE_UPD, undoDev));
	sdu->SetEnd();
	m_DevMapDoc->AddUndoMember(sdu);
}

//!< 删除设备，但要记录撤销信息
void CDevMgr::UndoDelDevice(UINT id)
{
	if(!m_DevMapDoc)		return;
	std::shared_ptr<CDeviceOne> device, child, parent,empty;
	device = GetDevice(id);
	if(!device)				return;
	std::shared_ptr<SDevUndo> sdu = std::shared_ptr<SDevUndo>(new SDevUndo(CGbl::UNDO_TYPE_DEL, device));
	m_DevMapDoc->AddUndoMember(sdu);
	parent = GetDevice(device->getParentID());
	if(device->m_ltChildID.size() > 0)
	{
		for (UINT childID : device->m_ltChildID)
			UndoDelDevice(childID);
	}
	m_vtDevice[id] = empty;
}

std::shared_ptr<InterfaceSet::CDSerial> CDevMgr::GetSerial(CString name)
{
	std::shared_ptr<InterfaceSet::CDSerial> empty;
	for (auto serial : m_ltSerial)
		if(name == serial->getName())
			return serial;
	return empty;
}

//!< 打开拓扑文件，参数 ： 名称 + 路径 + 版本号，最近修改时间
bool CDevMgr::OpenDevMgrFile(CString name, CString pathall, CString ver, CString stime)
{
	m_ScanSetInfo->LoadFile();
	if(!CGbl::SetSystemTimeFromStr(m_EditTime, stime))		::GetLocalTime(&m_EditTime);
	m_strName = name;
	m_strFileName = m_strName + _T(".") + DEVMGR_EXPAND_NAME;
	m_strVersion = ver;
	TiXmlDocument pTiXml(pathall);
	if(!pTiXml.LoadFile()){
		MessageBox(NULL, _T("设备拓扑文件格式错误！"), _T("错误"), MB_ICONEXCLAMATION);
		return false;
	}
	//!< 开始解析
	if(!SerializeXml(pTiXml.RootElement(), true))
		return false;
	//!< 设备解析自己的描述去
	for (auto device : m_vtDevice){
		if(!device)		continue;
	}
//	m_ScanSetInfo->LoadFile();
	return true;
}

void CDevMgr::OnCreate()
{
	m_vtDevice.clear();
	m_ltSerial.clear();
	if(m_DevMapDoc)		m_DevMapDoc->OnCloseDocument();
	SetModify();		//!< 新建的工程默认为被修改状态

	//!< 初始化路径信息
	m_strName = _T("设备拓扑");
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();	//!< 名字叫"设备拓扑"，工程名叫"p"
	m_strFileName = m_strName + _T(".") + DEVMGR_EXPAND_NAME;				//!< 路径时"p_设备拓扑.dtp"
	m_strVersion = _T("1.0");

	//!< 初始化串口配置
	std::shared_ptr<InterfaceSet::CDSerial> serial;
	std::list<UINT> ltComID;
	CGbl::GetSerialCount(ltComID);
	for (UINT id : ltComID)
	{
		serial = std::shared_ptr<InterfaceSet::CDSerial>(new InterfaceSet::CDSerial());
		serial->setNumber(id - 1);
		m_ltSerial.push_back(serial);
	}
}

void CDevMgr::OnClose()
{
	m_vtDevice.clear();
	m_ltSerial.clear();
	m_DevMapDoc = NULL;
	m_ScanSetInfo->Init();
}

void CDevMgr::SaveFile()
{
	m_ScanSetInfo->SaveFile();
	if(IsModify())		//!< 没被修改就不用保存了
	{
		std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
		CString pathAll = proj->GetPath() + m_strName + _T(".") + DEVMGR_EXPAND_NAME;
		TiXmlDocument pTiXml(pathAll);
		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< 起始声明
		pTiXml.LinkEndChild(decl);
		pTiXml.AddComment(_T("请勿手工编辑此文档"));							//!< 注释信息
		TiXmlElement *pRoot = new TiXmlElement("DocumentRoot");					//!< 写入根节点
		SerializeXml(pRoot, false);												//!< 添加工程节点,写操作
		pTiXml.LinkEndChild(pRoot);
		bool result = pTiXml.SaveFile();
	}
	CGbl::SetProgressStep(1);
	for (auto device : m_vtDevice)
	{
		if(device && device->IsProj())
			device->SaveXmlFile();
		CGbl::SetProgressStep(1);
	}
	m_bModify = false;
}

bool CDevMgr::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	if(bRead){
		//!< 软件版本，先不考虑
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		CString name, strValue;
		CComVariant cvr;
		//!< 子节点
		std::list<std::shared_ptr<CDeviceOne> > ltDevice;
		TiXmlElement* pElement = pNode->FirstChildElement();
		std::shared_ptr<CDeviceOne> device;
		std::shared_ptr<InterfaceSet::CDSerial> serial;
		m_ltSerial.clear();
		int index = 0;
		while(pElement)
		{
			name = pElement->Value();
			if(DEVINFO == name)
			{
				device = std::shared_ptr<CDeviceOne>(new CDeviceOne());
				if(device->SerializeXmlGeneral(pElement, bRead))
				{
					ltDevice.push_back(device);
				}
				else
				{
					OnClose();
					return false;
				}
			}
			else if(ETHERNET_INFO == name)
			{
				m_Ethernet->SerializeXml(pElement, bRead);
			}
			else if(SERIAL_INFO == name)
			{
				serial = std::shared_ptr<InterfaceSet::CDSerial>(new InterfaceSet::CDSerial());
				if(serial->SerializeXml(pElement, bRead))				m_ltSerial.push_back(serial);
			}
			pElement = pElement->NextSiblingElement();
		}
		UINT num = 0;
		m_vtDevice.clear();
		for (auto device : ltDevice)			num = max(num, device->getID());
		m_vtDevice.resize(num + 1);
		for (auto device : ltDevice)			m_vtDevice[device->getID()] = device;

		//!< 初始化所有计算机的接口
		InitComputerSerial();

		//!< 加载所有设备元素
		if(!CheckAndConnect())		return false;
	}
	else
	{
		pNode->SetAttribute(_T("FileType"), _T("EditDevMgrFile"));
		pNode->SetAttribute(_T("Version"), _T("1.0"));
		TiXmlElement* pElement;
		pElement = pNode->AddTiXmlChild((LPCTSTR)INFO_HEAD);

		//!< 保存设备信息
		for (auto device : m_vtDevice){
			if(!device)				continue;
			if(!device->IsProj())	continue;
			pElement = pNode->AddTiXmlChild((LPCTSTR)DEVINFO);
			device->SerializeXmlGeneral(pElement, bRead);
		}

		//!< 保存以太网配置信息
		pElement = pNode->AddTiXmlChild((LPCTSTR)ETHERNET_INFO);
		m_Ethernet->SerializeXml(pElement, bRead);

		//!< 保存串口配置信息
		for (auto serial : m_ltSerial){
			if(!serial)				continue;
			pElement = pNode->AddTiXmlChild((LPCTSTR)SERIAL_INFO);
			serial->SerializeXml(pElement, bRead);
		}
	}
	return true;
}

//!< 初始化串口信息，把计算机扫描出来的串口强行加载进链表中
void CDevMgr::InitComputerSerial()
{
	std::list<UINT> ltComID;
	CGbl::GetMe().GetSerialCount(ltComID);				//!< 得到当前计算机的所有串口号
	if(ltComID.empty())			return;
	std::shared_ptr<InterfaceSet::CDSerial> serial;
	std::map<UINT, std::shared_ptr<InterfaceSet::CDSerial> > mpSerial;		//!< 串口号与串口对象的键值表
	UINT maxID = 0, comID = 0;							//!< 获得最大的串口号
	ltComID.sort();
	maxID = max(maxID, ltComID.back());
	//!< 先将所有已经初始化的串口配置放进键值表中
	for (auto serial : m_ltSerial)
	{
		comID = serial->getNumber() + 1;
		if(maxID < comID)		maxID = comID;
		mpSerial[comID] = serial;
	}
	//!< 再来搞当前计算机的所有串口
	bool bChange = false;
	for (UINT id : ltComID)
	{
		if(mpSerial[id])		continue;
		bChange = true;
		serial = std::shared_ptr<InterfaceSet::CDSerial>(new InterfaceSet::CDSerial);
		serial->setNumber(id - 1);
		mpSerial[id] = serial;
	}
	//!< 最后将所有串口配置再导回去就OK了
	m_ltSerial.clear();
	for(int i = 0; i <= maxID; ++i)
		if(mpSerial[i + 1])
			m_ltSerial.push_back(mpSerial[i + 1]);
}

bool CDevMgr::CheckAndConnect()
{
	std::shared_ptr<CDeviceOne> parent;
	//!< 先清空所有设备的子设备列表
	for (auto child : m_vtDevice){
		if(!child)		continue;
		child->m_ltChildID.clear();
	}
	//!< 填充设备列表
	for (auto child : m_vtDevice){
		if(!child)		continue;
		if(child->getParentID() == UINT(-1))	continue;
		parent = GetDevice(child->getParentID());
		if(!parent)								return false;
		parent->AddChild(child->getID());
	}
	return true;
}

//!< 在拓扑中选中这个设备，并显示
void CDevMgr::ShowDevice(UINT id)
{
	OpenDoc();
	if(!m_DevMapDoc)	return;
	m_DevMapDoc->GetView()->ShowDeviceAt(id);
}

void CDevMgr::OnSetInf()
{
	CXTResizePropertySheet dlg(AFX_IDS_APP_TITLE, 0, 0, xtResizeNoVertical);
	dlg.m_psh.dwFlags |= PSH_NOAPPLYNOW;

	Dialog::CEtherNetSetDlg pageEtherNet;
	dlg.AddPage(&pageEtherNet);

	Dialog::CSerialSetDlg pageSerial;
	dlg.AddPage(&pageSerial);

	if (IDOK == dlg.DoModal()){
		pageEtherNet.m_Object.OnSaveModify(pageEtherNet.m_PropertyGrid);
		pageSerial.SaveModify();
		SetModify();
	}
}

void CDevMgr::SetDevWatch(const bool bWatch)
{
	FreshView();
}

//!< 设置扫描状态
void CDevMgr::SetDevScan(bool bScan)
{
	if(CProjectMgr::GetMe().IsScan() && bScan)		//!< 如果已经处于扫描了，系统还要扫描，那么就是刷新
	{
		CScanMgr::GetMe().ClearData();
		OpenDoc();									//!< 开始扫描了,打开拓扑图
		return;
	}
	if(!bScan){
		m_ScanMgr->DelAllScanDev();
		CheckAndConnect();
		if(m_DevMapDoc)	m_DevMapDoc->FreshMap();
		FreshView();
	}
}

//!< 查找符合该字符串的变量，返回匹配的数量
int CDevMgr::SearchDev(CString str, bool bMatchCase, bool bMatchWhole, bool bRegex /* = false */)
{
	int nMatchCount = 0;
	if(!bMatchCase && !bRegex)		str = str.MakeUpper();

	for (auto device : m_vtDevice)
	{
		if(device)
			if(device->DoSearch(str, bMatchWhole, bMatchCase, bRegex))
				++nMatchCount;
//		CGbl::SetProgressStep(1);
	}
	return nMatchCount;
}

//!< 如果存在拓扑也，刷一下页面
void CDevMgr::FreshView()
{
	POSITION pos = g_App.m_pDeviceDocMgr->GetFirstDocPosition();
	CDocument* pDoc = NULL;
	while(pos)
	{
		pDoc = g_App.m_pDeviceDocMgr->GetNextDoc(pos);
		if(pDoc)	pDoc->UpdateAllViews(NULL);
	}
}

//!< 获得一个和它相近的名字
CString CDevMgr::GetSimilarName(CString name)
{
	int number = 0;
	CString noNumberName = name;
	bool haveNumber = CGbl::GetNumberFromString(noNumberName, number, 1);
	//!< 如果这个字符串中不包含数字
	if(!haveNumber)
	{
		if(!GetDevice(name))	return name;
		name = name + _T("0");
		return GetSimilarName(name);
	}
	//!< 如果包含了数字
	if(!GetDevice(name))	return name;
	CString newName;
	newName.Format("%d", number + 1);
	//!< 如果都是9，表示+1时会进位，呵呵，这时如果前边的字符串最后还跟个零，为了工整起见，需要将最后的零去掉
	CString strNumber;
	strNumber.Format("%d",number);
	bool bAll9 = true;
	for(int i = 0; i < strNumber.GetLength(); ++i)
	{
		char c = strNumber.GetAt(i);
		if(c != '9')
		{
			bAll9 = false;
			break;
		}
	}
	if(bAll9 && noNumberName.GetLength() > 0 && noNumberName.Right(1) == _T("0"))
	{
		newName = noNumberName.Left(noNumberName.GetLength() - 1) + newName;
	}
	else
	{
		newName = noNumberName + newName;
	}
	return GetSimilarName(newName);
}

//!< 复制这个编号的设备到剪贴板
void CDevMgr::OnDevCopy(UINT id)
{
	m_ltDevClipBoard.clear();
	std::shared_ptr<CDeviceOne> device = GetDevice(id);
	ASSERT(device);
	device->OnCopy();
}

//!< 复制这个编号的设备以及他的所有从设备到剪贴板
void CDevMgr::OnDevCopyWithChild(UINT id)
{
	m_ltDevClipBoard.clear();
	std::shared_ptr<CDeviceOne> device = GetDevice(id);
	ASSERT(device);
	device->OnCopyWithChild();
}

//!< 剪切这个编号的设备以及他的所有从设备到剪贴板
void CDevMgr::OnDevCut(CDeviceMapDoc* pDoc, UINT id)
{
	m_ltDevClipBoard.clear();
	std::shared_ptr<CDeviceOne> device = GetDevice(id);
	ASSERT(device);
	device->OnCut(pDoc);
	pDoc->SetUndoEnd();
	DeleteDevice(id);
	CheckAndConnect();
}

void CDevMgr::SetScanInfo()
{
	Dialog::CScanSetDlg dlg;
	dlg.DoModal();
}

//!< 获得扫描回来的数据
void CDevMgr::OnScanRev(char* data, UINT num)
{
	if(!CProjectMgr::GetMe().GetProj())		return;
	m_ScanMgr->OnScanRev(data, num);
}

//!< 设备状态改变事件。nDeviceInterface
void CDevMgr::OnDeviceStatus(long nDeviceID,long nDeviceInterface,long nDeviceStatus)
{
	std::shared_ptr<CDeviceOne> device = GetDevice(nDeviceID);
	if(!device)		return;
	SYSTEMTIME t;
	GetLocalTime(&t);
	CString strTime;
	strTime.Format("%.4d/%.2d/%.2d %.2d:%.2d:%.2d(%.3d) —— ",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);

	if(nDeviceStatus == -1)			//!< 有设备要下线
	{
		device->SetOffLine();
		CString text = _T("系统提示-设备状态：设备 [") + device->getName() + _T("] 下线...");
		CGbl::PrintOut(strTime + text);
	}
	else							//!< 有设备上线
	{
		if(device->getParentID() == UINT(-1))		//!< 主设备
		{
			std::shared_ptr<CDeviceInterface> inf = device->GetInterfaceFromType(nDeviceInterface);
			if(!inf)		return;
			device->SetOffLine();
			inf->SetState(1);
			CString type = _T("以太网");
			if(nDeviceInterface == 1)	type = _T("串口");
			CString text = _T("系统提示-设备状态：设备 [") + device->getName() + _T("] 上线， 正在使用 [") + type + _T("] 通信...");
			CGbl::PrintOut(strTime + text);
		}
		else										//!< 从设备
		{
			device->SetOnLineInf(0, true);
			CString text = _T("系统提示-设备状态：设备 [") + device->getName() + _T("] 上线...");
			CGbl::PrintOut(strTime + text);
		}
	}
	FreshView();
}

void CDevMgr::OnBehavior(long lBehaviorID, long lDeviceID, VARIANT& varValue, long lResult)
{
	//!< 判断收到的是什么东西
	std::shared_ptr<CDeviceOne> device = GetDevice(lDeviceID);
	if(!device)			return;
	std::shared_ptr<XmlInfo::CXmlBehavior> xmlBehavior = device->GetXmlInfo()->getBehavior(lBehaviorID);
	if(!xmlBehavior)	return;

	//!< 为数据赋值
	VARIANT *var = (VARIANT *)varValue.byref;
	VARIANT *pValue=NULL;
	HRESULT hr = SafeArrayAccessData(var->parray, (void**)&pValue);
	long Low(0), High(0);								//!< 获取数组长度
	if(SUCCEEDED(hr)){
		hr = SafeArrayGetLBound(var->parray, 1, &Low);	//!< 维数索引从1开始
		hr = SafeArrayGetUBound(var->parray, 1, &High);	//!< 维数索引从1开始
	}
	else{
		SafeArrayUnaccessData(var->parray);
		return;
	}
	int varCount = High - Low;
	try
	{
	if(varCount > 0)
	{
		std::shared_ptr<MVC::Device::CDeviceParam> para;
		CComVariant cvrID, cvrValue, cvrIndex;
		cvrIndex = pValue[0];
		UINT uiID, uiIndex;
		if(cvrIndex.vt == VT_I4)		uiIndex = (UINT)cvrIndex.intVal;
		else if(cvrIndex.vt == VT_UI4)	uiIndex = cvrIndex.uintVal;
		else							return;
		for(int i = 0; i < varCount / 2; ++i)
		{
			cvrID = pValue[i * 2 + 1];
			if(cvrID.vt == VT_I4)		uiID = (UINT)cvrID.intVal;
			else if(cvrID.vt == VT_UI4)	uiID = cvrID.uintVal;
			else						continue;
			cvrValue = pValue[i * 2 + 2];

			para = device->GetParam(uiID);
			if(!para)					continue;
			para->setValue(cvrValue, uiIndex);
		}
	}
	}
	catch (...)
	{
		ASSERT(FALSE);
	}
	SafeArrayUnaccessData(var->parray);
	OnBehaviorOver();
}

//!< 将剪贴板里的设备粘贴到这个设备下面，如果不存在就直连工程师站
void CDevMgr::OnDevPaste(CDeviceMapDoc* pDoc, UINT id /* = UINT(-1) */)
{
	if(m_ltDevClipBoard.empty())			return;
	if(!pDoc)								return;
	//!< 父设备不能连了，退出
	std::shared_ptr<CDeviceOne> device, pastDev;
	device = GetDevice(id);
	if(device && !device->CanAddSlave())	return;

	//!< 从剪贴板中拷贝出原始数据，不能直接拿，而是要拷贝出来一套
	std::list<std::shared_ptr<CDeviceOne> > ltDevClipBoard;
	for (auto device : m_ltDevClipBoard)
	{
		pastDev = std::shared_ptr<CDeviceOne>(new CDeviceOne);
		pastDev->CopyFrom(*device);
		pastDev->setID(device->getID());
		pastDev->setParentID(device->getParentID());
		ltDevClipBoard.push_back(pastDev);
	}

	//!< 记录设备之间的连接关系
	std::map<std::shared_ptr<CDeviceOne>, UINT> m_mpDev_ParentID;		//!< 设备对应父设备编号
	std::map<UINT, std::shared_ptr<CDeviceOne> > m_mpID_Dev;			//!< 编号对应设备
	ltDevClipBoard.front()->setParentID(UINT(-1));						//!< 第一个设备没有父设备
	for (auto device : ltDevClipBoard)
	{
		m_mpDev_ParentID[device] = device->getParentID();
		m_mpID_Dev[device->getID()] = device;
	}

	//!< 如果是子设备，那么在加到工程中前，先处理成串口从设备，否则会影响接口不重叠的规则
	device = ltDevClipBoard.front();
	if(id != UINT(-1)){
		device->ResizeInterface(1);
		device->m_vtInterface[0]->SetType(16);
		device->m_vtInterface[0]->SetHandle(0);
	}

	//!< 将所有的孩子都粘贴到拓扑图中
	CString name;
	std::shared_ptr<SDevUndo> sdu;
	for (auto device : ltDevClipBoard)
	{
		name = GetSimilarName(device->getName());
		device->setName(name);
		//!< 重新建立父设备关系，因为剪贴板内的设备是树的前序遍历的循序存放的，所以保证了粘贴子设备时父设备已经粘贴完了，否则下面的算法就是错误的
		if(m_mpDev_ParentID[device] == UINT(-1))	device->setParentID(id);
		else										device->setParentID(m_mpID_Dev[m_mpDev_ParentID[device]]->getID());
		if(!AddNewDevice(device))		break;
		if(id == (UINT)-1)				device->SetSelect(true);		// 如果之前没有选中的设备,那么新添的设备将被设置为选中
		sdu = std::shared_ptr<SDevUndo>(new SDevUndo(CGbl::UNDO_TYPE_ADD, device));
		pDoc->AddUndoMember(sdu);
	}
	pDoc->SetUndoEnd();

	//!< 重建连接关系
	CheckAndConnect();
}

//!< 当行为结束后要通知所有监听者
void CDevMgr::OnBehaviorOver()
{
	for (auto br : m_ltBRequest)	br->OnDataReady();
}

//!< 注册监听者
void CDevMgr::RegisteBRequest(SBehaviorRequest* one)
{
	for (auto br : m_ltBRequest){
		if(br == one)	return;
	}
	m_ltBRequest.push_back(one);
}

//!< 注销监听者
void CDevMgr::UnRegisteBRequest(SBehaviorRequest* one)
{
	m_ltBRequest.remove(one);
}

//!< 获得该设备所使用的最大的超时时间
int MVC::Device::CDevMgr::GetMaxOverTime(std::shared_ptr<CDeviceOne> device)
{
	int over_time = 1000;
	for (auto inf : device->m_vtInterface)
	{
		if(!inf)						continue;
		if(inf->GetType() == 0)			over_time = max(over_time, m_Ethernet->getOverTime());
		else if(inf->GetType() == 1)
		{
			for (auto serial : m_ltSerial)
			{
				if(!serial)				continue;
				if(serial->getNumber() != 
					inf->GetSerialNo())	continue;
				over_time = max(over_time, serial->getOverTime());
			}
		}
		else
		{
			for (auto serial : m_ltSerial)
			{
				if(!serial)				continue;
				over_time = max(over_time, serial->getOverTime());
			}
		}
	}
	return over_time;
}
