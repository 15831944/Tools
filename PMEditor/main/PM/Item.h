#pragma once

namespace MVC{
namespace Item{
class CItemGroup;
class CPropertySource;
class CItem
{
public:
	enum{
		SRC_TYPE_MEM = 0,
		SRC_TYPE_IO = 1
	};

private:
	UINT m_uiID;									// ����ID��
	CString m_strName;								// ��������
	CString m_strTag;								// ������ǩ
	CString m_strValue;								// �ַ�����ʽ�洢�ı���ֵ
	CString m_strIOValue;							// �ַ�����ʽ�洢�ı���ԭʼֵ
	CString m_strDescription;						// �����ı�ע��Ϣ
	CComVariant m_cvrDefaultValue;					// ����Ĭ��ֵ
	CComVariant m_cvrValue;							// ����ֵ
	CComVariant m_cvrValueOld;						// ����ֵ�������ղŵ�ֵ������Ҫ����һ��
	CComVariant m_cvrIOValue;						// ����ԭʼֵ
	CComVariant m_cvrIOValueOld;					// ����ԭʼֵ�������ղŵ�ԭʼֵ������Ҫ����һ��
	UINT m_uiGroupID;								// ���������������ID��
	UINT m_uiValType;								// �������ͣ�0-bit��1-char��2-byte��3-short��4-word��5-int��6-dword��7-float��8-double
	UINT m_uiSrcType;								// ����Դ���ͣ�0�ڴ������1I/O����
	UINT m_uiAccessRight;							// ����Ȩ�ޣ�0�ɶ���д��1ֻ����2ֻд��3�޷�����
	BOOL m_bReservFlag;								// �Ƿ����óɱ���ֵ��true���ã�false������
	BOOL m_bReservDB;								// �Ƿ񱣴���ʷ����
	std::shared_ptr<CPropertySource> m_spSrcInfo;	// Դ����
	SYSTEMTIME m_stUpdateTime;						// ���һ���޸�ʱ��
	SYSTEMTIME m_stCreateTime;						// ����ʱ��

	UINT m_uiIRefCount;								// ���ü��������Ǳ�ʹ�õĴ���

public:
	UINT getID(){return m_uiID;}										// ��ñ���ID���
	CString getName(){return m_strName;}								// ��ñ�������
	CString getTag(){return m_strTag;}									// ��ñ�����ǩ
	CString getDescription(){return m_strDescription;}					// ��ñ�����ע��Ϣ
	CComVariant getDefault(){return m_cvrDefaultValue;}					// ��ñ�����Ĭ��ֵ
	CComVariant getValue(){return m_cvrValue;}							// ��ñ����ĵ�ǰֵ
	CComVariant getIOValue(){return m_cvrIOValue;}						// ��ñ����ĵ�ǰԭʼֵ
	CString getStrValue(bool ishex);															//!<��ñ������ַ���ʽ�ĵ�ǰֵ
	CString getStrIOValue(bool ishex);														//!<��ñ������ַ���ʽ�ĵ�ǰԭʼֵ
	CString GetFloatStr(float flt);													// ��ø����ַ���
	UINT getMyGroupID(){return m_uiGroupID;}							// ���������������ID��
	UINT getSrcType(){return m_uiSrcType;}								// ��ñ�������Դ���ͣ�0�ڴ棬1I/O
	UINT getValType(){return m_uiValType;}								// ���ֵ������
	UINT getAccessRight(){return m_uiAccessRight;}						// ��ñ����ķ������ͣ�0�ɶ���д��1ֻ����2ֻд
	BOOL getReservFlag(){return m_bReservFlag;}							// ��ñ����Ƿ�����Ϊ����ֵ
	BOOL getReservDB(){return m_bReservDB;}								// ��ñ����Ƿ񱣴���ʷ����
	std::shared_ptr<CPropertySource> getSrcInfo(){return m_spSrcInfo;}	// �������Դ���Զ��������ָ��
	SYSTEMTIME getCreateTime(){return m_stCreateTime;}					// ��ñ����Ĵ���ʱ��
	SYSTEMTIME getUpdateTime(){return m_stUpdateTime;}					// ��ñ������޸�ʱ��

	void setID(UINT id){m_uiID = id;}									// ���ñ�����ID���
	void setItemName(CString name){m_strName = name;}					// ���ñ���������
	void setTag(CString tag){m_strTag = tag;}							// ���ñ����ı�ǩ
	void setDescription(CString dsp){m_strDescription = dsp;}			// ���ñ�����ע��Ϣ
	void setDefault(CComVariant default_value){m_cvrDefaultValue=default_value;}// ���ñ�����Ĭ��ֵ
	void setMyGroupID(UINT id){m_uiGroupID = id;}						// ���������������ID��
	void setSrcType(UINT type){m_uiSrcType = type;}						// ���ñ���������Դ����
	void setAccessRight(UINT type){m_uiAccessRight = type;}				// ���ñ����ķ���Ȩ��
	void setReservFlag(BOOL flag){m_bReservFlag = flag;}				// ���ñ����Ƿ���ֵ
	void setReservDB(BOOL flag){m_bReservDB = flag;}					// ���ñ����Ƿ񱣴���ʷ����
	void OnUpdateInfo(){GetLocalTime(&m_stUpdateTime);}					// ���ñ������޸ĺ���޸�ʱ��
	void setUpdateTime(SYSTEMTIME st){m_stUpdateTime=st;}				// ���ø���ʱ��
	void setCreateTime(SYSTEMTIME st){m_stCreateTime=st;}				// ���ô���ʱ��
	void setValue(CComVariant value){m_cvrValue=value;}					// ���ñ����ĵ�ǰֵ
	void setIOValue(CComVariant value){m_cvrIOValue = value;}			// ���ñ����ĵ�ǰԭʼֵ

	void OnAdd(){m_uiIRefCount++;}							// ���ص����У����������������
	void OnDeleteFGroup(){/*��*/;}							// ������ȥ���������ü�������ʱ������ɾ��
	void OnDelete(){;}										// ����ɾ��һ�����������������ü������������б����飬ȥ�����ڸñ����ļ�¼
	UINT getRefCount(){return m_uiIRefCount;}				// ������ü���

public:
	CItem(CString name);									// 
	~CItem(void);
	void setValType(UINT type);								// ���ñ�����ֵ����
	void setValType(CString strType);						// ���ñ�����ֵ����
	void SetVarType();										// ȷ�ϱ���������
	void ChangeVarType(UINT type);							// ����valType��������ͣ�����������variant���͵�����ת��������Ӧ������

	bool Serialize(CArchive& ar);							// �����������Ϣ
	bool SerializeXml(TiXmlElement* pNode, bool bRead, bool iExport = false);	// ����xml��ʽ�ı༭��Ϣ,iExport=false������true���뵼��
	bool ReadFromConfig(CString strLine, int devID);		// ��Config�����ı������ж�ȡ����
	bool ReadFromPM(CString strLine, int devID);			// ��PM�����ı������ж�ȡ����

	CItem& operator = (CItem& item);						// ��ֵ����������ID������Ҳȫ������ȥ��
	bool operator == (CItem& item) const;					// �жϱ������Ƿ���item����һ��
	bool OnCloneMe(CItem& item, int index, int addrUnit, int nType = 0);	// Ҫ��¡һ���Լ���nType=0����������1С������
	CString GetCheckValue();								// ���һ������ֵ,�����ڴ�ű������
	CString GetValTypeStr();								// ���ַ�������ʽ���ع���ֵ����
	CString GetGroupName();									// ���ر��������������

	bool IsValueChange();									// �жϱ���ֵ�Ƿ�ı�
	bool IsIOValueChange();									// �жϱ���ԭʼֵ�Ƿ�ı�
	void WatchInit();										// ��س�ʼ�������״̬
	bool DoSearch(CString str, bool bMatchWhole, bool bAllCase, bool bRegex = false);	// ���ұ�������Ҫƥ����ı���ȫ�ַ�ƥ�䣬��Сдƥ��

private:
};
}
}