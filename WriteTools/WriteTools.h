
// WriteTools.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWriteToolsApp: 
// �йش����ʵ�֣������ WriteTools.cpp
//

class CWriteToolsApp : public CWinApp
{
public:
	CWriteToolsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWriteToolsApp theApp;