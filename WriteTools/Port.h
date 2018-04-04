#pragma once

#include "QMSL6.1/QMSL_inc/QLib.h"
#include "QMSL6.1/QMSL_inc/QLib_Defines.h"

// CPort

#define NV_PSN 2497
#define NV_WIFI 4678
#define NV_BT 447
#define NV_FLAG 2499
#define NV_IMEI 550

class CPort : public CWnd
{
	DECLARE_DYNAMIC(CPort)

public:
	CPort();
	virtual ~CPort();

protected:
	DECLARE_MESSAGE_MAP()

public:

	HANDLE m_digHandle;
	bool m_bConnected;

	void EunmPort();
	bool OpenPort(CString port, int delay);
	bool CPort::ReadSN();
	bool CPort::ReadBT();
	bool CPort::ReadWF();
	bool CPort::ReadFLAG();
	bool CPort::ReadIMEI();
	bool CPort::WritePSN(CString sn);
	bool CPort::WriteBT(CString bt);
	bool CPort::WriteWIFI(CString wf);
	bool CPort::WriteIMEI(CString ie);
	bool CPort::WriteFlag(CString fg);
	bool CPort::Check(CString code, int flag);
};


