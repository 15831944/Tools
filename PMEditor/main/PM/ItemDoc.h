#pragma once
#include "Item.h"

// CItemDoc 文档
namespace Dialog{class CItemInConfigDlg;}
namespace MVC{
namespace Item{
class CItem;
class CItemGrid;
class CItemView;

typedef struct tagItemUndo
{
	std::shared_ptr<CItem> m_Item;		//!< 记录被操作变量的指针，删除的时候用
	UINT m_uiEditType;						//!< 操作类型，0添加，1删除，2修改
	UINT m_uiEditInfo;						//!< 操作信息，根据被操作对象的不同，这里填的也不同
	bool m_bEnd;							//!< 是否是单次操作的结尾，一次操作可以对多个变量，所以有一个结尾标志

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
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	UINT m_uiGroupID;						//!< 所显示的变量组ID号
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

	bool AddItem(UINT id, std::shared_ptr<CItem> item, int maxID, UINT groupid=0, UINT uiAsk=0);	//!< 添加变量，指定ID，如果存在是否询问，0问，1不问覆盖，2问跳过
	void InItem(std::list<std::shared_ptr<CItem> > ltItem, Dialog::CItemInConfigDlg* dlg);		//!< 添加多个变量

	DECLARE_MESSAGE_MAP()
public:
	bool IsUndoEmpty(){return m_stItemUndo.empty();}
	bool IsRedoEmpty(){return m_stItemRedo.empty();}

public:
	CItemView* GetView();
	void SerializeIn(TiXmlElement* pNode);
	void SerializePlcConfig(CString strPath, CString strTitle, CString strExt);
	bool ItemInAdvanceOptions(std::list<std::shared_ptr<CItem> > ltItem);					//!< 变量导入或粘贴的高级选项处理
	void ItemInRenameItem(std::list<std::shared_ptr<CItem> > ltItem, UINT groupID = 0);	//!< 导入变量，自动重命名重名的变量
	void ItemInDelFileItem(std::list<std::shared_ptr<CItem> > ltItem, UINT groupID = 0);	//!< 导入变量，不导入重名的变量
	void ItemInDelMgrItem(std::list<std::shared_ptr<CItem> > ltItem, UINT groupID = 0);	//!< 导入变量，重名时用导入的变量覆盖变量表中的变量
	void ItemInRenameStrItem(std::list<std::shared_ptr<CItem> > ltItem, CString strName, UINT groupID = 0);	//!< 导入变量，重名的补加字符串

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