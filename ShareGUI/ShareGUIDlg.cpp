// ShareGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShareGUI.h"
#include "ShareGUIDlg.h" 
#include "LogDlg.h"
#include <process.h>

#include "utf8.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUFF_LEN 4096
#define KEY_LEN 8

BOOL CShareGUIDlg::m_bIsProcessing = FALSE;
CLogDlg* CShareGUIDlg::m_dlgLog = NULL;
CPangolinListCtrl CShareGUIDlg::m_ListCtrl;

// CShareGUIDlg dialog
long filesize(FILE *fpStream)
{
     long curpos, length;
     curpos = ftell(fpStream);
     fseek(fpStream, 0L, SEEK_END);
     length = ftell(fpStream);
     fseek(fpStream, curpos, SEEK_SET);
     return length;
}
 
CString GetFileName(LPCTSTR lpszFilePath)
{
    CString csFilename = lpszFilePath;
    int n = csFilename.ReverseFind('\\');
    if(n>=0)
    {
        csFilename = csFilename.Right(csFilename.GetLength()-n-1);
    }
    return csFilename;
}




CShareGUIDlg::CShareGUIDlg(CWnd* pParent /*=NULL*/)
: CDialog(CShareGUIDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShareGUIDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShareGUIDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_DROPFILES()
    ON_WM_QUERYDRAGICON()
    ON_MESSAGE(ID_UPDATE_URL,OnUpdateUrl)
    ON_MESSAGE(ID_UPDATE_PROGRESS,OnUpdateProgress)
    ON_MESSAGE(ID_FINISH, OnSendFinish)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CShareGUIDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON_LOG, &CShareGUIDlg::OnBnClickedButtonLog)
    ON_BN_CLICKED(IDC_BUTTON_QUIT, &CShareGUIDlg::OnBnClickedButtonQuit)
END_MESSAGE_MAP()


// CShareGUIDlg message handlers

BOOL CShareGUIDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here
    DragAcceptFiles();

    if (!m_dlgLog)
    {
        m_dlgLog = new CLogDlg();
        m_dlgLog->Create(IDD_LOGDLG);
    }

    ::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0,(SWP_NOSIZE|SWP_NOMOVE)); 
    m_ProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
    textInfo = (CStatic*)GetDlgItem(IDC_STATIC);

    m_ProgressCtrl->ShowWindow(SW_HIDE);

    CFont * cFont=new CFont;
    cFont->CreateFont(30,0,0,0,FW_SEMIBOLD,FALSE,FALSE,0,
                    ANSI_CHARSET,OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                    DEFAULT_PITCH&FF_SWISS,L"Arial");

    textInfo->SetFont(cFont);
    SetDlgItemText(IDC_STATIC,L"Drag File Here");
    SetWindowText(L"Pangolin!");

    int iWidth  = 432;
    int iHeight = 103;
    CPoint pt = CPoint(11,220);
    CRect rect(pt.x,pt.y, pt.x + iWidth, pt.y+iHeight);
    m_ListCtrl.Create(this, 1111);
    m_ListCtrl.MoveWindow(rect);
    m_ListCtrl.ShowWindow(SW_SHOW);
    
    ////m_ListCtrl->ShowWindow(SW_HIDE);


    
    return TRUE;  // return TRUE  unless you set the focus to a control
}


void CShareGUIDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CShareGUIDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CShareGUIDlg::OnBnClickedOk()
{
 
}

DWORD WINAPI CShareGUIDlg::workerThread( LPVOID lpParam )
{
    //isProcessing = TRUE;
    TRANS_DATA *data = (TRANS_DATA*) lpParam; 
    AfxSocketInit();
    CTimeOutSocket aSocket;
    aSocket.Create();
    aSocket.SetTimeOut(10000);
    if(aSocket.Connect(L"xyc.netease.com", 8002))
    {
        
        long ulSize = filesize(data->fp);
        char szBuf[1024] = {0};
        sprintf_s(szBuf,"%s\n%ld\n",data->filename,ulSize);
        aSocket.Send(szBuf, strlen(szBuf));

        char szRecValue[1024] = {0};
        int recvCnt = 0;

        while((recvCnt = aSocket.Receive((void *)szRecValue, 1024))!=0 )
        {
            CString line(szRecValue);
            
            CString *key = NULL;
            CString *name = NULL;
            CString *csFullurl = NULL;
            TASK_ITEM *item = NULL;
            
            SaveLog(line);
            
            if (line.Find(L"KEY") != -1)
            {  
                if (!item)
                    item = new TASK_ITEM();
                key = new CString(line.Mid(line.Find(L"KEY")+4,KEY_LEN));
                item->name = key;
            }
            if (line.Find(L"NAME") != -1)
            {
                if (!item)
                    item = new TASK_ITEM();
                int pos = line.Find(L"NAME")+5;
                int posend = line.Find(L'\n',pos);
                name = new CString(line.Mid(pos,posend-pos));
                if (item)
                {    
                    csFullurl = new CString();
                    csFullurl->Format(L"http://xyc.netease.com:8001/f/%s/%s", *key, *name);
                    item->thread_id = GetCurrentThreadId();
                    item->name = name;
                    item->url = csFullurl;
                    ::PostMessage(data->target_hwnd,ID_UPDATE_URL,(WPARAM)item,NULL);
                }
                
            }
            if (line.Find(L"REQ") != -1)
            {
                int offset,length;
                CString tmp = line.Mid(line.Find(L"REQ"));
                _stscanf(tmp,L"REQ %d %d", &offset, &length);


                char buff_send[BUFF_LEN + 6] = {0};
                char compress_flag[] = "PLAIN\n";
                char buf[BUFF_LEN]={0};
                fseek(data->fp,offset,0);
                fread(buf,length,1,data->fp);
                memcpy(buff_send, compress_flag, strlen(compress_flag));
                memcpy(buff_send + strlen(compress_flag), buf, length);

                aSocket.Send(buff_send,length + strlen(compress_flag));

                int percent = ((offset + length)*1.0 /  ulSize*1.0 )* 100;
                if (percent !=0)
                    ::PostMessage(data->target_hwnd, ID_UPDATE_PROGRESS, (WPARAM)percent, (LPARAM)GetCurrentThreadId());
            }
            if (line.Find(L"QUIT") != -1)
                break;
            memset(szRecValue,0,sizeof(szRecValue));
        }
    }
    else{
        ::MessageBox(data->target_hwnd,L"Server Down!",NULL,NULL);
    }
    aSocket.KillTimeOut();
    ::PostMessage(data->target_hwnd, ID_FINISH, (WPARAM)GetCurrentThreadId(), NULL);
    //isProcessing = FALSE;
    
    return 0;

}

LRESULT CShareGUIDlg::OnUpdateUrl( WPARAM wparam,LPARAM lparam )
{
    TASK_ITEM *item = (TASK_ITEM*) wparam;
    
    
    m_ListCtrl.Insert(item);
    m_ListCtrl.PutStringToClipboard(*(item->url));

    delete item->key;
    delete item->name;
    delete item->url;
    delete item;
    return 0;
}

LRESULT CShareGUIDlg::OnUpdateProgress( WPARAM wparam,LPARAM lparam )
{
   
    int percent = (int)wparam;
    DWORD tid = (DWORD)lparam;
    m_ListCtrl.UpdatePercent(tid, percent);
    return 0;
}

void CShareGUIDlg::OnDropFiles( HDROP hDropInfo )
{
    wchar_t *lpszFileName=new wchar_t[512];
    int nFileCount;
    nFileCount=::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 512);

    if(nFileCount == 1)
    {
        UINT nChars=::DragQueryFile(hDropInfo, 0, &lpszFileName[0], 512);
        CString str(&lpszFileName[0], nChars);
        FILE* fp = _wfopen(str, L"rb");
        
        CString filename = GetFileName(str);
        char * utf8Filename = (char*)utf_convert_back(filename.GetBuffer(0));
        TRANS_DATA *data = new TRANS_DATA();
        data->fp = fp;
        data->target_hwnd = GetSafeHwnd();
        data->filename = utf8Filename;
        
        DWORD tid;
        if (!m_bIsProcessing)
        {
            CreateThread( NULL, 0, CShareGUIDlg::workerThread, (LPVOID)data, 0, &tid); 
        }
        else{
            ::AfxMessageBox(L"Waiting list is not empty!");
        } 
    }
    else
    {
        ::AfxMessageBox(L"Oops! Unknown Error!");
    }

    ::DragFinish(hDropInfo);       //ÊÍ·ÅÄÚ´æ
 
}

 
LRESULT CShareGUIDlg::OnSendFinish( WPARAM wparam,LPARAM lparam )
{
    DWORD tid = (DWORD)wparam;
    ::MessageBox(GetSafeHwnd(),L"File sent successful!",L"Pangolin",MB_ICONINFORMATION);
    m_ListCtrl.Remove(tid);
    return 0;
}

void CShareGUIDlg::OnBnClickedButtonLog()
{
    // TODO: Add your control notification handler code here
   
    m_dlgLog->ShowWindow(SW_SHOW);
}


BOOL CShareGUIDlg::AppendTaskItem( TASK_ITEM * item )
{
    //m_ListCtrl->InsertColumn()
    return 0;
}

void CShareGUIDlg::SaveLog( CString &line )
{
    if (line.GetLength()>0)
    {
        if(m_dlgLog)
        {
            CString logTmp = line,csTid;
            logTmp.Replace(L"\n",L"\r\n");
            csTid.Format(L"TID:%d: ",GetCurrentThreadId());
            logTmp = csTid + logTmp;
            CString *log = new CString(logTmp);
            ::PostMessage(m_dlgLog->GetSafeHwnd(),ID_APPEND_LOG, (WPARAM)log, NULL);
        }
    }
}
void CShareGUIDlg::OnBnClickedButtonQuit()
{
    OnOK();
    // TODO: Add your control notification handler code here
}
