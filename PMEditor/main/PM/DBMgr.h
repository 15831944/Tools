#pragma once
#include "PropertyDB.h"

#define  DBM_CONFIG  7001      //!<DBM��������Ϣ 
namespace Servers{
namespace DB{
class CDBMgr
{
private:
	SYSTEMTIME m_EditTime;			//!< ����޸��¼�����ʼ�������ı���ʱ��
	bool m_bModify;					//!< �Ƿ��޸�
//	bool m_bActive;					//!< �Ƿ�����
	CString m_strPassWord;			//!< ���ݿ�����
	UINT m_uiDiskMin;				//!< ������С�����ռ�
	float m_fSaveTime;				//!< �洢ʱ����,��С0.01
	UINT m_uiSaveType;				//!< �洢��ʽ��0ֱ�Ӵ洢��1�仯�ٴ洢����ǰ��ƽ��ֵ�洢��
	//UINT m_uiSaveAverage;			//!< ƽ��ֵ����
	UINT m_uiChangeRate;			//!< �仯�ʣ���100��
	UINT m_uiTbRowMax;				//!< �ֱ����ޣ�Ĭ��100000����Сֵ100
	bool m_bBackUp;					//!< �����Ƿ�����
	CString m_strBackupPath;		//!< ����·��
	UINT m_uiBackupDay;				//!< ����ʱ��������
	UINT m_uiBackupHour;			//!< ����ʱ�䣬��
	CPropertyDB m_DbProperty;		//!< ���ݿ����ö���
	UINT 	m_nMySqlPort;						//!<���ݿ�ͨѶ�˿�
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
	UINT getTbRowMax(){ return m_uiTbRowMax; }				//!< �ֱ����ޣ�Ĭ��100000����Сֵ100
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
	bool SerializeXml(TiXmlElement* pNode, bool bRead);			//!< ����xml��ʽ�ı༭��Ϣ
	void OnClose();
	bool IsActive();					// �Ƿ���Ҫ�������ݿ�

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