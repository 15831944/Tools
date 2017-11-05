// AddItemGroupDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PMApp.h"
#include "MyHelp.h"
#include "Gbl.h"
#include "AddItemGroupDlg.h"
#include "ItemMgr.h"
#include "ItemGroup.h"


// CAddItemGroupDlg �Ի���
using namespace Dialog;
IMPLEMENT_DYNAMIC(CAddItemGroupDlg, CDialog)

CAddItemGroupDlg::CAddItemGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddItemGroupDlg::IDD, pParent)
{
	m_strGroupName = _T("");
	SetParentName(_T("System"));
	m_strOldGroupName = _T("");
	m_bShowTree = true;
	m_GroupTree.SetOwner(this);
	m_CulItem = NULL;
}

CAddItemGroupDlg::~CAddItemGroupDlg()
{
}

void CAddItemGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GROUP_NAME, m_strGroupName);
	DDV_MaxChars(pDX, m_strGroupName, 100);
	DDX_Text(pDX, IDC_PARENT_NAME, m_strParentName);
	DDX_Control(pDX, IDC_GROUP_TREE, m_GroupTree);
}

CAddItemGroupDlg& CAddItemGroupDlg::GetMe()
{
	static CAddItemGroupDlg dlg;
	return dlg;
}

BEGIN_MESSAGE_MAP(CAddItemGroupDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_SHOWTREE, &CAddItemGroupDlg::OnBnClickedBtShowtree)
	ON_BN_CLICKED(IDCANCEL, &CAddItemGroupDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CAddItemGroupDlg::OnBnClickedOk)
	ON_WM_CREATE()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CAddItemGroupDlg ��Ϣ�������
INT_PTR CAddItemGroupDlg::DoModal(CString parentName/* = _T("System")*/, UINT groupID/* = -1*/)
{
	SetParentName(parentName);
	m_strOldGroupName = m_strGroupName;
	if(groupID != -1)
	{
		m_uiGroupID = groupID;
		std::shared_ptr<MVC::Item::CItemGroup> group = MVC::Item::CItemMgr::GetMe().GetGroup(groupID);
		if(group){
			m_strGroupName = group->getName();
			m_uiParentID = group->getParentID();
			m_strParentName = MVC::Item::CItemMgr::GetMe().GetGroup(m_uiParentID)->getName();
		}
	}
	else
		m_uiGroupID = -1;
	return CDialog::DoModal();
}

int Dialog::CAddItemGroupDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CAddItemGroupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	MVC::Item::CItemMgr* mgr = &MVC::Item::CItemMgr::GetMe();
	// ��ʼ������
	if(m_uiGroupID == -1)
	{
		if(m_strGroupName == _T(""))		m_strGroupName = _T("�½�������");
		else{
			int id = 1;
			CString strHead = m_strGroupName;
			//if(CGbl::GetNumberFromString(strHead, id, 1))	++id;
			CGbl::GetNumberFromString(strHead, id, 1);
			CString name;
			do {									// ���������Ϊ���ҵ�һ�������������һ��������ı���������
				if(id == 0)		name = strHead;
				else{
					char tmp = strHead.GetAt(strHead.GetLength() - 1);
					name.Format("%d",id);
					name = strHead + name;
				}
				if(!mgr->GetGroup(name))			break;
			} while (++id);							// ��������Ͼ�����ѭ�������Ժ�߲��ÿ���û�ҵ������⣬һ�����ҵ�
			m_strGroupName = name;
		}
		SetWindowText(_T("�½�������"));
		UpdateData(FALSE);
	}
	else
	{
		SetWindowText(_T("�޸ı�����"));
		UpdateData(FALSE);
	}

	// ��ʼ�����οؼ�
	m_CulItem = NULL;
	m_GroupTree.InitImg();
	std::shared_ptr<MVC::Item::CItemGroup> group = mgr->GetGroup(0);
	if(group){
		HTREEITEM root = m_GroupTree.InsertItem(group->getName(), 6, 7, TVI_ROOT, TVI_LAST);
		AddGroupChild(root, group->getID());
		m_GroupTree.Expand(root, TVE_EXPAND);
		if(group->getID() == m_uiParentID)
			m_CulItem = root;
	}
	if(m_CulItem){
		m_GroupTree.SelectItem(m_CulItem);
		m_GroupTree.SelectDropTarget(m_CulItem);
	}

	// ��ʼ�����������
	if(!mgr->GetGroup(m_strParentName))
	{
		SetParentName(group->getName());
		UpdateData(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAddItemGroupDlg::AddGroupChild(HTREEITEM root, UINT parentID)
{
	MVC::Item::CItemMgr* mgr = &MVC::Item::CItemMgr::GetMe();
	if(!mgr->GetGroup(parentID))		return;
	HTREEITEM item = NULL;
	std::list<UINT> ltChildID = mgr->GetGroup(parentID)->getGroupIDList();
	for(UINT i : ltChildID){
		if(!mgr->GetGroup(i))	continue;
		item = m_GroupTree.InsertItem(mgr->GetGroup(i)->getName(), 6, 7, root, TVI_LAST);
		AddGroupChild(item, i);
		m_GroupTree.Expand(item, TVE_EXPAND);
		if(i == m_uiParentID){
			m_CulItem = item;
		}
	}
}

void CAddItemGroupDlg::OnBnClickedBtShowtree()
{
	CRect rect;
	GetWindowRect(&rect);
	if(m_bShowTree){		// �����ǰ����ʾ����ô���ڸĳ�����
		GetDlgItem(IDC_GROUP_TREE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BT_SHOWTREE)->SetWindowText(_T(">>"));
		MoveWindow(rect.left, rect.top, 227, 152, TRUE);
	}
	else{					// �����ǰ�����أ���ô���ڸĳ���ʾ
		GetDlgItem(IDC_GROUP_TREE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BT_SHOWTREE)->SetWindowText(_T("<<"));
		MoveWindow(rect.left, rect.top, 408, 260, TRUE);
	}
	m_bShowTree = !m_bShowTree;
	UpdateData(FALSE);
}

void CAddItemGroupDlg::OnBnClickedCancel()
{
	SetParentName(m_strOldGroupName);
	OnCancel();
}

void CAddItemGroupDlg::OnBnClickedOk()
{
	UpdateData();
	m_strGroupName = m_strGroupName.Trim();
	UpdateData(FALSE);
	if(m_strGroupName == _T("")){
		MessageBox(_T("���Ʋ���Ϊ�գ�"), _T("����"), MB_OK|MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_GROUP_NAME));
		return;
	}
	if(!CGbl::GetCorrectFileName(m_strGroupName, true)){
		GotoDlgCtrl(GetDlgItem(IDC_GROUP_NAME));
		return;
	}
	if(m_uiGroupID == -1)		// �½�����
	{	// �����µı������Ƿ��Ѿ�����
		std::shared_ptr<MVC::Item::CItemGroup> group = MVC::Item::CItemMgr::GetMe().GetGroup(m_strGroupName);
		if(group && group->getID() != m_uiGroupID){
			MessageBox(_T("����Ϊ��") + m_strGroupName + _T("���ı������Ѵ���"), _T("����"), MB_OK|MB_ICONEXCLAMATION);
			GotoDlgCtrl(GetDlgItem(IDC_GROUP_NAME));
			return;
		}
	}
	else						// �޸ı���
	{
		std::shared_ptr<MVC::Item::CItemGroup> group = MVC::Item::CItemMgr::GetMe().GetGroup(m_uiGroupID);
		std::shared_ptr<MVC::Item::CItemGroup> parent = MVC::Item::CItemMgr::GetMe().GetGroup(m_strParentName);
		if(group->IsChildGroup(parent->getID(), true))
		{
			MessageBox(_T("������ϵ����Ƕ�ף�"), _T("����"), MB_OK);
			GotoDlgCtrl(GetDlgItem(IDC_GROUP_TREE));
			return;
		}
		std::shared_ptr<MVC::Item::CItemGroup> groupTmp = MVC::Item::CItemMgr::GetMe().GetGroup(m_strGroupName);
		if(m_strGroupName != group->getName() && groupTmp)
		{
			MessageBox(_T("����Ϊ��") + m_strGroupName + _T("���ı������Ѵ���"), _T("����"), MB_OK|MB_ICONEXCLAMATION);
			GotoDlgCtrl(GetDlgItem(IDC_GROUP_NAME));
			return;
		}
	}
	OnOK();
}

void CAddItemGroupDlg::OnTreeDblClick(CTreeCtrl* treeCtrl, HTREEITEM item)
{
	OnTreeLClick(treeCtrl, item);
}

void CAddItemGroupDlg::OnTreeLClick(CTreeCtrl* treeCtrl, HTREEITEM item)
{
	UpdateData(TRUE);
	SetParentName(treeCtrl->GetItemText(item));
	UpdateData(FALSE);
}

void CAddItemGroupDlg::OnTreeRClick(CTreeCtrl* treeCtrl, HTREEITEM item, CPoint point)
{
	OnTreeLClick(treeCtrl, item);
}

void CAddItemGroupDlg::OnTreeKeyDown(CTreeCtrl* treeCtrl, HTREEITEM item, UINT nChar)
{
	if(nChar == VK_RETURN){
		treeCtrl->SelectDropTarget(item);
		OnTreeLClick(treeCtrl, item);
	}
}

void CAddItemGroupDlg::SetParentName(CString name)
{
	std::shared_ptr<MVC::Item::CItemGroup> group;
	group = MVC::Item::CItemMgr::GetMe().GetGroup(name);
	if(!group)		return;
	m_strParentName = group->getName();
	m_uiParentID = group->getID();
}

BOOL Dialog::CAddItemGroupDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("������"));
	return CDialog::OnHelpInfo(pHelpInfo);
}
