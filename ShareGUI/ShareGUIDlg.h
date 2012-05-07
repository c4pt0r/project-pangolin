// ShareGUIDlg.h : header file
//

#pragma once
#include <afxsock.h>
#include <afxcmn.h>
#include <stdio.h>
#include "LogDlg.h"
#include "TimeOutSocket.h"
#include "PangolinListCtrl.h"


enum{
    ID_UPDATE_PROGRESS = WM_USER + 1,
    ID_UPDATE_URL,
    ID_FINISH,
    ID_QUIT,
};
// CShareGUIDlg dialog
class CShareGUIDlg : public CDialog
{
// Construction
public:
	CShareGUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SHAREGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    
   
// Implementation
protected:
	HICON m_hIcon;
    CStatic *textInfo;
    CProgressCtrl *m_ProgressCtrl;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon(); 
    afx_msg LRESULT OnUpdateUrl(WPARAM wparam,LPARAM lparam);
    afx_msg LRESULT OnUpdateProgress(WPARAM wparam,LPARAM lparam);
    afx_msg LRESULT OnSendFinish(WPARAM wparam,LPARAM lparam);
    afx_msg void OnDropFiles(HDROP hDropInfo);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedButtonLog();
	DECLARE_MESSAGE_MAP()
public:
    
    static DWORD WINAPI workerThread(LPVOID lpParam);

    static void SaveLog( CString &line );
   
    static BOOL m_bIsProcessing;
    static CLogDlg *m_dlgLog;
    static CPangolinListCtrl m_ListCtrl;

private:
    BOOL AppendTaskItem(TASK_ITEM * item);
    
public:
    afx_msg void OnBnClickedButtonQuit();
};
