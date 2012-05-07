// LogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShareGUI.h"
#include "LogDlg.h"


// CLogDlg dialog

IMPLEMENT_DYNAMIC(CLogDlg, CDialog)

CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogDlg::IDD, pParent)
{

}

CLogDlg::~CLogDlg()
{
}

void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialog)
    ON_MESSAGE(ID_APPEND_LOG, OnAppendLog)
END_MESSAGE_MAP()


// CLogDlg message handlers

BOOL CLogDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    editBox = (CEdit*) GetDlgItem(IDC_EDIT1);
    editBox->Clear();
    // TODO:  Add extra initialization here
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CLogDlg::OnAppendLog( WPARAM wparam, LPARAM lparam)
{
    CString *csLog = (CString*)wparam;
    if (csLog != NULL)
    {
        AppendStringToEdit(*csLog, *editBox);
        delete csLog;
    }
    return 0;
}

void CLogDlg::AppendStringToEdit( const CString &str, CEdit &edit )
{
    edit.SetSel(-1, -1);
    edit.ReplaceSel(str); 
}