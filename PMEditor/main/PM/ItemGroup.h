#pragma once

namespace MVC{
namespace Item{
class CItem;
class CItemDoc;
class CItemGroup
{
private:
	UINT m_uiID;								//!< ����
	CString m_strName;							//!< ������
	UINT m_uiParentID;							//!< �����ID��

public:
	CItemDoc* m_ItemDoc;						//!< ��ʾ�������ĵ�

public:
	UINT getID(){return m_uiID;}
	CString getName(){return m_strName;}
	UINT getParentID(){return m_uiParentID;}
	void setID(UINT id){m_uiID = id;}
	void setParentID(UINT id){m_uiParentID = id;}
	void setName(CString name){m_strName = name;}

public:
	CItemGroup(CString name, UINT parentid);
	~CItemGroup(void);

	bool Serialize(CArchive& ar);					//!< �����������Ϣ
	bool SerializeXml(TiXmlElement* pNode, bool bRead);		//!< ����xml��ʽ�ı༭��Ϣ

	std::list<UINT> getGroupIDList();				//!< ��������б�
	std::list<UINT> getMyItem();					//!< ����ӱ����б�
	void InitDefault();								//!< ��ʼ����Ĭ���飬����System��
	bool IsChildGroup(UINT id, bool incMe = false);	//!< �жϴ˱������Ƿ����Լ������飬���ٲ㶼�У�incMe�Ƿ�����Լ�
//	void OnDelete(bool silenced=true);				//!< ��ɾ���Լ�ʱ�������ʾ��Ĭ�Ͳ���ʾ�û�ֱ��ɾ��
};
}
}