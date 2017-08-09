#pragma once
#include "PropertyRongYu.h"

namespace Servers{
namespace RongYu{
class CRongYuMgr
{
private:
	bool m_bModify;						// �Ƿ��޸�
	bool m_bActive;						// �Ƿ�ʹ��
	int m_nType;						// ͨ������,0UDP,1TCP
	int m_nPort;						// �˿ں�
	WORD m_nSn;							// ��֤��
	CPropertyRongYu m_RongYuProperty;	// �������ö���

private:
	CRongYuMgr(void);
	~CRongYuMgr(void);

public:
	static CRongYuMgr& GetMe(){static CRongYuMgr one; return one;}
	bool IsModify(){return m_bModify;}
	bool IsActive(){return m_bActive;}
	int GetType(){return m_nType;}
	int GetPort(){return m_nPort;}
	WORD GetSN(){return m_nSn;}
	void SetModify(){m_bModify = true;}
	void OnClose(){m_bActive = false;}							//!< �Ƿ�����

public:
	void OnCreate();
	void OnSetRongYu();			// ������������
	bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< ����xml��ʽ�ı༭��Ϣ
	void SetActived(bool actived);
	void SetType(int type);
	void SetPort(int port);
	void SetSN(WORD sn);
};
}}