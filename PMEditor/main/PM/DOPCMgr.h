#pragma once
#include "PropertyDOPC.h"

namespace Servers{
namespace DOPC{
class CDOPCMgr
{
private:
	bool m_bModify;					//!< 是否被修改
	bool m_bActive;					//!< 是否启用，默认不启动
	CPropertyDOPC m_DopcProperty;	//!< OPC服务器配置对象

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
	bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< 保存xml格式的编辑信息
	void OnClose();
	void setActived(bool actived);
};
}}