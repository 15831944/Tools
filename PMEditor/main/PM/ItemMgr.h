#pragma once
//#include "DataStruct.h"

#include "ItemProperty.h"
#include "PropertyItems.h"

namespace MVC{
namespace Item{
class CItemGroup;
class CItem;
class CScriptItem;
class CItemDoc;

class CItemMgr
{
public:
	std::vector< std::shared_ptr<CItem> > m_vtItem;				//!< ��������
	std::map< CString, std::shared_ptr<CItem> > m_mpItem;			//!< ������,����������ڿ��ټ���
	std::list<std::shared_ptr<CItem> > m_ltItemClipBoard;			//!< �����ļ�����
	std::vector< std::shared_ptr<CItemGroup> > m_vtItemGroup;		//!< ����������
	std::list<UINT> m_ltEditItemID;									//!< ��ǰ�ոձ��޸ĵı���ID�б�
	CItemDoc* m_pItemDoc;
	CPropertyItem m_ItemProperty;									//!< ����������
	CPropertyItems m_ItemPropertys;									//!< ��������Ĺ�������

private:
	SYSTEMTIME m_EditTime;		//!< ����޸ĵ��¼�
	bool m_bModify;				//!< �Ƿ��޸�
	CString m_strName;
	CString m_strFileName;
	CString m_strVersion;

public:
	bool IsModify(){return m_bModify;}
	CString getName(){return m_strName;}
	CString getFileName(){return m_strFileName;}
	CString getVersion(){return m_strVersion;}

	void SetModify(){m_bModify = true;}

private:
	CItemMgr(void);
	~CItemMgr(void);

	bool Serialize(CArchive& ar);									//!< �����������Ϣ
	bool SerializeXmlItem(TiXmlElement* pNode, bool bRead);				//!< ����xml��ʽ�ı༭��Ϣ
	bool SerializeOut(TiXmlElement* pNode, std::list<UINT> ltItem);	//!< ����xml��ʽ�ı��������ļ�

public:
	static CItemMgr& GetMe();

	CString GetEditTime();
	void OnCreate();
	void OnClose();
	bool OpenItemFile(CString name, CString pathall, CString ver, CString stime);	//!< �򿪱������ļ������� �� ���� + ·�� + �汾�ţ�����޸�ʱ��
	void OpenDoc();
	void SaveItemFile();												//!< �����ļ�
	void OutItem(std::list<UINT> ltItem);							//!< ��������

	std::list<UINT> GetItemIDAll();
	std::list<UINT> GetItemIDMem();
	std::list<UINT> GetItemIDIO();
	std::shared_ptr<CItem> GetItem(ULONG id);					//!< ���id�ű���
	std::shared_ptr<CItem> GetItem(CString name);				//!< ������ֵ���name�ĵ�һ������
	std::shared_ptr<CItem> GetItemFast(CString name);			//!< ��ñ���,�ÿ��ٵĻ�÷�ʽ
	std::shared_ptr<CItem> GetFirstItem();					//!< ��õ�һ������
	std::shared_ptr<CItem> GetLastItem();						//!< ������һ������
	std::shared_ptr<CItemGroup> GetGroup(UINT id);			//!< ���id�ű�����
	std::shared_ptr<CItemGroup> GetGroup(CString name);		//!< �������Ϊname�ı����飬�����鲻������
	bool FindItem(ULONG id);									//!< ���ұ��Ϊid�ı���
	bool FindGroup(UINT id);									//!< ���ұ��Ϊid�ı�����
	bool AddGroup(std::shared_ptr<CItemGroup> group,UINT parentid=0);			//!< ��ӱ����飬��ʱ�����黹û��ID����������
	bool AddItem(std::shared_ptr<CItem> item,int maxID,UINT groupid);//!< ��ӱ���,��ʱ������û��ID�����ڱ�����������
	bool AddItemBack(std::shared_ptr<CItem> item, int maxID, UINT groupid);	//!< �ڱ�������������
//	bool AddItem(UINT id, std::shared_ptr<CItem> item, UINT groupid=0, UINT uiAsk=0);	//!< ��ӱ�����ָ��ID����������Ƿ�ѯ�ʣ�0�ʣ�1���ʸ��ǣ�2������
	void RemoveItemBackEmpty();									//!< ɾ����β�Ŀձ���
	void DeleteItem(ULONG id);									//!< ɾ�����Ϊid�ı���
	void RemoveGroup(UINT groupid);								//!< �Ƴ�������
	UINT GetItemSize();											//!< ��ñ�������ʵ����
	UINT GetGroupSize();										//!< ��ñ��������ʵ����
	void ExChangeItem(UINT id1, UINT id2);						//!< ��������������
	void WriteItemValue(UINT id, bool proj, CString value);		//!< д��ĳ��������ֵ����Ҫͨ�ţ�proj��ʾ�Ƿ���д����ֵ
	void SetItemWatch(const bool watch);						//!< �ı���״̬
	void ReadItemValue(bool bProj);								//!< �ӷ������ж�ȡ����ֵ��true��ʾ������ֵ��false��ʾ��IOֵ
	void ShowItem(UINT id);										//!< ��ʾ�����Ϊid�ı�����idΪ-1��ʾ��ʾ����һ����
	int SearchItem(CString str, bool bMatchCase, bool bMatchWhole, bool bRegex = false);	//!< ���ҷ��ϸ��ַ����ı���������ƥ�������
	CString GetSimilarName(CString name);						//!< ���һ����������֣�����Ҳ���ԣ�����֤û�б��Ժͱ���ʹ�õ�

	void AddEditItem(UINT id);									//!< ��Ӹոձ��޸ĵı���
	void SetEditEnd();											//!< �����������������
	void FreshAllGrid();										//!< ˢ�����б����ʾ
};
}
}
