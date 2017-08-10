#pragma once

namespace MVC{
namespace Item{
class CItem;
class CPropertySource
{
private:
	CItem* m_pParent;				//!< �����ı���
	CString m_strScriptText;		//!< ��ֵ�ű��ı�

	UINT m_uiDeviceID;				//!< �豸���
	UINT m_uiAreaID;				//!< �����
	UINT m_uiUnitIndex;				//!< Ѱַƫ��
	UINT m_uiBitIndex;				//!< λƫ��
	UINT m_uiIOType;				//!< IO���ͣ��������ݿ������ļ�
	UINT m_uiFreshTime;				//!< ����ˢ��ʱ��,��λ1����
	BOOL m_bDelayFlag;				//!< �ɷ������ͺ�
	UINT m_uiConvertType;			//!< ת�����ͣ�0��ת����1�Ƿ�ȡ��(λ����ʹ��)��1����ת��
	CComVariant m_MinProjectValue;	//!< ��С����ֵ
	CComVariant m_MaxProjectValue;	//!< ��󹤳�ֵ
	CComVariant m_MinIOValue;		//!< ��СIOֵ
	CComVariant m_MaxIOValue;		//!< ���IOֵ

public:
	void setScriptText(CString text){m_strScriptText = text;}
	void setFreshTime(UINT fresh_time){if(fresh_time < 10) fresh_time = 10;	m_uiFreshTime = fresh_time;}
	void setDevID(UINT devid){m_uiDeviceID = devid;}
	void setAreaID(UINT areaid){m_uiAreaID = areaid;}
	void setUnitIndex(UINT index){m_uiUnitIndex = index;}
	void setIOType(UINT type){m_uiIOType = type;}
	void setBitIndex(UINT index){m_uiBitIndex = index;}
	void setDealyFlag(BOOL delay){m_bDelayFlag = delay;}
	void setConvertType(UINT type){m_uiConvertType = type; if(m_pParent->getValType() == 0)	m_uiConvertType = 0;}
	void setProjMax(CComVariant cvr){m_MaxProjectValue = cvr;}
	void setProjMin(CComVariant cvr){m_MinProjectValue = cvr;}
	void setIOMax(CComVariant cvr){m_MaxIOValue = cvr;}
	void setIOMin(CComVariant cvr){m_MinIOValue = cvr;}

	CItem* getItem(){return m_pParent;}
	CString getScriptText(){return m_strScriptText;}
	UINT getFreshTime(){return m_uiFreshTime;}
	UINT getDeviceID(){return m_uiDeviceID;}
	UINT getAreaID(){return m_uiAreaID;}
	UINT getUnitIndex(){return m_uiUnitIndex;}
	UINT getIOType(){return m_uiIOType;}
	UINT getBitIndex(){return m_uiBitIndex;}
	BOOL getDelayFlag(){return m_bDelayFlag;}
	UINT getConvertType(){return m_uiConvertType;}
	CComVariant getProjMax(){return m_MaxProjectValue;}
	CComVariant getProjMin(){return m_MinProjectValue;}
	CComVariant getIOMax(){return m_MaxIOValue;}
	CComVariant getIOMin(){return m_MinIOValue;}

private:
	int GetTypeFromStr(CString strType);				//!< �������ƻ������

public:
	CPropertySource(CItem* item);
	~CPropertySource(void);

	bool Serialize(CArchive& ar);						//!< �����������Ϣ
	bool SerializeXml(TiXmlElement* pNode, bool bRead, bool iExport = false);
	void ReadFromPMExcel(std::vector<CString>& vtCell);					//!< ��������Դ����
	bool SetConfigAddr(int devID, CString strAddr, CString strType);	//!< ����Config�ı�����ַ����

	UINT GetOperateType();								//!< ��õ�ǰIO����ֵ���ٸ���λѰַ����

	CPropertySource& operator = (CPropertySource& src);
	bool operator == (CPropertySource& src);
	CString GetAreaString();							//!< ���ظõ�ַ��Ӧ���ַ���
};
}
}