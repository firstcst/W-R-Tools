
// WriteToolsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteTools.h"
#include "WriteToolsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern CWriteToolsDlg *global_dlg = NULL;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWriteToolsDlg 对话框



CWriteToolsDlg::CWriteToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WRITETOOLS_DIALOG, pParent)
	, m_edit_sn(_T(""))
	, m_edit_bt(_T(""))
	, m_edit_wf(_T(""))
	, m_edit_imei1(_T(""))
	, m_edit_imei2(_T(""))
	, m_readradio_ctrl(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWriteToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_com_port);
	DDX_Control(pDX, IDC_EDIT_SN, m_edit_ctrl);
	DDX_Text(pDX, IDC_EDIT_SN, m_edit_sn);
	DDX_Control(pDX, IDC_RICHEDIT_MSG, m_rich_msg);
	DDX_Control(pDX, IDC_EDIT_BT, m_edit_btctrl);
	DDX_Text(pDX, IDC_EDIT_BT, m_edit_bt);
	DDX_Control(pDX, IDC_EDIT_WF, m_edit_wfctrl);
	DDX_Text(pDX, IDC_EDIT_WF, m_edit_wf);
	DDX_Control(pDX, IDC_EDIT_IMEI1, m_edit_imctrl1);
	DDX_Text(pDX, IDC_EDIT_IMEI1, m_edit_imei1);
	DDX_Control(pDX, IDC_EDIT_IMEI2, m_edit_imctrl2);
	DDX_Text(pDX, IDC_EDIT_IMEI2, m_edit_imei2);
	DDX_Control(pDX, IDC_SN_CHECK, m_check1);
	DDX_Control(pDX, IDC_BT_CHECK, m_check2);
	DDX_Control(pDX, IDC_WIFI_CHECK, m_check3);
	DDX_Control(pDX, IDC_IMEI1_CHECK, m_check4);
	DDX_Control(pDX, IDC_IMEI2_CHECK, m_check5);
	DDX_Control(pDX, IDC_FLAG_CHECK, m_check6);
}

BEGIN_MESSAGE_MAP(CWriteToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CWriteToolsDlg::OnBnClickedOk)
	ON_CBN_DROPDOWN(IDC_COMBO_PORT, &CWriteToolsDlg::OnDropdownComboPort)
	ON_COMMAND(IDC_READ_RADIO, &CWriteToolsDlg::OnReadRadio)
	ON_COMMAND(IDC_WRITE_RADIO, &CWriteToolsDlg::OnWriteRadio)
	ON_BN_CLICKED(IDC_SN_CHECK, &CWriteToolsDlg::OnClickedSnCheck)
	ON_BN_CLICKED(IDC_BT_CHECK, &CWriteToolsDlg::OnClickedBtCheck)
	ON_BN_CLICKED(IDC_WIFI_CHECK, &CWriteToolsDlg::OnClickedWifiCheck)
	ON_BN_CLICKED(IDC_IMEI1_CHECK, &CWriteToolsDlg::OnClickedImei1Check)
	ON_BN_CLICKED(IDC_IMEI2_CHECK, &CWriteToolsDlg::OnClickedImei2Check)
	ON_COMMAND(IDC_CHECK_RADIO, &CWriteToolsDlg::OnCheckRadio)
END_MESSAGE_MAP()


// CWriteToolsDlg 消息处理程序

BOOL CWriteToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

//	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	global_dlg = this;      //此项未定义的话，后续相关类调用global_dlg指针会报错，初始化类指针

	//初始化显示界面为读取状态
	//((CButton *)GetDlgItem(IDC_READ_RADIO))->SetCheck(TRUE);
	global_dlg->OnWriteRadio();	//初始化状态为写号。
	CheckRadioButton(IDC_WRITE_RADIO, IDC_CHECK_RADIO, IDC_WRITE_RADIO);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWriteToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWriteToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWriteToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWriteToolsDlg::OnDropdownComboPort()
{
	// TODO: 在此添加控件通知处理程序代码
	//CPort m_comprt;
	m_comprt.EunmPort();
}

bool CWriteToolsDlg::ReadCode()
{
	CString port;
	int idx = m_com_port.GetCurSel();	//com port选定与否返回值，-1为未做选择。

	if (idx != -1)
	{
		m_com_port.GetLBText(idx, port);	//将获得的com port值存到idx中。
	}
	else
	{
		//m_rich_msg.SetWindowTextA("未选择com port!");
		//m_rich_msg.ReplaceSel("未选择com port!\r\n" );
		global_dlg->PringMsg("未选择com port!");
	}
	if (!m_comprt.OpenPort(port, 5))
	{
		return false;
	}
	if(1 == m_check1.GetCheck())
	{
		m_comprt.ReadSN();	//调用ReadSN
	}
	if (1 == m_check2.GetCheck())
	{
		m_comprt.ReadBT();	//调用ReadBT
	}
	if (1 == m_check3.GetCheck())
	{
		m_comprt.ReadWF();	//调用ReadWF
	}
	if (1 == m_check4.GetCheck())
	{
		m_comprt.ReadIMEI();
	}
	if (1 == m_check6.GetCheck())
	{
		m_comprt.ReadFLAG();
	}
	return true;
}

void Read_CODE(LPVOID parm)
{
	CWriteToolsDlg *dlg = (CWriteToolsDlg *)parm;
	dlg->ReadCode();
}

//void Write_CODE(LPVOID parm)
//{
//	CWriteToolsDlg *dlg1 = (CWriteToolsDlg *)parm;
//	if(1 == dlg1->m_check1.GetCheck())
//	{
//		int len = dlg1->m_edit_sn.GetLength();;
//		unsigned char str;
//
//		if (len == 15)
//		{
//			for (int i = 0; i < len; )
//			{
//				str = (dlg1->m_edit_sn.GetAt(i));
//				if ((str >= 'A'&&str <= 'Z') || (str >= '0'&&str <= '9'))
//				{
//					if (i == len - 1)
//					{
//						dlg1->WriteCode_SN(dlg1->m_edit_sn, 0);
//						break;
//					}
//					else
//					{
//						i++;
//					}
//				}
//				else
//				{
//					//global_dlg->m_rich_msg.SetWindowTextA("输入内容不符合要求！\r\n");
//					global_dlg->PringMsg("输入内容不符合要求,请输入大写字母与数字组合！");
//					dlg1->m_edit_ctrl.SetWindowTextA("");
//					break;
//				}
//			}
//		}
//		else
//		{
//			//global_dlg->m_rich_msg.SetWindowTextA("输入内容未达设定位数！\r\n");
//			if (len == 0)
//			{
//				global_dlg->PringMsg("输入内容不能为空！");
//				dlg1->m_edit_ctrl.SetWindowTextA("");
//			}
//			else
//			{
//				global_dlg->PringMsg("输入内容长度有误！");
//				dlg1->m_edit_ctrl.SetWindowTextA("");
//			}
//		}
//	}
//	if (1 == dlg1->m_check2.GetCheck())
//	{
//		//Write_BT();
//	}
//}
bool CWriteToolsDlg::WriteCode_SN(CString code, int type)
{
	CString port;
	int idx = m_com_port.GetCurSel();	//com port选定与否返回值，-1为未做选择。

	if (idx != -1)
	{
		m_com_port.GetLBText(idx, port);	//将获得的com port值存到idx中。
	}
	else
	{
		//m_rich_msg.SetWindowTextA("未选择com port!");
		//m_rich_msg.ReplaceSel("未选择com port!\r\n" );
		global_dlg->PringMsg("未选择com port!");
		global_dlg->m_edit_ctrl.SetWindowTextA("");
		return false;
	}
	if (!m_comprt.OpenPort(port, 5))
	{
		return false;
	}

	//if (!m_comprt.WritePSN(code))
	//{
	//	return false;
	//}
	//if (1 == m_check1.GetCheck())
	//{
		m_comprt.WritePSN(code);
		PringMsg(code + ("已写入"));
		m_edit_ctrl.SetWindowTextA("");	//编辑框内容清除
	//}
	//if (1 == m_check2.GetCheck())
	//{
	//	m_comprt.WriteBT(code);
	//	PringMsg(code + ("已写入"));
	//	m_edit_ctrl.SetWindowTextA("");	//编辑框内容清除
	//}
	return true;
}

bool CWriteToolsDlg::WriteCode_BT(CString code, int type)
{
	CString port;
	int idx = m_com_port.GetCurSel();	//com port选定与否返回值，-1为未做选择。

	if (idx != -1)
	{
		m_com_port.GetLBText(idx, port);	//将获得的com port值存到idx中。
	}
	else
	{
		//m_rich_msg.SetWindowTextA("未选择com port!");
		//m_rich_msg.ReplaceSel("未选择com port!\r\n" );
		global_dlg->PringMsg("未选择com port!");
	}
	if (!m_comprt.OpenPort(port, 5))
	{
		return false;
	}

	//if (!m_comprt.WritePSN(code))
	//{
	//	return false;
	//}
	//PringMsg(code);
	m_comprt.WriteBT(code);
	PringMsg("BT已写入");
	return true;
}

bool CWriteToolsDlg::WriteCode_WF(CString code, int type)
{
	CString port;
	int idx = m_com_port.GetCurSel();	//com port选定与否返回值，-1为未做选择。
	unsigned int len = code.GetLength();
	code.MakeUpper();
	if (idx != -1)
	{
		m_com_port.GetLBText(idx, port);	//将获得的com port值存到idx中。
	}
	else
	{
		//m_rich_msg.SetWindowTextA("未选择com port!");
		//m_rich_msg.ReplaceSel("未选择com port!\r\n" );
		global_dlg->PringMsg("未选择com port!");
	}
	if (!m_comprt.OpenPort(port, 5))
	{
		return false;
	}
	if (len == 12)
	{
		m_comprt.WriteWIFI(code);
	}
	else
	{
		PringMsg("WIFI输入内容位数有误！");
	}
	return true;
}

bool CWriteToolsDlg::WriteCode_IE(CString code, int type)
{
	CString port;
	int idx = m_com_port.GetCurSel();	//com port选定与否返回值，-1为未做选择。
	unsigned int len = code.GetLength();
	if (idx != -1)
	{
		m_com_port.GetLBText(idx, port);	//将获得的com port值存到idx中。
	}
	else
	{
		//m_rich_msg.SetWindowTextA("未选择com port!");
		//m_rich_msg.ReplaceSel("未选择com port!\r\n" );
		global_dlg->PringMsg("未选择com port!");
	}
	if (!m_comprt.OpenPort(port, 5))
	{
		return false;
	}
	if (len == 14)
	{
		m_comprt.WriteIMEI(code);
	}
	else
	{
		PringMsg("写入位数有误！");
	}
	return true;
}

void Write_SN(LPVOID parm)
{
	CWriteToolsDlg *dlg1 = (CWriteToolsDlg *)parm;
	int len = dlg1->m_edit_sn.GetLength();;
	unsigned char str;

	if (len == 15)
  {
	  for (int i = 0; i < len; )
		{
			str = (dlg1->m_edit_sn.GetAt(i));
			if (( str>='A'&&str<='Z') ||(str>='0'&&str<='9'))
			{
				if(i==len-1)
				{
					dlg1->WriteCode_SN(dlg1->m_edit_sn, 0);
					break;
				}
				else
				{
					i++;
				}
			}
			else
			{
				//global_dlg->m_rich_msg.SetWindowTextA("输入内容不符合要求！\r\n");
				global_dlg->PringMsg("输入内容不符合要求,请输入大写字母与数字组合！");
				dlg1->m_edit_ctrl.SetWindowTextA("");
				break;
			}
		}
	}
	else
	{
		//global_dlg->m_rich_msg.SetWindowTextA("输入内容未达设定位数！\r\n");
		if (len == 0)
		{
			global_dlg->PringMsg("输入内容不能为空！");
			dlg1->m_edit_ctrl.SetWindowTextA("");
		}
		else
		{
			global_dlg->PringMsg("输入内容长度有误！");
			dlg1->m_edit_ctrl.SetWindowTextA("");
		}
		//global_dlg->PringMsg("输入内容未达设定位数！");
	}
}

void Write_BT(LPVOID parm)
{
	CWriteToolsDlg *dlg1 = (CWriteToolsDlg *)parm;
	int len = dlg1->m_edit_bt.GetLength();;
	unsigned char str;

	if (len == 12)
	{
		for (int i = 0; i < len; )
		{
			str = (dlg1->m_edit_bt.GetAt(i));
			if ((str >= 'A'&&str <= 'Z') || (str >= '0'&&str <= '9'))
			{
				if (i == len - 1)
				{
					dlg1->WriteCode_BT(dlg1->m_edit_bt, 0);
					break;
				}
				else
				{
					i++;
				}
			}
			else
			{
				//global_dlg->m_rich_msg.SetWindowTextA("输入内容不符合要求！\r\n");
				global_dlg->PringMsg("输入内容不符合要求,请输入大写字母与数字组合！");
				dlg1->m_edit_ctrl.SetWindowTextA("");
				break;
			}
		}
	}
	else
	{
		//global_dlg->m_rich_msg.SetWindowTextA("输入内容未达设定位数！\r\n");
		if (len == 0)
		{
			global_dlg->PringMsg("输入内容不能为空！");
			dlg1->m_edit_ctrl.SetWindowTextA("");
		}
		else
		{
			global_dlg->PringMsg("输入内容长度有误！");
			dlg1->m_edit_ctrl.SetWindowTextA("");
		}
		//global_dlg->PringMsg("输入内容未达设定位数！");
	}
}

void Write_WF(LPVOID parm)
{
	//CWriteToolsDlg *dlg1 = (CWriteToolsDlg *)parm;
	//dlg1->Write_WF(dlg1->m_edit_wifi, 0);
	CWriteToolsDlg *dlg2 = (CWriteToolsDlg *)parm;
	int len = dlg2->m_edit_wf.GetLength();
	unsigned char str;
	if (len == 12)
	{
		for (int i = 0; i < len; )
		{
			str = (dlg2->m_edit_wf.GetAt(i));
			if ((str >= 'A'&&str <= 'F') || (str >= '0'&&str <= '9') || (str >= 'a'&&str <= 'f'))
			{
				if (i == 11)
				{
					dlg2->WriteCode_WF(dlg2->m_edit_wf, 0);
					break;
				}
				else
				{
					i++;
				}
			}
			else
			{
				//global_dlg->m_rich_msg.SetWindowTextA("输入内容不符合要求！\r\n");
				global_dlg->PringMsg("输入内容不符合要求,请重新输入！");
				break;
			}
		}
	}
	else
	{
		//global_dlg->m_rich_msg.SetWindowTextA("输入内容未达设定位数！\r\n");
		if (len == 0)
		{
			global_dlg->PringMsg("输入内容不能为空！");
		}
		else
		{
			global_dlg->PringMsg("输入内容未达设定位数！");
		}
		//global_dlg->PringMsg("输入内容未达设定位数！");
	}
}

void Write_IMEI(LPVOID parm)
{
	//CWriteToolsDlg *dlg1 = (CWriteToolsDlg *)parm;
	//dlg1->Write_IE(dlg1->m_edit_imei, 0);
	CWriteToolsDlg *dlg2 = (CWriteToolsDlg *)parm;
	int len = dlg2->m_edit_imei1.GetLength();
	unsigned char str;
	if (len == 14)
	{
		for (int i = 0; i < len; )
		{
			str = (dlg2->m_edit_imei1.GetAt(i));
			if ((str >= '0'&&str <= '9') && (dlg2->m_edit_imei1.GetAt(0) == '8') && (dlg2->m_edit_imei1.GetAt(1) == '6'))
			{
				if (i == 13)
				{
					dlg2->WriteCode_IE(dlg2->m_edit_imei1, 0);
					break;
				}
				else
				{
					i++;
				}
			}
			else
			{
				//global_dlg->m_rich_msg.SetWindowTextA("输入内容不符合要求！\r\n");
				global_dlg->PringMsg("输入内容有误！请输入以86开头的十四位数字组合！");
				break;
			}
		}
	}
	else
	{
		//global_dlg->m_rich_msg.SetWindowTextA("输入内容未达设定位数！\r\n");
		if (len == 0)
		{
			global_dlg->PringMsg("输入内容不能为空！");
		}
		else
		{
			global_dlg->PringMsg("输入内容未达设定位数！");
		}
		//global_dlg->PringMsg("输入内容未达设定位数！");
	}
}

void CWriteToolsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	UpdateData(true);
	DWORD dThreadId;  //句柄
	//if(((CButton *)GetDlgItem(IDC_READ_RADIO))->GetCheck())
	//{
		hHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Read_CODE, this, NULL, &dThreadId);
	//}

	UpdateData(false);
}

void CWriteToolsDlg::PringMsg(CString msg)
{
	m_rich_msg.ReplaceSel(msg + "\r\n");
	int i= m_rich_msg.GetLineCount();
	m_rich_msg.LineScroll(i,0);
}

void CWriteToolsDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}

void CWriteToolsDlg::OnReadRadio()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(true);
	m_edit_ctrl.ShowWindow(false);
	GetDlgItem(IDC_STATIC_SN)->ShowWindow(false);
	m_edit_btctrl.ShowWindow(false);
	GetDlgItem(IDC_STATIC_BT)->ShowWindow(false);
	m_edit_wfctrl.ShowWindow(false);
	GetDlgItem(IDC_STATIC_WIFI)->ShowWindow(false);
	m_edit_imctrl1.ShowWindow(false);
	GetDlgItem(IDC_STATIC_IMEI1)->ShowWindow(false);
	m_edit_imctrl2.ShowWindow(false);
	GetDlgItem(IDC_STATIC_IMEI2)->ShowWindow(false);
	GetDlgItem(IDC_STATIC_ITME2)->ShowWindow(false);
	//m_rich_msg.Clear();
	m_rich_msg.SetWindowTextA("");

		//初始化Item包含项状态,全部非选中
	m_check1.SetCheck(1);
	m_check2.SetCheck(0);
	m_check3.SetCheck(0);
	m_check4.SetCheck(0);
	m_check5.SetCheck(0);
	UpdateData(false);
}

void CWriteToolsDlg::OnWriteRadio()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(true);
	m_edit_ctrl.ShowWindow(true);
	m_edit_ctrl.SetReadOnly(false);
	GetDlgItem(IDC_STATIC_SN)->ShowWindow(true);
	m_edit_btctrl.ShowWindow(true);
	m_edit_btctrl.SetReadOnly(true);
	GetDlgItem(IDC_STATIC_BT)->ShowWindow(true);
	m_edit_wfctrl.ShowWindow(true);
	m_edit_wfctrl.SetReadOnly(true);
	GetDlgItem(IDC_STATIC_WIFI)->ShowWindow(true);
	m_edit_imctrl1.ShowWindow(true);
	m_edit_imctrl1.SetReadOnly(true);
	GetDlgItem(IDC_STATIC_IMEI1)->ShowWindow(true);
	m_edit_imctrl2.ShowWindow(true);
	m_edit_imctrl2.SetReadOnly(true);
	GetDlgItem(IDC_STATIC_IMEI2)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_ITME2)->ShowWindow(true);
	//m_rich_msg.Clear();//非只读属性使用此方法
	m_rich_msg.SetWindowTextA("");
	//初始化Item包含项状态,全部非选中
	m_check1.SetCheck(1);
	m_check2.SetCheck(0);
	m_check3.SetCheck(0);
	m_check4.SetCheck(0);
	m_check5.SetCheck(0);
	UpdateData(false);
}

void CWriteToolsDlg::OnCheckRadio()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(true);
	m_edit_ctrl.ShowWindow(true);
	m_edit_ctrl.SetReadOnly(false);
	GetDlgItem(IDC_STATIC_SN)->ShowWindow(true);
	m_edit_btctrl.ShowWindow(true);
	m_edit_btctrl.SetReadOnly(true);
	GetDlgItem(IDC_STATIC_BT)->ShowWindow(true);
	m_edit_wfctrl.ShowWindow(true);
	m_edit_wfctrl.SetReadOnly(true);
	GetDlgItem(IDC_STATIC_WIFI)->ShowWindow(true);
	m_edit_imctrl1.ShowWindow(true);
	m_edit_imctrl1.SetReadOnly(true);
	GetDlgItem(IDC_STATIC_IMEI1)->ShowWindow(true);
	m_edit_imctrl2.ShowWindow(true);
	m_edit_imctrl2.SetReadOnly(true);
	GetDlgItem(IDC_STATIC_IMEI2)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_ITME2)->ShowWindow(true);
	//m_rich_msg.Clear();
	m_rich_msg.SetWindowTextA("");
	//初始化Item包含项状态,全部非选中
	m_check1.SetCheck(1);
	m_check2.SetCheck(0);
	m_check3.SetCheck(0);
	m_check4.SetCheck(0);
	m_check5.SetCheck(0);
	UpdateData(false);

}

void CWriteToolsDlg::OnClickedSnCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDC_WRITE_RADIO)
	{
		if (m_check1.GetCheck())
		{
			m_edit_ctrl.SetReadOnly(false);
		}
		else
		{
			m_edit_ctrl.SetReadOnly(true);
		}
	}
}

void CWriteToolsDlg::OnClickedBtCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDC_WRITE_RADIO)
	{
		if (m_check2.GetCheck())
		{
			m_edit_btctrl.SetReadOnly(false);
		}
		else
		{
			m_edit_btctrl.SetReadOnly(true);
		}
	}
}

void CWriteToolsDlg::OnClickedWifiCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDC_WRITE_RADIO)
	{
		if (m_check3.GetCheck())
		{
			m_edit_wfctrl.SetReadOnly(false);
		}
		else
		{
			m_edit_wfctrl.SetReadOnly(true);
		}
	}
}

void CWriteToolsDlg::OnClickedImei1Check()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDC_WRITE_RADIO)
	{
		if (m_check4.GetCheck())
		{
			m_edit_imctrl1.SetReadOnly(false);
		}
		else
		{
			m_edit_imctrl1.SetReadOnly(true);
		}
	}
}

void CWriteToolsDlg::OnClickedImei2Check()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDC_WRITE_RADIO)
	{
		if (m_check5.GetCheck())
		{
			m_edit_imctrl2.SetReadOnly(false);
		}
		else
		{
			m_edit_imctrl2.SetReadOnly(true);
		}
	}
}



