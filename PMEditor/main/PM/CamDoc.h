#pragma once
#include "Camera.h"

// CCamDoc �ĵ�
namespace MVC{
namespace Camera{
class CCamera;
class CCamGrid;
class CCamView;

typedef struct tagCamUndo
{
	std::shared_ptr<CCamera> m_Item;		//!< ��¼������������ָ�룬ɾ����ʱ����
	UINT m_uiEditType;						//!< �������ͣ�0��ӣ�1ɾ����2�޸�
	UINT m_uiEditInfo;						//!< ������Ϣ�����ݱ���������Ĳ�ͬ���������Ҳ��ͬ
	bool m_bEnd;							//!< �Ƿ��ǵ��β����Ľ�β��һ�β������ԶԶ��������������һ����β��־

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
	virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
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

	//bool AddItem(UINT id, std::shared_ptr<CCamera> item, int maxID, UINT groupid=0, UINT uiAsk=0);	//!< ��ӱ�����ָ��ID����������Ƿ�ѯ�ʣ�0�ʣ�1���ʸ��ǣ�2������

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