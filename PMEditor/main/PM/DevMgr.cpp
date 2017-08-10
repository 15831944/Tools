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
		if(CProjectMgr::GetMe().IsWatch())	m_DevMapDoc->SetTitle(_T("�豸����ͼ�����״̬��"));
		else			m_DevMapDoc->SetTitle(_T("�豸����ͼ"));
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
	if(infType == 1)	++count;				//!< ���ڵĵ�ַ���һ��
	std::list<UINT> ltAddr;
	for (auto device : m_vtDevice)
	{
		if(!device)								continue;
		if(device->getDevType() != devType)		continue;	//!< ���Ͳ�ͬ������һ���豸
		if(device->getLevel() != level)			continue;	//!< ����ͬ������һ���豸
		ltAddr.clear();
		device->GetWholeAddr(ltAddr);
		if(ltAddr.size() != count)				continue;	//!< �ӿ�����ͬ������һ���豸
		int i = 0;
		bool bFind = true;
		for(std::list<UINT>::iterator iter = ltAddr.begin(); iter != ltAddr.end(); ++iter, ++i)
		{
			if(*iter == (UINT)plAddr[i])		continue;
			bFind = false;
			break;
		}
		//!< ����豸������Ҫ���豸��
		if(bFind)								return device;
	}
	return empty;
}

//!< ������й����豸�������б�����defIndex�ŵ��豸
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

//!< ����ӵ��豸һ��Ҫ���������豸����Ϊ��Ҫ��������г�ʼ��
std::shared_ptr<CDeviceOne> CDevMgr::AddNewDevice(std::shared_ptr<CDeviceOne> device)
{
	if(!device)						return device;
	if(!device->GetXmlInfo())		return device;
	for(UINT i = 0; i < m_vtDevice.size(); ++i)
	{
		if(m_vtDevice[i])			continue;
		device->setID(i);									//!< �ҵ���һ���յ�λ�ã���device�ı�Ÿ���
		m_vtDevice[i] = device;
		device->OnAddNew();			// ��������¼�
		if(device->IsProj())		AwayFromSameInf(device);
		return device;
	}
	//!< ���ǰ��û�пյ�λ�ã����������
	if(m_vtDevice.size() < CGbl::GetMe().getMaxDevNum())	//!< �������ܳ���Ȩ��ֵ
	{
		device->setID((UINT)m_vtDevice.size());				//!< ���ñ���ID��
		m_vtDevice.push_back(device);
		device->OnAddNew();			// ��������¼�
		if(device->IsProj())		AwayFromSameInf(device);
		return device;
	}
	//!< �����ˣ���������
	MessageBox(NULL, _T("�������������ƣ��޷�������豸��"), _T("����"), MB_OK);
	std::shared_ptr<CDeviceOne> empty;
	return empty;
}

//!< ��֤�豸ӵ������˲�ͬ�Ľӿ�
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

		CGbl::PrintOut(_T("information:�豸") + strID + _T(":Ϊ��֤ͨ�Žӿڲ��ص������ѽ��豸 \"") + device->getName() + \
			_T("\" �Ľӿ� \"") + vtStr[i] + _T("\" �Զ��ĳ� \"") + str + _T("\""));
	}
}

//!< ��֤�豸ӵ������˲�ͬ�Ľӿڣ������ͬ����������
void CDevMgr::MakeSureDfrtInf(std::shared_ptr<CDeviceOne> device)
{
	for (auto other : m_vtDevice)
	{
		if(!other)											continue;
		if(!other->IsProj())								continue;		//!< ɨ���������豸��Ȼ�Ͳ�����
		if(other->getParentID() != device->getParentID())	continue;
		for (auto otherInf : other->m_vtInterface)
		{
			for (auto inf : device->m_vtInterface)
			{
				if(otherInf == inf)							continue;
				if(otherInf->GetName() != inf->GetName())	continue;
				//if(!CGbl::RegexIP(otherInf->GetName()))						//!<�����ظ��ж϶˿��Ƿ���ͬ
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
				MakeSureDfrtInf(device);					//!< ������˻����ٴεݹ��飬�����ٴ��ظ�
				return;
			}
		}
	}
}

//!< ����豸����ʵ����
UINT CDevMgr::GetDeviceSize()
{
	UINT size = 0;
	for (auto device : m_vtDevice)
		if(device && device->IsProj())
			++size;
	return size;
}

//!< ɾ���豸�����������豸
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

	//!< ֪ͨ���м����ߣ��豸���˸ı���
	for (auto br : m_ltBRequest)	br->OnDeviceChange();
}

//!< ɾ�����豸�����к��ӣ��������ӵĺ��ӣ��ȵ�
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

//!< �޸��豸����Ҫ��¼������Ϣ
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

//!< ɾ���豸����Ҫ��¼������Ϣ
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

//!< �������ļ������� �� ���� + ·�� + �汾�ţ�����޸�ʱ��
bool CDevMgr::OpenDevMgrFile(CString name, CString pathall, CString ver, CString stime)
{
	m_ScanSetInfo->LoadFile();
	if(!CGbl::SetSystemTimeFromStr(m_EditTime, stime))		::GetLocalTime(&m_EditTime);
	m_strName = name;
	m_strFileName = m_strName + _T(".") + DEVMGR_EXPAND_NAME;
	m_strVersion = ver;
	TiXmlDocument pTiXml(pathall);
	if(!pTiXml.LoadFile()){
		MessageBox(NULL, _T("�豸�����ļ���ʽ����"), _T("����"), MB_ICONEXCLAMATION);
		return false;
	}
	//!< ��ʼ����
	if(!SerializeXml(pTiXml.RootElement(), true))
		return false;
	//!< �豸�����Լ�������ȥ
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
	SetModify();		//!< �½��Ĺ���Ĭ��Ϊ���޸�״̬

	//!< ��ʼ��·����Ϣ
	m_strName = _T("�豸����");
	std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();	//!< ���ֽ�"�豸����"����������"p"
	m_strFileName = m_strName + _T(".") + DEVMGR_EXPAND_NAME;				//!< ·��ʱ"p_�豸����.dtp"
	m_strVersion = _T("1.0");

	//!< ��ʼ����������
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
	if(IsModify())		//!< û���޸ľͲ��ñ�����
	{
		std::shared_ptr<CProject> proj = CProjectMgr::GetMe().GetProj();
		CString pathAll = proj->GetPath() + m_strName + _T(".") + DEVMGR_EXPAND_NAME;
		TiXmlDocument pTiXml(pathAll);
		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "GB2312", "" );	//!< ��ʼ����
		pTiXml.LinkEndChild(decl);
		pTiXml.AddComment(_T("�����ֹ��༭���ĵ�"));							//!< ע����Ϣ
		TiXmlElement *pRoot = new TiXmlElement("DocumentRoot");					//!< д����ڵ�
		SerializeXml(pRoot, false);												//!< ��ӹ��̽ڵ�,д����
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
		//!< ����汾���Ȳ�����
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		CString name, strValue;
		CComVariant cvr;
		//!< �ӽڵ�
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

		//!< ��ʼ�����м�����Ľӿ�
		InitComputerSerial();

		//!< ���������豸Ԫ��
		if(!CheckAndConnect())		return false;
	}
	else
	{
		pNode->SetAttribute(_T("FileType"), _T("EditDevMgrFile"));
		pNode->SetAttribute(_T("Version"), _T("1.0"));
		TiXmlElement* pElement;
		pElement = pNode->AddTiXmlChild((LPCTSTR)INFO_HEAD);

		//!< �����豸��Ϣ
		for (auto device : m_vtDevice){
			if(!device)				continue;
			if(!device->IsProj())	continue;
			pElement = pNode->AddTiXmlChild((LPCTSTR)DEVINFO);
			device->SerializeXmlGeneral(pElement, bRead);
		}

		//!< ������̫��������Ϣ
		pElement = pNode->AddTiXmlChild((LPCTSTR)ETHERNET_INFO);
		m_Ethernet->SerializeXml(pElement, bRead);

		//!< ���洮��������Ϣ
		for (auto serial : m_ltSerial){
			if(!serial)				continue;
			pElement = pNode->AddTiXmlChild((LPCTSTR)SERIAL_INFO);
			serial->SerializeXml(pElement, bRead);
		}
	}
	return true;
}

//!< ��ʼ��������Ϣ���Ѽ����ɨ������Ĵ���ǿ�м��ؽ�������
void CDevMgr::InitComputerSerial()
{
	std::list<UINT> ltComID;
	CGbl::GetMe().GetSerialCount(ltComID);				//!< �õ���ǰ����������д��ں�
	if(ltComID.empty())			return;
	std::shared_ptr<InterfaceSet::CDSerial> serial;
	std::map<UINT, std::shared_ptr<InterfaceSet::CDSerial> > mpSerial;		//!< ���ں��봮�ڶ���ļ�ֵ��
	UINT maxID = 0, comID = 0;							//!< ������Ĵ��ں�
	ltComID.sort();
	maxID = max(maxID, ltComID.back());
	//!< �Ƚ������Ѿ���ʼ���Ĵ������÷Ž���ֵ����
	for (auto serial : m_ltSerial)
	{
		comID = serial->getNumber() + 1;
		if(maxID < comID)		maxID = comID;
		mpSerial[comID] = serial;
	}
	//!< �����㵱ǰ����������д���
	bool bChange = false;
	for (UINT id : ltComID)
	{
		if(mpSerial[id])		continue;
		bChange = true;
		serial = std::shared_ptr<InterfaceSet::CDSerial>(new InterfaceSet::CDSerial);
		serial->setNumber(id - 1);
		mpSerial[id] = serial;
	}
	//!< ������д��������ٵ���ȥ��OK��
	m_ltSerial.clear();
	for(int i = 0; i <= maxID; ++i)
		if(mpSerial[i + 1])
			m_ltSerial.push_back(mpSerial[i + 1]);
}

bool CDevMgr::CheckAndConnect()
{
	std::shared_ptr<CDeviceOne> parent;
	//!< ����������豸�����豸�б�
	for (auto child : m_vtDevice){
		if(!child)		continue;
		child->m_ltChildID.clear();
	}
	//!< ����豸�б�
	for (auto child : m_vtDevice){
		if(!child)		continue;
		if(child->getParentID() == UINT(-1))	continue;
		parent = GetDevice(child->getParentID());
		if(!parent)								return false;
		parent->AddChild(child->getID());
	}
	return true;
}

//!< ��������ѡ������豸������ʾ
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

//!< ����ɨ��״̬
void CDevMgr::SetDevScan(bool bScan)
{
	if(CProjectMgr::GetMe().IsScan() && bScan)		//!< ����Ѿ�����ɨ���ˣ�ϵͳ��Ҫɨ�裬��ô����ˢ��
	{
		CScanMgr::GetMe().ClearData();
		OpenDoc();									//!< ��ʼɨ����,������ͼ
		return;
	}
	if(!bScan){
		m_ScanMgr->DelAllScanDev();
		CheckAndConnect();
		if(m_DevMapDoc)	m_DevMapDoc->FreshMap();
		FreshView();
	}
}

//!< ���ҷ��ϸ��ַ����ı���������ƥ�������
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

//!< �����������Ҳ��ˢһ��ҳ��
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

//!< ���һ���������������
CString CDevMgr::GetSimilarName(CString name)
{
	int number = 0;
	CString noNumberName = name;
	bool haveNumber = CGbl::GetNumberFromString(noNumberName, number, 1);
	//!< �������ַ����в���������
	if(!haveNumber)
	{
		if(!GetDevice(name))	return name;
		name = name + _T("0");
		return GetSimilarName(name);
	}
	//!< �������������
	if(!GetDevice(name))	return name;
	CString newName;
	newName.Format("%d", number + 1);
	//!< �������9����ʾ+1ʱ���λ���Ǻǣ���ʱ���ǰ�ߵ��ַ�����󻹸����㣬Ϊ�˹����������Ҫ��������ȥ��
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

//!< ���������ŵ��豸��������
void CDevMgr::OnDevCopy(UINT id)
{
	m_ltDevClipBoard.clear();
	std::shared_ptr<CDeviceOne> device = GetDevice(id);
	ASSERT(device);
	device->OnCopy();
}

//!< ���������ŵ��豸�Լ��������д��豸��������
void CDevMgr::OnDevCopyWithChild(UINT id)
{
	m_ltDevClipBoard.clear();
	std::shared_ptr<CDeviceOne> device = GetDevice(id);
	ASSERT(device);
	device->OnCopyWithChild();
}

//!< ���������ŵ��豸�Լ��������д��豸��������
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

//!< ���ɨ�����������
void CDevMgr::OnScanRev(char* data, UINT num)
{
	if(!CProjectMgr::GetMe().GetProj())		return;
	m_ScanMgr->OnScanRev(data, num);
}

//!< �豸״̬�ı��¼���nDeviceInterface
void CDevMgr::OnDeviceStatus(long nDeviceID,long nDeviceInterface,long nDeviceStatus)
{
	std::shared_ptr<CDeviceOne> device = GetDevice(nDeviceID);
	if(!device)		return;
	SYSTEMTIME t;
	GetLocalTime(&t);
	CString strTime;
	strTime.Format("%.4d/%.2d/%.2d %.2d:%.2d:%.2d(%.3d) ���� ",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);

	if(nDeviceStatus == -1)			//!< ���豸Ҫ����
	{
		device->SetOffLine();
		CString text = _T("ϵͳ��ʾ-�豸״̬���豸 [") + device->getName() + _T("] ����...");
		CGbl::PrintOut(strTime + text);
	}
	else							//!< ���豸����
	{
		if(device->getParentID() == UINT(-1))		//!< ���豸
		{
			std::shared_ptr<CDeviceInterface> inf = device->GetInterfaceFromType(nDeviceInterface);
			if(!inf)		return;
			device->SetOffLine();
			inf->SetState(1);
			CString type = _T("��̫��");
			if(nDeviceInterface == 1)	type = _T("����");
			CString text = _T("ϵͳ��ʾ-�豸״̬���豸 [") + device->getName() + _T("] ���ߣ� ����ʹ�� [") + type + _T("] ͨ��...");
			CGbl::PrintOut(strTime + text);
		}
		else										//!< ���豸
		{
			device->SetOnLineInf(0, true);
			CString text = _T("ϵͳ��ʾ-�豸״̬���豸 [") + device->getName() + _T("] ����...");
			CGbl::PrintOut(strTime + text);
		}
	}
	FreshView();
}

void CDevMgr::OnBehavior(long lBehaviorID, long lDeviceID, VARIANT& varValue, long lResult)
{
	//!< �ж��յ�����ʲô����
	std::shared_ptr<CDeviceOne> device = GetDevice(lDeviceID);
	if(!device)			return;
	std::shared_ptr<XmlInfo::CXmlBehavior> xmlBehavior = device->GetXmlInfo()->getBehavior(lBehaviorID);
	if(!xmlBehavior)	return;

	//!< Ϊ���ݸ�ֵ
	VARIANT *var = (VARIANT *)varValue.byref;
	VARIANT *pValue=NULL;
	HRESULT hr = SafeArrayAccessData(var->parray, (void**)&pValue);
	long Low(0), High(0);								//!< ��ȡ���鳤��
	if(SUCCEEDED(hr)){
		hr = SafeArrayGetLBound(var->parray, 1, &Low);	//!< ά��������1��ʼ
		hr = SafeArrayGetUBound(var->parray, 1, &High);	//!< ά��������1��ʼ
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

//!< ������������豸ճ��������豸���棬��������ھ�ֱ������ʦվ
void CDevMgr::OnDevPaste(CDeviceMapDoc* pDoc, UINT id /* = UINT(-1) */)
{
	if(m_ltDevClipBoard.empty())			return;
	if(!pDoc)								return;
	//!< ���豸�������ˣ��˳�
	std::shared_ptr<CDeviceOne> device, pastDev;
	device = GetDevice(id);
	if(device && !device->CanAddSlave())	return;

	//!< �Ӽ������п�����ԭʼ���ݣ�����ֱ���ã�����Ҫ��������һ��
	std::list<std::shared_ptr<CDeviceOne> > ltDevClipBoard;
	for (auto device : m_ltDevClipBoard)
	{
		pastDev = std::shared_ptr<CDeviceOne>(new CDeviceOne);
		pastDev->CopyFrom(*device);
		pastDev->setID(device->getID());
		pastDev->setParentID(device->getParentID());
		ltDevClipBoard.push_back(pastDev);
	}

	//!< ��¼�豸֮������ӹ�ϵ
	std::map<std::shared_ptr<CDeviceOne>, UINT> m_mpDev_ParentID;		//!< �豸��Ӧ���豸���
	std::map<UINT, std::shared_ptr<CDeviceOne> > m_mpID_Dev;			//!< ��Ŷ�Ӧ�豸
	ltDevClipBoard.front()->setParentID(UINT(-1));						//!< ��һ���豸û�и��豸
	for (auto device : ltDevClipBoard)
	{
		m_mpDev_ParentID[device] = device->getParentID();
		m_mpID_Dev[device->getID()] = device;
	}

	//!< ��������豸����ô�ڼӵ�������ǰ���ȴ���ɴ��ڴ��豸�������Ӱ��ӿڲ��ص��Ĺ���
	device = ltDevClipBoard.front();
	if(id != UINT(-1)){
		device->ResizeInterface(1);
		device->m_vtInterface[0]->SetType(16);
		device->m_vtInterface[0]->SetHandle(0);
	}

	//!< �����еĺ��Ӷ�ճ��������ͼ��
	CString name;
	std::shared_ptr<SDevUndo> sdu;
	for (auto device : ltDevClipBoard)
	{
		name = GetSimilarName(device->getName());
		device->setName(name);
		//!< ���½������豸��ϵ����Ϊ�������ڵ��豸������ǰ�������ѭ���ŵģ����Ա�֤��ճ�����豸ʱ���豸�Ѿ�ճ�����ˣ�����������㷨���Ǵ����
		if(m_mpDev_ParentID[device] == UINT(-1))	device->setParentID(id);
		else										device->setParentID(m_mpID_Dev[m_mpDev_ParentID[device]]->getID());
		if(!AddNewDevice(device))		break;
		if(id == (UINT)-1)				device->SetSelect(true);		// ���֮ǰû��ѡ�е��豸,��ô������豸��������Ϊѡ��
		sdu = std::shared_ptr<SDevUndo>(new SDevUndo(CGbl::UNDO_TYPE_ADD, device));
		pDoc->AddUndoMember(sdu);
	}
	pDoc->SetUndoEnd();

	//!< �ؽ����ӹ�ϵ
	CheckAndConnect();
}

//!< ����Ϊ������Ҫ֪ͨ���м�����
void CDevMgr::OnBehaviorOver()
{
	for (auto br : m_ltBRequest)	br->OnDataReady();
}

//!< ע�������
void CDevMgr::RegisteBRequest(SBehaviorRequest* one)
{
	for (auto br : m_ltBRequest){
		if(br == one)	return;
	}
	m_ltBRequest.push_back(one);
}

//!< ע��������
void CDevMgr::UnRegisteBRequest(SBehaviorRequest* one)
{
	m_ltBRequest.remove(one);
}

//!< ��ø��豸��ʹ�õ����ĳ�ʱʱ��
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
