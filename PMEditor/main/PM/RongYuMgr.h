#pragma once
#include "PropertyRongYu.h"

namespace Servers{
namespace RongYu{
class CRongYuMgr
{
private:
	bool m_bModify;						// 是否被修改
	bool m_bActive;						// 是否使能
	int m_nType;						// 通信类型,0UDP,1TCP
	int m_nPort;						// 端口号
	WORD m_nSn;							// 验证号
	CPropertyRongYu m_RongYuProperty;	// 冗余配置对象

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
	void OnClose(){m_bActive = false;}							//!< 是否启用

public:
	void OnCreate();
	void OnSetRongYu();			// 配置冗余属性
	bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< 保存xml格式的编辑信息
	void SetActived(bool actived);
	void SetType(int type);
	void SetPort(int port);
	void SetSN(WORD sn);
};
}}