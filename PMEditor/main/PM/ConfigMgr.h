#pragma once

namespace Servers{
namespace Config{
class CConfig;
class CConfigMgr
{
public:
	std::list<boost::shared_ptr<CConfig> > m_ltConfig;

public:
	static CConfigMgr& GetMe(){static CConfigMgr one; return one;}

private:
	CConfigMgr(void){;}
	~CConfigMgr(void){;}

public:
	bool AddConfig(int id);									//!< ���һ��Config,�������豸ID
	boost::shared_ptr<CConfig> GetConfig(const int& id);	//!< ���һ��Config
	int GetConfigOpenCount();								//!< ��ñ��򿪵�DConfig����
};
}}