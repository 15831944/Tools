#pragma once
#include "ObjectInProperty.h"

namespace Servers{
namespace RongYu{
class CPropertyRongYu : public Dialog::CObjectInProperty
{
public:
	CPropertyRongYu(void){;}
	~CPropertyRongYu(void){;}

private:
	virtual void OnShowHelp();										//!< ��ʾ������Ϣ
	virtual void ShowInfo(CXTPPropertyGrid& grid);					//!< ��ʾ���ĺ���
	virtual CString GetTitle();										//!< ��ñ����������
	virtual bool OnSaveModify(CXTPPropertyGrid& grid);				//!< Ҫ���汻�ı������
};
}}