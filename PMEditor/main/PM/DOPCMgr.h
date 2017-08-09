#pragma once
#include "PropertyDOPC.h"

namespace Servers{
namespace DOPC{
class CDOPCMgr
{
private:
	bool m_bModify;					//!< �Ƿ��޸�
	bool m_bActive;					//!< �Ƿ����ã�Ĭ�ϲ�����
	CPropertyDOPC m_DopcProperty;	//!< OPC���������ö���

private:
	CDOPCMgr(void);
	~CDOPCMgr(void);

public:
	bool IsModify(){return m_bModify;}
	bool IsActive(){return m_bActive;}

	void SetModify(){m_bModify = true;}

public:
	static CDOPCMgr& GetMe();
	void OnCreate();
	void OnSetDOPC();
	bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< ����xml��ʽ�ı༭��Ϣ
	void OnClose();
	void setActived(bool actived);
};
}}