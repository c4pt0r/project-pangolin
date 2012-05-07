// ShareGUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CShareGUIApp:
// See ShareGUI.cpp for the implementation of this class
//

class CShareGUIApp : public CWinApp
{
public:
	CShareGUIApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

// 创建线程
struct TRANS_DATA{
    DWORD    thread_id;  // use as taskid
    char *  filename;
    FILE *  fp;
    HWND    target_hwnd;
};

struct TASK_ITEM{
    CString *key;
    CString *name;
    CString *url;
    DWORD   thread_id; // use as taskid
};

extern CShareGUIApp theApp;