#pragma once
#include "ObjectInProperty.h"

namespace Servers{
namespace DOPC{
class CPropertyDOPC : public Dialog::CObjectInProperty
{
public:
	CPropertyDOPC(void);
	~CPropertyDOPC(void);

private:
	virtual void OnShowHelp();										//!< ��ʾ������Ϣ
	virtual void ShowInfo(CXTPPropertyGrid& grid);					//!< ��ʾ���ĺ���
	virtual CString GetTitle();										//!< ��ñ����������
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);				//!< Ҫ���汻�ı������
};
}}