#pragma once

namespace XmlInfo{class CXmlParaInfo;}
namespace MVC{namespace Device{
class CDeviceOne;
class CDeviceParam
{
public:
	vector<CComVariant> m_vtValue;		//!< ��Ӧֵ�����飬�±���Ƕ�Ӧ��Ϊ������ֵ��������豸���;�1��������16������ֵ

private:
	UINT m_uiParaID;			//!< ���
	CString m_strName;			//!< �������ƣ���ʵûʲô��
	UINT m_uiType;				//!< ��������
	boost::shared_ptr<XmlInfo::CXmlParaInfo> m_spXmlParam;	//!< 

private:
	void setName(CString name){m_strName = name;}
	void setType(UINT type){m_uiType = type;}

public:
	UINT getParaID(){return m_uiParaID;}
	CString getName(){return m_strName;}
	UINT getType(){return m_uiType;}
	boost::shared_ptr<XmlInfo::CXmlParaInfo> getXmlPara(){return m_spXmlParam;}
	CDeviceParam& operator = (CDeviceParam& para);

public:
	CDeviceParam(void);
	~CDeviceParam(void);

	void InitParaType(boost::shared_ptr<XmlInfo::CXmlParaInfo> xmlPara, UINT groupNum=1);	//!< �½�����ʱҪ��ʼ������
	void SetMaxSize(UINT size);					//!< ���ò�����ֵ�����������ԭ��С�Ͳ�����
	bool SerializeXml(TiXmlElement* pNode, bool bRead, CDeviceOne* dev);
	CComVariant getValue(UINT index);
	void setValue(CComVariant cv, UINT index);
	void ChangeType(UINT vt);
	

private:
	void setParaID(UINT paraID, CDeviceOne* projDev);						//!< ���ò���
};
}}