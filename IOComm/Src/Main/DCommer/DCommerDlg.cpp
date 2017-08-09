// DCommerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DCommer.h"
#include "DCommerDlg.h"
#include "Gbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const UINT LIMIT_OUTPUT = 50000;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
// �Ի�������
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD){}
void CAboutDlg::DoDataExchange(CDataExchange* pDX){CDialog::DoDataExchange(pDX);}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CDCommerDlg �Ի���
CDCommerDlg::CDCommerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDCommerDlg::IDD, pParent)
	, m_uiOverTime(1000)
	, m_nLen(0)
	, m_nRadioHex(0)
	, m_nSendType(0)
	, m_bWatch(FALSE)
	, m_uiXunHuanNum(1)
	, m_uiComState(0)
	, m_uiXunHuanCount(0)
	, m_uiDelay(1000)
	, m_nID(0)
	, m_nPort(11000)
	, m_nWatchPort(12000)
	, m_uiIP(3232235957)		//!< 192.168.1.181�ĵ���
	, m_nCommType(0)
	, m_uiCountSend(0)		//!< ���͵�����
	, m_uiCountRcv(0)		//!< ���յ�����
	, m_uiCountBad(0)		//!< ��������
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_UDP = CreateComm(Comm::NET_UDP);
	m_UDPWatch = CreateComm(Comm::NET_UDP_WATCH);
	m_TCPClient = CreateComm(Comm::NET_TCPCLIENT);
	//m_Serial = CreateComm(Comm::NET_SERIALBOOST);
	m_Serial = CreateComm(Comm::NET_SERIAL);
}

CDCommerDlg::~CDCommerDlg()
{
	m_Comm->DisWatch(this);
	m_UDPWatch->DisWatch(this);
	m_TCPClient->DisWatch(this);
}

void CDCommerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OVERTIME, m_uiOverTime);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
	DDX_Text(pDX, IDC_EDIT_RECEIVE, m_strReceive);
	DDX_Control(pDX, IDC_CB_COM, m_cbCom);
	DDX_Control(pDX, IDC_CB_BAUD, m_cbBaud);
	DDX_Control(pDX, IDC_CB_CHECK, m_cbCheck);
	DDX_Control(pDX, IDC_CB_DATA, m_cbData);
	DDX_Control(pDX, IDC_CB_STOP, m_cbStop);
	DDX_Radio(pDX, IDC_RADIO_TYPE, m_nRadioHex);
	DDX_Radio(pDX, IDC_RADIO_SENDTYPE, m_nSendType);
	DDX_Check(pDX, IDC_CHECK_WATCH, m_bWatch);
	DDX_Text(pDX, IDC_EDIT_XUNHUAN, m_uiXunHuanNum);
	DDX_Text(pDX, IDC_EDIT_DELAY, m_uiDelay);
	DDX_Control(pDX, IDC_EDIT_RECEIVE, m_CtrlRev);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDX_Text(pDX, IDC_EDIT_PORT2, m_nWatchPort);
	DDX_IPAddress(pDX, IDC_IP, m_uiIP);
	DDX_Radio(pDX, IDC_RADIO_NET1, m_nCommType);
	DDX_Text(pDX, IDC_EDIT_COUNT_SEND, m_uiCountSend);
	DDX_Text(pDX, IDC_EDIT_COUNT_RCV, m_uiCountRcv);
	DDX_Text(pDX, IDC_EDIT_COUNT_BAD, m_uiCountBad);
}

BEGIN_MESSAGE_MAP(CDCommerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BT_SEND, &CDCommerDlg::OnBnClickedBtSend)
	ON_BN_CLICKED(IDC_BT_SEND, &CDCommerDlg::OnBnClickedBtSend)
	ON_BN_CLICKED(IDC_BT_CRC, &CDCommerDlg::OnBnClickedBtCrc)
	ON_BN_CLICKED(IDC_BT_BCC, &CDCommerDlg::OnBnClickedBtBcc)
	ON_BN_CLICKED(IDCANCEL, &CDCommerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BT_CLEAN_SEND, &CDCommerDlg::OnBnClickedBtCleanSend)
	ON_BN_CLICKED(IDC_BT_CLEAR_REV, &CDCommerDlg::OnBnClickedBtClearRev)
	ON_BN_CLICKED(IDC_BT_CLEAN_STATE, &CDCommerDlg::OnBnClickedBtCleanState)
	ON_BN_CLICKED(IDC_RADIO_SENDTYPE, &CDCommerDlg::OnBnClickedRadioSendtype)
	ON_BN_CLICKED(IDC_RADIO_SENDTYPE1, &CDCommerDlg::OnBnClickedRadioSendtype)
	ON_BN_CLICKED(IDC_CHECK_WATCH, &CDCommerDlg::OnBnClickedCheckWatch)
	ON_BN_CLICKED(IDC_RADIO_NET1, &CDCommerDlg::OnBnClickedRadioNet1)
	ON_BN_CLICKED(IDC_RADIO_NET2, &CDCommerDlg::OnBnClickedRadioNet1)
	ON_BN_CLICKED(IDC_RADIO_NET3, &CDCommerDlg::OnBnClickedRadioNet1)
	ON_BN_CLICKED(IDABOUT, &CDCommerDlg::OnBnClickedAbout)
END_MESSAGE_MAP()


// CDCommerDlg ��Ϣ�������

BOOL CDCommerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	CGbl::InitReport();
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	std::list<UINT> ltCom;
	CGbl::GetSerialCount(ltCom);
	CString str;
	foreach(UINT id, ltCom)
	{
		str.Format("COM%d", id);
		m_cbCom.AddString(str);
	}
	if(ltCom.size() > 0)	m_cbCom.SetCurSel(0);

	m_cbBaud.AddString("1200");
	m_cbBaud.AddString("2400");
	m_cbBaud.AddString("4800");
	m_cbBaud.AddString("9600");
	m_cbBaud.AddString("19200");
	m_cbBaud.AddString("38400");
	m_cbBaud.AddString("57600");
	m_cbBaud.AddString("115200");
	m_cbBaud.SetCurSel(3);

	m_cbCheck.AddString("��У��");
	m_cbCheck.AddString("��У��");
	m_cbCheck.AddString("żУ��");
	m_cbCheck.AddString("0У��");
	m_cbCheck.AddString("1У��");
	m_cbCheck.SetCurSel(0);

	m_cbData.AddString("8");
	m_cbData.AddString("7");
	m_cbData.SetCurSel(0);

	m_cbStop.AddString("1");
	m_cbStop.AddString("2");
	m_cbStop.SetCurSel(0);

	SetWindowText("DCommer V" + CGbl::getVersionStr(1) + "." + CGbl::getVersionStr(2));
	InitCommType();
	SetTimer(2, 300, NULL);
	UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDCommerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDCommerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CDCommerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CDCommerDlg::BindSerial()
{
	CString strInfo, str, strError;
	if(m_nCommType == 0)
	{
		m_cbCom.GetWindowText(str);		strInfo = str;
		m_cbBaud.GetWindowText(str);	strInfo = strInfo + ":" + str;
		m_cbCheck.GetWindowText(str);	strInfo = strInfo + ":" + str;
		m_cbData.GetWindowText(str);	strInfo = strInfo + ":" + str;
		m_cbStop.GetWindowText(str);	strInfo = strInfo + ":" + str;
		strError = _T("�󶨴���ʧ�ܣ�");
	}
	else if(m_nCommType == 1 || m_nCommType == 2)
	{
		strInfo.Format("%d", (WORD)m_nPort);
		str = CGbl::GetIPFromNumber(m_uiIP);
		std::vector<CString> vtStr;
		CGbl::SpliteBy(str, _T("."), vtStr);
		strInfo = vtStr[3] + _T(".") + vtStr[2] + _T(".") + vtStr[1] + _T(".") + vtStr[0] + _T(":") + strInfo;
		strError = _T("�����ʼ��ʧ�ܣ�");
	}
	if(strInfo == m_Comm->GetInfo())	return true;
	if(!m_Comm->SetInfo(strInfo) || !m_Comm->StartCom())
	{
		m_strReceive = strError;
		UpdateData(FALSE);
		return false;
	}
	return true;
}

void CDCommerDlg::OnBnClickedBtSend()
{
	UpdateData(TRUE);
	if(m_nSendType == 0)		//!< ���η��ͺͽ���
	{
		if(!SetData() || !BindSerial())		return;
		if(m_uiOverTime < 100)			m_uiOverTime = 100;
		if(m_uiOverTime > 3600000)		m_uiOverTime = 3600000;
//		for(int i = 0; i < 10; ++i)		//!< ����10������
		m_Comm->Send(true, m_Data, m_nLen, m_uiOverTime, this);
		m_nID = 0;
		m_uiCountSend++;
		ClearRev();
	}
	else						//!< ѭ�����ͺͽ���
	{
		if(m_uiComState == 0)
		{
			if(!SetData() || !BindSerial())	return;
			GetDlgItem(IDC_BT_SEND)->SetWindowText("ֹͣ");
			m_uiComState = 1;
//			if(m_uiDelay < m_uiOverTime)	m_uiDelay = m_uiOverTime;
			m_uiXunHuanCount = 0;
			SetTimer(1, m_uiDelay, NULL);
			SendXunHuan();
			m_nID = 0;
			ClearRev();
		}
		else
		{
			GetDlgItem(IDC_BT_SEND)->SetWindowText("����");
			m_uiComState = 0;
			KillTimer(1);
		}
	}
}

void CDCommerDlg::SendXunHuan()
{
	if(m_uiXunHuanNum <= m_uiXunHuanCount++)
	{
		GetDlgItem(IDC_BT_SEND)->SetWindowText("����");
		m_uiComState = 0;
		m_uiXunHuanCount = 0;
		return;
	}
	if(m_uiOverTime < 100)			m_uiOverTime = 100;
	if(m_uiOverTime > 3600000)		m_uiOverTime = 3600000;
	m_uiCountSend++;
	UpdateData(FALSE);
	m_Comm->Send(true, m_Data, m_nLen, m_uiOverTime, this);
}

void CDCommerDlg::OnBnClickedBtCrc()
{
	UpdateData(TRUE);
	if(!SetData())	return;
	short scrc = CGbl::CRC16(m_Data, m_nLen);
	byte* bcrc = (byte *)&scrc;
	CString str;
	str.Format("%.2X%.2X", bcrc[1], bcrc[0]);
	m_strSend += str;
	UpdateData(FALSE);
}

void CDCommerDlg::OnBnClickedBtBcc()
{
	UpdateData(TRUE);
	if(!SetData())	return;
	byte bcc = CGbl::BCC(m_Data, m_nLen);
	CString str;
	str.Format("%.2X", bcc);
	m_strSend += str;
	UpdateData(FALSE);
}

void CDCommerDlg::OnBnClickedCancel()
{
	OnCancel();
}

//!< ���ַ���������Ҫ���͵ı���
bool CDCommerDlg::SetData()
{
	UpdateData(TRUE);
	memset(m_Data, 0, 64000);
	CString strSrc = m_strSend;
	strSrc.Remove(' ');
	int len = strSrc.GetLength();
	if(len % 2 == 1)
	{
		MessageBox("���ݳ���ӦΪż����", "��ʾ", MB_OK | MB_ICONWARNING);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_SEND));
		return false;
	}
	CString str = strSrc;
	str.MakeUpper();
	for(int i = 0; i < len; i += 2)
	{
		int val = 0;
		if(sscanf(str.Mid(i, 2), "%X", &val) != 1)
		{
			MessageBox("����\"" + strSrc.Mid(i, 2) + "\"��ʶ��", "��ʾ", MB_OK | MB_ICONWARNING);
			GotoDlgCtrl(GetDlgItem(IDC_EDIT_SEND));
			return false;
		}
		m_Data[i / 2] = (byte)val;
	}
	m_nLen = len / 2;
	return true;
}

void CDCommerDlg::OnReceive(Comm::IComm* commer, byte* data, int len, CString info /* _T("") */)
{
	//UpdateData(TRUE);
	CString str, text;
	if(m_nRadioHex == 0)
	{
		for(int i = 0; i < len; ++i)
		{
			str.Format("%.2X ", data[i]);
			text += str;
		}
	}
	else
	{
		data[len] = 0;
		char* cStr;
		int i = 0;
		do 
		{
			cStr = (char *)&data[i];
			text += (CString)cStr;
			i = i + strlen(cStr) + 1;
		} while (i < len);
	}
	str.Format("(%d) ", ++m_nID);
	str = str + CGbl::GetCulTime();
	CString strLen;
	strLen.Format("    �ֽ���: %d  ", len);
	str = str + strLen;
	if(info != _T(""))		str = str + info;
	text = str + _T("\r\n") + text;
	m_strReceive = text + _T("\r\n\r\n") + m_strReceive;
	CheckOutput();
	//m_CtrlRev.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	m_uiCountRcv++;
	UpdateData(FALSE);
}

void CDCommerDlg::OnOverTime(Comm::IComm* commer)
{
	UpdateData(TRUE);
	CString text;
	text.Format("(%d) ", ++m_nID);
	text = text + CGbl::GetCulTime() + _T("\r\n��ʱ");
	m_strReceive = text + _T("\r\n\r\n") + m_strReceive;
	m_uiCountBad++;
	//m_CtrlRev.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	UpdateData(FALSE);
}

void CDCommerDlg::OnBnClickedBtCleanSend()
{
	UpdateData(TRUE);
	if(!SetData())	return;
	CString str, text;
	for(int i = 0; i < m_nLen; ++i)
	{
		str.Format("%.2X ", m_Data[i]);
		text += str;
	}
	m_strSend = text;
	UpdateData(FALSE);
}

void CDCommerDlg::OnBnClickedBtCleanState()
{
	m_uiCountSend = m_uiCountRcv = m_uiCountBad = 0;
	UpdateData(FALSE);
}

void CDCommerDlg::OnBnClickedBtClearRev()
{
	m_strSend = _T("");
	ClearRev();
}

void CDCommerDlg::ClearRev()
{
	m_strReceive = _T("");
	UpdateData(FALSE);
}

void CDCommerDlg::OnBnClickedRadioSendtype()
{
	int nSendType = m_nSendType;
	UpdateData(TRUE);
	if(nSendType == m_nSendType)	return;
	if(m_nSendType == 0)
	{
		GetDlgItem(IDC_EDIT_XUNHUAN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_XUNHUAN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_DELAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DELAY)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_XUNHUAN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_XUNHUAN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_DELAY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DELAY)->ShowWindow(SW_SHOW);
	}
}

//!< ѡ��֮��ֱ���������
void CDCommerDlg::OnBnClickedCheckWatch()
{
	UpdateData(TRUE);
	if(m_bWatch)	//!< �������
	{
		if(m_nCommType == 0)
		{
			BindSerial();
			m_Comm->StartCom();
			m_Comm->WatchData(this);
		}
		else if(m_nCommType == 2)
		{
			BindSerial();
			if(!m_TCPClient->WatchData(this))
				m_strReceive = _T("����ʧ��");
			else
				m_strReceive = _T("���ӳɹ�");
			UpdateData(FALSE);
			return;
		}
		else
		{
			CString info;
			info.Format("%d",m_nWatchPort);
			if(!m_UDPWatch->SetInfo(info) || !m_UDPWatch->WatchData(this))
			{
				m_strReceive = _T("�����˿ڰ�ʧ��");
				UpdateData(FALSE);
				return;
			}
		}
	}
	else			//!< ֹͣ���
	{
		m_Comm->DisWatch(this);
		if(m_nCommType == 0)		m_Comm->DisWatch(this);
		else if(m_nCommType == 1)	m_UDPWatch->DisWatch(this);
		else						m_TCPClient->DisWatch(this);
	}
}

unsigned long lastRcvCount = 0;
void CDCommerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		if(m_uiComState == 1)	SendXunHuan();
	}
	else if (nIDEvent == 2)
	{
		if (lastRcvCount != m_uiCountRcv){
			CDataExchange dx(this, FALSE);
			DDX_Text(&dx, IDC_EDIT_RECEIVE, m_strReceive);
			lastRcvCount = m_uiCountRcv;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CDCommerDlg::OnBnClickedRadioNet1()
{
	int commType = m_nCommType;
	UpdateData(TRUE);
	if(commType != m_nCommType)	InitCommType();
	if(m_Comm == m_TCPClient){
		GetDlgItem(IDC_CHECK_WATCH)->SetWindowText(_T("����"));
		GetDlgItem(IDC_EDIT_PORT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DATA_PORT)->ShowWindow(SW_HIDE);
	}
	else{
		GetDlgItem(IDC_CHECK_WATCH)->SetWindowText(_T("���"));
		GetDlgItem(IDC_EDIT_PORT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DATA_PORT)->ShowWindow(SW_SHOW);
	}
}

//!< ��ͨ�����͸ı�ʱ���³�ʼ��
void CDCommerDlg::InitCommType()
{
	m_Serial->StopCom();
	m_UDP->StopCom();
	m_UDPWatch->StopCom();
	m_TCPClient->StopCom();
	Comm::IComm* oldComm = m_Comm;
	if(m_nCommType == 0)		//!< ��ǰ�Ǵ���ͨ��
	{
		GetDlgItem(IDC_CB_COM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CB_BAUD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CB_CHECK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CB_DATA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CB_STOP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_STOP)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_IP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PORT2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_COM_IP)->SetWindowText(_T("����"));
		GetDlgItem(IDC_STATIC_CHECK_PROT)->SetWindowText(_T("У��"));
		GetDlgItem(IDC_STATIC_DATA_PORT)->SetWindowText(_T("����λ"));

		m_Comm = m_Serial;
	}
	else if(m_nCommType == 1 || m_nCommType == 2)	//!< ��ǰ��UDP/TCPͨ��
	{
		GetDlgItem(IDC_CB_COM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CB_BAUD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CB_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CB_DATA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CB_STOP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_STOP)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_IP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PORT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PORT2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_COM_IP)->SetWindowText(_T("Ŀ��IP"));
		GetDlgItem(IDC_STATIC_CHECK_PROT)->SetWindowText(_T("Ŀ�Ķ˿�"));
		GetDlgItem(IDC_STATIC_DATA_PORT)->SetWindowText(_T("�����˿�"));

		if(m_nCommType == 1)		m_Comm = m_UDP;
		else						m_Comm = m_TCPClient;
	}

	//!< ����˳���TCP,�����϶Ͽ�TCP����
	if(oldComm == m_TCPClient && m_Comm != m_TCPClient)
		m_TCPClient->DisWatch(this);
	m_bWatch = false;
	m_uiXunHuanNum = 0;
	KillTimer(1);
	UpdateData(FALSE);
}


void CDCommerDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

//!< ������ƣ�ֻ�����LIMIT_OUTPUT���ַ���ͨ������
void CDCommerDlg::CheckOutput()
{
	UINT len = m_strReceive.GetLength();
	if(len <= LIMIT_OUTPUT)	return;
	m_strReceive = m_strReceive.Left(LIMIT_OUTPUT);
}
