#pragma once

enum{
    ID_APPEND_LOG = WM_USER + 100,
};
// CLogDlg dialog

class CLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogDlg)

public:
	CLogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogDlg();

// Dialog Data
	enum { IDD = IDD_LOGDLG };
    
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg LRESULT OnAppendLog(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
private:
    CEdit *editBox;
    CString log;

    inline void AppendStringToEdit(const CString &str, CEdit &edit);
};
