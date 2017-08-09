#pragma once
#include "ObjectInProperty.h"

namespace Dialog{class CSerialSetDlg;}
namespace MVC{namespace Device{namespace InterfaceSet{
class CPropertySerial : public Dialog::CObjectInProperty
{
private:
	CString m_strSerialName;	//!< ��������

public:
	CPropertySerial();
	~CPropertySerial(void);
	Dialog::CSerialSetDlg* m_pParent;

public:
	void SetInfo(CString serialName);									//!< ���ô���

	virtual CString GetTitle();											//!< ��ñ����������
	virtual void OnShowHelp();											//!< ��ʾ������Ϣ
	virtual void ShowInfo(CXTPPropertyGrid& grid);						//!< ��ʾ���ĺ���
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT ID);			//!< ��ĳ���ֵ�����ı�ʱ
	virtual bool OnSaveModify(CXTPPropertyGrid& grid){return true;}
	virtual void OnCloseGrid(){;}
};
}}}