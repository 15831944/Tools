#include "StdAfx.h"
#include "PMApp.h"
#include "Gbl.h"
#include "Camera.h"
#include "CamMgr.h"
#include "PropertyCam.h"

const UINT ID_BUTTON_PATH = 1002;
const CString CAM_TITLE = _T(" 摄像头属性表");
const CString CAM_NEW = _T("新建摄像头");

const CString CAM_BASE_INFO = _T("基本信息");
const CString CAM_BASE_INFO_TOOLTIP = _T("基本信息");
const CString CAM_RECORD_INFO = _T("录制信息");
const CString CAM_RECORD_INFO_TOOLTIP = _T("录制信息");
const CString CAM_BROADCAST_INFO = _T("广播信息");
const CString CAM_BROADCAST_INFO_TOOLTIP = _T("广播信息");
const CString CAM_TAG = _T("说明");
const CString CAM_TAG_TOOLTIP = _T("选择多行输入时，键盘Enter键表示换行，键盘Ctrl+Enter组合键表示确认输入");
const CString CAM_RECORD = _T("启动录制功能");
const CString CAM_RECORD_TOOLTIP = _T("启动录制功能后，系统每隔一个小时将最近录制一小时的视频信息保存到本地，新建立文件名自动命名为 年-月-日 小时，如2011-01-01 12.wmv");
//const CString CAM_AUTOSAVE = _T("是否自动保存视频文件");
//const CString CAM_AUTOSAVE_TOOLTIP = _T("是否自动保存视频文件");
const CString CAM_PATH = _T("视频文件存储位置");
const CString CAM_PATH_TOOLTIP = _T("设定视频文件保存的文件夹");
const CString CAM_ENCODE = _T("视频编码格式");
const CString CAM_ENCODE_TOOLTIP = _T("视频编码格式");
const CString CAM_DISTINGUISH = _T("录制分辨率");			//!< ,160*120,176*144,......,720*480,默认320*240
const CString CAM_DISTINGUISH_TOOLTIP = _T("录制分辨率");
const CString CAM_FPS = _T("录制帧速率");					//!< 29.97,15,10,5,1,0.5 fps,默认1fps
const CString CAM_FPS_TOOLTIP = _T("录制帧速率，单位fps，帧每秒");
const CString CAM_BROAD = _T("是否启动网络广播");
const CString CAM_BROAD_TOOLTIP = _T("启动网络广播后，用户可以在其他计算机上通过访问该服务器指定URL来监视视频信息");
const CString CAM_PORT = _T("广播端口号");
const CString CAM_PORT_TOOLTIP = _T("如果启用广播服务，本摄像头的广播端口号不能重复，或被占用");
const CString CAM_MAXCLIENT = _T("最大连接数");
const CString CAM_MAXCLIENT_TOOLTIP = _T("启动网络广播后，可支持用户连接的最大数目，范围 5~50");
const CString CAM_BROADENCODE = _T("广播视频编码格式");
const CString CAM_BROADENCODE_TOOLTIP = _T("广播视频编码格式");
const CString CAM_BROADDISTINGUISH = _T("广播分辨率");		//!< 160*120,176*144,......,720*480
const CString CAM_BROADDISTINGUISH_TOOLTIP = _T("广播分辨率");
const CString CAM_BROADFPS = _T("广播帧速率");				//!< 29.97,15,10,5,1,0.5 fps
const CString CAM_BROADFPS_TOOLTIP = _T("广播帧速率，单位fps，帧每秒");

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

//!< 显示帮助
void MVC::Camera::CPropertyCamera::OnShowHelp()
{
	SoftInfo::CMyHelp::GetMe().ShowHelp(_T("摄像头属性"));
}

//!< 显示表格的函数
void MVC::Camera::CPropertyCamera::ShowInfo(CXTPPropertyGrid& grid)
{
	if(m_bAdd){
		CreateNew();
		grid.SetWindowText(_T("新建变量"));
	}
	else{
		CreateEdit();
		grid.SetWindowText(_T("修改变量"));
	}

	CXTPPropertyGridItem *pGroup, *pItem;			//!< 属性组
	CXTPPropertyGridInplaceButton* pButton;			//!< 查找路径的
	CCamMgr* itemMgr = &CCamMgr::GetMe();
	std::list<CString> strList, boolList;			//!< 下拉列表的内容
	int defIndex;
	CString defText;
	boolList.push_back(_T("No"));
	boolList.push_back(_T("Yes"));

	//!< 变量基本信息
	pGroup = grid.AddCategory(CAM_BASE_INFO);
	pGroup->SetTooltip(CAM_BASE_INFO_TOOLTIP);
	pGroup->Expand();

	//!< 变量标签
	AddItemMultiText(*pGroup, CAM_TAG, CAM_TAG_TOOLTIP, m_ShowItem->GetTag(), CAM_TAG_ID);

	pGroup = grid.AddCategory(CAM_RECORD_INFO);
	pGroup->SetTooltip(CAM_RECORD_INFO_TOOLTIP);
	pGroup->Expand();

	//!< 是否录制
	AddItemList(*pGroup, CAM_RECORD, CAM_RECORD_TOOLTIP, boolList, m_ShowItem->IsRecord() ? 1 : 0, CAM_RECORD_ID);

// 	//!< 是否自动保存
// 	AddItemList(*pGroup, CAM_AUTOSAVE, CAM_AUTOSAVE_TOOLTIP, boolList, m_ShowItem->IsAutoSave() ? 1 : 0, CAM_AUTOSAVE_ID);

	//!< 变量路径
	grid.SetVariableItemsHeight(TRUE);
	CXTPPropertyGridItem* pItemText = AddItemText(*pGroup, CAM_PATH, CAM_PATH_TOOLTIP, m_ShowItem->GetFilePath(), CAM_PATH_ID);
	int lineCount = m_ShowItem->GetFilePath().GetLength() * 12 / pItemText->GetValueRect().Width();
	pItemText->SetMultiLinesCount(lineCount);
	pButton = pItemText->GetInplaceButtons()->AddButton(new CXTPPropertyGridInplaceButton(ID_BUTTON_PATH));
	pButton->SetCaption(_T("浏览"));

	//!< 编码格式
	strList.clear();
	strList.push_back(_T("WMV2"));
	AddItemList(*pGroup, CAM_ENCODE, CAM_ENCODE_TOOLTIP, strList, 0, CAM_ENCODE_ID);

	//!< 录制分辨率
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

	//!< 录制帧速
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

	//!< 是否广播
	AddItemList(*pGroup, CAM_BROAD, CAM_BROAD_TOOLTIP, boolList, m_ShowItem->IsBroadCast() ? 1 : 0, CAM_BROAD_ID);

	//!< 端口号
	AddItemNumber(*pGroup, CAM_PORT, CAM_PORT_TOOLTIP, m_ShowItem->GetPort(), CAM_PORT_ID);

	//!< 端口号
	AddItemNumber(*pGroup, CAM_MAXCLIENT, CAM_MAXCLIENT_TOOLTIP, m_ShowItem->GetMaxClient(), CAM_MAXCLIENT_ID);

	//!< 广播编码格式
	strList.clear();
	strList.push_back(_T("WMV2"));
	AddItemList(*pGroup, CAM_BROADENCODE, CAM_BROADENCODE_TOOLTIP, strList, 0, CAM_BROADENCODE_ID);

	//!< 广播录制分辨率
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

	//!< 广播录制帧速
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

//!< 表格初始化完成后
void MVC::Camera::CPropertyCamera::OnGridFirstShow(CXTPPropertyGrid& grid)
{
	ShowAndHide(grid);
	EnableAndDisenable(grid);
}

//!< 获得本窗体的名称
CString MVC::Camera::CPropertyCamera::GetTitle()
{
	if(m_bAdd)			return CAM_NEW;
	else if(!m_Item)	return _T("");
	return CAM_TITLE;
}

//!< 当关闭表格时
void MVC::Camera::CPropertyCamera::OnCloseGrid()
{
	if(m_bAdd){			//!< 如果是添加，则记录到m_NewItem中
		*m_NewItem = *m_ShowItem;
	}
	else{				//!< 
	}
}

//!< 当某一项被修改时
void MVC::Camera::CPropertyCamera::OnItemModify(CXTPPropertyGrid& grid, UINT id)
{
	CXTPPropertyGridItem* item = grid.FindItem(id);
	if(!item)				return;
	CString itemValue = item->GetValue();
	if(id == CAM_PORT_ID)						// 端口
	{
		int iVal = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(iVal < 0)		iVal = 8080;
		if(iVal > 65535)	iVal = 8080;
		((CXTPPropertyGridItemNumber *)item)->SetNumber(iVal);
	}
	else if(id == CAM_MAXCLIENT_ID)				// 最大客户数
	{
		int iVal = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
		if(iVal < 5)		iVal = 5;
		if(iVal > 50)		iVal = 50;
		((CXTPPropertyGridItemNumber *)item)->SetNumber(iVal);
	}
	ShowAndHide(grid);
	EnableAndDisenable(grid);
}

//!< 要保存被改变的内容
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
		if(!item->IsValueChanged())		continue;			//!< 只有被修改的才会进行保存，优化了一下
		itemID = item->GetID();
		itemValue = item->GetValue();

		if(itemID == CAM_TAG_ID){
			m_ShowItem->SetTag(itemValue.Trim());			// = _T("说明");
		}
		else if(itemID == CAM_RECORD_ID){					// = _T("是否录制");
			m_ShowItem->SetRecord(item->GetConstraints()->GetCurrent());
		}
// 		else if(itemID == CAM_AUTOSAVE_ID){					// = _T("是否自动保存");
// 			m_ShowItem->SetAutoSave(item->GetConstraints()->GetCurrent());
// 		}
		else if(itemID == CAM_PATH_ID){						// = _T("保存路径");
			m_ShowItem->SetFilePath(itemValue.Trim());
		}
		else if(itemID == CAM_DISTINGUISH_ID){				// = _T("分辨率");
			m_ShowItem->SetDistinguish(itemValue.Trim());
		}
		else if(itemID == CAM_FPS_ID){						// = _T("帧速");
			m_ShowItem->SetFPS(itemValue.Trim());
		}
		else if(itemID == CAM_BROAD_ID){					// = _T("广播");
			m_ShowItem->SetBroadCast(item->GetConstraints()->GetCurrent());
		}
		else if(itemID == CAM_PORT_ID){						// = _T("端口");
			int nPort = ((CXTPPropertyGridItemNumber *)item)->GetNumber();
			if(ExistSamePort(nPort)){
				AfxMessageBox(_T("广播端口号不能重复..."), MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			m_ShowItem->SetPort(nPort);
		}
		else if(itemID == CAM_MAXCLIENT_ID){				// = _T("最大连接数");
			m_ShowItem->SetMaxClient(((CXTPPropertyGridItemNumber *)item)->GetNumber());
		}
		else if(itemID == CAM_BROADDISTINGUISH_ID){			// = _T("分辨率");
			m_ShowItem->SetBroadDistinguish(itemValue.Trim());
		}
		else if(itemID == CAM_BROADFPS_ID){					// = _T("帧速");
			m_ShowItem->SetBroadFPS(itemValue.Trim());
		}
		item->SetDefaultValue(item->GetValue());
	}
	if(m_ShowItem->IsBroadCast()){
		if(ExistSamePort(m_ShowItem->GetPort())){
			AfxMessageBox(_T("广播端口号不能重复..."), MB_OK | MB_ICONEXCLAMATION);
			return false;
		}
	}
	if(m_bAdd){
		m_bAdd = false;
		grid.SetWindowText(_T("修改变量"));
		if(!m_NewItem)		m_NewItem = std::shared_ptr<CCamera>(new CCamera());
		*m_NewItem = *m_ShowItem;							//!< m_NewItem每次不一样
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

//!< 按钮被按下
void MVC::Camera::CPropertyCamera::OnButtonClick(CXTPPropertyGrid& grid, UINT btID)
{
	if(ID_BUTTON_PATH == btID)
	{
		CXTBrowseDialog dlg(g_App.GetMainWnd());
		dlg.SetTitle(_T("请选择文件夹"));
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

//!< 在这里统一使能
void MVC::Camera::CPropertyCamera::EnableAndDisenable(CXTPPropertyGrid& grid)
{
	CXTPPropertyGridItem *item, *tmp;

	//!< 录制与保存
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

	//!< 广播
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

//!< 端口是否重了
bool MVC::Camera::CPropertyCamera::ExistSamePort(int nPort)
{
	if(!m_ShowItem->IsBroadCast())						return false;
	CCamMgr* camMgr = &CCamMgr::GetMe();
	std::shared_ptr<CCamera> one;
	for (auto one : camMgr->m_vtCam)
	{
		if(!one)										continue;
		if(!one->IsBroadCast())							continue;
		if(one->GetID() == m_ShowItem->GetID())			continue;	//!< 同一个不用管
		if(one->GetPort() == nPort)						return true;
	}
	return false;
}

