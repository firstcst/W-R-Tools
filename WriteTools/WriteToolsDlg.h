
// WriteToolsDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Port.h"

// CWriteToolsDlg 对话框
class CWriteToolsDlg : public CDialogEx
{
// 构造
public:
	CWriteToolsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WRITETOOLS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_com_port;
	CEdit m_edit_ctrl;
	CString m_edit_sn;
	CRichEditCtrl m_rich_msg;
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
public:
	HANDLE hHandle;
	bool CWriteToolsDlg::WriteCode_SN(CString code, int type);
	bool CWriteToolsDlg::WriteCode_BT(CString code, int type);
	bool CWriteToolsDlg::WriteCode_WF(CString code, int type);
	bool CWriteToolsDlg::WriteCode_IE(CString code, int type);
	bool CWriteToolsDlg::ReadCode();
	//bool CWriteToolsDlg::WriteCode(CString code, int type);
	bool CWriteToolsDlg::PortStatus();
	bool CWriteToolsDlg::Checkcode(CString code, int type);
	afx_msg void OnDropdownComboPort();
	void PringMsg(CString msg);
public:
	CPort m_comprt;
	afx_msg void OnReadRadio();
	CEdit m_edit_btctrl;
	CString m_edit_bt;
	CEdit m_edit_wfctrl;
	CString m_edit_wf;
	CEdit m_edit_imctrl1;
	CString m_edit_imei1;
	CEdit m_edit_imctrl2;
	CString m_edit_imei2;
	afx_msg void OnWriteRadio();
	int m_readradio_ctrl;
	CButton m_check1;
	CButton m_check2;
	CButton m_check3;
	CButton m_check4;
	CButton m_check5;
	afx_msg void OnClickedSnCheck();
	afx_msg void OnClickedBtCheck();
	afx_msg void OnClickedWifiCheck();
	afx_msg void OnClickedImei1Check();
	afx_msg void OnClickedImei2Check();
	afx_msg void OnCheckRadio();
	CButton m_check6;
//	int m_write_radio;
//	int m_read_radio;
//	int m_check_radio;
};
