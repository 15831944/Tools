#include "StdAfx.h"
#include "Gbl.h"
#include "ObjectInProperty.h"
#include "CustomItems.h"

using namespace Dialog;

CXTPPropertyGridItem* CObjectInProperty::AddItemText(CXTPPropertyGridItem& parent, CString title, CString tooltip, CString value, UINT id)
{
	CXTPPropertyGridItem* pItem = parent.AddChildItem(new CXTPPropertyGridItem(_T("")));
	pItem->SetCaption(title);
	pItem->SetTooltip(tooltip);
	if(tooltip != title)			pItem->SetDescription(tooltip);
	pItem->SetValue(value);
	pItem->SetDefaultValue(pItem->GetValue());
	pItem->SetID(id);
	return pItem;
}

CXTPPropertyGridItem* CObjectInProperty::AddItemMultiText(CXTPPropertyGridItem& parent, CString title, CString tooltip, CString value, UINT id)
{
	CXTPPropertyGridItem* pItem = parent.AddChildItem(new CXTPPropertyGridItemMultilineString(title, value));
	pItem->SetTooltip(tooltip);
	if(tooltip != title)			pItem->SetDescription(tooltip);
	pItem->SetDefaultValue(pItem->GetValue());
	pItem->SetID(id);
	return pItem;
}

CXTPPropertyGridItem* CObjectInProperty::AddItemNumber(CXTPPropertyGridItem& parent, CString title, CString tooltip, long value, UINT id, BOOL empty)
{
	CXTPPropertyGridItemNumber* pItem = (CXTPPropertyGridItemNumber*)parent.AddChildItem(new CXTPPropertyGridItemNumber(_T(""),value));
	pItem->SetCaption(title);
	pItem->SetTooltip(tooltip);
	if(tooltip != title)			pItem->SetDescription(tooltip);
	if(empty)
	{
		pItem->SetNumber(-1);
		pItem->SetDefaultValue(_T("-1"));
	}
	else
	{
		pItem->SetDefaultValue(pItem->GetValue());
	}
	pItem->SetID(id);
	return pItem;
}

CXTPPropertyGridItem* CObjectInProperty::AddItemDouble(CXTPPropertyGridItem& parent, CString title, CString tooltip, double value, UINT id, BOOL empty, int fFmt /* = 2 */)
{
	CXTPPropertyGridItemDouble* pItem;
	if(fFmt < 0)		fFmt = 0;
	if(fFmt > 6)		fFmt = 6;
	CString strFormat;
	strFormat.Format(".%d", fFmt);
	strFormat = _T("%") + strFormat + _T("f");
	if(empty)	pItem = (CXTPPropertyGridItemDouble*)parent.AddChildItem(new CXTPPropertyGridItemDouble(_T(""), 0, strFormat));
	else		pItem = (CXTPPropertyGridItemDouble*)parent.AddChildItem(new CXTPPropertyGridItemDouble(_T(""), value, strFormat));
	pItem->SetCaption(title);
	pItem->SetTooltip(tooltip);
	if(tooltip != title)			pItem->SetDescription(tooltip);
	if(empty)
	{
		pItem->SetDouble(-1);
		pItem->SetDefaultValue(_T("-1.00"));
	}
	else
	{
		pItem->SetDefaultValue(pItem->GetValue());
	}
	pItem->SetID(id);
	return pItem;
}

CXTPPropertyGridItem* CObjectInProperty::AddItemIP(CXTPPropertyGridItem& parent, CString title, CString tooltip, UINT ip, UINT id, BOOL empty)
{
	CXTPPropertyGridItem* pItem = parent.AddChildItem(new PropertyItem::CCustomItemIPAddress(_T("")));
	pItem->SetCaption(title);
	pItem->SetTooltip(tooltip);
	if(tooltip != title)			pItem->SetDescription(tooltip);
	if(empty)
	{
		pItem->SetValue(_T(""));
		pItem->SetDefaultValue(_T(""));
	}
	else
	{
		pItem->SetValue(CGbl::GetIPFromNumber(ip));
		pItem->SetDefaultValue(pItem->GetValue());
	}
	pItem->SetID(id);
	return pItem;
}

CXTPPropertyGridItem* CObjectInProperty::AddItemList(CXTPPropertyGridItem& parent, CString title, CString tooltip, std::list<CString>& items, int def, UINT id)
{
	CXTPPropertyGridItem* pItem = parent.AddChildItem(new CXTPPropertyGridItem(_T("")));
	pItem->SetCaption(title);
	pItem->SetTooltip(tooltip);
	if(tooltip != title)			pItem->SetDescription(tooltip);
	CXTPPropertyGridItemConstraints* pList = pItem->GetConstraints();
	for (CString str : items)		pList->AddConstraint(str);

	if(pList->GetCount() > 0)		pItem->SetFlags(xtpGridItemHasComboButton);	// 列出下拉按钮
	if(def == -1)																// -1表示没有一个事默认值
	{
		pItem->SetValue(_T(""));
		pItem->SetDefaultValue(_T(""));
	}
	else if(def >= 0 && def < pList->GetCount())								// 存在一个默认值
	{
		pList->SetCurrent(def);
		pItem->SetValue(pList->GetAt(def));
		pItem->SetDefaultValue(pItem->GetValue());
	}
	pItem->SetID(id);
	return pItem;
}

void CObjectInProperty::SetList(CXTPPropertyGrid& grid, UINT id, std::list<CString>& items, int def)
{
	CXTPPropertyGridItem* item = grid.FindItem(id);
	if(!item)		return;
	CXTPPropertyGridItemConstraints* pList = item->GetConstraints();
	pList->RemoveAll();
	for (CString str : items)		pList->AddConstraint(str);

	if(pList->GetCount() > 0)		item->SetFlags(xtpGridItemHasComboButton);
	if(def == -1){
		SetText(grid, id, _T(""), true);
	}
	else if(def >=0 && def < pList->GetCount()){
		SetText(grid, id, pList->GetAt(def), true);
	}
}

void CObjectInProperty::SetText(CXTPPropertyGrid& grid, UINT id, CString value, bool def)
{
	CXTPPropertyGridItem* item = grid.FindItem(id);
	if(!item)		return;
	item->SetValue(value);
	if(def)		item->SetDefaultValue(value);
}

CXTPPropertyGridItem* CObjectInProperty::AddItemVariant(CXTPPropertyGridItem& parent, CString title, CString tooltip, CComVariant var, UINT id, BOOL empty)
{
	if(var.vt == VT_BOOL)			return AddItemNumber(parent, title, tooltip, var.boolVal?1:0, id, empty);
	else if(var.vt == VT_I1)		return AddItemNumber(parent, title, tooltip, (long)var.cVal, id, empty);
	else if(var.vt == VT_I2)		return AddItemNumber(parent, title, tooltip, (long)var.iVal, id, empty);
	else if(var.vt == VT_I4)		return AddItemNumber(parent, title, tooltip, (long)var.intVal, id, empty);
	else if(var.vt == VT_UI1)		return AddItemNumber(parent, title, tooltip, (long)var.bVal, id, empty);
	else if(var.vt == VT_UI2)		return AddItemNumber(parent, title, tooltip, (long)var.uiVal, id, empty);
	else if(var.vt == VT_UI4)		return AddItemNumber(parent, title, tooltip, (long)var.uintVal, id, empty);
	else if(var.vt == VT_R4)		return AddItemDouble(parent, title, tooltip, var.fltVal, id, empty);
	else if(var.vt == VT_R8)		return AddItemDouble(parent, title, tooltip, var.dblVal, id, empty);
	else if(var.vt == VT_BSTR)		return AddItemText(parent, title, tooltip, var, id);

	return NULL;
}