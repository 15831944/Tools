#pragma once
#include "PropertyDB.h"

#define  DBM_CONFIG  7001      //!<DBM的配置信息 
namespace Servers{
namespace DB{
class CDBMgr
{
private:
	SYSTEMTIME m_EditTime;			//!< 最后修改事件，起始就是最后的保存时间
	bool m_bModify;					//!< 是否被修改
//	bool m_bActive;					//!< 是否启用
	CString m_strPassWord;			//!< 数据库密码
	UINT m_uiDiskMin;				//!< 磁盘最小报警空间
	float m_fSaveTime;				//!< 存储时间间隔,最小0.01
	UINT m_uiSaveType;				//!< 存储方式，0直接存储，1变化再存储（以前是平均值存储）
	//UINT m_uiSaveAverage;			//!< 平均值次数
	UINT m_uiChangeRate;			//!< 变化率，乘100了
	UINT m_uiTbRowMax;				//!< 分表上限，默认100000，最小值100
	bool m_bBackUp;					//!< 备份是否启用
	CString m_strBackupPath;		//!< 备份路径
	UINT m_uiBackupDay;				//!< 备份时间间隔，天
	UINT m_uiBackupHour;			//!< 备份时间，点
	CPropertyDB m_DbProperty;		//!< 数据库配置对象
	UINT 	m_nMySqlPort;						//!<数据库通讯端口
private:
	CDBMgr(void);
	~CDBMgr(void);

public:
	bool IsModify(){return m_bModify;}
	CString getPassWord(){return m_strPassWord;}
	UINT getDiskMin(){return m_uiDiskMin;}
	float getSaveTime(){return m_fSaveTime;}
	UINT getSaveType(){return m_uiSaveType;}
	//UINT getSaveAverage(){return m_uiSaveAverage;}
	UINT getChangeRate(){ return m_uiChangeRate; }
	UINT getTbRowMax(){ return m_uiTbRowMax; }				//!< 分表上限，默认100000，最小值100
	CString getBackupPath(){return m_strBackupPath;}
	UINT getBackupDay(){return m_uiBackupDay;}
	UINT getBackupHour(){return m_uiBackupHour;}
	UINT   getMySqlPort(){return m_nMySqlPort;}
	bool IsBackUp(){return m_bBackUp;}

	void SetModify(){m_bModify = true;}
     
	bool IsNeed(UINT dwData);   
	void OnReceive(COPYDATASTRUCT* pCopyDataStruct); 
	void InitStateXml(COPYDATASTRUCT& pCopyDataStruct);
public:
	static CDBMgr& GetMe();
	CString GetEditTime();
	void SetEditTime(CString stime);
	void OnCreate();
	void OnSetDB();
	bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< 保存xml格式的编辑信息
	void OnClose();
	bool IsActive();					// 是否需要激活数据库

//	void setActived(bool actived);
	void setPassWord(CString passWord);
	void setDiskMin(UINT deskMin);
	void setSaveTime(float time);
	void setSaveType(UINT type);
	//void setSaveAverage(UINT ave);
	void setChangeRate(UINT rate);
	void setTbRowMax(UINT size);
	void setBackupPath(CString path);
	void setBackupDay(UINT day);
	void setBackupHour(UINT hour);
	void setBackUp(bool backUp = true);
	void setMySqlPort(UINT port);
};
}}