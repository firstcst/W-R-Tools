// Port.cpp : ʵ���ļ�
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


// CPort ��Ϣ�������

void CPort::EunmPort()
{
	CString port;
	USHORT ignorenum = 2;
	USHORT availablenum = 20;		//��������ø���
	USHORT ignorelist[2] = {1,2};
	USHORT availablelist[20];   //�����б����洢�޶�ֵ

	memset(availablelist,0,20);

	QLIB_GetAvailablePhonesPortList(&availablenum,availablelist,ignorenum,ignorelist);   //��ȡ�������ֻ��˿�

	global_dlg->m_com_port.ResetContent();		//��������������ֹ��ʾ���ݵ���
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

	QLIB_SetLibraryMode(QLIB_LIB_MODE_QPHONEMS);//����PHONEģʽ
	QLIB_GetLibraryVersion(cLibVersion);//��ȡLibrary�汾

	if (port == "AUTO")
	{
		m_digHandle = QLIB_ConnectServerWithWait(QLIB_COM_AUTO_DETECT,delay*1000);//�Զ���ȡ�˿ڲ�����
	}
	else if (port.Find("COM") == 0)
	{
		port.Replace("COM","");
		m_digHandle = QLIB_ConnectServerWithWait(atoi(port),delay*1000);//ָ��ȡ�˿ڹ�����
	}
	else
	{
		return false;
	}

	m_bConnected = QLIB_IsPhoneConnected(m_digHandle);//�ж϶˿��Ƿ����ӳɹ�

	if (m_bConnected)
	{
		QLIB_GetComPortNumber(m_digHandle,&uPort);//��ȡ���ӳɹ��Ķ˿ں�
		return true;
	}
	else
	{
		QLIB_DisconnectServer(m_digHandle);//�Ͽ���ǰ���Ӷ˿�
	}

	return false;
}

//���ݶ�ȡģ��
bool CPort::ReadSN()
{
	unsigned char itemdata[128];
	unsigned short iStatus;
	CString SN;

	memset(itemdata,0,128);
	unsigned char result = QLIB_DIAG_NV_READ_F(m_digHandle, NV_PSN, itemdata, 128, &iStatus);
	SN = CString(itemdata);
	/*global_dlg->m_rich_msg.ReplaceSel("д���SNΪ��" + SN + "\r\n");*/
	global_dlg->PringMsg("д���SNΪ��" + SN);
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
	global_dlg->m_rich_msg.ReplaceSel("д���BT MACΪ��");
	//global_dlg->PringMsg("д���BT MACΪ��");
	for (int i = 5; i>=0 ;i--)
	{
			sprintf_s(bt,"%02x", itemdata[i]);	//��ʮ���������ݸ�ʽ��������ַ���
			BT = CString(bt);
			BT.MakeUpper();	//���ַ����е�Сд��ĸת��Ϊ��д
			global_dlg->m_rich_msg.ReplaceSel(BT);
	}
//	global_dlg->m_rich_msg.ReplaceSel("д���BT MACΪ��" + BT + "\r\n");
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
	global_dlg->m_rich_msg.ReplaceSel("д���WIFI MACΪ��");
	//global_dlg->PringMsg("д���WIFI MACΪ��");
	for (int i = 0; i<6; i++)
	{
		sprintf_s(wf, "%02x", itemdata[i]);	//��ʮ��������ʽ�����������λ�Զ�����
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
	/*global_dlg->m_rich_msg.ReplaceSel("FLAG״̬Ϊ��" + FLAG + "\r\n");*/
	global_dlg->PringMsg("FLAG״̬Ϊ��" + FLAG);

	//sprintf_s(FLAG1, "%c", itemdata[0]);
	//FLAG = CString(FLAG1);
	FLAG =char (itemdata[0]);
	/*global_dlg->m_rich_msg.ReplaceSel("FLAG_sn״̬Ϊ��" + FLAG + "\r\n");*/
	global_dlg->PringMsg("FLAG_sn״̬Ϊ��" + FLAG);

	//sprintf_s(FLAG1, "%c", itemdata[1]);
	//FLAG = CString(FLAG1);
	FLAG = char(itemdata[1]);
	//global_dlg->m_rich_msg.ReplaceSel("FLAG_bt״̬Ϊ��" + FLAG + "\r\n");
	global_dlg->PringMsg("FLAG_bt״̬Ϊ��" + FLAG);

	//sprintf_s(FLAG1, "%c", itemdata[2]);
	//FLAG = CString(FLAG1);
	FLAG = char(itemdata[2]);
	//�ж�ft����״̬
	if (FLAG == "P")	
	{
		//global_dlg->m_rich_msg.ReplaceSel("FLAG_ft״̬Ϊ��" + FLAG + "\r\n");
		//global_dlg->m_rich_msg.ReplaceSel("FTվλ�Ѳ���ͨ�����뿪ʼ��һվλ���ԣ�\r\n");
		global_dlg->PringMsg("FLAG_ft״̬Ϊ��" + FLAG);
		global_dlg->PringMsg("FTվλ�Ѳ���ͨ�����뿪ʼ��һվλ����!");

	}
	else if(FLAG=="U")
	{
		//global_dlg->m_rich_msg.ReplaceSel("FTվλδ���ԣ��뿪ʼ��վ���ԣ�\r\n");
		global_dlg->PringMsg("FTվλδ���ԣ��뿪ʼ��վ����!");
	}
	else
	{
		//global_dlg->m_rich_msg.ReplaceSel("FTվλ����ʧ�ܣ�\r\n");
		global_dlg->PringMsg("FTվλ����ʧ�ܣ�\r\n");
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
	global_dlg->m_rich_msg.ReplaceSel("д���IMEI��Ϊ��");
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

		IMEI = CString(_strrev(iemi));	//�ַ��������
		global_dlg->m_rich_msg.ReplaceSel(IMEI);
	}
	global_dlg->PringMsg("");
	return true;
}

//����д��ģ��
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
			global_dlg->PringMsg("������������");
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
				 global_dlg->PringMsg("������������");
				 return false;
			 }
	 }
	for (int j = 0; j<6; j++)
	{
		itemdata[j] = ((WF[2 * j] & 0x0f)<<4) + (WF[2 * j + 1] & 0x0f);
	}
	unsigned char result = QLIB_DIAG_NV_WRITE_F(m_digHandle, NV_WIFI, itemdata, 6, &iStatus);
	global_dlg->PringMsg("WIFI:"+ wf + ("��д��"));
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
			//	global_dlg->PringMsg("��������������������86��ͷ��ʮ��λ������ϣ�");
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
	global_dlg->PringMsg("IMEI:"+ie + ("��д��"));
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