#pragma once
#include "Camera.h"

// CCamDoc 文档
namespace MVC{
namespace Camera{
class CCamera;
class CCamGrid;
class CCamView;

typedef struct tagCamUndo
{
	std::shared_ptr<CCamera> m_Item;		//!< 记录被操作变量的指针，删除的时候用
	UINT m_uiEditType;						//!< 操作类型，0添加，1删除，2修改
	UINT m_uiEditInfo;						//!< 操作信息，根据被操作对象的不同，这里填的也不同
	bool m_bEnd;							//!< 是否是单次操作的结尾，一次操作可以对多个变量，所以有一个结尾标志

	tagCamUndo();
	tagCamUndo(UINT type, std::shared_ptr<CCamera> item, UINT info);
}SCamUndo, *PSCamUndo;

class CCamDoc : public CDocument
{
	DECLARE_DYNCREATE(CCamDoc)
public:
	CCamDoc();
	virtual ~CCamDoc();

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
	std::stack< std::shared_ptr<SCamUndo> > m_stItemUndo;
	std::stack< std::shared_ptr<SCamUndo> > m_stItemRedo;

protected:
	void ClearUndo();
	void ClearRedo();
	void UndoAdd();
	void UndoDel();
	void UndoUpd();
	void RedoAdd();
	void RedoDel();
	void RedoUpd();

	CCamGrid* GetGrid();
	void RedoOne(std::shared_ptr<SCamUndo> redo);
	virtual BOOL OnNewDocument();

	//bool AddItem(UINT id, std::shared_ptr<CCamera> item, int maxID, UINT groupid=0, UINT uiAsk=0);	//!< 添加变量，指定ID，如果存在是否询问，0问，1不问覆盖，2问跳过

	DECLARE_MESSAGE_MAP()
public:
	bool IsUndoEmpty(){return m_stItemUndo.empty();}
	bool IsRedoEmpty(){return m_stItemRedo.empty();}

public:
	CCamView* GetView();

	void ShowAllItem();
	void OnUndo();
	void OnRedo();
	void AddUndoMember(std::shared_ptr<SCamUndo> itemUndo);
	void SetUndoEnd();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetTitle(LPCTSTR lpszTitle);
};
}
}