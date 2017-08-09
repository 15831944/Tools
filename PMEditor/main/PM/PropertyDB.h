#pragma once
#include "ObjectInProperty.h"

namespace Servers{
namespace DB{
class CPropertyDB : public Dialog::CObjectInProperty
{
public:
	CPropertyDB(void);
	~CPropertyDB(void);
private:
	virtual void OnShowHelp();										//!< ��ʾ������Ϣ
	virtual void ShowInfo(CXTPPropertyGrid& grid);					//!< ��ʾ���ĺ���
	virtual void OnGridFirstShow(CXTPPropertyGrid& grid);			//!< ����ʼ����ɺ�
	virtual void EnableAndDisenable(CXTPPropertyGrid& grid);		//!< ������ͳһʹ��
	virtual CString GetTitle();										//!< ��ñ����������
	virtual void OnCloseGrid();										//!< ���رձ��ʱ
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT id);		//!< ��ĳһ��޸�ʱ
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);				//!< Ҫ���汻�ı������
	virtual void OnButtonClick(CXTPPropertyGrid& grid, UINT btID);	//!< ��ť������
};
}}