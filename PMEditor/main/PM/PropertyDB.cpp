#include "StdAfx.h"
#include "DBMgr.h"
#include "ProjectMgr.h"
#include "Project.h"
#include "PropertyDB.h"


const UINT ID_BUTTON_PATH = 1000;

const CString ITEM_TITLE = _T("���ݿ�����");

const CString BASE_INFO = _T("������Ϣ");
const CString ACTIVE = _T("�������ݿ�");
const CString PASSWORD = _T("���ݿ�����");
const CString MYSQLPORT=_T("���ݿ�˿�");
const CString DISKMIN = _T("Ӳ�̱���ֵ����λ��M��");
const CString SAVETIME = _T("�洢���ڣ���λ���룩");
const CString SAVETYPE = _T("�洢��ʽ");
//const CString SAVEAVERAGE = _T("ƽ��ֵ����");
const CString SAVECHANGERATE = _T("��С�仯��");
const CString TBROWMAX = _T("���ݿ�ֱ�����");
const CString BACKUPACTIVE = _T("���ö�ʱ����");
const CString BACKUPPATH = _T("���ݿⱸ��·��");
const CString BACKUPDAY = _T("���������������λ���죩");
const CString BACKUPHOUR = _T("����ʱ�䣨��λ���㣩");

const CString ACTIVE_TOOLTIP = _T("�Ƿ��������ݿ�,Ĭ������");
const CString PASSWORD_TOOLTIP = _T("�������ݿ������");
const CString MYSQLPORT_TOOLTIP=_T("�������ݿ�Ķ˿ںţ�Ĭ��Ϊ3306��");
const CString DISKMIN_TOOLTIP = _T("���ݿ���̵�ʣ��ռ�С��ָ����Сʱ��������������");
const CString SAVETIME_TOOLTIP = _T("����ֵ�洢���ݿ��ʱ����, 0.01~3600000");
const CString SAVETYPE_TOOLTIP = _T("ֱ�Ӵ洢��ÿ��ˢ������ֱ�ӱ�����ʷ����\r\n�ı��ٴ洢�����������ı�ʱ�ٱ�����ʷ����");
//const CString SAVEAVERAGE_TOOLTIP = _T("ƽ������, 1��999");
const CString SAVECHANGERATE_TOOLITP = _T("��С�仯�ʣ�0~100��0��ʾֻҪ�仯�ʹ洢");
const CString TBROWMAX_TOOLITP = _T("����ʷ���ݿ��м�¼�����������趨ֵ�󣬿����½���Ĭ��ֵΪ100000����СֵΪ100");
const CString BACKUPACTIVE_TOOLTIP = _T("�������ݿⶨʱ���ݹ���");
const CString BACKUPPATH_TOOLTIP = _T("�������ݿ⵽ָ����ַ");
const CString BACKUPDAY_TOOLTIP = _T("���ݵ����������ÿ�����������һ�α���");
const CString BACKUPHOUR_TOOLTIP = _T("���ݵľ���ʱ�䣬��ʾ���ݵ���ļ����ӽ��б���");

const UINT ACTIVE_ID = 1;
const UINT PASSWORD_ID = ACTIVE_ID + 1;
const UINT MYSQLPORT_ID = PASSWORD_ID + 1;
const UINT DISKMIN_ID = MYSQLPORT_ID+ 1;
const UINT SAVETIME_ID = DISKMIN_ID + 1;
const UINT SAVETYPE_ID = SAVETIME_ID + 1;
const UINT SAVECHANGERATE_ID = SAVETYPE_ID + 1;
const UINT TBROWMAX_ID = SAVECHANGERATE_ID + 1;
const UINT BACKUPACTIVE_ID = TBROWMAX_ID + 1;
const UINT BACKUPPATH_ID = BACKUPACTIVE_ID + 1;
const UINT BACKUPDAY_ID = BACKUPPATH_ID + 1;
const UINT BACKUPHOUR_ID = BACKUPDAY_ID + 1;

using namespace Servers;
using namespace DB;
CPropertyDB::CPropertyDB(void)
{
}

CPropertyDB::~CPropertyDB(void)
{
}

//!< ��ñ����������
CString CPropertyDB::GetTitle()
{
	return ITEM_TITLE;
}

//!< ��ʾ���ĺ���
void CPropertyDB::ShowInfo(CXTPPropertyGrid& grid)
{
	Servers::DB::CDBMgr* dbMgr = &DB::CDBMgr::GetMe();
	CXTPPropertyGridItem* pGroup;					//!< ������
	CXTPPropertyGridInplaceButton* pButton;
	std::list<CString> strList;

	//!< ����������Ϣ
	pGroup = grid.AddCategory(BASE_INFO);
	pGroup->SetTooltip(BASE_INFO);
	pGroup->Expand();

	//!< ���ݿ����룬���ȡ����ͨ���жϱ����Ƿ񱣴���ʷ�������趨���ֵ
// 	strList.clear();
// 	strList.push_back(_T("No"));
// 	strList.push_back(_T("Yes"));
// 	AddItemList(*pGroup, ACTIVE, ACTIVE_TOOLTIP, strList, dbMgr->IsActive() ? 1 : 0, ACTIVE_ID);

	//!< ���ݿ�����
	AddItemText(*pGroup, PASSWORD, PASSWORD_TOOLTIP, dbMgr->getPassWord(), PASSWORD_ID)->SetPasswordMask('*');

	//!<���ݿ����Ӷ˿�

	AddItemNumber(*pGroup, MYSQLPORT, MYSQLPORT_TOOLTIP, dbMgr->getMySqlPort(), MYSQLPORT_ID);

	//!< Ӳ�̱���ֵ
	AddItemNumber(*pGroup, DISKMIN, DISKMIN_TOOLTIP, dbMgr->getDiskMin(), DISKMIN_ID);

	//!< �洢����
	AddItemDouble(*pGroup, SAVETIME, SAVETIME_TOOLTIP, dbMgr->getSaveTime(), SAVETIME_ID, FALSE, 2);

	//!< �ֱ�����
	AddItemNumber(*pGroup, TBROWMAX, TBROWMAX_TOOLITP, dbMgr->getTbRowMax(), TBROWMAX_ID);

	//!< �洢��ʽ
	strList.clear();
	strList.push_back(_T("ֱ�Ӵ洢"));
	strList.push_back(_T("�ı��ٴ洢"));
	AddItemList(*pGroup, SAVETYPE, SAVETYPE_TOOLTIP, strList, dbMgr->getSaveType(), SAVETYPE_ID);

	//!< �仯��
	AddItemNumber(*pGroup, SAVECHANGERATE, SAVECHANGERATE_TOOLITP, dbMgr->getChangeRate(), SAVECHANGERATE_ID);

	//!< ����ʹ��
	std::list<CString> boolList;			//!< �����б������
	boolList.push_back(_T("No"));
	boolList.push_back(_T("Yes"));
	AddItemList(*pGroup, BACKUPACTIVE, BACKUPACTIVE_TOOLTIP, boolList, dbMgr->IsBackUp() ? 1 : 0, BACKUPACTIVE_ID);

	//!< ���ݿⱸ��·��
	CXTPPropertyGridItem* pItemText = AddItemText(*pGroup, BACKUPPATH, BACKUPPATH_TOOLTIP, dbMgr->getBackupPath(), BACKUPPATH_ID);
	pButton = pItemText->GetInplaceButtons()->AddButton(new CXTPPropertyGridInplaceButton(ID_BUTTON_PATH));
	pButton->SetCaption(_T("�༭"));

	//!< �����������
	AddItemNumber(*pGroup, BACKUPDAY, BACKUPDAY_TOOLTIP, dbMgr->getBackupDay(), BACKUPDAY_ID);

	//!< ����ʱ��
	strList.clear();
	CString text;
	for(int i = 0; i < 24; ++i){
		text.Format("%d��",i);
		strList.push_back(text);
	}
	AddItemList(*pGroup, BACKUPHOUR, BACKUPHOUR_TOOLTIP, strList, dbMgr->getBackupHour(), BACKUPHOUR_ID);
}

//!< ����ʼ����ɺ�
void CPropertyDB::OnGridFirstShow(CXTPPropertyGrid& grid)
{
	EnableAndDisenable(grid);
}

//!< ������ͳһʹ��
void CPropertyDB::EnableAndDisenable(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *item, *tmp;

	//!< ����ʹ��
	item = grid.FindItem(BACKUPACTIVE_ID);
	if(item->GetConstraints()->GetCurrent() == 0){
		tmp = grid.FindItem(BACKUPPATH_ID);		if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(BACKUPDAY_ID);		if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(BACKUPHOUR_ID);		if(tmp)		tmp->SetReadOnly(TRUE);
	}
	else{
		tmp = grid.FindItem(BACKUPPATH_ID);		if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(BACKUPDAY_ID);		if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(BACKUPHOUR_ID);		if(tmp)		tmp->SetReadOnly(FALSE);
	}

	//!< �洢��ʽ
	item = grid.FindItem(SAVETYPE_ID);
	if(item->GetConstraints()->GetCurrent() != 1){			// 1�Ǹı��ٴ洢����Ҫ���û���仯�ٷֱ�
		tmp = grid.FindItem(SAVECHANGERATE_ID);	if(tmp)		tmp->SetHidden(TRUE);
	}
	else{
		tmp = grid.FindItem(SAVECHANGERATE_ID);	if (tmp)	tmp->SetHidden(FALSE);
	}
}

//!< ���رձ��ʱ
void CPropertyDB::OnCloseGrid()
{
}

//!< ��ĳһ��޸�ʱ
void CPropertyDB::OnItemModify(CXTPPropertyGrid& grid, UINT id)
{
	EnableAndDisenable(grid);
	CXTPPropertyGridItem *item;

	//!< �洢����
	if(SAVETIME_ID == id)
	{
		item = grid.FindItem(SAVETIME_ID);
		float fValue = ((CXTPPropertyGridItemDouble *)item)->GetDouble();
		//int nValue = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(fValue <= 0.0099999)			fValue = 600.0;
		if(fValue >= 3600000.00001)		fValue = 3600000.0;
		((CXTPPropertyGridItemDouble *)item)->SetDouble(fValue);
	}

	//!< �仯��
	if(SAVECHANGERATE_ID == id)
	{
		item = grid.FindItem(SAVECHANGERATE_ID);
		int nValue = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(nValue < 0)			nValue = 0;
		if(nValue > 100)		nValue = 100;
		((CXTPPropertyGridItemNumber *)item)->SetNumber(nValue);
	}

	//!< �ֱ�����
	if (TBROWMAX_ID == id)
	{
		item = grid.FindItem(TBROWMAX_ID);
		UINT nValue = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if (nValue < 100)		nValue = 100;
		((CXTPPropertyGridItemNumber *)item)->SetNumber(nValue);
	}
}

//!< Ҫ���汻�ı������
bool CPropertyDB::OnSaveModify(CXTPPropertyGrid& grid)
{
	int num = grid.GetCount();
	CXTPPropertyGridItem* item;
	CString itemValue;
	UINT itemID;
	CComVariant cvr;
	Servers::DB::CDBMgr* dbMgr = &DB::CDBMgr::GetMe();
	for(int i = 0; i < num; ++i)
	{
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			//!< ֻ�б��޸ĵĲŻ���б��棬�Ż���һ��
		itemID = item->GetID();
		itemValue = item->GetValue();
		cvr = itemValue.Trim();
		
// 		if(itemID == ACTIVE_ID){							// = _T("�������ݿ�");
// 			dbMgr->setActived(item->GetConstraints()->GetCurrent() == 1 ? true : false);
// 		}
		if(itemID == PASSWORD_ID){						// = _T("���ݿ�����");
			dbMgr->setPassWord(itemValue.Trim());
		}
		else if(itemID==MYSQLPORT_ID){					//=_T("���ݿ�˿�");
			cvr.ChangeType(VT_I4);
			dbMgr->setMySqlPort(cvr.intVal);
		}
		else if(itemID == DISKMIN_ID){						// = _T("Ӳ����С����");
			cvr.ChangeType(VT_I4);
			dbMgr->setDiskMin((UINT)cvr.intVal);
		}
		else if(itemID == SAVETIME_ID){						// = _T("�洢���ڣ���");
			cvr.ChangeType(VT_R4);
			dbMgr->setSaveTime(cvr.fltVal);			
		}
		else if(itemID == SAVETYPE_ID){						// = _T("�洢��ʽ");
			cvr.ChangeType(VT_I4);
			dbMgr->setSaveType(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == SAVECHANGERATE_ID){				// = _T("�仯��");
			cvr.ChangeType(VT_I4);
			dbMgr->setChangeRate((UINT)cvr.intVal);
		}
		else if (itemID == TBROWMAX_ID){					// = _T("�ֱ�����");
			cvr.ChangeType(VT_UI4);
			dbMgr->setTbRowMax((UINT)cvr.intVal);
		}
		else if(itemID == BACKUPACTIVE_ID){					// = _T("����ʹ��");
			dbMgr->setBackUp(item->GetConstraints()->GetCurrent() == 1 ? true : false);
		}
		else if(itemID == BACKUPPATH_ID){					// = _T("���ݵ�ַ");
			dbMgr->setBackupPath(itemValue.Trim());
		}
		else if(itemID == BACKUPDAY_ID){					// = _T("�����������");
			cvr.ChangeType(VT_I4);
			dbMgr->setBackupDay((UINT)cvr.intVal);
		}
		else if(itemID == BACKUPHOUR_ID){					// = _T("����ʱ�䣬��");
			dbMgr->setBackupHour((UINT)item->GetConstraints()->GetCurrent());
		}
	}
	return true;
}

//!< ��ť������
void CPropertyDB::OnButtonClick(CXTPPropertyGrid& grid, UINT btID)
{
	if(btID == ID_BUTTON_PATH)
	{
		CXTBrowseDialog dlg(&grid);
		dlg.SetTitle(_T("ѡ�񱸷����ڵ�Ŀ¼"));
		CString defPath = CProjectMgr::GetMe().GetProj()->GetPath();
		if (!defPath.IsEmpty())
		{
			TCHAR path[_MAX_PATH];
			STRCPY_S(path, _MAX_PATH, defPath);
			dlg.SetSelPath(path);
		}
		if (dlg.DoModal() == IDOK)
		{
			defPath = dlg.GetSelPath();
			if(defPath.GetAt(defPath.GetLength() - 1) != '\\')
				defPath = defPath + _T("\\");
			CXTPPropertyGridItem* item = grid.FindItem(BACKUPPATH_ID);
			item->SetValue(defPath);
		}
	}
}

//!< ��ʾ������Ϣ
void CPropertyDB::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("��ʷ���ݿ�"));
}
