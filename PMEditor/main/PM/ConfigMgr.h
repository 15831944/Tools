#pragma once

namespace Servers{
namespace Config{
class CConfig;
class CConfigMgr
{
public:
	std::list<std::shared_ptr<CConfig> > m_ltConfig;

public:
	static CConfigMgr& GetMe(){static CConfigMgr one; return one;}

private:
	CConfigMgr(void){;}
	~CConfigMgr(void){;}

public:
	bool AddConfig(int id);									//!< 添加一个Config,参数是设备ID
	std::shared_ptr<CConfig> GetConfig(const int& id);	//!< 获得一个Config
	int GetConfigOpenCount();								//!< 获得被打开的DConfig数量
};
}}