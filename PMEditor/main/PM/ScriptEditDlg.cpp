// ScriptEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DXPEditor.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "ScriptEditDlg.h"
#include "ItemMgr.h"
#include "Item.h"
#include "SourceProperty.h"

using namespace Dialog;
using namespace ReportItem;

IMPLEMENT_SERIAL(CItemRecord, CXTPReportRecord, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CItemRecordIcon, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
//////////////////////////////////////////////////////////////////////////
// CMessageRecordItemIcon

CItemRecordIcon::CItemRecordIcon(BOOL bMem)
{
	m_bMem = bMem;
	UpdateIcon();
}

void CItemRecordIcon::UpdateIcon()
{
	SetIconIndex(m_bMem ? 4 : 3);
}

int CItemRecordIcon::Compare(CXTPReportColumn* /*pColumn*/, CXTPReportRecordItem* pItem)
{
	return int(m_bMem) - int(((CItemRecordIcon *)pItem)->m_bMem);
}

CString CItemRecordIcon::GetGroupCaption(CXTPReportColumn* /*pColumn*/)
{
	if (m_bMem)
		return _T("Message status: 内存变量");
	else
		return _T("Message status: IO变量");
}

int CItemRecordIcon::CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem)
{
	return GetGroupCaption(pColumn).Compare(pItem->GetGroupCaption(pColumn));
}

void CItemRecordIcon::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportRecordItem::DoPropExchange(pPX);

	PX_Bool(pPX, _T("type"), m_bMem);
}


//////////////////////////////////////////////////////////////////////////
CItemRecord::CItemRecord()
{
	m_ID = UINT(-1);
}
CItemRecord::CItemRecord(std::shared_ptr<MVC::Item::CItem> item)
{
	m_ID = item->getID();
	CString valTypeArray[9] = {_T("位"), _T("有符号字节"), _T("无符号字节"), _T("有符号字"), _T("无符号字"),
		_T("有符号双字"), _T("无符号双字"), _T("浮点"), _T("双精度")};
	CString srcTypeArray[2] = {_T("内存变量"), _T("I/O变量")};
	ASSERT(item);
//	AddItem(new CItemRecordIcon(item->getSrcType() == 0));
	AddItem(new CXTPReportRecordItemText(_T("")));
	AddItem(new CXTPReportRecordItemText(item->getName()));
	AddItem(new CXTPReportRecordItemNumber(item->getID()));
	AddItem(new CXTPReportRecordItemText(valTypeArray[item->getValType() % 9]));
	AddItem(new CXTPReportRecordItemText(srcTypeArray[item->getSrcType() % 2]));
	AddItem(new CXTPReportRecordItemText(item->getDescription()));
}
CItemRecord::~CItemRecord()
{

}

void CItemRecord::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
	CXTPReportRecord::GetItemMetrics(pDrawArgs, pItemMetrics);
}

//////////////////////////////////////////////////////////////////////////

// CScriptEditDlg 对话框

IMPLEMENT_DYNAMIC(CScriptEditDlg, CXTResizeDialog)

CScriptEditDlg::CScriptEditDlg(CString strScript, CString strName, CString strDescription)
	: CXTResizeDialog(CScriptEditDlg::IDD, NULL)
{
	m_strScriptText = strScript;
	m_strItemName = strName;
	m_strDescription = strDescription;
	m_NowEditType = 0;
}

CScriptEditDlg::~CScriptEditDlg()
{
}

void CScriptEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCRIPT_TEXT, m_ScriptTextCtrl);
	DDX_Control(pDX, IDC_ITEM_LIST, m_ItemCtrl);
	DDX_Control(pDX, IDC_SCRIPT_GROUP, m_cbItemGroup);
	DDX_Control(pDX, IDC_SCRIPT_DEVICE, m_cbItemDevice);
	DDX_Text(pDX, IDC_ITEM_NAME, m_strItemName);
	DDX_Text(pDX, IDC_ITEM_DESCRIPT, m_strDescription);
}


BEGIN_MESSAGE_MAP(CScriptEditDlg, CXTResizeDialog)
	ON_BN_CLICKED(IDOK, &CScriptEditDlg::OnBnClickedOk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SCRIPT_0, IDC_SCRIPT_MORE, OnClickScriptBtn)
	ON_EN_CHANGE(IDC_SCRIPT_TEXT, &CScriptEditDlg::OnEnChangeScriptText)
	ON_BN_CLICKED(ID_SCRIPT_CHECK, &CScriptEditDlg::OnBnClickedScriptCheck)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CScriptEditDlg 消息处理程序
BOOL CScriptEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//!< 初始化脚本编译器
	m_ScriptMgr.SetLanguage(_T("VBScript"));

	//!< 初始化脚本文本框
	m_ScriptTextCtrl.SetEventMask(m_ScriptTextCtrl.GetEventMask() | ENM_CHANGE);
	m_NowEditType = 3;		//!< 进入初始化，为了不增加撤销元素
	m_OldScript.text = m_strScriptText;
	m_OldScript.begin = 0;
	m_OldScript.end = m_OldScript.text.GetLength() - 1;
	m_ScriptTextCtrl.SetWindowText(m_OldScript.text);

	//!< 初始化表格
	m_ItemCtrl.SetOwner(this);
	CBitmap bp1, bp2;
	VERIFY(bp1.LoadBitmap(IDB_BMP_ITEMMEMORY));
	VERIFY(bp2.LoadBitmap(IDB_BMP_ITEMIO));

	if (!m_ImgList.Create(16,16, ILC_COLOR32|ILC_MASK, 0, 1))		return -1;
	m_ImgList.Add(&bp1, RGB(255, 255, 255));
	m_ImgList.Add(&bp2, RGB(255, 255, 255));
	m_ItemCtrl.SetImageList(&m_ImgList);

	m_ItemCtrl.AddColumn(new CXTPReportColumn(COL_NAME, _T("名称"), 20));
	m_ItemCtrl.AddColumn(new CXTPReportColumn(COL_ID, _T("编号"), 12));
	m_ItemCtrl.AddColumn(new CXTPReportColumn(COL_VALTYPE, _T("值类型"), 20));
	m_ItemCtrl.AddColumn(new CXTPReportColumn(COL_SRCTYPE, _T("数据源类型"), 20));
	m_ItemCtrl.AddColumn(new CXTPReportColumn(COL_DESCRIPT, _T("备注"), 28));

	MVC::Item::CItemMgr* itemMgr = &MVC::Item::CItemMgr::GetMe();
	std::shared_ptr<MVC::Item::CItem> item;
	for (auto item : itemMgr->m_vtItem){
		if(!item)		continue;
		m_ItemCtrl.AddRecord(new CItemRecord(item));
	}
	m_ItemCtrl.Populate();
	m_ItemCtrl.GetPaintManager()->m_strNoItems = _T("");	// 去掉"There are no items to show."

	// Set control resizing.
	SetResize(IDC_ITEM_LIST, SZ_TOP_LEFT, SZ_TOP_CENTER);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	//!< 初始化撤销与恢复按钮的状态
	SetUndoRedoState();

	//!< 初始化筛选的两个列表
	m_cbItemGroup.AddString(_T("全部"));
	m_cbItemDevice.AddString(_T("全部"));
	m_cbItemGroup.SetCurSel(0);
	m_cbItemDevice.SetCurSel(0);

	UpdateData(FALSE);
	return TRUE;
}

void CScriptEditDlg::OnBnClickedOk()
{
	m_ScriptTextCtrl.GetWindowText(m_strScriptText);
	OnOK();
}

void CScriptEditDlg::OnClickScriptBtn(WPARAM wParam)
{
	CString text;
	switch(UINT(wParam))
	{
	case IDC_SCRIPT_0:	case IDC_SCRIPT_1:	case IDC_SCRIPT_2:	case IDC_SCRIPT_3:	case IDC_SCRIPT_4:
	case IDC_SCRIPT_5:	case IDC_SCRIPT_6:	case IDC_SCRIPT_7:	case IDC_SCRIPT_8:	case IDC_SCRIPT_9:
		text.Format("%d",(UINT)wParam - IDC_SCRIPT_0);		ReplaceSel(text);		break;
	case IDC_SCRIPT_POINT:		ReplaceSel(_T("."));				break;
	case IDC_SCRIPT_PLUS:		ReplaceSel(_T("+"));				break;
	case IDC_SCRIPT_SUB:		ReplaceSel(_T("-"));				break;
	case IDC_SCRIPT_MUL:		ReplaceSel(_T("*"));				break;
	case IDC_SCRIPT_DIV:		ReplaceSel(_T("/"));				break;
	case IDC_SCRIPT_SQU:		ReplaceSel(_T("^2"));				break;
	case IDC_SCRIPT_POW:		ReplaceSel(_T("^N次方"), 3, 0);		break;
	case IDC_SCRIPT_SQR:		ReplaceSel(_T("sqr(开方数)"), 3, 1);break;
	case IDC_SCRIPT_SIN:		ReplaceSel(_T("sin(正弦数)"), 3, 1);break;
	case IDC_SCRIPT_COS:		ReplaceSel(_T("cos(余弦数)"), 3, 1);break;
	case IDC_SCRIPT_TAN:		ReplaceSel(_T("tan(正切数)"), 3, 1);break;
	case IDC_SCRIPT_AND:		ReplaceSel(_T(" And "));			break;
	case IDC_SCRIPT_OR:			ReplaceSel(_T(" Or "));				break;
	case IDC_SCRIPT_KUOHAO0:	ReplaceSel(_T("("));				break;
	case IDC_SCRIPT_KUOHAO1:	ReplaceSel(_T(")"));				break;
	case IDC_SCRIPT_CLEAR:		ScriptClear();						break;
	case IDC_SCRIPT_UNDO:		ScriptUndo();						break;
	case IDC_SCRIPT_REDO:		ScriptRedo();						break;
	case IDC_SCRIPT_MORE:		ScriptMore();						break;
	}
}

//!< 向脚本框中添加数据，后两个参数主要为了在显示时添加到选中文本的一部分
void CScriptEditDlg::ReplaceSel(CString text, UINT backSelLen /* = 0 */, UINT selId /* = 0 */)
{
	m_ScriptTextCtrl.ReplaceSel(text, FALSE);		//!< 设置光标位置
	if(backSelLen != 0)
	{
		long sel;
		m_ScriptTextCtrl.GetSel(sel, sel);
		m_ScriptTextCtrl.SetSel(sel - selId - backSelLen, sel - selId);
	}
	m_ScriptTextCtrl.SetFocus();
}

//!< 清空脚本内容
void CScriptEditDlg::ScriptClear()
{
	m_ScriptTextCtrl.SetWindowText(_T(""));			//!< 设置光标位置
	m_ScriptTextCtrl.SetFocus();
}

//!< 撤销
void CScriptEditDlg::ScriptUndo()
{
	if(m_strUndo.empty())	return;
	m_NowEditType = 1;
	STextMemory me;
	m_ScriptTextCtrl.GetWindowText(me.text);
	m_ScriptTextCtrl.GetSel(me.begin, me.end);
	m_strRedo.push(me);								//!< 增加重复元素

	me = m_strUndo.top();							//!< 获得撤销元素
	m_ScriptTextCtrl.SetWindowText(me.text);
	m_ScriptTextCtrl.SetSel(me.begin, me.end);		//!< 设置光标位置
	m_strUndo.pop();								//!< 删除撤销元素
	m_ScriptTextCtrl.SetFocus();
	SetUndoRedoState();								//!< 设置撤销重复状态
}

//!< 重复
void CScriptEditDlg::ScriptRedo()
{
	if(m_strRedo.empty())	return; 
	m_NowEditType = 2;
	STextMemory me;
	m_ScriptTextCtrl.GetWindowText(me.text);
	m_ScriptTextCtrl.GetSel(me.begin, me.end);
	m_strUndo.push(me);								//!< 增加撤销元素

	me = m_strRedo.top();							//!< 获得重复元素
	m_ScriptTextCtrl.SetWindowText(me.text);
	m_strRedo.pop();								//!< 删除重复元素
	m_ScriptTextCtrl.SetFocus();
	m_ScriptTextCtrl.SetSel(me.begin, me.end);		//!< 设置光标位置
	SetUndoRedoState();								//!< 设置撤销重复状态
}

//!< 更多内容
void CScriptEditDlg::ScriptMore()
{
	AfxMessageBox("More information required!");
}

//!< 当数据改变时，判断并设置一下撤销和重复这两个按钮的状态
void CScriptEditDlg::SetUndoRedoState()
{
	if(m_strUndo.empty())		GetDlgItem(IDC_SCRIPT_UNDO)->EnableWindow(FALSE);
	else						GetDlgItem(IDC_SCRIPT_UNDO)->EnableWindow(TRUE);
	if(m_strRedo.empty())		GetDlgItem(IDC_SCRIPT_REDO)->EnableWindow(FALSE);
	else						GetDlgItem(IDC_SCRIPT_REDO)->EnableWindow(TRUE);
}

BOOL CScriptEditDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == 0x59)			//!< 'Y'
		{
			if(GetKeyState(VK_CONTROL) < 0)
			{
				ScriptRedo();
				return TRUE;
			}
		}
		else if(pMsg->wParam == 0x5A)		//!< 'Z'
		{
			if(GetKeyState(VK_CONTROL) < 0)
			{
				ScriptUndo();
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CScriptEditDlg::OnEnChangeScriptText()
{
	STextMemory script;
	m_ScriptTextCtrl.GetWindowText(script.text);
	if(m_NowEditType == 3)			//!< 初始化状态
	{
		m_OldScript = script;
		m_NowEditType = 0;
		return;
	}
	if(script.text == m_OldScript.text)		return;

	if(m_NowEditType == 0){
		m_strUndo.push(m_OldScript);
		m_ScriptTextCtrl.GetSel(script.begin, script.end);
		m_OldScript = script;

		while(!m_strRedo.empty())	m_strRedo.pop();
	}
	else
		m_NowEditType = 0;

	SetUndoRedoState();
	return;
}

void CScriptEditDlg::OnReportDblClick(DReport::CDReport* report, UINT row)
{
	CXTPReportRow* xtpRow = m_ItemCtrl.GetFocusedRow();
	CItemRecord* record = (CItemRecord *)xtpRow->GetRecord();
	if(!record)						return;
	MVC::Item::CItemMgr* mgr = &MVC::Item::CItemMgr::GetMe();
	std::shared_ptr<MVC::Item::CItem> item = mgr->GetItem(record->m_ID);
	if(!item)						return;
	CString name = item->getName();
	name = _T("[") + name + _T("]");
	ReplaceSel(name);
}

//!< 开始检查
void CScriptEditDlg::OnBnClickedScriptCheck()
{
	CString strScript, strError;
	m_ScriptTextCtrl.GetWindowText(strScript);
	strScript = strScript.Trim();

	//!< 输出处理
	if(strScript == _T("") || CheckScript(strScript, strError))
		MessageBox(_T("测试正常"), _T("信息"), MB_ICONINFORMATION);
	else
		MessageBox(strError, _T("错误"), MB_ICONEXCLAMATION);
}

//!< 检查这个的脚本的正确定，返回正确与否，和错误信息
bool CScriptEditDlg::CheckScript(CString strScript, CString& strError)
{
	
	//!< 1.拆分文本
	std::vector<CString> vtStr;
	CGbl::SpliteBy(strScript, _T("["), vtStr);

	//!< 2.替换变量，赋假值
	MVC::Item::CItemMgr* mgr = &MVC::Item::CItemMgr::GetMe();
	std::shared_ptr<MVC::Item::CItem> item;
	CString script;
	for(int i = 0; i < (int)vtStr.size(); ++i)
	{
		script = vtStr[i];
		int index = script.Find("]");
		if(index == -1)		continue;	//!< 第一个可能没有 ]
		CString itemName = script.Left(index);
		item = mgr->GetItem(itemName);
		if(!item){
			strError = _T("名称为 \"") + itemName + _T("\" 的变量没有找到！");
			return false;
		}
		CString itemValue = item->GetCheckValue();
		script.Delete(0, index + 1);
		script = _T("(") + itemValue + _T(")") + script;
		vtStr[i] = script;
	}

	//!< 3.重新组合
	strScript = _T("");
	for (auto script : vtStr)
		strScript = strScript + script;
	strScript = _T("Answer = ") + strScript;

	//!< 4.调用脚本组件运行，截获错误信息
	return OnExcute(strScript, strError);
}

//!< 执行脚本
bool CScriptEditDlg::OnExcute(CString strScript, CString &strError)
{
	m_ScriptMgr.SetTimeout(-1);
	try{
		if(!m_ScriptMgr.ExecuteStatement(strScript)){
			strError = m_ScriptMgr.GetErrorString();
			return false;
		}
		else
			return true;
	}
	catch(...){
		strError = m_ScriptMgr.GetErrorString();
	}
	return false;
}

BOOL Dialog::CScriptEditDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("内存变量赋值脚本"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
