#pragma once
#include "ObjectInProperty.h"

namespace MVC{
namespace Item{
class CItem;
class CPropertyItem : public Dialog::CObjectInProperty
{
private:
	bool m_bAdd;		//!< ��ʾ�Ƿ������״̬��true��ʾ��ӱ�����false��ʾ�޸ı���
	UINT m_GroupID;		//!< ��������ӻ����޸ģ���Ҫ��¼һ�±����������ID��
	CItem* m_Item;		//!< ��ǰ��ָ��ı�����������½������Ϊ��
	std::shared_ptr<CItem> m_ShowItem;			//!< Ҫ��ʾ�õı���

public:
	std::shared_ptr<CItem> m_NewItem;				//!< ��¼�ϴ���ӵı�����������Ϣ��������ʱ��ʾ�ϴ���ӵ���Ϣ
	void SetType(bool add = true){m_bAdd = add;}
	void SetItem(CItem* item){m_Item = item;}
	void SetGroupID(UINT id){m_GroupID = id;}
	void OnChangeType(CXTPPropertyGrid& grid);						//!< �����ͷ����ı�ʱ��������Ͱ�������ֵ���͡�����Դ���͡�IO����������

public:
	CPropertyItem(void);
	~CPropertyItem(void);

private:
	virtual void OnShowHelp();										//!< ��ʾ����
	virtual void ShowInfo(CXTPPropertyGrid& grid);					//!< ��ʾ���ĺ���
	virtual void OnGridFirstShow(CXTPPropertyGrid& grid);			//!< ����ʼ����ɺ�
	virtual CString GetTitle();										//!< ��ñ����������
	virtual void OnCloseGrid();										//!< ���رձ��ʱ
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT id);		//!< ��ĳһ��޸�ʱ
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);				//!< Ҫ���汻�ı������
	virtual void OnButtonClick(CXTPPropertyGrid& grid, UINT btID);	//!< ��ť������
	void CreateNew();
	void CreateEdit();

	void ShowAndHide(CXTPPropertyGrid& grid);						//!< ������ͳһ����ʾ������
	void EnableAndDisenable(CXTPPropertyGrid& grid);				//!< ������ͳһʹ��
	bool IfRightItemName(CString strName);							//!< �������Ƿ�Ϸ�
};
}
}