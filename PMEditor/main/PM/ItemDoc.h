#pragma once
#include "Item.h"

// CItemDoc �ĵ�
namespace Dialog{class CItemInConfigDlg;}
namespace MVC{
namespace Item{
class CItem;
class CItemGrid;
class CItemView;

typedef struct tagItemUndo
{
	std::shared_ptr<CItem> m_Item;		//!< ��¼������������ָ�룬ɾ����ʱ����
	UINT m_uiEditType;						//!< �������ͣ�0��ӣ�1ɾ����2�޸�
	UINT m_uiEditInfo;						//!< ������Ϣ�����ݱ���������Ĳ�ͬ���������Ҳ��ͬ
	bool m_bEnd;							//!< �Ƿ��ǵ��β����Ľ�β��һ�β������ԶԶ��������������һ����β��־

	tagItemUndo();
	tagItemUndo(UINT type, std::shared_ptr<CItem> item, UINT info);
}SItemUndo, *PSItemUndo;

class CItemDoc : public CDocument
{
	DECLARE_DYNCREATE(CItemDoc)
public:
	CItemDoc();
	virtual ~CItemDoc();

#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	UINT m_uiGroupID;						//!< ����ʾ�ı�����ID��
	std::stack< std::shared_ptr<SItemUndo> > m_stItemUndo;
	std::stack< std::shared_ptr<SItemUndo> > m_stItemRedo;

protected:
	void ClearUndo();
	void ClearRedo();
	void UndoAdd();
	void UndoDel();
// 	void UndoGup();
	void UndoUpd();
	void RedoAdd();
	void RedoDel();
// 	void RedoGup();
	void RedoUpd();

	CItemGrid* GetGrid();
	void RedoOne(std::shared_ptr<SItemUndo> redo);
	virtual BOOL OnNewDocument();

	bool AddItem(UINT id, std::shared_ptr<CItem> item, int maxID, UINT groupid=0, UINT uiAsk=0);	//!< ��ӱ�����ָ��ID����������Ƿ�ѯ�ʣ�0�ʣ�1���ʸ��ǣ�2������
	void InItem(std::list<std::shared_ptr<CItem> > ltItem, Dialog::CItemInConfigDlg* dlg);		//!< ��Ӷ������

	DECLARE_MESSAGE_MAP()
public:
	bool IsUndoEmpty(){return m_stItemUndo.empty();}
	bool IsRedoEmpty(){return m_stItemRedo.empty();}

public:
	CItemView* GetView();
	void SerializeIn(TiXmlElement* pNode);
	void SerializePlcConfig(CString strPath, CString strTitle, CString strExt);
	bool ItemInAdvanceOptions(std::list<std::shared_ptr<CItem> > ltItem);					//!< ���������ճ���ĸ߼�ѡ���
	void ItemInRenameItem(std::list<std::shared_ptr<CItem> > ltItem, UINT groupID = 0);	//!< ����������Զ������������ı���
	void ItemInDelFileItem(std::list<std::shared_ptr<CItem> > ltItem, UINT groupID = 0);	//!< ��������������������ı���
	void ItemInDelMgrItem(std::list<std::shared_ptr<CItem> > ltItem, UINT groupID = 0);	//!< �������������ʱ�õ���ı������Ǳ������еı���
	void ItemInRenameStrItem(std::list<std::shared_ptr<CItem> > ltItem, CString strName, UINT groupID = 0);	//!< ��������������Ĳ����ַ���

	void ShowAllItem();
	void ShowGroup(UINT groupid);
	void OnUndo();
	void OnRedo();
	void AddUndoMember(std::shared_ptr<SItemUndo> itemUndo);
	void SetUndoEnd();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetTitle(LPCTSTR lpszTitle);
};
}
}