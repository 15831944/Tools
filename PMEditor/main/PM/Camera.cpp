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
:m_nID(-1)								//!< ���
,m_bRecord(true)						//!< �Ƿ�����¼�ƹ���
//,m_bAutoSave(true)						//!< �Ƿ��Զ�������Ƶ�ļ�
,m_strEncodeType(_T("WMV2"))			//!< ��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
,m_strDistinguish(_T("320*240"))		//!< ¼�Ʒֱ���,160*120,176*144,......,720*480
,m_strFPS(_T("1fps"))					//!< ¼��֡����29.97,15,10,5,1,0.5 fps,Ĭ��1fps
,m_bBroadCast(true)						//!< �Ƿ���������㲥
,m_nMaxClient(5)						//!< ���������,Ĭ��5,��Χ0~50
,m_strBroadEncodeType(_T("WMV2"))		//!< �㲥��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
,m_strBroadDistinguish(_T("320*240"))	//!< �㲥�ֱ���,160*120,176*144,......,720*480
,m_strBroadFPS(_T("15fps"))				//!< �㲥֡����29.97,15,10,5,1,0.5 fps
{
	m_strFilePath = CGbl::GetSpecialPath(CSIDL_PERSONAL);
	m_nPort = CCamMgr::GetMe().GetDifferentPort(8080);			//!< �㲥�˿ں�,Ĭ��8080
}

CCamera::~CCamera(void)
{
}

//!< ���
void CCamera::SetID(int id)
{
	m_nID = id;
	SetModify();
}

//!< ����
void CCamera::SetTag(CString strTag)
{
	if(m_strTag == strTag)					return;
	m_strTag = strTag;
	SetModify();
}

//!< �Ƿ�����¼�ƹ���
void CCamera::SetRecord(bool bRecord)
{
	if(m_bRecord == bRecord)				return;
	m_bRecord = bRecord;
	SetModify();
}

//!< �Ƿ��Զ�������Ƶ�ļ�
//void CCamera::SetAutoSave(bool bAutoSave){m_bAutoSave = bAutoSave; SetModify();}

//!< ��Ƶ�ļ��洢λ��,����'\',Ĭ���ҵ��ĵ�
void CCamera::SetFilePath(CString strFilePath)
{
	if(m_strFilePath == strFilePath)		return;
	m_strFilePath = strFilePath;
	SetModify();
}

//!< ��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
void CCamera::SetEncodeType(CString strEncodeType)
{
	if(m_strEncodeType == strEncodeType)	return;
	m_strEncodeType = strEncodeType;
	SetModify();
}

//!< ¼�Ʒֱ���,160*120,176*144,......,720*480,Ĭ��320*240
void CCamera::SetDistinguish(CString strDistinguish)
{
	if(m_strDistinguish == strDistinguish)	return;
	m_strDistinguish = strDistinguish;
	SetModify();
}

//!< ¼��֡����29.97,15,10,5,1,0.5 fps,Ĭ��1fps
void CCamera::SetFPS(CString strFPS)
{
	if(m_strFPS == strFPS)					return;
	m_strFPS = strFPS;
	SetModify();
}

//!< �Ƿ���������㲥
void CCamera::SetBroadCast(bool bBroadCast)
{
	if(m_bBroadCast  == bBroadCast)			return;
	m_bBroadCast = bBroadCast;
	SetModify();
}

//!< �㲥�˿ں�,8080
void CCamera::SetPort(int nPort)
{
	if(nPort < 0)							nPort = 0;
	else if(nPort > 65535)					nPort = 65535;
	if(m_nPort == nPort)					return;
	m_nPort = nPort;
	SetModify();
}

//!< ���������,Ĭ��5,��Χ0~50
void CCamera::SetMaxClient(int nMaxClient)
{
	if(nMaxClient < 5)						nMaxClient = 5;
	else if(nMaxClient > 50)				nMaxClient = 50;
	if(m_nMaxClient == nMaxClient)			return;
	m_nMaxClient = nMaxClient;
	SetModify();
}

//!< �㲥��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
void CCamera::SetBroadEncodeType(CString strBroadEncodeType)
{
	if(m_strBroadEncodeType == strBroadEncodeType)		return;
	m_strBroadEncodeType = strBroadEncodeType;
	SetModify();
}

//!< �㲥�ֱ���,160*120,176*144,......,720*480
void CCamera::SetBroadDistinguish(CString strBroadDistinguish)
{
	if(m_strBroadDistinguish == strBroadDistinguish)	return;
	m_strBroadDistinguish = strBroadDistinguish;
	SetModify();
}

//!< �㲥֡����29.97,15,10,5,1,0.5 fps
void CCamera::SetBroadFPS(CString strBroadFPS)
{
	if(m_strBroadFPS == strBroadFPS)					return;
	m_strBroadFPS = strBroadFPS;
	SetModify();
}

//!< ����Ϊ�ı�
void CCamera::SetModify()
{
	CCamMgr::GetMe().SetModify(true);
}


//!< ����xml��ʽ�ı༭��Ϣ,iExport=false����
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

//!< ���ұ�������Ҫƥ����ı���ȫ�ַ�ƥ�䣬��Сдƥ��
bool CCamera::DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex /* = false */)
{
	CString strInfo;
	bool bMatch = false;
	int nMatchCount = 0;
	CComVariant cvr;
	strInfo.Format("%d", m_nID);
	strInfo = _T("Information:����ͷ") + strInfo + _T(" �����") + strInfo + _T("��  ����  ");

	//!< ����Ƿ�ƥ��
	cvr = m_nID;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("���"), strInfo))		++nMatchCount;

	//!< �����Ƿ�ƥ��
	cvr = m_strTag;
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("˵��"), strInfo))		++nMatchCount;

	//!< ��ַ�Ƿ�ƥ��
	cvr = GetPort();
	if(CGbl::SearchT(cvr, str, bMatchWhole, bAllCase, bRegex, _T("�˿�"), strInfo))		++nMatchCount;

	if(nMatchCount > 0){
		CGbl::PrintOut(strInfo);
		return true;
	}
	return false;
}

//!< ��ֵ����������ID������Ҳȫ������ȥ��
CCamera& CCamera::operator = (CCamera& item)
{
	m_nID = item.GetID();
	m_strTag = item.GetTag();								//!< ˵��
	m_bRecord = item.IsRecord();							//!< �Ƿ�����¼�ƹ���
	//m_bAutoSave = item.IsAutoSave();						//!< �Ƿ��Զ�������Ƶ�ļ�
	m_strFilePath = item.GetFilePath();						//!< ��Ƶ�ļ��洢λ��,����'\',Ĭ���ҵ��ĵ�
	m_strEncodeType = item.GetEncodeType();					//!< ��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
	m_strDistinguish = item.GetDistinguish();				//!< ¼�Ʒֱ���,160*120,176*144,......,720*480,Ĭ��320*240
	m_strFPS = item.GetFPS();								//!< ¼��֡����29.97,15,10,5,1,0.5 fps,Ĭ��1fps
	m_bBroadCast = item.IsBroadCast();						//!< �Ƿ���������㲥
	m_nPort = item.GetPort();								//!< �㲥�˿ں�,8080
	m_nMaxClient = item.GetMaxClient();						//!< ���������,Ĭ��5,��Χ0~50
	m_strBroadEncodeType = item.GetBroadEncodeType();		//!< �㲥��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
	m_strBroadDistinguish = item.GetBroadDistinguish();		//!< �㲥�ֱ���,160*120,176*144,......,720*480
	m_strBroadFPS = item.GetBroadFPS();						//!< �㲥֡����29.97,15,10,5,1,0.5 fps
	return *this;
}

//!< �жϱ������Ƿ���item����һ��
bool CCamera::operator == (CCamera& item) const
{
	if(m_strTag != item.GetTag())							return false;	//!< ˵��
	if(m_bRecord != item.IsRecord())						return false;	//!< �Ƿ�����¼�ƹ���
	//if(m_bAutoSave != item.IsAutoSave())					return false;	//!< �Ƿ��Զ�������Ƶ�ļ�
	if(m_strFilePath != item.GetFilePath())					return false;	//!< ��Ƶ�ļ��洢λ��,����'\',Ĭ���ҵ��ĵ�
	if(m_strEncodeType != item.GetEncodeType())				return false;	//!< ��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
	if(m_strDistinguish != item.GetDistinguish())			return false;	//!< ¼�Ʒֱ���,160*120,176*144,......,720*480,Ĭ��320*240
	if(m_strFPS != item.GetFPS())							return false;	//!< ¼��֡����29.97,15,10,5,1,0.5 fps,Ĭ��1fps
	if(m_bBroadCast != item.IsBroadCast())					return false;	//!< �Ƿ���������㲥
	if(m_nPort != item.GetPort())							return false;	//!< �㲥�˿ں�,8080
	if(m_nMaxClient != item.GetMaxClient())					return false;	//!< ���������,Ĭ��5,��Χ0~50
	if(m_strBroadEncodeType != item.GetBroadEncodeType())	return false;	//!< �㲥��Ƶ�����ʽ,Ŀǰֻ��ѡWMV2
	if(m_strBroadDistinguish != item.GetBroadDistinguish())	return false;	//!< �㲥�ֱ���,160*120,176*144,......,720*480
	if(m_strBroadFPS != item.GetBroadFPS())					return false;	//!< �㲥֡����29.97,15,10,5,1,0.5 fps
	return true;
}
