// ProjGuideDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PMApp.h"
#include "AboutDlg.h"
#include "ProjGuideDlg.h"


// CProjGuideDlg 对话框
const UINT COL_PROJ_NAME = 0;								//!!< 工程名
const UINT COL_PROJ_DESC = COL_PROJ_NAME + 1;				//!!< 备注
const UINT COL_PROJ_AUTHOR = COL_PROJ_DESC + 1;				//!!< 作者
const UINT COL_PROJ_VERSION = COL_PROJ_AUTHOR + 1;			//!!< 版本
const UINT COL_PROJ_CREATE_TIME = COL_PROJ_VERSION + 1;		//!!< 创建时间
const UINT COL_PROJ_PATH = COL_PROJ_CREATE_TIME + 1;		//!!< 路径

using namespace Dialog;
using namespace ReportProj;
IMPLEMENT_SERIAL(CProjRecord, CXTPReportRecord, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CProjRecordIcon, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
//////////////////////////////////////////////////////////////////////////
// CMessageRecordItemIcon

CProjRecordIcon::CProjRecordIcon(){UpdateIcon();}
void CProjRecordIcon::UpdateIcon(){SetIconIndex(4);}
CString CProjRecordIcon::GetGroupCaption(CXTPReportColumn* /*pColumn*/){return _T("Message status : 工程");}
int CProjRecordIcon::CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem)
{return GetGroupCaption(pColumn).Compare(pItem->GetGroupCaption(pColumn));}
void CProjRecordIcon::DoPropExchange(CXTPPropExchange* pPX){CXTPReportRecordItem::DoPropExchange(pPX);}

//////////////////////////////////////////////////////////////////////////
CProjRecord::CProjRecord()
{m_uiID = UINT(-1);}
CProjRecord::CProjRecord(UINT i){
	m_uiID = i;
	CString text;
	text.Format("Solution%d",i);
	AddItem(new CXTPReportRecordItemText(text));
	AddItem(new CXTPReportRecordItemText(text + _T(" Description.")));
	text.Format("Author%d",i);
	AddItem(new CXTPReportRecordItemText(text));
	text.Format("Version%d",i);
	AddItem(new CXTPReportRecordItemText(text));
	text.Format("Time%d",i);
	AddItem(new CXTPReportRecordItemText(text));
	text.Format("Path%d",i);
	AddItem(new CXTPReportRecordItemText(text));
}
CProjRecord::~CProjRecord(){}
void CProjRecord::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{CXTPReportRecord::GetItemMetrics(pDrawArgs, pItemMetrics);}



IMPLEMENT_DYNAMIC(CProjGuideDlg, CDialogSampleDlgBase)

CProjGuideDlg::CProjGuideDlg(CWnd* pParent /*=NULL*/)
	: CDialogSampleDlgBase(CProjGuideDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CProjGuideDlg::~CProjGuideDlg()
{
}

void CProjGuideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSampleDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROJ_CTRL, m_ProjCtrl);
}

BEGIN_MESSAGE_MAP(CProjGuideDlg, CXTResizeDialog)
	ON_WM_PAINT()
	ON_COMMAND(ID_PROJ_NEW, &CProjGuideDlg::OnProjNew)
	ON_COMMAND(ID_PROJ_DEL, &CProjGuideDlg::OnProjDel)
	ON_COMMAND(ID_PROJ_OPEN, &CProjGuideDlg::OnProjOpen)
	ON_COMMAND(ID_PROJ_BACKUP, &CProjGuideDlg::OnProjBackup)
	ON_COMMAND(ID_IOSERVER, &CProjGuideDlg::OnIDIOServer)
	ON_COMMAND(ID_RUN, &CProjGuideDlg::OnRun)
	ON_COMMAND(ID_HELP, &CProjGuideDlg::OnHelp)
	ON_COMMAND(ID_CANCLE, &CProjGuideDlg::OnCancel)
END_MESSAGE_MAP()


// CProjGuideDlg 消息处理程序

BOOL CProjGuideDlg::OnInitDialog()
{
	CDialogSampleDlgBase::OnInitDialog();
	// 初始化ToolBar
	VERIFY(InitCommandBars());
	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->SetMenu(_T("Menu Bar"), IDR_PROJ_MENU);
	m_ToolBar = pCommandBars->Add(_T("Standard"), xtpBarTop);
	m_ToolBar->LoadToolBar(IDR_PROJ_TOOLBAR);

	((CXTPControlButton*)m_ToolBar->GetControls()->FindControl(xtpControlButton, ID_PROJ_NEW, TRUE, FALSE))->SetStyle(xtpButtonIconAndCaption);
	((CXTPControlButton*)m_ToolBar->GetControls()->FindControl(xtpControlButton, ID_PROJ_OPEN, TRUE, FALSE))->SetStyle(xtpButtonIconAndCaption);
	((CXTPControlButton*)m_ToolBar->GetControls()->FindControl(xtpControlButton, ID_PROJ_SEARCH, TRUE, FALSE))->SetStyle(xtpButtonIconAndCaption);
	((CXTPControlButton*)m_ToolBar->GetControls()->FindControl(xtpControlButton, ID_PROJ_BACKUP, TRUE, FALSE))->SetStyle(xtpButtonIconAndCaption);
	((CXTPControlButton*)m_ToolBar->GetControls()->FindControl(xtpControlButton, ID_PROJ_RECOVER, TRUE, FALSE))->SetStyle(xtpButtonIconAndCaption);
	((CXTPControlButton*)m_ToolBar->GetControls()->FindControl(xtpControlButton, ID_PROJ_DEL, TRUE, FALSE))->SetStyle(xtpButtonIconAndCaption);
	((CXTPControlButton*)m_ToolBar->GetControls()->FindControl(xtpControlButton, ID_IOSERVER, TRUE, FALSE))->SetStyle(xtpButtonIconAndCaption);
	((CXTPControlButton*)m_ToolBar->GetControls()->FindControl(xtpControlButton, ID_RUN, TRUE, FALSE))->SetStyle(xtpButtonIconAndCaption);
	((CXTPControlButton*)m_ToolBar->GetControls()->FindControl(xtpControlButton, ID_HELP, TRUE, FALSE))->SetStyle(xtpButtonIconAndCaption);
	((CXTPControlButton*)m_ToolBar->GetControls()->FindControl(xtpControlButton, ID_CANCLE, TRUE, FALSE))->SetStyle(xtpButtonIconAndCaption);

	m_ToolBar->GetControls()->CreateOriginalControls();

	XTPImageManager()->SetIcons(IDR_MAINFRAME);
	pCommandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowWindowsDefault);
	pCommandBars->GetToolTipContext()->SetStyle(xtpToolTipOffice);
	LoadCommandBars(_T("CommandBars"));

	// 初始化表格
	m_ProjCtrl.SetOwner(this);
	CBitmap bp1, bp2;
	VERIFY(bp1.LoadBitmap(IDB_BMP_ITEMMEMORY));
	VERIFY(bp2.LoadBitmap(IDB_BMP_ITEMIO));

	if (!m_ImgList.Create(16,16, ILC_COLOR32|ILC_MASK, 0, 1))		return -1;
	m_ImgList.Add(&bp1, RGB(255, 255, 255));
	m_ImgList.Add(&bp2, RGB(255, 255, 255));
	m_ProjCtrl.SetImageList(&m_ImgList);

	m_ProjCtrl.ResetContent();
	m_ProjCtrl.AddColumn(new CXTPReportColumn(COL_PROJ_NAME, _T("名称"), 17));
	m_ProjCtrl.AddColumn(new CXTPReportColumn(COL_PROJ_PATH, _T("路径"), 40));
	m_ProjCtrl.AddColumn(new CXTPReportColumn(COL_PROJ_AUTHOR, _T("作者"), 10));
	m_ProjCtrl.AddColumn(new CXTPReportColumn(COL_PROJ_VERSION, _T("版本"), 10));
	m_ProjCtrl.AddColumn(new CXTPReportColumn(COL_PROJ_CREATE_TIME, _T("创建时间"), 18));
	m_ProjCtrl.AddColumn(new CXTPReportColumn(COL_PROJ_DESC, _T("备注"), 25));
	InitData();
	m_ProjCtrl.Populate();
	OnReportLClick(&m_ProjCtrl, 0);
	RepositionTooBar();

	// Set control resizing.
	SetResize(IDC_PROJ_CTRL, SZ_TOP_LEFT, SZ_TOP_CENTER);

	//CXTPPaintManager::SetTheme(xtpThemeWhidbey);
	CXTPPaintManager::SetTheme(xtpThemeVisualStudio2010);
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE);

	FreshToolBar();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CProjGuideDlg::FreshToolBar()
{
	if(!m_ToolBar)		return;
}

void CProjGuideDlg::InitData()
{
	for(int i = 0; i < 3; i++)
		m_ProjCtrl.AddRecord(new CProjRecord(i));
}

void CProjGuideDlg::RepositionTooBar()
{
	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);

	if ((GetStyle() & WS_MINIMIZE) || (rcClientStart.IsRectEmpty()))		return;
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

void Dialog::CProjGuideDlg::OnPaint()
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
		CDialogSampleDlgBase::OnPaint();
	}
}

void Dialog::CProjGuideDlg::OnProjNew()
{
}

void Dialog::CProjGuideDlg::OnProjDel()
{
}

void Dialog::CProjGuideDlg::OnProjOpen()
{
}

void Dialog::CProjGuideDlg::OnProjBackup()
{
}

void Dialog::CProjGuideDlg::OnIDIOServer()
{
}

void Dialog::CProjGuideDlg::OnRun()
{
	// 运行服务器

	OnCancel();
}

void Dialog::CProjGuideDlg::OnHelp()
{
	CAboutDlg dlg;
	dlg.DoModal();
	FreshToolBar();
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE);
}
