#pragma once
#include "MyHelp.h"

namespace Dialog{
//!< �������ݹ�����ĸ���
class CObjectInProperty
{
public:
	CObjectInProperty(){;}
	virtual ~CObjectInProperty(){;}
	virtual void OnShowHelp() = 0;											//!< ��ʾ����
	virtual void ShowInfo(CXTPPropertyGrid& grid) = 0;						//!< ��ʾ���ĺ���
	virtual void OnGridFirstShow(CXTPPropertyGrid& grid){;}					//!< ����ʼ����ɺ�
	virtual CString GetTitle()=0;											//!< ��ñ����������
	virtual void OnCloseGrid(){;}											//!< ���رձ��ʱ
	virtual bool OnSaveModify(CXTPPropertyGrid& grid) = 0;					//!< Ҫ�����޸ĵ�����
	virtual void OnItemModify(CXTPPropertyGrid& grid, UINT ID){;}			//!< ��ĳ���ֵ�����ı�ʱ
	virtual void OnButtonClick(CXTPPropertyGrid& grid, UINT btID){;}		//!< ��ť������

	//!< ��ӱ༭�ı���
	CXTPPropertyGridItem* AddItemText(CXTPPropertyGridItem& parent, CString title, CString tooltip, CString value, UINT id);
	//!< ��ӿɶ��б༭���ı���
	CXTPPropertyGridItem* AddItemMultiText(CXTPPropertyGridItem& parent, CString title, CString tooltip, CString value, UINT id);
	//!< ���������
	CXTPPropertyGridItem* AddItemNumber(CXTPPropertyGridItem& parent, CString title, CString tooltip, long value, UINT id, BOOL empty = FALSE);
	//!< ��Ӹ�����,���һ��������ʾ����С�������λ
	CXTPPropertyGridItem* AddItemDouble(CXTPPropertyGridItem& parent, CString title, CString tooltip, double value, UINT id, BOOL empty = FALSE, int fFmt = 2);
	//!< ��������б���
	CXTPPropertyGridItem* AddItemList(CXTPPropertyGridItem& parent, CString title, CString tooltip, std::list<CString>& items, int def, UINT id);
	//!< ���Variant���ֻʶ��bool,char,byte,short,word,int32,uint32,float,double,bstr������
	CXTPPropertyGridItem* AddItemVariant(CXTPPropertyGridItem& parent, CString title, CString tooltip, CComVariant var, UINT id, BOOL empty = FALSE);
	//!< ���IP������
	CXTPPropertyGridItem* AddItemIP(CXTPPropertyGridItem& parent, CString title, CString tooltip, UINT ip, UINT id, BOOL empty = FALSE);
	//!< �����б��������
	void SetList(CXTPPropertyGrid& grid, UINT id, std::list<CString>& items, int def);
	void SetText(CXTPPropertyGrid& grid, UINT id, CString value, bool def);
};
}