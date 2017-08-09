#pragma once

namespace MVC{
namespace Item{
class CItemGroup;
class CPropertySource;
class CPropertyAlarm;
class CItem
{
public:
	enum{
		SRC_TYPE_MEM = 0,
		SRC_TYPE_IO = 1
	};

private:
	UINT m_uiID;									//!< ����ID��
	CString m_strName;								//!< ��������
	CString m_strTag;								//!< ������ǩ
	CString m_strValue;								//!< �ַ�����ʽ�洢�ı���ֵ				
	CString m_strIOValue;							//!< �ַ�����ʽ�洢�ı���ԭʼֵ		
	CString m_strDescription;						//!< �����ı�ע��Ϣ
	CComVariant m_cvrDefaultValue;					//!< ����Ĭ��ֵ
	CComVariant m_cvrValue;							//!< ����ֵ
	CComVariant m_cvrValueOld;						//!< ����ֵ�������ղŵ�ֵ������Ҫ����һ��
	CComVariant m_cvrIOValue;						//!< ����ԭʼֵ
	CComVariant m_cvrIOValueOld;					//!< ����ԭʼֵ�������ղŵ�ԭʼֵ������Ҫ����һ��
	UINT m_uiGroupID;								//!< ���������������ID��
	UINT m_uiValType;								//!< �������ͣ�0-bit��1-char��2-byte��3-short��4-word��5-int��6-dword��7-float��8-double
	UINT m_uiSrcType;								//!< ����Դ���ͣ�0�ڴ������1I/O����
	UINT m_uiAccessRight;							//!< ����Ȩ�ޣ�0�ɶ���д��1ֻ����2ֻд��3�޷�����
	BOOL m_bReservFlag;								//!< �Ƿ����óɱ���ֵ��true���ã�false������
	BOOL m_bReservDB;								//!< �Ƿ񱣴���ʷ����
	BOOL m_bModbus485;								//!< �Ƿ񵼳�Modbus����,PAC������Modbus����������,��Ҫ�����������
	boost::shared_ptr<CPropertySource> m_spSrcInfo;	//!< Դ����
	boost::shared_ptr<CPropertyAlarm> m_spAlarmInfo;//!< ��������
	SYSTEMTIME m_stUpdateTime;						//!< ���һ���޸�ʱ��
	SYSTEMTIME m_stCreateTime;						//!< ����ʱ��

	UINT m_uiIRefCount;								//!< ���ü��������Ǳ�ʹ�õĴ���
	bool m_bAlarmBit;								//!< λ�����Ƿ񱨾�
	bool m_bAlarmLolo;								//!< �������Ƿ񱨾�
	bool m_bAlarmLow;								//!< �����Ƿ񱨾�
	bool m_bAlarmHigh;								//!< �����Ƿ񱨾�
	bool m_bAlarmHihi;								//!< �������Ƿ񱨾�
	bool m_bAlarmAim;								//!< Ŀ���Ƿ񱨾�
	bool m_bAlarmShift;								//!< �仯���Ƿ񱨾�

public:
	UINT getID(){return m_uiID;}										//!< ��ñ���ID���
	CString getName(){return m_strName;}								//!< ��ñ�������
	CString getTag(){return m_strTag;}									//!< ��ñ�����ǩ
	CString getDescription(){return m_strDescription;}					//!< ��ñ�����ע��Ϣ
	CComVariant getDefault(){return m_cvrDefaultValue;}					//!< ��ñ�����Ĭ��ֵ
	CComVariant getValue(){return m_cvrValue;}							//!< ��ñ����ĵ�ǰֵ
	CComVariant getIOValue(){return m_cvrIOValue;}						//!< ��ñ����ĵ�ǰԭʼֵ
	CString getStrValue(bool ishex);															//!<��ñ������ַ���ʽ�ĵ�ǰֵ
	CString getStrIOValue(bool ishex);														//!<��ñ������ַ���ʽ�ĵ�ǰԭʼֵ
	CString GetFloatStr(float flt);													//!< ��ø����ַ���
	UINT getMyGroupID(){return m_uiGroupID;}							//!< ���������������ID��
	UINT getSrcType(){return m_uiSrcType;}								//!< ��ñ�������Դ���ͣ�0�ڴ棬1I/O
	UINT getValType(){return m_uiValType;}								//!< ���ֵ������
	UINT getAccessRight(){return m_uiAccessRight;}						//!< ��ñ����ķ������ͣ�0�ɶ���д��1ֻ����2ֻд
	BOOL getReservFlag(){return m_bReservFlag;}							//!< ��ñ����Ƿ�����Ϊ����ֵ
	BOOL getReservDB(){return m_bReservDB;}								//!< ��ñ����Ƿ񱣴���ʷ����
	BOOL getModbus485(){return m_bModbus485;}							//!< �Ƿ񵼳�Modbus����
	boost::shared_ptr<CPropertySource> getSrcInfo(){return m_spSrcInfo;}	//!< �������Դ���Զ��������ָ��
	boost::shared_ptr<CPropertyAlarm> getAlarmInfo(){return m_spAlarmInfo;}	//!< ��ñ������Զ��������ָ��
	SYSTEMTIME getCreateTime(){return m_stCreateTime;}					//!< ��ñ����Ĵ���ʱ��
	SYSTEMTIME getUpdateTime(){return m_stUpdateTime;}					//!< ��ñ������޸�ʱ��

	bool IsAlarmBit(){return m_bAlarmBit;}								//!< �жϱ����Ƿ��ڱ���״̬
	bool IsAlarmLolo(){return m_bAlarmLolo;}							//!< �жϱ����Ƿ��ڱ���״̬
	bool IsAlarmLow(){return m_bAlarmLow;}								//!< �жϱ����Ƿ��ڱ���״̬
	bool IsAlarmHigh(){return m_bAlarmHigh;}							//!< �жϱ����Ƿ��ڱ���״̬
	bool IsAlarmHihi(){return m_bAlarmHihi;}							//!< �жϱ����Ƿ��ڱ���״̬
	bool IsAlarmAim(){return m_bAlarmAim;}								//!< �жϱ����Ƿ��ڱ���״̬
	bool IsAlarmShift(){return m_bAlarmShift;}							//!< �жϱ����Ƿ��ڱ���״̬

	void setID(UINT id){m_uiID = id;}									//!< ���ñ�����ID���
	void setItemName(CString name){m_strName = name;}					//!< ���ñ���������
	void setTag(CString tag){m_strTag = tag;}							//!< ���ñ����ı�ǩ
	void setDescription(CString dsp){m_strDescription = dsp;}			//!< ���ñ�����ע��Ϣ
	void setDefault(CComVariant default_value){m_cvrDefaultValue=default_value;}//!< ���ñ�����Ĭ��ֵ
	void setMyGroupID(UINT id){m_uiGroupID = id;}						//!< ���������������ID��
	void setSrcType(UINT type){m_uiSrcType = type;}						//!< ���ñ���������Դ����
	void setAccessRight(UINT type){m_uiAccessRight = type;}				//!< ���ñ����ķ���Ȩ��
	void setReservFlag(BOOL flag){m_bReservFlag = flag;}				//!< ���ñ����Ƿ���ֵ
	void setReservDB(BOOL flag){m_bReservDB = flag;}					//!< ���ñ����Ƿ񱣴���ʷ����
	void setModbus485(BOOL flag){m_bModbus485 = flag;}					//!< �Ƿ񵼳�Modbus����
	void OnUpdateInfo(){GetLocalTime(&m_stUpdateTime);}					//!< ���ñ������޸ĺ���޸�ʱ��
	void setUpdateTime(SYSTEMTIME st){m_stUpdateTime=st;}				//!< ���ø���ʱ��
	void setCreateTime(SYSTEMTIME st){m_stCreateTime=st;}				//!< ���ô���ʱ��
	void setValue(CComVariant value){m_cvrValue=value;}					//!< ���ñ����ĵ�ǰֵ
	void setIOValue(CComVariant value){m_cvrIOValue = value;}			//!< ���ñ����ĵ�ǰԭʼֵ

	void OnAdd(){m_uiIRefCount++;}							//!< ���ص����У����������������
	void OnDeleteFGroup(){/*��*/;}							//!< ������ȥ���������ü�������ʱ������ɾ��
	void OnDelete(){;}										//!< ����ɾ��һ�����������������ü������������б����飬ȥ�����ڸñ����ļ�¼
	UINT getRefCount(){return m_uiIRefCount;}				//!< ������ü���

public:
	CItem(CString name);									//!< 
	~CItem(void);
	void setValType(UINT type);								//!< ���ñ�����ֵ����
	void setValType(CString strType);						//!< ���ñ�����ֵ����
	void SetVarType();										//!< ȷ�ϱ���������
	void ChangeVarType(UINT type);							//!< ����valType��������ͣ�����������variant���͵�����ת��������Ӧ������

	bool Serialize(CArchive& ar);							//!< �����������Ϣ
	bool SerializeXml(TiXmlElement* pNode, bool bRead, bool iExport = false);	//!< ����xml��ʽ�ı༭��Ϣ,iExport=false������true���뵼��
	//bool ReadFromPlcConfig(COperation& reader, int nRow, int devID);			//!< ��PLC_Config�����ı������ж�ȡ����
	bool ReadFromPlcConfig(CString strLine, int devID);		//!< ��PLC_Config�����ı������ж�ȡ����
	//bool ReadFromDView(COperation& reader, int nRow, int devID);				//!< ��DView�����ı������ж�ȡ����
	bool ReadFromDView(CString strLine, int devID);			//!< ��DView�����ı������ж�ȡ����

	CItem& operator = (CItem& item);						//!< ��ֵ����������ID������Ҳȫ������ȥ��
	bool operator == (CItem& item) const;					//!< �жϱ������Ƿ���item����һ��
	bool OnCloneMe(CItem& item, int index, int addrUnit, int nType = 0);	//!< Ҫ��¡һ���Լ���nType=0����������1С������
	void OnCloneDealIOInfo(CItem& item, int addrUnit);
	CString GetCheckValue();								//!< ���һ������ֵ,�����ڴ�ű������
	CString GetValTypeStr();								//!< ���ַ�������ʽ���ع���ֵ����
	CString GetGroupName();									//!< ���ر��������������
	void SetAlarm(UINT type, bool alarm = false);			//!< ���ñ���
	bool IsAlarm();											//!< �ж��Ƿ��ڱ���״̬
	bool IsValueChange();									//!< �жϱ���ֵ�Ƿ�ı�
	bool IsIOValueChange();									//!< �жϱ���ԭʼֵ�Ƿ�ı�
	void WatchInit();										//!< ��س�ʼ�������״̬
	bool DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex = false);	//!< ���ұ�������Ҫƥ����ı���ȫ�ַ�ƥ�䣬��Сдƥ��

private:
};
}
}