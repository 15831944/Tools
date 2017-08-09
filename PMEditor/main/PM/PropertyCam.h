#pragma once
#include "ObjectInProperty.h"

namespace MVC{
namespace Camera{
class CCamera;
class CPropertyCamera : public Dialog::CObjectInProperty
{
private:
	bool m_bAdd;									//!< ��ʾ�Ƿ������״̬��true��ʾ��ӱ�����false��ʾ�޸ı���
	CCamera* m_Item;								//!< ��ǰ��ָ��ı�����������½������Ϊ��
	boost::shared_ptr<CCamera> m_ShowItem;			//!< Ҫ��ʾ�õı���

public:
	boost::shared_ptr<CCamera> m_NewItem;			//!< ��¼�ϴ���ӵı�����������Ϣ��������ʱ��ʾ�ϴ���ӵ���Ϣ
	void SetType(bool add = true){m_bAdd = add;}
	void SetItem(CCamera* item){m_Item = item;}

public:
	CPropertyCamera(void);
	~CPropertyCamera(void);

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

	void ShowAndHide(CXTPPropertyGrid& grid){;}						//!< ������ͳһ����ʾ������
	void EnableAndDisenable(CXTPPropertyGrid& grid);				//!< ������ͳһʹ��
	bool ExistSamePort(int nPort);									//!< �˿��Ƿ�����
};
}}
