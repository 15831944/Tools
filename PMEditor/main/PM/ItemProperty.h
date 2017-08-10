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

	UINT m_DevIDOld;	//!< ��¼�ɵ������豸�ڵ��Ӧ�豸�ı��,�����µıȽ�,���һ���Ͳ�ˢ�±������б���

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
	void OnDeviceChange(CXTPPropertyGrid& grid);
	void OnAreaChange(CXTPPropertyGrid& grid);

	void ShowAndHide(CXTPPropertyGrid& grid);						//!< ������ͳһ����ʾ������
	void EnableAndDisenable(CXTPPropertyGrid& grid);				//!< ������ͳһʹ��
	void OnChangeDev(CXTPPropertyGrid& grid);						//!< ���ı������豸ʱ
	void OnChangeArea(CXTPPropertyGrid& grid);						//!< ���ı�������ʱ
	void OnChangeIOType(CXTPPropertyGrid& grid);					//!< ���ı�������������ʱ
	bool IfRightItemName(CString strName);							//!< �������Ƿ�Ϸ�
	bool IfSameArea(CString name1, UINT id2);						//!< �ж��������豸�Ƿ������ȫ��ͬ����
};
}
}