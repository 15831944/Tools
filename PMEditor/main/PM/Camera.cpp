#include "StdAfx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "Camera.h"
#include "CamMgr.h"

using namespace MVC;
using namespace Camera;

const CString CAM_ID = _T("ID");
const CString CAM_TAG = _T("Tag");
const CString CAM_RECORD = _T("Record");
const CString CAM_PATH = _T("Path");
const CString CAM_ENCODE = _T("EncodeType");
const CString CAM_DISTINGUISH = _T("Distinguish");
const CString CAM_FPS = _T("FPS");
const CString CAM_BROADCAST = _T("Broadcast");
const CString CAM_PORT = _T("Port");
const CString CAM_MAXCLIENT = _T("MaxClient");
const CString CAM_BROADENCODE = _T("BroadEncodeType");
const CString CAM_BROADDISTINGUISH = _T("BroadDistinguish");
const CString CAM_BROADFPS = _T("BroadFPS");

CCamera::CCamera(void)
:m_nID(-1)								//!< 编号
,m_bRecord(true)						//!< 是否启动录制功能
//,m_bAutoSave(true)						//!< 是否自动保存视频文件
,m_strEncodeType(_T("WMV2"))			//!< 视频编码格式,目前只能选WMV2
,m_strDistinguish(_T("320*240"))		//!< 录制分辨率,160*120,176*144,......,720*480
,m_strFPS(_T("1fps"))					//!< 录制帧速率29.97,15,10,5,1,0.5 fps,默认1fps
,m_bBroadCast(true)						//!< 是否启动网络广播
,m_nMaxClient(5)						//!< 最大连接数,默认5,范围0~50
,m_strBroadEncodeType(_T("WMV2"))		//!< 广播视频编码格式,目前只能选WMV2
,m_strBroadDistinguish(_T("320*240"))	//!< 广播分辨率,160*120,176*144,......,720*480
,m_strBroadFPS(_T("15fps"))				//!< 广播帧速率29.97,15,10,5,1,0.5 fps
{
	m_strFilePath = CGbl::GetSpecialPath(CSIDL_PERSONAL);
	m_nPort = CCamMgr::GetMe().GetDifferentPort(8080);			//!< 广播端口号,默认8080
}

CCamera::~CCamera(void)
{
}

//!< 编号
void CCamera::SetID(int id)
{
	m_nID = id;
	SetModify();
}

//!< 名称
void CCamera::SetTag(CString strTag)
{
	if(m_strTag == strTag)					return;
	m_strTag = strTag;
	SetModify();
}

//!< 是否启动录制功能
void CCamera::SetRecord(bool bRecord)
{
	if(m_bRecord == bRecord)				return;
	m_bRecord = bRecord;
	SetModify();
}

//!< 是否自动保存视频文件
//void CCamera::SetAutoSave(bool bAutoSave){m_bAutoSave = bAutoSave; SetModify();}

//!< 视频文件存储位置,最后带'\',默认我的文档
void CCamera::SetFilePath(CString strFilePath)
{
	if(m_strFilePath == strFilePath)		return;
	m_strFilePath = strFilePath;
	SetModify();
}

//!< 视频编码格式,目前只能选WMV2
void CCamera::SetEncodeType(CString strEncodeType)
{
	if(m_strEncodeType == strEncodeType)	return;
	m_strEncodeType = strEncodeType;
	SetModify();
}

//!< 录制分辨率,160*120,176*144,......,720*480,默认320*240
void CCamera::SetDistinguish(CString strDistinguish)
{
	if(m_strDistinguish == strDistinguish)	return;
	m_strDistinguish = strDistinguish;
	SetModify();
}

//!< 录制帧速率29.97,15,10,5,1,0.5 fps,默认1fps
void CCamera::SetFPS(CString strFPS)
{
	if(m_strFPS == strFPS)					return;
	m_strFPS = strFPS;
	SetModify();
}

//!< 是否启动网络广播
void CCamera::SetBroadCast(bool bBroadCast)
{
	if(m_bBroadCast  == bBroadCast)			return;
	m_bBroadCast = bBroadCast;
	SetModify();
}

//!< 广播端口号,8080
void CCamera::SetPort(int nPort)
{
	if(nPort < 0)							nPort = 0;
	else if(nPort > 65535)					nPort = 65535;
	if(m_nPort == nPort)					return;
	m_nPort = nPort;
	SetModify();
}

//!< 最大连接数,默认5,范围0~50
void CCamera::SetMaxClient(int nMaxClient)
{
	if(nMaxClient < 5)						nMaxClient = 5;
	else if(nMaxClient > 50)				nMaxClient = 50;
	if(m_nMaxClient == nMaxClient)			return;
	m_nMaxClient = nMaxClient;
	SetModify();
}

//!< 广播视频编码格式,目前只能选WMV2
void CCamera::SetBroadEncodeType(CString strBroadEncodeType)
{
	if(m_strBroadEncodeType == strBroadEncodeType)		return;
	m_strBroadEncodeType = strBroadEncodeType;
	SetModify();
}

//!< 广播分辨率,160*120,176*144,......,720*480
void CCamera::SetBroadDistinguish(CString strBroadDistinguish)
{
	if(m_strBroadDistinguish == strBroadDistinguish)	return;
	m_strBroadDistinguish = strBroadDistinguish;
	SetModify();
}

//!< 广播帧速率29.97,15,10,5,1,0.5 fps
void CCamera::SetBroadFPS(CString strBroadFPS)
{
	if(m_strBroadFPS == strBroadFPS)					return;
	m_strBroadFPS = strBroadFPS;
	SetModify();
}

//!< 设置为改变
void CCamera::SetModify()
{
	CCamMgr::GetMe().SetModify(true);
}


//!< 保存xml格式的编辑信息,iExport=false正常
bool CCamera::SerializeXml(TiXmlElement* pNode, bool bRead)
{
	if(bRead)
	{
		CString name, strValue;
		CComVariant cv;
		TiXmlAttribute* pAttr = pNode->FirstAttribute();
		int iValue;
		while(pAttr)
		{
			name = pAttr->Name();
			strValue = pAttr->Value();
			cv = strValue;

			if(name == CAM_ID)						{cv.ChangeType(VT_I4);		m_nID = cv.intVal;}
			else if(name == CAM_TAG)				{m_strTag = strValue;}
			else if(name == CAM_RECORD)				{m_bRecord = strValue == _T("true") ? true : false;}
			else if(name == CAM_PATH)				{m_strFilePath = strValue;}
			else if(name == CAM_ENCODE)				{m_strEncodeType = strValue;}
			else if(name == CAM_DISTINGUISH)		{m_strDistinguish = strValue;}
			else if(name == CAM_FPS)				{m_strFPS = strValue;}
			else if(name == CAM_BROADCAST)			{m_bBroadCast = strValue == _T("true") ? true : false;}
			else if(name == CAM_PORT)				{cv.ChangeType(VT_I4);		m_nPort = cv.intVal;}
			else if(name == CAM_MAXCLIENT)			{cv.ChangeType(VT_I4);		m_nMaxClient = cv.intVal;}
			else if(name == CAM_BROADENCODE)		{m_strBroadEncodeType = strValue;}
			else if(name == CAM_BROADDISTINGUISH)	{m_strBroadDistinguish = strValue;}
			else if(name == CAM_BROADFPS)			{m_strBroadFPS = strValue;}
			pAttr = pAttr->Next();
		}
	}
	else
	{
		pNode->SetAttribute(CAM_ID, GetID());
		pNode->SetAttribute(CAM_TAG, GetTag());
		pNode->SetAttribute(CAM_RECORD, IsRecord() ? _T("true") : _T("false"));
		pNode->SetAttribute(CAM_PATH, GetFilePath());
		pNode->SetAttribute(CAM_ENCODE, GetEncodeType());
		pNode->SetAttribute(CAM_DISTINGUISH, GetDistinguish());
		pNode->SetAttribute(CAM_FPS, GetFPS());
		pNode->SetAttribute(CAM_BROADCAST, IsBroadCast() ? _T("true") : _T("false"));
		pNode->SetAttribute(CAM_PORT, GetPort());
		pNode->SetAttribute(CAM_MAXCLIENT, GetMaxClient());
		pNode->SetAttribute(CAM_BROADENCODE, GetBroadEncodeType());
		pNode->SetAttribute(CAM_BROADDISTINGUISH, GetBroadDistinguish());
		pNode->SetAttribute(CAM_BROADFPS, GetBroadFPS());
	}
	return true;
}

//!< 查找本变量，要匹配的文本，全字符匹配，大小写匹配
bool CCamera::DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex /* = false */)
{
	CString strInfo;
	bool bMatch = false;
	int nMatchCount = 0;
	CComVariant cvr;
	strInfo.Format("%d", m_nID);
	strInfo = _T("Information:摄像头") + strInfo + _T(" （编号") + strInfo + _T("）  ——  ");

	//!< 编号是否匹配
	cvr = m_nID;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("编号"), strInfo))		++nMatchCount;

	//!< 名称是否匹配
	cvr = m_strTag;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("说明"), strInfo))		++nMatchCount;

	//!< 地址是否匹配
	cvr = GetPort();
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("端口"), strInfo))		++nMatchCount;

	if(nMatchCount > 0){
		CGbl::PrintOut(strInfo);
		return true;
	}
	return false;
}

//!< 赋值操作，包括ID和名称也全都赋过去了
CCamera& CCamera::operator = (CCamera& item)
{
	m_nID = item.GetID();
	m_strTag = item.GetTag();								//!< 说明
	m_bRecord = item.IsRecord();							//!< 是否启动录制功能
	//m_bAutoSave = item.IsAutoSave();						//!< 是否自动保存视频文件
	m_strFilePath = item.GetFilePath();						//!< 视频文件存储位置,最后带'\',默认我的文档
	m_strEncodeType = item.GetEncodeType();					//!< 视频编码格式,目前只能选WMV2
	m_strDistinguish = item.GetDistinguish();				//!< 录制分辨率,160*120,176*144,......,720*480,默认320*240
	m_strFPS = item.GetFPS();								//!< 录制帧速率29.97,15,10,5,1,0.5 fps,默认1fps
	m_bBroadCast = item.IsBroadCast();						//!< 是否启动网络广播
	m_nPort = item.GetPort();								//!< 广播端口号,8080
	m_nMaxClient = item.GetMaxClient();						//!< 最大连接数,默认5,范围0~50
	m_strBroadEncodeType = item.GetBroadEncodeType();		//!< 广播视频编码格式,目前只能选WMV2
	m_strBroadDistinguish = item.GetBroadDistinguish();		//!< 广播分辨率,160*120,176*144,......,720*480
	m_strBroadFPS = item.GetBroadFPS();						//!< 广播帧速率29.97,15,10,5,1,0.5 fps
	return *this;
}

//!< 判断本变量是否与item属性一样
bool CCamera::operator == (CCamera& item) const
{
	if(m_strTag != item.GetTag())							return false;	//!< 说明
	if(m_bRecord != item.IsRecord())						return false;	//!< 是否启动录制功能
	//if(m_bAutoSave != item.IsAutoSave())					return false;	//!< 是否自动保存视频文件
	if(m_strFilePath != item.GetFilePath())					return false;	//!< 视频文件存储位置,最后带'\',默认我的文档
	if(m_strEncodeType != item.GetEncodeType())				return false;	//!< 视频编码格式,目前只能选WMV2
	if(m_strDistinguish != item.GetDistinguish())			return false;	//!< 录制分辨率,160*120,176*144,......,720*480,默认320*240
	if(m_strFPS != item.GetFPS())							return false;	//!< 录制帧速率29.97,15,10,5,1,0.5 fps,默认1fps
	if(m_bBroadCast != item.IsBroadCast())					return false;	//!< 是否启动网络广播
	if(m_nPort != item.GetPort())							return false;	//!< 广播端口号,8080
	if(m_nMaxClient != item.GetMaxClient())					return false;	//!< 最大连接数,默认5,范围0~50
	if(m_strBroadEncodeType != item.GetBroadEncodeType())	return false;	//!< 广播视频编码格式,目前只能选WMV2
	if(m_strBroadDistinguish != item.GetBroadDistinguish())	return false;	//!< 广播分辨率,160*120,176*144,......,720*480
	if(m_strBroadFPS != item.GetBroadFPS())					return false;	//!< 广播帧速率29.97,15,10,5,1,0.5 fps
	return true;
}
