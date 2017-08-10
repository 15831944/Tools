#include "StdAfx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "Camera.h"
#include "CamMgr.h"
#include "PropertyCam.h"

const UINT ID_BUTTON_PATH = 1002;
const CString CAM_TITLE = _T(" ����ͷ���Ա�");
const CString CAM_NEW = _T("�½�����ͷ");

const CString CAM_BASE_INFO = _T("������Ϣ");
const CString CAM_BASE_INFO_TOOLTIP = _T("������Ϣ");
const CString CAM_RECORD_INFO = _T("¼����Ϣ");
const CString CAM_RECORD_INFO_TOOLTIP = _T("¼����Ϣ");
const CString CAM_BROADCAST_INFO = _T("�㲥��Ϣ");
const CString CAM_BROADCAST_INFO_TOOLTIP = _T("�㲥��Ϣ");
const CString CAM_TAG = _T("˵��");
const CString CAM_TAG_TOOLTIP = _T("ѡ���������ʱ������Enter����ʾ���У�����Ctrl+Enter��ϼ���ʾȷ������");
const CString CAM_RECORD = _T("����¼�ƹ���");
const CString CAM_RECORD_TOOLTIP = _T("����¼�ƹ��ܺ�ϵͳÿ��һ��Сʱ�����¼��һСʱ����Ƶ��Ϣ���浽���أ��½����ļ����Զ�����Ϊ ��-��-�� Сʱ����2011-01-01 12.wmv");
//const CString CAM_AUTOSAVE = _T("�Ƿ��Զ�������Ƶ�ļ�");
//const CString CAM_AUTOSAVE_TOOLTIP = _T("�Ƿ��Զ�������Ƶ�ļ�");
const CString CAM_PATH = _T("��Ƶ�ļ��洢λ��");
const CString CAM_PATH_TOOLTIP = _T("�趨��Ƶ�ļ�������ļ���");
const CString CAM_ENCODE = _T("��Ƶ�����ʽ");
const CString CAM_ENCODE_TOOLTIP = _T("��Ƶ�����ʽ");
const CString CAM_DISTINGUISH = _T("¼�Ʒֱ���");			//!< ,160*120,176*144,......,720*480,Ĭ��320*240
const CString CAM_DISTINGUISH_TOOLTIP = _T("¼�Ʒֱ���");
const CString CAM_FPS = _T("¼��֡����");					//!< 29.97,15,10,5,1,0.5 fps,Ĭ��1fps
const CString CAM_FPS_TOOLTIP = _T("¼��֡���ʣ���λfps��֡ÿ��");
const CString CAM_BROAD = _T("�Ƿ���������㲥");
const CString CAM_BROAD_TOOLTIP = _T("��������㲥���û������������������ͨ�����ʸ÷�����ָ��URL��������Ƶ��Ϣ");
const CString CAM_PORT = _T("�㲥�˿ں�");
const CString CAM_PORT_TOOLTIP = _T("������ù㲥���񣬱�����ͷ�Ĺ㲥�˿ںŲ����ظ�����ռ��");
const CString CAM_MAXCLIENT = _T("���������");
const CString CAM_MAXCLIENT_TOOLTIP = _T("��������㲥�󣬿�֧���û����ӵ������Ŀ����Χ 5~50");
const CString CAM_BROADENCODE = _T("�㲥��Ƶ�����ʽ");
const CString CAM_BROADENCODE_TOOLTIP = _T("�㲥��Ƶ�����ʽ");
const CString CAM_BROADDISTINGUISH = _T("�㲥�ֱ���");		//!< 160*120,176*144,......,720*480
const CString CAM_BROADDISTINGUISH_TOOLTIP = _T("�㲥�ֱ���");
const CString CAM_BROADFPS = _T("�㲥֡����");				//!< 29.97,15,10,5,1,0.5 fps
const CString CAM_BROADFPS_TOOLTIP = _T("�㲥֡���ʣ���λfps��֡ÿ��");

const UINT CAM_TAG_ID = 1;
const UINT CAM_RECORD_ID = CAM_TAG_ID + 1;
//const UINT CAM_AUTOSAVE_ID = CAM_RECORD_ID + 1;
const UINT CAM_PATH_ID = CAM_RECORD_ID + 1;
const UINT CAM_ENCODE_ID = CAM_PATH_ID + 1;
const UINT CAM_DISTINGUISH_ID = CAM_ENCODE_ID + 1;
const UINT CAM_FPS_ID = CAM_DISTINGUISH_ID + 1;
const UINT CAM_BROAD_ID = CAM_FPS_ID + 1;
const UINT CAM_PORT_ID = CAM_BROAD_ID + 1;
const UINT CAM_MAXCLIENT_ID = CAM_PORT_ID + 1;
const UINT CAM_BROADENCODE_ID = CAM_MAXCLIENT_ID + 1;
const UINT CAM_BROADDISTINGUISH_ID = CAM_BROADENCODE_ID + 1;
const UINT CAM_BROADFPS_ID = CAM_BROADDISTINGUISH_ID + 1;

using namespace MVC;
using namespace Camera;

MVC::Camera::CPropertyCamera::CPropertyCamera(void)
{
	m_NewItem = std::shared_ptr<CCamera>(new CCamera());
	m_ShowItem = std::shared_ptr<CCamera>(new CCamera());
}

MVC::Camera::CPropertyCamera::~CPropertyCamera(void)
{
}

//!< ��ʾ����
void MVC::Camera::CPropertyCamera::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("����ͷ����"));
}

//!< ��ʾ���ĺ���
void MVC::Camera::CPropertyCamera::ShowInfo(CXTPPropertyGrid& grid)
{
	if(m_bAdd){
		CreateNew();
		grid.SetWindowText(_T("�½�����"));
	}
	else{
		CreateEdit();
		grid.SetWindowText(_T("�޸ı���"));
	}

	CXTPPropertyGridItem *pGroup, *pItem;			//!< ������
	CXTPPropertyGridInplaceButton* pButton;			//!< ����·����
	CCamMgr* itemMgr = &CCamMgr::GetMe();
	std::list<CString> strList, boolList;			//!< �����б������
	int defIndex;
	CString defText;
	boolList.push_back(_T("No"));
	boolList.push_back(_T("Yes"));

	//!< ����������Ϣ
	pGroup = grid.AddCategory(CAM_BASE_INFO);
	pGroup->SetTooltip(CAM_BASE_INFO_TOOLTIP);
	pGroup->Expand();

	//!< ������ǩ
	AddItemMultiText(*pGroup, CAM_TAG, CAM_TAG_TOOLTIP, m_ShowItem->GetTag(), CAM_TAG_ID);

	pGroup = grid.AddCategory(CAM_RECORD_INFO);
	pGroup->SetTooltip(CAM_RECORD_INFO_TOOLTIP);
	pGroup->Expand();

	//!< �Ƿ�¼��
	AddItemList(*pGroup, CAM_RECORD, CAM_RECORD_TOOLTIP, boolList, m_ShowItem->IsRecord() ? 1 : 0, CAM_RECORD_ID);

// 	//!< �Ƿ��Զ�����
// 	AddItemList(*pGroup, CAM_AUTOSAVE, CAM_AUTOSAVE_TOOLTIP, boolList, m_ShowItem->IsAutoSave() ? 1 : 0, CAM_AUTOSAVE_ID);

	//!< ����·��
	grid.SetVariableItemsHeight(TRUE);
	CXTPPropertyGridItem* pItemText = AddItemText(*pGroup, CAM_PATH, CAM_PATH_TOOLTIP, m_ShowItem->GetFilePath(), CAM_PATH_ID);
	int lineCount = m_ShowItem->GetFilePath().GetLength() * 12 / pItemText->GetValueRect().Width();
	pItemText->SetMultiLinesCount(lineCount);
	pButton = pItemText->GetInplaceButtons()->AddButton(new CXTPPropertyGridInplaceButton(ID_BUTTON_PATH));
	pButton->SetCaption(_T("���"));

	//!< �����ʽ
	strList.clear();
	strList.push_back(_T("WMV2"));
	AddItemList(*pGroup, CAM_ENCODE, CAM_ENCODE_TOOLTIP, strList, 0, CAM_ENCODE_ID);

	//!< ¼�Ʒֱ���
	defIndex = -1;
	defText = m_ShowItem->GetDistinguish();
	strList.clear();
	strList.push_back(_T("160*120"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("176*144"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("240*176"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("240*180"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("320*240"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("352*240"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("352*288"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("640*240"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("640*288"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("640*480"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("720*240"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("720*288"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("720*480"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	AddItemList(*pGroup, CAM_DISTINGUISH, CAM_DISTINGUISH_TOOLTIP, strList, defIndex, CAM_DISTINGUISH_ID);

	//!< ¼��֡��
	defIndex = -1;
	defText = m_ShowItem->GetFPS();
	strList.clear();
	strList.push_back(_T("29.97fps"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("15fps"));		if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("10fps"));		if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("5fps"));		if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("1fps"));		if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("0.5fps"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	AddItemList(*pGroup, CAM_FPS, CAM_FPS_TOOLTIP, strList, defIndex, CAM_FPS_ID);

	pGroup = grid.AddCategory(CAM_BROADCAST_INFO);
	pGroup->SetTooltip(CAM_BROADCAST_INFO_TOOLTIP);
	pGroup->Expand();

	//!< �Ƿ�㲥
	AddItemList(*pGroup, CAM_BROAD, CAM_BROAD_TOOLTIP, boolList, m_ShowItem->IsBroadCast() ? 1 : 0, CAM_BROAD_ID);

	//!< �˿ں�
	AddItemNumber(*pGroup, CAM_PORT, CAM_PORT_TOOLTIP, m_ShowItem->GetPort(), CAM_PORT_ID);

	//!< �˿ں�
	AddItemNumber(*pGroup, CAM_MAXCLIENT, CAM_MAXCLIENT_TOOLTIP, m_ShowItem->GetMaxClient(), CAM_MAXCLIENT_ID);

	//!< �㲥�����ʽ
	strList.clear();
	strList.push_back(_T("WMV2"));
	AddItemList(*pGroup, CAM_BROADENCODE, CAM_BROADENCODE_TOOLTIP, strList, 0, CAM_BROADENCODE_ID);

	//!< �㲥¼�Ʒֱ���
	defIndex = -1;
	defText = m_ShowItem->GetBroadDistinguish();
	strList.clear();
	strList.push_back(_T("160*120"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("176*144"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("240*176"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("240*180"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("320*240"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("352*240"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("352*288"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("640*240"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("640*288"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("640*480"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("720*240"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("720*288"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("720*480"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	AddItemList(*pGroup, CAM_BROADDISTINGUISH, CAM_BROADDISTINGUISH_TOOLTIP, strList, defIndex, CAM_BROADDISTINGUISH_ID);

	//!< �㲥¼��֡��
	defIndex = -1;
	defText = m_ShowItem->GetBroadFPS();
	strList.clear();
	strList.push_back(_T("29.97fps"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("15fps"));		if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("10fps"));		if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("5fps"));		if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("1fps"));		if(defText == strList.back())	defIndex = strList.size() - 1;
	strList.push_back(_T("0.5fps"));	if(defText == strList.back())	defIndex = strList.size() - 1;
	AddItemList(*pGroup, CAM_BROADFPS, CAM_BROADFPS_TOOLTIP, strList, defIndex, CAM_BROADFPS_ID);
}

//!< ����ʼ����ɺ�
void MVC::Camera::CPropertyCamera::OnGridFirstShow(CXTPPropertyGrid& grid)
{
	ShowAndHide(grid);
	EnableAndDisenable(grid);
}

//!< ��ñ����������
CString MVC::Camera::CPropertyCamera::GetTitle()
{
	if(m_bAdd)			return CAM_NEW;
	else if(!m_Item)	return _T("");
	return CAM_TITLE;
}

//!< ���رձ��ʱ
void MVC::Camera::CPropertyCamera::OnCloseGrid()
{
	if(m_bAdd){			//!< �������ӣ����¼��m_NewItem��
		*m_NewItem = *m_ShowItem;
	}
	else{				//!< 
	}
}

//!< ��ĳһ��޸�ʱ
void MVC::Camera::CPropertyCamera::OnItemModify(CXTPPropertyGrid& grid, UINT id)
{
	CXTPPropertyGridItem* item = grid.FindItem(id);
	if(!item)				return;
	CString itemValue = item->GetValue();
	if(id == CAM_PORT_ID)						// �˿�
	{
		int iVal = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(iVal < 0)		iVal = 8080;
		if(iVal > 65535)	iVal = 8080;
		((CXTPPropertyGridItemNumber *)item)->SetNumber(iVal);
	}
	else if(id == CAM_MAXCLIENT_ID)				// ���ͻ���
	{
		int iVal = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(iVal < 5)		iVal = 5;
		if(iVal > 50)		iVal = 50;
		((CXTPPropertyGridItemNumber *)item)->SetNumber(iVal);
	}
	ShowAndHide(grid);
	EnableAndDisenable(grid);
}

//!< Ҫ���汻�ı������
bool MVC::Camera::CPropertyCamera::OnSaveModify(CXTPPropertyGrid& grid)
{
	int num = grid.GetCount();
	CXTPPropertyGridItem* item;
	CString itemValue;
	UINT itemID;
	CComVariant cvr;
	CCamMgr* itemMgr = &CCamMgr::GetMe();

	for(int i = 0; i < num; ++i){
		item = grid.GetItem(i);
		if(!item->IsValueChanged())		continue;			//!< ֻ�б��޸ĵĲŻ���б��棬�Ż���һ��
		itemID = item->GetID();
		itemValue = item->GetValue();

		if(itemID == CAM_TAG_ID){
			m_ShowItem->SetTag(itemValue.Trim());			// = _T("˵��");
		}
		else if(itemID == CAM_RECORD_ID){					// = _T("�Ƿ�¼��");
			m_ShowItem->SetRecord(item->GetConstraints()->GetCurrent());
		}
// 		else if(itemID == CAM_AUTOSAVE_ID){					// = _T("�Ƿ��Զ�����");
// 			m_ShowItem->SetAutoSave(item->GetConstraints()->GetCurrent());
// 		}
		else if(itemID == CAM_PATH_ID){						// = _T("����·��");
			m_ShowItem->SetFilePath(itemValue.Trim());
		}
		else if(itemID == CAM_DISTINGUISH_ID){				// = _T("�ֱ���");
			m_ShowItem->SetDistinguish(itemValue.Trim());
		}
		else if(itemID == CAM_FPS_ID){						// = _T("֡��");
			m_ShowItem->SetFPS(itemValue.Trim());
		}
		else if(itemID == CAM_BROAD_ID){					// = _T("�㲥");
			m_ShowItem->SetBroadCast(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == CAM_PORT_ID){						// = _T("�˿�");
			int nPort = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
			if(ExistSamePort(nPort)){
				AfxMessageBox(_T("�㲥�˿ںŲ����ظ�..."), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			m_ShowItem->SetPort(nPort);
		}
		else if(itemID == CAM_MAXCLIENT_ID){				// = _T("���������");
			m_ShowItem->SetMaxClient(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		else if(itemID == CAM_BROADDISTINGUISH_ID){			// = _T("�ֱ���");
			m_ShowItem->SetBroadDistinguish(itemValue.Trim());
		}
		else if(itemID == CAM_BROADFPS_ID){					// = _T("֡��");
			m_ShowItem->SetBroadFPS(itemValue.Trim());
		}
		item->SetDefaultValue(item->GetValue());
	}
	if(m_ShowItem->IsBroadCast()){
		if(ExistSamePort(m_ShowItem->GetPort())){
			AfxMessageBox(_T("�㲥�˿ںŲ����ظ�..."), MB_OK | MB_ICONEXCLAMATION);
			return false;
		}
	}
	if(m_bAdd){
		m_bAdd = false;
		grid.SetWindowText(_T("�޸ı���"));
		if(!m_NewItem)		m_NewItem = std::shared_ptr<CCamera>(new CCamera());
		*m_NewItem = *m_ShowItem;							//!< m_NewItemÿ�β�һ��
		std::shared_ptr<CCamera> newItem = std::shared_ptr<CCamera>(new CCamera());
		*newItem = *m_ShowItem;
		CCamMgr::GetMe().AddItem(newItem);
		m_NewItem->SetID(newItem->GetID());
	}
	else{
		if(m_Item)
			*m_Item = * m_ShowItem;
	}
	return true;
}

//!< ��ť������
void MVC::Camera::CPropertyCamera::OnButtonClick(CXTPPropertyGrid& grid, UINT btID)
{
	if(ID_BUTTON_PATH == btID)
	{
		CXTBrowseDialog dlg(g_App.GetMainWnd());
		dlg.SetTitle(_T("��ѡ���ļ���"));
		TCHAR path[_MAX_PATH];
		STRCPY_S(path, _MAX_PATH, m_ShowItem->GetFilePath());
		dlg.SetSelPath(path);

		if (dlg.DoModal() == IDOK)
		{
			CString path = dlg.GetSelPath();
			if(!path.IsEmpty())
			{
				if(path.Right(1) != "\\")	path = path + _T("\\");
				m_ShowItem->SetFilePath(path);
			}
		}
		CXTPPropertyGridItem* item = grid.FindItem(CAM_PATH_ID);
		if(item)		item->SetValue(m_ShowItem->GetFilePath());
	}
}

void MVC::Camera::CPropertyCamera::CreateNew()
{
	if(!m_ShowItem)		m_ShowItem = std::shared_ptr<CCamera>(new CCamera());
	*m_ShowItem = *m_NewItem;
	m_ShowItem->SetPort(CCamMgr::GetMe().GetDifferentPort(8080));
}

void MVC::Camera::CPropertyCamera::CreateEdit()
{
	if(!m_ShowItem)		m_ShowItem = std::shared_ptr<CCamera>(new CCamera());
	if(!m_Item)			return;
	*m_ShowItem = *m_Item;
}

//!< ������ͳһʹ��
void MVC::Camera::CPropertyCamera::EnableAndDisenable(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *item, *tmp;

	//!< ¼���뱣��
	item = grid.FindItem(CAM_RECORD_ID);
	if(item->GetConstraints()->GetCurrent() == 0){
//		tmp = grid.FindItem(CAM_AUTOSAVE_ID);			if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(CAM_PATH_ID);				if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(CAM_ENCODE_ID);				if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(CAM_DISTINGUISH_ID);		if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(CAM_FPS_ID);				if(tmp)		tmp->SetReadOnly(TRUE);
	}
	else{
		tmp = grid.FindItem(CAM_ENCODE_ID);				if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(CAM_DISTINGUISH_ID);		if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(CAM_FPS_ID);				if(tmp)		tmp->SetReadOnly(FALSE);
//		item = grid.FindItem(CAM_AUTOSAVE_ID);			if(item)	item->SetReadOnly(FALSE);
//		if(item->GetConstraints()->GetCurrent() == 0){
//			tmp = grid.FindItem(CAM_PATH_ID);			if(tmp)		tmp->SetReadOnly(TRUE);
//		}
//		else{
		tmp = grid.FindItem(CAM_PATH_ID);				if(tmp)		tmp->SetReadOnly(FALSE);
//		}
	}

	//!< �㲥
	item = grid.FindItem(CAM_BROAD_ID);
	if(item->GetConstraints()->GetCurrent() == 0){
		tmp = grid.FindItem(CAM_PORT_ID);				if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(CAM_MAXCLIENT_ID);			if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(CAM_BROADENCODE_ID);		if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(CAM_BROADDISTINGUISH_ID);	if(tmp)		tmp->SetReadOnly(TRUE);
		tmp = grid.FindItem(CAM_BROADFPS_ID);			if(tmp)		tmp->SetReadOnly(TRUE);
	}
	else{
		tmp = grid.FindItem(CAM_PORT_ID);				if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(CAM_MAXCLIENT_ID);			if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(CAM_BROADENCODE_ID);		if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(CAM_BROADDISTINGUISH_ID);	if(tmp)		tmp->SetReadOnly(FALSE);
		tmp = grid.FindItem(CAM_BROADFPS_ID);			if(tmp)		tmp->SetReadOnly(FALSE);
	}
}

//!< �˿��Ƿ�����
bool MVC::Camera::CPropertyCamera::ExistSamePort(int nPort)
{
	if(!m_ShowItem->IsBroadCast())						return false;
	CCamMgr* camMgr = &CCamMgr::GetMe();
	std::shared_ptr<CCamera> one;
	for (auto one : camMgr->m_vtCam)
	{
		if(!one)										continue;
		if(!one->IsBroadCast())							continue;
		if(one->GetID() == m_ShowItem->GetID())			continue;	//!< ͬһ�����ù�
		if(one->GetPort() == nPort)						return true;
	}
	return false;
}

