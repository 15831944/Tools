#pragma once

namespace XmlInfo{
class CXmlParaInfo;
class CXmlBehavior;
class CXmlArea;
class CXmlProtocol;
class CXmlInterface;
class CXmlEnum;
class CXmlProgramInfo;
class CXmlDevice
{
public:
	bool m_bOpen;														//!< �豸�����ļ��Ƿ��Ѿ����򿪲�����
	bool m_bPara2User;													//!< �����Ƿ�¶���û�
	bool m_bProgrammable;												//!< �Ƿ���Զ�����
	UINT m_uiID;														//!< �豸�ı�ţ����������ļ��е�
	CString m_strCompany;												//!< ������˾����
	CString m_strPathName;												//!< �����ļ�·����������·��
	CString m_strVersion;												//!< �汾��
	UINT m_uiDevType;													//!< ���ͱ�ţ��豸�����еģ�Ӧ����m_uiID���
	UINT m_uiDevIcon;													//!< �豸ͼ��
	CString m_strName;													//!< ��������
	UINT m_uiInf2Main;													//!< ֧���������豸��ͨ�Žӿڣ�1��̫����2���ڣ�4ZigBee��֧�֡���λ�򡱲���
	UINT m_uiInf2Slave;													//!< ֧�����Ӵ��豸��ͨ�Žӿڣ�1��̫����2���ڣ�4ZigBee��֧�֡���λ�򡱲���
	UINT m_uiMaxInf2Slave;												//!< ֧��������Ӷ��ٸ����豸

	std::vector<boost::shared_ptr<CXmlParaInfo> > m_vtPara;				//!< �����б�
	std::vector<boost::shared_ptr<CXmlBehavior> > m_vtBehavior;			//!< ��Ϊ�б�
	std::vector<boost::shared_ptr<CXmlArea> > m_vtArea;					//!< ���б�
	std::vector<boost::shared_ptr<CXmlProtocol> > m_vtProtocol;			//!< Э���б�
	std::vector<boost::shared_ptr<CXmlInterface> > m_vtInterface;		//!< �ӿ��б�
	std::vector<boost::shared_ptr<CXmlEnum> > m_vtEnum;					//!< ö���б�
	std::vector<boost::shared_ptr<CXmlProgramInfo> > m_vtProgram;		//!< ���ܿ���Ϣ����

public:
	UINT getID(){return m_uiID;}										//!< ����豸���
	UINT getIcon(){return m_uiDevIcon;}									//!< �豸��ͼ��
	CString getCompany(){return m_strCompany;}							//!< ���������˾����
	CString getPathName(){return m_strPathName;}						//!< ��������ļ�·��
	CString getVersion(){return m_strVersion;}							//!< ��ð汾��
	UINT getDevType(){return m_uiDevType;}								//!< ����豸����
	CString getName(){return m_strName;}								//!< ����豸����
	UINT getInf2Main(){return m_uiInf2Main;}							//!< ����������豸��ͨ�Žӿڣ�1��̫����2���ڣ�4������֧�֡���λ�򡱲���
	UINT getInf2Slave(){return m_uiInf2Slave;}							//!< ������Ӵ��豸��ͨ�Žӿڣ�1��̫����2���ڣ�4������֧�֡���λ�򡱲���
	UINT getMaxInf2Slave(){return m_uiMaxInf2Slave;}					//!< ���������Ӷ��ٸ����豸

	bool IsPara2User(){return m_bPara2User;}							//!< �Ƿ���Խ�������¶���û�
	bool IsSupportEthernet(){return bool((m_uiInf2Main & 1) || (m_uiInf2Main & 0x20));}	//!< �Ƿ�֧����̫��
	bool IsSupportSerial(){return bool(m_uiInf2Main & 2);}				//!< �Ƿ�֧�ִ���
	bool IsProgrammable(){return m_bProgrammable;}						//!< �Ƿ���Զ�����

public:
	CXmlDevice(void);
	~CXmlDevice(void);

	boost::shared_ptr<CXmlArea> getArea(UINT id);
	boost::shared_ptr<CXmlArea> getArea(CString name);
	boost::shared_ptr<CXmlArea> GetAreaNameList(std::list<CString>& strList, int& defIndex, bool bSrot);//!< ��������б�defIndex������ʱ��ʾareaID������ȥʱ��ʾ�б��Ĭ������������Ĭ����
	int GetAreaID(CString name);

	boost::shared_ptr<CXmlBehavior> getBehavior(UINT id);
	std::list<boost::shared_ptr<CXmlBehavior> > getBehavior(CString name);
	CString GetAllBevID(CString strBev, bool bRead);					//!< ������и����Ƶ���Ϊ,��-�ָ�

	boost::shared_ptr<CXmlParaInfo> getPara(UINT id);

	boost::shared_ptr<CXmlProtocol> getProtocol(CString name);
	boost::shared_ptr<CXmlProtocol> getProtocol(UINT id);

	boost::shared_ptr<CXmlInterface> getInterface(UINT id);

	boost::shared_ptr<CXmlEnum> getEnum(UINT id);

	bool OpenXml();
	bool SerializeMgr(TiXmlElement* pNode, bool bRead);					//!< �����豸�Ĺ�����Ϣ
	bool SerializeXml(TiXmlElement* pNode);								//!< �����豸��Ϣ�����豸�����ļ��н���

private:
	template<class T>
	bool SerializeT(TiXmlElement* pNode, std::vector<boost::shared_ptr<T> >& vtObject, UINT maxNum, CString name);
	void SayError(CString text, UINT maxNum);							//!< �����ļ�����text�ǽ����Ľڵ�����maxNUM���������
	void OnCheck();														//!< У��һ��
};
}