// PropertyDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "PMApp.h"
#include "PropertyDlg.h"
#include "ObjectInProperty.h"

using namespace Dialog;
// CPropertyDlg 对话框
//IMPLEMENT_DYNAMIC(CPropertyDlg, CDialog)

CPropertyDlg::CPropertyDlg(CWnd* pParent /*=NULL*/)
	: CPropertyGridDlgBase(CPropertyDlg::IDD, pParent)
{
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPropertyDlg::~CPropertyDlg()
{
}

CPropertyDlg& CPropertyDlg::GetMe()
{
	static CPropertyDlg dlg;
	return dlg;
}

void CPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyGridDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLACEHOLDER, m_GridHolder);
}

BEGIN_MESSAGE_MAP(CPropertyDlg, CPropertyGridDlgBase)
	ON_BN_CLICKED(ID_APPLAY, &CPropertyDlg::OnBnClickedApplay)
	ON_WM_PAINT()
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

BOOL Dialog::CPropertyDlg::OnInitDialog()
{
	CPropertyGridDlgBase::OnInitDialog();
	GetDlgItem(ID_APPLAY)->ShowWindow(SW_HIDE);

	if(!m_Object)		return TRUE;
	CRect rc;
	m_GridHolder.GetWindowRect(&rc);
	ScreenToClient(&rc);

	if(m_PropertyGrid.Create(rc, this, IDC_PROPERTY_GRID))
		m_Object->ShowInfo(m_PropertyGrid);

	SetWindowText(m_Object->GetTitle());					//!< 设置标题
	m_PropertyGrid.SetFont(GetFont());						//!< 设置一下字体，以确保每次打开的字体都一样
	m_PropertyGrid.HighlightChangedItems(TRUE);				//!< 高亮被修改的部分
//	m_PropertyGrid.SetShowInplaceButtonsAlways(TRUE);		//!< 保持功能图标，表示每个item的值类型
	//m_PropertyGrid.SetTheme(xtpGridThemeNativeWinXP);		//!< 设置主题
	m_PropertyGrid.SetTheme(xtpGridThemeVisualStudio2010);

	SetResize(IDC_PROPERTY_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(ID_APPLAY, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	UpdateData(TRUE);
	m_Object->OnGridFirstShow(m_PropertyGrid);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void Dialog::CPropertyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPropertyGridDlgBase::OnPaint();
	}
}

HCURSOR CPropertyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//!< 点击应用
void CPropertyDlg::OnBnClickedApplay()
{
	if(m_Object){
		m_Object->OnSaveModify(m_PropertyGrid);		//!< 修改原来的客户菜单（只修改和原来不一样的地方）
		m_PropertyGrid.Refresh();					//!< 回到做菜前的各项准备，可以马上对刚才的菜进行更改，因为还没端上去，等待客户的指示
	}
}

void CPropertyDlg::OnOK()
{
	if(m_Object){
//		OnBnClickedApplay();				//!< 复用了上面的环节
		if(!m_Object->OnSaveModify(m_PropertyGrid))		return;
		m_PropertyGrid.Refresh();
		m_Object->OnCloseGrid();			//!< 告诉客户菜做完了，我要洗碗刷锅了（还想要可以再点）
	}
	CPropertyGridDlgBase::OnOK();
}

void CPropertyDlg::OnCancel()
{
	//!< 用户通知现在这样就行了，我等不及了
	//!< 洗碗、刷锅、撤火……
	CPropertyGridDlgBase::OnCancel();
}

INT_PTR Dialog::CPropertyDlg::DoModal(CObjectInProperty& object)
{
	m_Object = &object;						//!< 记录菜单
	return CXTResizeDialog::DoModal();		//!< 进入做菜环节
}

BOOL Dialog::CPropertyDlg::DestroyWindow()
{
	m_Object = NULL;						//!< 菜单可以撕了
	m_PropertyGrid.ResetContent();			//!< 菜做完了，洗碗刷锅
	return CXTResizeDialog::DestroyWindow();
}

LRESULT Dialog::CPropertyDlg::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_INPLACEBUTTONDOWN)		//!< 如果按下了按钮
	{
		CXTPPropertyGridInplaceButton* pButton = (CXTPPropertyGridInplaceButton*)lParam;
		m_Object->OnButtonClick(m_PropertyGrid, pButton->GetID());
// 		UINT id = pButton->GetID();
// 		id = pButton->GetItem()->GetID();
// 		if (pButton->GetItem()->GetID() == 1000 || pButton->GetID() == XTP_ID_PROPERTYGRID_EXPANDBUTTON) // 2 Buttons
// 		{
// 			AfxMessageBox(_T("Expand button"));
// 			return TRUE;
// 		}
	}

	if (wParam == XTP_PGN_SORTORDER_CHANGED)		//!< 排序方式改变
	{
// 		m_nSort = m_wndPropertyGrid.GetPropertySort();
// 		UpdateData(FALSE);
// 
// 		TRACE(_T("SortOrder Changed.\n"));
	}
	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)		//!< 项的值被改变
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		if(m_Object)		m_Object->OnItemModify(m_PropertyGrid, pItem->GetID());		//!< 修改了菜的那些色香味都得写在菜单上
// 		TRACE(_T("Value Changed. Caption = %s, ID = %i, Value = %s\n"), pItem->GetCaption(), pItem->GetID(), pItem->GetValue());
// 
// 		if (DYNAMIC_DOWNCAST(CXTPPropertyGridItemEnum, pItem))
// 		{
// 			if (pItem->GetMetrics(TRUE, FALSE))
// 			{
// 				pItem->GetMetrics(TRUE, FALSE)->m_nImage = ((CXTPPropertyGridItemEnum*)pItem)->GetEnum();
// 			}
// 		}
// 
// 		if (pItem->GetID() == 501) // Dynamic Options
// 		{
// 			CXTPPropertyGridItems* pSiblingItems = pItem->GetParentItem()->GetChilds();
// 
// 			for (int i = 0; i < pSiblingItems->GetCount(); i++)
// 			{
// 				if (pSiblingItems->GetAt(i) != pItem)
// 				{
// 					pSiblingItems->GetAt(i)->SetHidden(!((CXTPPropertyGridItemBool*)pItem)->GetBool());
// 				}
// 			}
// 		}
// 
	}
	if (wParam == XTP_PGN_SELECTION_CHANGED)		//!< 选中项改变
	{
// 		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
// 		if (pItem)
// 		{
// 			if (pItem->IsKindOf(RUNTIME_CLASS(CXTPPropertyGridItemColor)))
// 			{
// 				TRACE(_T("Color Item. Selection Changed. Item = %s\n"), pItem->GetCaption());
// 			}
// 
// 			if (pItem->GetID() == ID_ITEM_VERSION_LANGUAGE)
// 			{
// 				//pItem->SetDescription(pItem->GetDescription() + "!");
// 			}
// 
// 		}
	}
	if (wParam == XTP_PGN_EDIT_CHANGED)				//!< Edit内容改变
	{
// 		CXTPPropertyGridInplaceEdit* pEdit = DYNAMIC_DOWNCAST(CXTPPropertyGridInplaceEdit, (CWnd*)lParam);
// 		if (pEdit && pEdit->GetItem())
// 		{
// 			// Custom Validation
// 			if (pEdit->GetItem()->GetID() == ID_ITEM_VERSION_LANGUAGE)
// 			{
// 				CString str;
// 				pEdit->CEdit::GetWindowText(str);
// 
// 				if (str.GetLength() > 30)
// 				{
// 					MessageBeep((UINT)-1);
// 					pEdit->SetSel(0, -1);
// 					pEdit->ReplaceSel(str.Left(30));
// 				}
// 			}
// 			// Custom Validation
// 			if (pEdit->GetItem()->GetCaption() == _T("ItemsInMRUList"))
// 			{
// 				CString str;
// 				pEdit->CEdit::GetWindowText(str);
// 
// 				int i = _ttoi(str);
// 				if (i > 20)
// 				{
// 					MessageBeep((UINT)-1);
// 					pEdit->SetSel(0, -1);
// 					pEdit->ReplaceSel(_T("20"));
// 				}
// 			}
// 		}
	}
	return 0;
}

BOOL Dialog::CPropertyDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	if(m_Object)	m_Object->OnShowHelp();
	return CXTResizeDialog::OnHelpInfo(pHelpInfo);
}
