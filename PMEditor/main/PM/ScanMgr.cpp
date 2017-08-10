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

//!< 从data的index位置上读size大小的信息放到t中
template <class T>
bool CScanMgr::ReadData(T& t, int& index, const char* data, int size /* = 0 */, const int maxsize /* = -1 */)
{
	if(index < 0 || size < 0)						return false;
	if(maxsize > 0 && maxsize < (index + size))		return false;	//!< 判断越界
	if(size == 0)				size = sizeof(T);
	memcpy(&t, &data[index], size);
	index += size;
	return true;
}

//!< 删除所有扫描设备
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
		if(device->IsProj())						//!< 将工程设备中的非工程接口删掉
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
	CString strError = _T("扫描数据包错误");
	if(m_uiOldNum == num)								//!< 如果上一次的数据一摸一样，就不用解析了。
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
	i += 32;																			//!< 前32个字节保留
	if(!ReadData(uiVersion, i, data, 4, num))			goto SHOW_ERROR;				//!< 解析版本号设备类型

	if(uiVersion == 0)
	{
		for(;i < num;)
		{
			device = std::shared_ptr<CDeviceOne>(new CDeviceOne());
			device->m_vtInterface.clear();

			//!< 解析设备类型
			if(!ReadData(uiDevType, i, data, 4, num))	goto SHOW_ERROR;
			device->setDevType(uiDevType);

			//!< 解析DevID
			if(!ReadData(pChar[0], i, data, 32, num))	goto SHOW_ERROR;
			device->setStrID(CString(pChar));

			//!< 解析接口数量
			if(!ReadData(count, i, data, 4, num))		goto SHOW_ERROR;
			if(count < 1 || count > 5)
			{
				CString strCount;
				strCount.Format("%d", count);
				strError = _T("设备 \"") + device->getStrID() + _T("\" 的接口数量为 ") + strCount + _T("，扫描信息错误！");
				goto SHOW_ERROR;
			}
			int nVal;
			for(int j = 0; j < count; ++j)
			{
				inf = std::shared_ptr<CDeviceInterface>((new CDeviceInterface(device.get())));
				inf->SetID(j);
				if(!ReadData(nVal, i, data, 4, num))			goto SHOW_ERROR;	//!< 解析接口类型
				inf->SetType(nVal);
				if(inf->GetType() == 0){
					if(!ReadData(nVal, i, data, 4, num))		goto SHOW_ERROR;	//!< 解析IP地址
					inf->SetDoMain(CGbl::GetIPFromNumber(nVal));
				}
				else if(inf->GetType() == 1){
					if(!ReadData(nVal, i, data, 4, num))		goto SHOW_ERROR;	//!< 解析串口号，COM1还是COM2等
					inf->SetSerialNo(nVal);
					if(!ReadData(nVal, i, data, 4, num))		goto SHOW_ERROR;	//!< 解析模块地址
					inf->SetHandle(nVal);
				}
				else if(inf->GetType() == 16){
					if(!ReadData(nVal, i, data, 4, num))		goto SHOW_ERROR;	//!< 解析Handle号
					inf->SetHandle(nVal);
					if(!ReadData(nVal, i, data, 4, num))		goto SHOW_ERROR;	//!< 解析接口所在层数
					inf->SetScanLevel(nVal);
					if(!ReadData(pChar[0], i, data, 32, num))	goto SHOW_ERROR;	//!< 解析接口所属父设备的ParentID
					inf->SetStrParentID(CString(pChar));
				}
				else{
					goto SHOW_ERROR;
				}
				inf->SetState(1);		//!< 设置为在线状态
				device->m_vtInterface.push_back(inf);
			}
			m_ltScanDev.push_back(device);
		}
	}

	CDevMgr* devMgr = &CDevMgr::GetMe();
	if(!OnCheckDev())		return;		//!< 验证数据的正确性
	ReBuildData();						//!< 重建数据的存放循序，由主到此
	OnConnectToProj();					//!< 
	if(devMgr->m_DevMapDoc)	devMgr->m_DevMapDoc->FreshMap();
	return;

SHOW_ERROR:
	CGbl::PrintOut(strError);			//!< 如果想看是哪里错了，可以根据当前i的值来确定解析到扫描位置了
}

//!< 检查返回的数据的正确性
bool CScanMgr::OnCheckDev()
{
	CString str;

	//!< 一定能找到父设备
	std::shared_ptr<CDeviceOne> device;
	std::shared_ptr<CDeviceInterface> inf;
	for (auto device : m_ltScanDev)
	{
		for (auto inf : device->m_vtInterface)
		{
			if(inf->getStrParentID() != _T("") && !GetDevice(inf->getStrParentID()))
			{
				str = _T("设备 \"") + device->getStrID() + _T("\" 找不到父设备 \"") + inf->getStrParentID() + _T("\"，扫描信息错误！");
				CGbl::PrintOut(str);
				return false;
			}
		}
	}

	//!< m_strDevID不能重复
	for (auto device : m_ltScanDev)
	{
		if(GetDeviceCount(device->getStrID()) > 1)
		{
			str = _T("DeviceID： \"") + device->getStrID() + _T("\" 重复， 扫描信息错误！");
			CGbl::PrintOut(str);
			return false;
		}
	}
	return true;
}

//!< 重建数据关系
void CScanMgr::ReBuildData()
{
	ReBuildOneParent();
	ReBuildMain();
}

//!< 重建数据关系，保证每个设备都最多只有一个父设备
void CScanMgr::ReBuildOneParent()
{
	//!< 处理方式：当发现一个设备有多个接口连父设备的话，将其中层数最小的并指向同一个父设备的接口留下，其余的都删了
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
			if(minLevel == inf->getScanLevel())	//!< 平级的不删，记录起来
			{
				if(minStrParentID == inf->getStrParentID())		//!< 因为只能有1个父设备，所以必须与记录的比较一下
					ltInf.push_back(inf);
			}
			else								//!< 级别小的，以前记录的都删掉，重新记录
			{
				minLevel = inf->getScanLevel();
				minStrParentID = inf->getStrParentID();
				ltInf.clear();
				ltInf.push_back(inf);
			}
		}
		if(device->m_vtInterface.size() == ltInf.size())	continue;	//!< 没变化

		//!< 报告接口删除
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
		strError = _T("设备ID(\"") + device->getStrID() + _T("\")冲突，设备类型为\"") + device->getStrType() + _T("\"");
		strMinInf = _T("                系统保留接口") + strMinInf;
		strDelInf = _T("                移除冲突接口") + strDelInf;
		CGbl::PrintOut(strError);
		CGbl::PrintOut(strMinInf);
		CGbl::PrintOut(strDelInf);

		//!< 接口删除
		device->m_vtInterface.clear();
		for (auto inf : ltInf)
			device->m_vtInterface.push_back(inf);
	}
}

//!< 重建数据关系，将原设备信息以树形的结构前序遍历的循序重新排布
void CScanMgr::ReBuildMain()
{
	std::shared_ptr<CDeviceOne> device;
	std::list<std::shared_ptr<CDeviceOne> > ltDevice;
	for (auto device : m_ltScanDev)
	{	//!< 只针对父设备
		if(device->getStrParentID() != _T(""))	continue;
		ltDevice.push_back(device);
		ReBuildSlave(device, ltDevice);
	}
	m_ltScanDev.clear();
	m_ltScanDev = ltDevice;
}

//!< 重建从设备关系
void CScanMgr::ReBuildSlave(std::shared_ptr<CDeviceOne> device, std::list<std::shared_ptr<CDeviceOne> >& ltDevice)
{
	std::shared_ptr<CDeviceOne> child;
	for (auto child : m_ltScanDev)
	{	//!< 针对从设备，递归后边所有级
		if(child->getStrParentID() != device->getStrID())	continue;
		ltDevice.push_back(child);
		ReBuildSlave(child, ltDevice);
	}
}

//!< 连接到工程中
void CScanMgr::OnConnectToProj()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> scanOne, projOne, matchDev, empty;

	//!< 先将工程中的设备都置为离线
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

	//!< 遍历扫描设备
	int nMatch = 0;				//!< 匹配结果，0x1strID相同，0x02类型相同，0x04地址相同
	for (auto scanOne : m_ltScanDev)
	{
		matchDev = empty;
		//!< 这次遍历找地址相同的工程设备，优先比对工程设备
		for (auto projOne : devMgr->m_vtDevice)
		{
			if(!projOne)			continue;
			if(!projOne->IsProj())	continue;	//!< 优先比对工程设备
			nMatch = IsSameDevice(scanOne, projOne);
			//!< 如果找到工程中的设备，则进行对应
			if(nMatch == 0x03)			//!< 地址类型都相同
			{
				SetOnLine(scanOne, projOne);
				projOne->setStrID(scanOne->getStrID());
				for (auto inf : projOne->m_vtInterface)
					inf->SetStrParentID(scanOne->getStrParentID());
				matchDev = projOne;
				break;
			}
		}

		if(matchDev)	continue;				//!< 如果没找到匹配设备，再看看扫描设备有没有匹配的
		for (auto projOne : devMgr->m_vtDevice)
		{
			if(!projOne)			continue;
			if(projOne->IsProj())	continue;	//!< 工程设备已经比对完了
			nMatch = IsSameDevice(scanOne, projOne);
			//!< 如果找到工程中的设备，则进行对应
			if(nMatch == 0x03)			//!< 地址类型都相同
			{
				SetOnLine(scanOne, projOne);
				projOne->setStrID(scanOne->getStrID());
				for (auto inf : projOne->m_vtInterface)
					inf->SetStrParentID(scanOne->getStrParentID());
				matchDev = projOne;
				break;
			}
		}

		//!< 如果没找到匹配设备，那么证明是新上来的设备
		if(matchDev)	continue;
		int maxNumber = devMgr->GetDeviceSize();
		CString str;
		str.Format("%d", maxNumber);
		scanOne->setName(devMgr->GetSimilarName(scanOne->getStrType() + _T("_") + str));
		scanOne->setProj(false);
		scanOne->InitDevType(scanOne->getDevType());
		//!< 如果这个扫描是从设备，那么就要找自己的主设备了
		if(scanOne->getStrParentID() != _T(""))
		{
			std::shared_ptr<CDeviceOne> parent = GetDeviceFromProj(scanOne->getStrParentID());
			if(!parent)	return;
			ASSERT(parent);
			scanOne->setParentID(parent->getID());
		}
		devMgr->AddNewDevice(scanOne);
	}

	//!< 将工程所有离线状态下的扫描设备删除
	DelOffLineScanDev();

	//!< 重建CDevMgr里的设备的链接关系
	OnBuildConnect();
}

//!< 重建CDevMgr里的设备的链接关系
void CScanMgr::OnBuildConnect()
{
	CDevMgr* devMgr = &CDevMgr::GetMe();
	std::shared_ptr<CDeviceOne> child, parent;
//	//!< 先清空所有设备的子设备列表
//	foreach(child, devMgr->m_vtDevice){
//		if(!child)								continue;
//		if(child->getStrID() == _T(""))			continue;
//		child->m_ltChildID.clear();
//	}
	//!< 填充设备列表
	for (auto child : devMgr->m_vtDevice){
		if(!child)								continue;
		if(child->getStrParentID() == _T(""))	continue;
		parent = GetDeviceFromProj(child->getStrParentID());
		if(!parent)								ASSERT(FALSE);
		parent->AddChild(child->getID());
	}
}

//!< 删除掉所有不在线的工程中的扫描设备
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
		if(device->IsProj())					//!< 工程设备，要删除其不在线的非工程接口
		{
			ltInf.clear();
			ltDelInf.clear();
			for (auto inf : device->m_vtInterface)
			{
				if(inf)							ltInf.push_back(inf);		//!< 记录了该所有接口
			}
			for (auto inf : ltInf)
			{
				if(inf->IsProj())				continue;					//!< 工程的接口不删
				if(inf->GetState() == 0)		ltDelInf.push_back(inf);	//!< 记录要删除的接口
			}
			for (auto inf : ltDelInf)				ltInf.remove(inf);			//!< 删除了所有不在线的接口
			device->m_vtInterface.resize(ltInf.size());						//!< 将剩下的接口赋给设备
			int i = 0;
			for (auto inf : ltInf)
			{
				device->m_vtInterface[i] = inf;
				inf->SetID(i);
				++i;
			}
		}
		else									//!< 非工程设备，删除不在线的非工程接口，如果没有接口了就删除设备
		{
			ltInf.clear();
			ltDelInf.clear();
			for (auto inf : device->m_vtInterface)
			{
				if(inf)							ltInf.push_back(inf);		//!< 记录了该所有接口
			}
			for (auto inf : ltInf)
			{
				if(inf->GetState() == 0)		ltDelInf.push_back(inf);	//!< 记录要删除的接口
			}
			for (auto inf : ltDelInf)				ltInf.remove(inf);			//!< 删除了所有不在线的接口
			if(ltInf.empty())					//!< 如果没接口了就删设备
			{
				ltDelDevID.push_back(device->getID());
			}
			else								//!< 将剩下的接口赋给设备
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

//!< 设置工程设备上线
bool CScanMgr::SetOnLine(std::shared_ptr<CDeviceOne> scanDev, std::shared_ptr<CDeviceOne> projDev)
{
	std::shared_ptr<CDeviceInterface> scanInf, projInf;
	for (auto scanInf : scanDev->m_vtInterface)
	{
		bool bFind = false;
		for (auto projInf : projDev->m_vtInterface)
		{
			if(scanInf->GetName(false) != projInf->GetName(false))	continue;
			projInf->SetState(scanInf->GetState());			//!< 将接口匹配的设备置成上线（或在线未配置）
			bFind = true;
		}
		if(!bFind)		//!< 如果有个接口工程设备没有，那么就创建个接口
		{
			scanInf->SetProj(false);
			scanInf->SetID(projDev->m_vtInterface.size());
			projDev->m_vtInterface.push_back(scanInf);
		}
	}
	return true;
}

//!< 从扫描设备列表里，找到该设备
std::shared_ptr<CDeviceOne> CScanMgr::GetDevice(CString strDevID)
{
	std::shared_ptr<CDeviceOne> device, empty;
	for (auto device : m_ltScanDev)
		if(device->getStrID() == strDevID)
			return device;
	return empty;
}

//!< 从工程设备列表里，找到该设备
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

//!< 找到该strDevID的设备的数量
UINT CScanMgr::GetDeviceCount(CString strDevID)
{
	UINT count = 0;
	std::shared_ptr<CDeviceOne> device;
	for (auto device : m_ltScanDev)
		if(device->getStrID() == strDevID)
			++count;
	return count;
}

//!< 是否是同一个设备，0x01类型相同，0x02地址相同
int CScanMgr::IsSameDevice(std::shared_ptr<CDeviceOne> scanDev, std::shared_ptr<CDeviceOne> projDev)
{
	ASSERT(scanDev && projDev);
	int rlt = 0;

	//!< 类型一样
	if(scanDev->getDevType() == projDev->getDevType())		rlt |= 0x01;

	//!< 地址一样
	if(!IsSameParent(scanDev, projDev))						return rlt;
	std::shared_ptr<CDeviceInterface> projInf, scanInf;
	bool bMatchAddr = false;
	for (auto projInf : projDev->m_vtInterface)
	{
		if(projInf->GetState() != 0)						return rlt;			//!< 如果工程设备存在一个在线接口时，不用找了，返回false
		for (auto scanInf : scanDev->m_vtInterface)
		{
			if(scanInf->GetName(false) == projInf->GetName(false))	bMatchAddr = true;	//!< 如果至少存在一个相同地址，则表明地址匹配了
		}
	}
	if(bMatchAddr)											rlt |= 0x02;		//!< 地址匹配了
	return rlt;
}

//!< 是否有同一个父亲
bool CScanMgr::IsSameParent(std::shared_ptr<CDeviceOne> scanDev, std::shared_ptr<CDeviceOne> projDev)
{
	//!< 如果父设备不同，那么肯定不是同一个地址
	CDevMgr* devMgr = &CDevMgr::GetMe();
	if(scanDev->getStrParentID() == _T("") && projDev->getParentID() == UINT(-1))		return true;
	else if(scanDev->getStrParentID() == _T("") && projDev->getParentID() != UINT(-1))	return false;
	else if(scanDev->getStrParentID() != _T("") && projDev->getParentID() == UINT(-1))	return false;

	std::shared_ptr<CDeviceOne> devFromID, devFromStrID;
	devFromStrID = GetDeviceFromProj(scanDev->getStrParentID());
	devFromID = devMgr->GetDevice(projDev->getParentID());
	return devFromStrID == devFromID;
}
