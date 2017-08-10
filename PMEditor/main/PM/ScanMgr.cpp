#include "StdAfx.h"
#include "Gbl.h"
#include "DevMgr.h"
#include "DeviceMapDoc.h"
#include "DeviceOne.h"
#include "DeviceInterface.h"
#include "ScanMgr.h"

using namespace MVC;
using namespace Device;

CScanInterface::CScanInterface(void)
:CDeviceInterface(NULL)
,m_uiLevel(0)
{
}


CScanMgr::CScanMgr(void)
:m_uiOldNum(0)
{
}

CScanMgr::~CScanMgr(void)
{
}

CScanMgr& CScanMgr::GetMe()
{
	static CScanMgr one;
	return one;
}

//!< ��data��indexλ���϶�size��С����Ϣ�ŵ�t��
template <class T>
bool CScanMgr::ReadData(T& t, int& index, const char* data, int size /* = 0 */, const int maxsize /* = -1 */)
{
	if(index < 0 || size < 0)						return false;
	if(maxsize > 0 && maxsize < (index + size))		return false;	//!< �ж�Խ��
	if(size == 0)				size = sizeof(T);
	memcpy(&t, &data[index], size);
	index += size;
	return true;
}

//!< ɾ������ɨ���豸
void CScanMgr::DelAllScanDev()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> device;
	std::shared_ptr<CDeviceInterface> inf, emptyInf;
	int size;
	for (auto device : devMgr->m_vtDevice)
	{
		if(!device)									continue;
		device->SetOffLine();
		if(device->IsProj())						//!< �������豸�еķǹ��̽ӿ�ɾ��
		{
			size = device->m_vtInterface.size();
			int i;
			for(i = 0; i < size; ++i)
			{
				inf = device->m_vtInterface[i];
				if(!inf)							continue;
				if(!inf->IsProj())					break;
			}
			device->m_vtInterface.resize(i);
			continue;
		}
		devMgr->DeleteDevice(device->getID());
	}
}

void CScanMgr::OnScanRev(const char* data, const int num)
{
	CString strError = _T("ɨ�����ݰ�����");
	if(m_uiOldNum == num)								//!< �����һ�ε�����һ��һ�����Ͳ��ý����ˡ�
	{
		if(memcmp(data, m_cOldData, int(num)) == 0)		return;
	}
	m_uiOldNum = num;
	memcpy(m_cOldData, data, num);
	std::shared_ptr<CDeviceOne> curProjDev, newProjDev, device;
	std::shared_ptr<CDeviceInterface> inf;
	char pChar[33];
	pChar[32] = 0;
	int i = 0, count = 0, j = 0;
	UINT uiDevType = 0, uiHandle = 0, uiVersion = 0;
	m_ltScanDev.clear();

	if(num < 36)										goto SHOW_ERROR;
	i += 32;																			//!< ǰ32���ֽڱ���
	if(!ReadData(uiVersion, i, data, 4, num))			goto SHOW_ERROR;				//!< �����汾���豸����

	if(uiVersion == 0)
	{
		for(;i < num;)
		{
			device = std::shared_ptr<CDeviceOne>(new CDeviceOne());
			device->m_vtInterface.clear();

			//!< �����豸����
			if(!ReadData(uiDevType, i, data, 4, num))	goto SHOW_ERROR;
			device->setDevType(uiDevType);

			//!< ����DevID
			if(!ReadData(pChar[0], i, data, 32, num))	goto SHOW_ERROR;
			device->setStrID(CString(pChar));

			//!< �����ӿ�����
			if(!ReadData(count, i, data, 4, num))		goto SHOW_ERROR;
			if(count < 1 || count > 5)
			{
				CString strCount;
				strCount.Format("%d", count);
				strError = _T("�豸 \"") + device->getStrID() + _T("\" �Ľӿ�����Ϊ ") + strCount + _T("��ɨ����Ϣ����");
				goto SHOW_ERROR;
			}
			int nVal;
			for(int j = 0; j < count; ++j)
			{
				inf = std::shared_ptr<CDeviceInterface>((new CDeviceInterface(device.get())));
				inf->SetID(j);
				if(!ReadData(nVal, i, data, 4, num))			goto SHOW_ERROR;	//!< �����ӿ�����
				inf->SetType(nVal);
				if(inf->GetType() == 0){
					if(!ReadData(nVal, i, data, 4, num))		goto SHOW_ERROR;	//!< ����IP��ַ
					inf->SetDoMain(CGbl::GetIPFromNumber(nVal));
				}
				else if(inf->GetType() == 1){
					if(!ReadData(nVal, i, data, 4, num))		goto SHOW_ERROR;	//!< �������ںţ�COM1����COM2��
					inf->SetSerialNo(nVal);
					if(!ReadData(nVal, i, data, 4, num))		goto SHOW_ERROR;	//!< ����ģ���ַ
					inf->SetHandle(nVal);
				}
				else if(inf->GetType() == 16){
					if(!ReadData(nVal, i, data, 4, num))		goto SHOW_ERROR;	//!< ����Handle��
					inf->SetHandle(nVal);
					if(!ReadData(nVal, i, data, 4, num))		goto SHOW_ERROR;	//!< �����ӿ����ڲ���
					inf->SetScanLevel(nVal);
					if(!ReadData(pChar[0], i, data, 32, num))	goto SHOW_ERROR;	//!< �����ӿ��������豸��ParentID
					inf->SetStrParentID(CString(pChar));
				}
				else{
					goto SHOW_ERROR;
				}
				inf->SetState(1);		//!< ����Ϊ����״̬
				device->m_vtInterface.push_back(inf);
			}
			m_ltScanDev.push_back(device);
		}
	}

	CDevMgr* devMgr = &CDevMgr::GetMe();
	if(!OnCheckDev())		return;		//!< ��֤���ݵ���ȷ��
	ReBuildData();						//!< �ؽ����ݵĴ��ѭ����������
	OnConnectToProj();					//!< 
	if(devMgr->m_DevMapDoc)	devMgr->m_DevMapDoc->FreshMap();
	return;

SHOW_ERROR:
	CGbl::PrintOut(strError);			//!< ����뿴��������ˣ����Ը��ݵ�ǰi��ֵ��ȷ��������ɨ��λ����
}

//!< ��鷵�ص����ݵ���ȷ��
bool CScanMgr::OnCheckDev()
{
	CString str;

	//!< һ�����ҵ����豸
	std::shared_ptr<CDeviceOne> device;
	std::shared_ptr<CDeviceInterface> inf;
	for (auto device : m_ltScanDev)
	{
		for (auto inf : device->m_vtInterface)
		{
			if(inf->getStrParentID() != _T("") && !GetDevice(inf->getStrParentID()))
			{
				str = _T("�豸 \"") + device->getStrID() + _T("\" �Ҳ������豸 \"") + inf->getStrParentID() + _T("\"��ɨ����Ϣ����");
				CGbl::PrintOut(str);
				return false;
			}
		}
	}

	//!< m_strDevID�����ظ�
	for (auto device : m_ltScanDev)
	{
		if(GetDeviceCount(device->getStrID()) > 1)
		{
			str = _T("DeviceID�� \"") + device->getStrID() + _T("\" �ظ��� ɨ����Ϣ����");
			CGbl::PrintOut(str);
			return false;
		}
	}
	return true;
}

//!< �ؽ����ݹ�ϵ
void CScanMgr::ReBuildData()
{
	ReBuildOneParent();
	ReBuildMain();
}

//!< �ؽ����ݹ�ϵ����֤ÿ���豸�����ֻ��һ�����豸
void CScanMgr::ReBuildOneParent()
{
	//!< ����ʽ��������һ���豸�ж���ӿ������豸�Ļ��������в�����С�Ĳ�ָ��ͬһ�����豸�Ľӿ����£�����Ķ�ɾ��
	std::shared_ptr<CDeviceOne> device;
	std::shared_ptr<CDeviceInterface> inf, minLevelInf;
	std::list<std::shared_ptr<CDeviceInterface> > ltInf,ltDelInf;
	UINT minLevel = UINT(-1);
	CString minStrParentID = _T("");
	for (auto device : m_ltScanDev)
	{
		if(device->m_vtInterface.size() == 1)	continue;
		ltInf.clear();
		ltDelInf.clear();
		minLevel = UINT(-1);
		minStrParentID = _T("");
		for (auto inf : device->m_vtInterface)
		{
			ltDelInf.push_back(inf);
			if(minLevel < inf->getScanLevel())	continue;
			if(minLevel == inf->getScanLevel())	//!< ƽ���Ĳ�ɾ����¼����
			{
				if(minStrParentID == inf->getStrParentID())		//!< ��Ϊֻ����1�����豸�����Ա������¼�ıȽ�һ��
					ltInf.push_back(inf);
			}
			else								//!< ����С�ģ���ǰ��¼�Ķ�ɾ�������¼�¼
			{
				minLevel = inf->getScanLevel();
				minStrParentID = inf->getStrParentID();
				ltInf.clear();
				ltInf.push_back(inf);
			}
		}
		if(device->m_vtInterface.size() == ltInf.size())	continue;	//!< û�仯

		//!< ����ӿ�ɾ��
		CString strError, strMinInf, strDelInf;
		for (auto inf : ltInf)
		{
			ltDelInf.remove(inf);
			if(strMinInf == _T(""))		strMinInf = _T("\"") + inf->GetName() + _T("\"");
			else						strMinInf = strMinInf + _T(",\"") + inf->GetName() + _T("\"");
		}
		for (auto inf : ltDelInf)
		{
			if(strDelInf == _T(""))		strDelInf = _T("\"") + inf->GetName() + _T("\"");
			else						strDelInf = strDelInf + _T(",\"") + inf->GetName() + _T("\"");
		}
		strError = _T("�豸ID(\"") + device->getStrID() + _T("\")��ͻ���豸����Ϊ\"") + device->getStrType() + _T("\"");
		strMinInf = _T("                ϵͳ�����ӿ�") + strMinInf;
		strDelInf = _T("                �Ƴ���ͻ�ӿ�") + strDelInf;
		CGbl::PrintOut(strError);
		CGbl::PrintOut(strMinInf);
		CGbl::PrintOut(strDelInf);

		//!< �ӿ�ɾ��
		device->m_vtInterface.clear();
		for (auto inf : ltInf)
			device->m_vtInterface.push_back(inf);
	}
}

//!< �ؽ����ݹ�ϵ����ԭ�豸��Ϣ�����εĽṹǰ�������ѭ�������Ų�
void CScanMgr::ReBuildMain()
{
	std::shared_ptr<CDeviceOne> device;
	std::list<std::shared_ptr<CDeviceOne> > ltDevice;
	for (auto device : m_ltScanDev)
	{	//!< ֻ��Ը��豸
		if(device->getStrParentID() != _T(""))	continue;
		ltDevice.push_back(device);
		ReBuildSlave(device, ltDevice);
	}
	m_ltScanDev.clear();
	m_ltScanDev = ltDevice;
}

//!< �ؽ����豸��ϵ
void CScanMgr::ReBuildSlave(std::shared_ptr<CDeviceOne> device, std::list<std::shared_ptr<CDeviceOne> >& ltDevice)
{
	std::shared_ptr<CDeviceOne> child;
	for (auto child : m_ltScanDev)
	{	//!< ��Դ��豸���ݹ������м�
		if(child->getStrParentID() != device->getStrID())	continue;
		ltDevice.push_back(child);
		ReBuildSlave(child, ltDevice);
	}
}

//!< ���ӵ�������
void CScanMgr::OnConnectToProj()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> scanOne, projOne, matchDev, empty;

	//!< �Ƚ������е��豸����Ϊ����
	std::shared_ptr<CDeviceInterface> inf;
	for (auto projOne : devMgr->m_vtDevice)
	{
		if(!projOne)	continue;
		projOne->setStrID(_T(""));
		for (auto inf : projOne->m_vtInterface){
			inf->SetStrParentID(_T(""));
			inf->SetState(0);
		}
	}

	//!< ����ɨ���豸
	int nMatch = 0;				//!< ƥ������0x1strID��ͬ��0x02������ͬ��0x04��ַ��ͬ
	for (auto scanOne : m_ltScanDev)
	{
		matchDev = empty;
		//!< ��α����ҵ�ַ��ͬ�Ĺ����豸�����ȱȶԹ����豸
		for (auto projOne : devMgr->m_vtDevice)
		{
			if(!projOne)			continue;
			if(!projOne->IsProj())	continue;	//!< ���ȱȶԹ����豸
			nMatch = IsSameDevice(scanOne, projOne);
			//!< ����ҵ������е��豸������ж�Ӧ
			if(nMatch == 0x03)			//!< ��ַ���Ͷ���ͬ
			{
				SetOnLine(scanOne, projOne);
				projOne->setStrID(scanOne->getStrID());
				for (auto inf : projOne->m_vtInterface)
					inf->SetStrParentID(scanOne->getStrParentID());
				matchDev = projOne;
				break;
			}
		}

		if(matchDev)	continue;				//!< ���û�ҵ�ƥ���豸���ٿ���ɨ���豸��û��ƥ���
		for (auto projOne : devMgr->m_vtDevice)
		{
			if(!projOne)			continue;
			if(projOne->IsProj())	continue;	//!< �����豸�Ѿ��ȶ�����
			nMatch = IsSameDevice(scanOne, projOne);
			//!< ����ҵ������е��豸������ж�Ӧ
			if(nMatch == 0x03)			//!< ��ַ���Ͷ���ͬ
			{
				SetOnLine(scanOne, projOne);
				projOne->setStrID(scanOne->getStrID());
				for (auto inf : projOne->m_vtInterface)
					inf->SetStrParentID(scanOne->getStrParentID());
				matchDev = projOne;
				break;
			}
		}

		//!< ���û�ҵ�ƥ���豸����ô֤�������������豸
		if(matchDev)	continue;
		int maxNumber = devMgr->GetDeviceSize();
		CString str;
		str.Format("%d", maxNumber);
		scanOne->setName(devMgr->GetSimilarName(scanOne->getStrType() + _T("_") + str));
		scanOne->setProj(false);
		scanOne->InitDevType(scanOne->getDevType());
		//!< ������ɨ���Ǵ��豸����ô��Ҫ���Լ������豸��
		if(scanOne->getStrParentID() != _T(""))
		{
			std::shared_ptr<CDeviceOne> parent = GetDeviceFromProj(scanOne->getStrParentID());
			if(!parent)	return;
			ASSERT(parent);
			scanOne->setParentID(parent->getID());
		}
		devMgr->AddNewDevice(scanOne);
	}

	//!< ��������������״̬�µ�ɨ���豸ɾ��
	DelOffLineScanDev();

	//!< �ؽ�CDevMgr����豸�����ӹ�ϵ
	OnBuildConnect();
}

//!< �ؽ�CDevMgr����豸�����ӹ�ϵ
void CScanMgr::OnBuildConnect()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> child, parent;
//	//!< ����������豸�����豸�б�
//	foreach(child, devMgr->m_vtDevice){
//		if(!child)								continue;
//		if(child->getStrID() == _T(""))			continue;
//		child->m_ltChildID.clear();
//	}
	//!< ����豸�б�
	for (auto child : devMgr->m_vtDevice){
		if(!child)								continue;
		if(child->getStrParentID() == _T(""))	continue;
		parent = GetDeviceFromProj(child->getStrParentID());
		if(!parent)								ASSERT(FALSE);
		parent->AddChild(child->getID());
	}
}

//!< ɾ�������в����ߵĹ����е�ɨ���豸
void CScanMgr::DelOffLineScanDev()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> device;
	std::shared_ptr<CDeviceInterface> inf;
	std::list<std::shared_ptr<CDeviceInterface> > ltInf, ltDelInf;
	std::list<UINT> ltDelDevID;
	bool bOnLine = false;
	for (auto device : devMgr->m_vtDevice)
	{
		if(!device)								continue;
		if(device->IsProj())					//!< �����豸��Ҫɾ���䲻���ߵķǹ��̽ӿ�
		{
			ltInf.clear();
			ltDelInf.clear();
			for (auto inf : device->m_vtInterface)
			{
				if(inf)							ltInf.push_back(inf);		//!< ��¼�˸����нӿ�
			}
			for (auto inf : ltInf)
			{
				if(inf->IsProj())				continue;					//!< ���̵Ľӿڲ�ɾ
				if(inf->GetState() == 0)		ltDelInf.push_back(inf);	//!< ��¼Ҫɾ���Ľӿ�
			}
			for (auto inf : ltDelInf)				ltInf.remove(inf);			//!< ɾ�������в����ߵĽӿ�
			device->m_vtInterface.resize(ltInf.size());						//!< ��ʣ�µĽӿڸ����豸
			int i = 0;
			for (auto inf : ltInf)
			{
				device->m_vtInterface[i] = inf;
				inf->SetID(i);
				++i;
			}
		}
		else									//!< �ǹ����豸��ɾ�������ߵķǹ��̽ӿڣ����û�нӿ��˾�ɾ���豸
		{
			ltInf.clear();
			ltDelInf.clear();
			for (auto inf : device->m_vtInterface)
			{
				if(inf)							ltInf.push_back(inf);		//!< ��¼�˸����нӿ�
			}
			for (auto inf : ltInf)
			{
				if(inf->GetState() == 0)		ltDelInf.push_back(inf);	//!< ��¼Ҫɾ���Ľӿ�
			}
			for (auto inf : ltDelInf)				ltInf.remove(inf);			//!< ɾ�������в����ߵĽӿ�
			if(ltInf.empty())					//!< ���û�ӿ��˾�ɾ�豸
			{
				ltDelDevID.push_back(device->getID());
			}
			else								//!< ��ʣ�µĽӿڸ����豸
			{
				device->m_vtInterface.resize(ltInf.size());
				int i = 0;
				for (auto inf : ltInf)
				{
					device->m_vtInterface[i] = inf;
					inf->SetID(i);
					++i;
				}
			}
		}
	}
	for (UINT id : ltDelDevID)
		devMgr->DeleteDevice(id);
}

//!< ���ù����豸����
bool CScanMgr::SetOnLine(std::shared_ptr<CDeviceOne> scanDev, std::shared_ptr<CDeviceOne> projDev)
{
	std::shared_ptr<CDeviceInterface> scanInf, projInf;
	for (auto scanInf : scanDev->m_vtInterface)
	{
		bool bFind = false;
		for (auto projInf : projDev->m_vtInterface)
		{
			if(scanInf->GetName(false) != projInf->GetName(false))	continue;
			projInf->SetState(scanInf->GetState());			//!< ���ӿ�ƥ����豸�ó����ߣ�������δ���ã�
			bFind = true;
		}
		if(!bFind)		//!< ����и��ӿڹ����豸û�У���ô�ʹ������ӿ�
		{
			scanInf->SetProj(false);
			scanInf->SetID(projDev->m_vtInterface.size());
			projDev->m_vtInterface.push_back(scanInf);
		}
	}
	return true;
}

//!< ��ɨ���豸�б���ҵ����豸
std::shared_ptr<CDeviceOne> CScanMgr::GetDevice(CString strDevID)
{
	std::shared_ptr<CDeviceOne> device, empty;
	for (auto device : m_ltScanDev)
		if(device->getStrID() == strDevID)
			return device;
	return empty;
}

//!< �ӹ����豸�б���ҵ����豸
std::shared_ptr<CDeviceOne> CScanMgr::GetDeviceFromProj(CString strDevID)
{
	std::shared_ptr<CDeviceOne> device, empty;
	CDevMgr* devMgr = &CDevMgr::GetMe();
	for (auto device : devMgr->m_vtDevice)
	{
		if(!device)							continue;
		if(device->getStrID() == strDevID)	return device;
	}
	return empty;
}

//!< �ҵ���strDevID���豸������
UINT CScanMgr::GetDeviceCount(CString strDevID)
{
	UINT count = 0;
	std::shared_ptr<CDeviceOne> device;
	for (auto device : m_ltScanDev)
		if(device->getStrID() == strDevID)
			++count;
	return count;
}

//!< �Ƿ���ͬһ���豸��0x01������ͬ��0x02��ַ��ͬ
int CScanMgr::IsSameDevice(std::shared_ptr<CDeviceOne> scanDev, std::shared_ptr<CDeviceOne> projDev)
{
	ASSERT(scanDev && projDev);
	int rlt = 0;

	//!< ����һ��
	if(scanDev->getDevType() == projDev->getDevType())		rlt |= 0x01;

	//!< ��ַһ��
	if(!IsSameParent(scanDev, projDev))						return rlt;
	std::shared_ptr<CDeviceInterface> projInf, scanInf;
	bool bMatchAddr = false;
	for (auto projInf : projDev->m_vtInterface)
	{
		if(projInf->GetState() != 0)						return rlt;			//!< ��������豸����һ�����߽ӿ�ʱ���������ˣ�����false
		for (auto scanInf : scanDev->m_vtInterface)
		{
			if(scanInf->GetName(false) == projInf->GetName(false))	bMatchAddr = true;	//!< ������ٴ���һ����ͬ��ַ���������ַƥ����
		}
	}
	if(bMatchAddr)											rlt |= 0x02;		//!< ��ַƥ����
	return rlt;
}

//!< �Ƿ���ͬһ������
bool CScanMgr::IsSameParent(std::shared_ptr<CDeviceOne> scanDev, std::shared_ptr<CDeviceOne> projDev)
{
	//!< ������豸��ͬ����ô�϶�����ͬһ����ַ
	CDevMgr* devMgr = &CDevMgr::GetMe();
	if(scanDev->getStrParentID() == _T("") && projDev->getParentID() == UINT(-1))		return true;
	else if(scanDev->getStrParentID() == _T("") && projDev->getParentID() != UINT(-1))	return false;
	else if(scanDev->getStrParentID() != _T("") && projDev->getParentID() == UINT(-1))	return false;

	std::shared_ptr<CDeviceOne> devFromID, devFromStrID;
	devFromStrID = GetDeviceFromProj(scanDev->getStrParentID());
	devFromID = devMgr->GetDevice(projDev->getParentID());
	return devFromStrID == devFromID;
}
