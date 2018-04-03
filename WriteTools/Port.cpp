// Port.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteTools.h"
#include "WriteToolsDlg.h"
#include "Port.h"

extern CWriteToolsDlg *global_dlg;

// CPort

IMPLEMENT_DYNAMIC(CPort, CWnd)

CPort::CPort()
{

}

CPort::~CPort()
{
}


BEGIN_MESSAGE_MAP(CPort, CWnd)
END_MESSAGE_MAP()


// CPort 消息处理程序

void CPort::EunmPort()
{
	CString port;
	USHORT ignorenum = 2;
	USHORT availablenum = 20;		//串口最大获得个数
	USHORT ignorelist[2] = {1,2};
	USHORT availablelist[20];   //串口列表最大存储限定值

	memset(availablelist,0,20);

	QLIB_GetAvailablePhonesPortList(&availablenum,availablelist,ignorenum,ignorelist);   //获取已连接手机端口

	global_dlg->m_com_port.ResetContent();		//清除已有所有项，防止显示内容叠加
//	global_dlg->m_com_port.AddString("AUTO");

	for (int i=0;i<availablenum;i++)
	{
		//availablelist[i] = i;
		port.Format("COM%d",availablelist[i]);
		//port.Format("COM%d", i+1);
		global_dlg->m_com_port.AddString(port);
	}
}

bool CPort::OpenPort(CString port, int delay)
{
	char cLibVersion[128];
	unsigned short uPort;

	QLIB_SetLibraryMode(QLIB_LIB_MODE_QPHONEMS);//设置PHONE模式
	QLIB_GetLibraryVersion(cLibVersion);//获取Library版本

	if (port == "AUTO")
	{
		m_digHandle = QLIB_ConnectServerWithWait(QLIB_COM_AUTO_DETECT,delay*1000);//自动获取端口并连接
	}
	else if (port.Find("COM") == 0)
	{
		port.Replace("COM","");
		m_digHandle = QLIB_ConnectServerWithWait(atoi(port),delay*1000);//指定取端口关连接
	}
	else
	{
		return false;
	}

	m_bConnected = QLIB_IsPhoneConnected(m_digHandle);//判断端口是否连接成功

	if (m_bConnected)
	{
		QLIB_GetComPortNumber(m_digHandle,&uPort);//获取连接成功的端口号
		return true;
	}
	else
	{
		QLIB_DisconnectServer(m_digHandle);//断开当前连接端口
	}

	return false;
}

//数据读取模块
bool CPort::ReadSN()
{
	unsigned char itemdata[128];
	unsigned short iStatus;
	CString SN;

	memset(itemdata,0,128);
	unsigned char result = QLIB_DIAG_NV_READ_F(m_digHandle, NV_PSN, itemdata, 128, &iStatus);
	SN = CString(itemdata);
	/*global_dlg->m_rich_msg.ReplaceSel("写入的SN为：" + SN + "\r\n");*/
	global_dlg->PringMsg("写入的SN为：" + SN);
	 return true;
}

bool CPort::ReadBT()
{
	unsigned char itemdata[12];
	unsigned short iStatus;
	char bt[6];
	CString BT;

	memset(itemdata,0,12);
	//memset(bt, 0, 128);
	unsigned char result = QLIB_DIAG_NV_READ_F(m_digHandle, NV_BT, itemdata,12 , &iStatus);
	global_dlg->m_rich_msg.ReplaceSel("写入的BT MAC为：");
	//global_dlg->PringMsg("写入的BT MAC为：");
	for (int i = 5; i>=0 ;i--)
	{
			sprintf_s(bt,"%02x", itemdata[i]);	//将十六进制数据格式化输出到字符串
			BT = CString(bt);
			BT.MakeUpper();	//将字符串中的小写字母转换为大写
			global_dlg->m_rich_msg.ReplaceSel(BT);
	}
//	global_dlg->m_rich_msg.ReplaceSel("写入的BT MAC为：" + BT + "\r\n");
	/*global_dlg->m_rich_msg.ReplaceSel("\r\n");*/
	global_dlg->PringMsg("");
	 return true;
}

bool CPort::ReadWF()
{
	unsigned char itemdata[12];
	unsigned short iStatus;
	char wf[6];
	CString WF;

	memset(itemdata,0,12);
	unsigned char result = QLIB_DIAG_NV_READ_F(m_digHandle, NV_WIFI, itemdata, 12, &iStatus);
	global_dlg->m_rich_msg.ReplaceSel("写入的WIFI MAC为：");
	//global_dlg->PringMsg("写入的WIFI MAC为：");
	for (int i = 0; i<6; i++)
	{
		sprintf_s(wf, "%02x", itemdata[i]);	//以十六进制形式输出，不足两位自动补零
		WF = CString(wf);
		//WF = char (itemdata[i]);
		WF.MakeUpper();
		global_dlg->m_rich_msg.ReplaceSel(WF);
	}
	/*global_dlg->m_rich_msg.ReplaceSel("\r\n");*/
	global_dlg->PringMsg("");
	 return true;
}

bool CPort::ReadFLAG()
{
	unsigned char itemdata[128];
	unsigned short iStatus;
	//char FLAG1[128];
	CString FLAG;

	memset(itemdata, 0, 128);
	unsigned char result = QLIB_DIAG_NV_READ_F(m_digHandle, NV_FLAG, itemdata, 128, &iStatus);
	FLAG = CString(itemdata);
	/*global_dlg->m_rich_msg.ReplaceSel("FLAG状态为：" + FLAG + "\r\n");*/
	global_dlg->PringMsg("FLAG状态为：" + FLAG);

	//sprintf_s(FLAG1, "%c", itemdata[0]);
	//FLAG = CString(FLAG1);
	FLAG =char (itemdata[0]);
	/*global_dlg->m_rich_msg.ReplaceSel("FLAG_sn状态为：" + FLAG + "\r\n");*/
	global_dlg->PringMsg("FLAG_sn状态为：" + FLAG);

	//sprintf_s(FLAG1, "%c", itemdata[1]);
	//FLAG = CString(FLAG1);
	FLAG = char(itemdata[1]);
	//global_dlg->m_rich_msg.ReplaceSel("FLAG_bt状态为：" + FLAG + "\r\n");
	global_dlg->PringMsg("FLAG_bt状态为：" + FLAG);

	//sprintf_s(FLAG1, "%c", itemdata[2]);
	//FLAG = CString(FLAG1);
	FLAG = char(itemdata[2]);
	//判定ft测试状态
	if (FLAG == "P")	
	{
		//global_dlg->m_rich_msg.ReplaceSel("FLAG_ft状态为：" + FLAG + "\r\n");
		//global_dlg->m_rich_msg.ReplaceSel("FT站位已测试通过，请开始下一站位测试！\r\n");
		global_dlg->PringMsg("FLAG_ft状态为：" + FLAG);
		global_dlg->PringMsg("FT站位已测试通过，请开始下一站位测试!");

	}
	else if(FLAG=="U")
	{
		//global_dlg->m_rich_msg.ReplaceSel("FT站位未测试，请开始本站测试！\r\n");
		global_dlg->PringMsg("FT站位未测试，请开始本站测试!");
	}
	else
	{
		//global_dlg->m_rich_msg.ReplaceSel("FT站位测试失败！\r\n");
		global_dlg->PringMsg("FT站位测试失败！\r\n");
	}
	return true;
}

bool CPort::ReadIMEI()
{
	unsigned char itemdata[9];
	unsigned short iStatus;
	char iemi[9];
	CString IMEI;

	memset(itemdata, 0, 9);
	unsigned char result = QLIB_DIAG_NV_READ_F(m_digHandle, NV_IMEI, itemdata, 9, &iStatus);
	global_dlg->m_rich_msg.ReplaceSel("写入的IMEI码为：");
	for (int i = 1; i < 9; i++)
	{
		if (i == 1)
		{
			itemdata[i] = (itemdata[i] & 0xf0)>>4;
			sprintf_s(iemi, "%x", itemdata[i]);
		}
		else
		{
			sprintf_s(iemi, "%02x", itemdata[i]);
		}

		IMEI = CString(_strrev(iemi));	//字符倒序输出
		global_dlg->m_rich_msg.ReplaceSel(IMEI);
	}
	global_dlg->PringMsg("");
	return true;
}

//数据写入模块
bool CPort::WritePSN(CString sn )
{
	unsigned char itemdata[128];
	unsigned short iStatus;
	int len=sn.GetLength();

	memset(itemdata,0,128);
	for (int i = 0; i < len; i++)
	{
		itemdata[i] = sn[i];
	}
	//itemdata[0] = 'P';
	//itemdata[1] = 'S';
	//itemdata[2] = 'N';
	//itemdata[3] = '1';
	//itemdata[4] = '2';
	//strcpy((char)itemdata,sn);
	unsigned char result = QLIB_DIAG_NV_WRITE_F(m_digHandle, NV_PSN,itemdata,128,&iStatus);
	return true;
}

bool CPort::WriteBT(CString bt)
{
	unsigned char itemdata[6];
	unsigned char itemdata1[6];
	unsigned short iStatus;
	unsigned char  BT[12];
	//char *p = bt.GetBuffer();
	//int len=sn.GetLength();

	memset(itemdata, 0, 6);

	//sn.Format("%02X", sn);
	for (int i = 0; i < 12; i++)
	{
		if (bt[i] >= '0' && bt[i] <= '9')
		{
			BT[i] = bt[i] - '0';
		}
		else if (bt[i] >= 'A' && bt[i] <= 'F')
		{
			BT[i] = bt[i] - 'A' + 10;
		}
		else
		{
			global_dlg->PringMsg("输入内容有误！");
		}
	}
	for (int j = 5; j >= 0; j--)
	{
		itemdata[5 - j] = ((BT[2 * j] & 0x0f) << 4) + (BT[2 * j + 1] & 0x0f);
	}
	unsigned char result = QLIB_DIAG_NV_WRITE_F(m_digHandle, NV_BT, itemdata, 6, &iStatus);
	return true;
}

bool CPort::WriteWIFI(CString wf)
{
	unsigned char itemdata[6];
	//char itemdata1[12];
	unsigned short iStatus;
	unsigned char  WF[12];
	//char *p = wf.GetBuffer();

	memset(itemdata, 0, 6);

	 for (int i = 0; i < 12; i++)
	 {
			 if (wf[i] >= '0' && wf[i] <= '9')
			 {
				 WF[i] = wf[i] - '0';
			 }
			 else if (wf[i] >= 'A' && wf[i] <= 'F')
			 {
				 WF[i] = wf[i] - 'A' + 10;
			 }
			 else
			 {
				 global_dlg->PringMsg("输入内容有误！");
				 return false;
			 }
	 }
	for (int j = 0; j<6; j++)
	{
		itemdata[j] = ((WF[2 * j] & 0x0f)<<4) + (WF[2 * j + 1] & 0x0f);
	}
	unsigned char result = QLIB_DIAG_NV_WRITE_F(m_digHandle, NV_WIFI, itemdata, 6, &iStatus);
	global_dlg->PringMsg("WIFI:"+ wf + ("已写入"));
	return true;
}

bool CPort::WriteIMEI(CString ie)
{
	unsigned char itemdata[9];
	unsigned short iStatus;
	unsigned char  IE[15];
	unsigned int len = ie.GetLength();
	unsigned int sum1=0, sum2=0, sum=0;
	memset(itemdata, 0, 9);
	itemdata[0] = 0x08;

	for (int i = 0; i < len; i++)
		{
			//if (ie[i] >= '0' && ie[i] <= '9'&&(ie[0]=='8')&&(ie[1]=='6'))
			//{
				IE[i] = ie[i] - '0';
				if (i%2 == 0)
				{
					sum1 = sum1 + IE[i];
				}
				else
				{
					sum2 = sum2 + ((IE[i] * 2) / 10) + ((IE[i] * 2) % 10);
				}
				sum = sum1 + sum2;
				IE[14] = (10 - (sum % 10))&0x0f;
			/*}*/
			//else
			//{
			//	global_dlg->PringMsg("输入内容有误！请输入以86开头的十四位数字组合！");
			//	return false;
			//}
		}
		for (int j = 1; j < 9; j++)
			{
				if (j == 1)
				{
					itemdata[j] = ((IE[2 * j - 2] & 0x0f) << 4) + (0x0A);
				}
				else
				{
					itemdata[j] = ((IE[2 * j - 2] & 0x0f) << 4) + (IE[2 * j - 3] & 0x0f);
				}
			}
	unsigned char result = QLIB_DIAG_NV_WRITE_F(m_digHandle, NV_IMEI, itemdata, 9, &iStatus);
	global_dlg->PringMsg("IMEI:"+ie + ("已写入"));
	return true;
}

bool CPort::WriteFlag(CString fg)
{
	unsigned char itemdata[128];
	unsigned short iStatus;
	int len = fg.GetLength();

	memset(itemdata, 0, 128);
	for (int i = 0; i < len; i++)
	{
		itemdata[i] = fg[i];
	}
	unsigned char result = QLIB_DIAG_NV_WRITE_F(m_digHandle, NV_FLAG, itemdata, 128, &iStatus);
	return true;
}