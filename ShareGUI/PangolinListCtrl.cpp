// PangolinListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PangolinListCtrl.h"


// CPangolinListCtrl

IMPLEMENT_DYNAMIC(CPangolinListCtrl, CListCtrl)

CPangolinListCtrl::CPangolinListCtrl()
{

}

CPangolinListCtrl::~CPangolinListCtrl()
{
}


BEGIN_MESSAGE_MAP(CPangolinListCtrl, CListCtrl)
    ON_WM_CREATE()
    ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
    ON_COMMAND(ID_POPUP_COPYURL, &CPangolinListCtrl::OnPopupCopyurl)
END_MESSAGE_MAP()



// CPangolinListCtrl message handlers



int CPangolinListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CListCtrl::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    m_imgList.Create(2, 18, ILC_COLOR16|ILC_MASK, 18, 1);
    SetImageList(&m_imgList, LVSIL_SMALL);


    InsertColumn(0, _T("TID"), LVCFMT_CENTER, 430/10 * 1 );
    InsertColumn(1, _T("Filename"), LVCFMT_LEFT, 430/10 * 2  );
    InsertColumn(2, _T("URL"), LVCFMT_LEFT, 430/10 * 5 );
    InsertColumn(3, _T("Percent"), LVCFMT_LEFT,  430/10 * 2 );
    
    ModifyStyle(NULL, LVS_SHOWSELALWAYS);
    ListView_SetExtendedListViewStyle(m_hWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES /*| LVS_EX_HEADERDRAGDROP*/);
     
    BOOL b = m_Menu.LoadMenu(IDR_MENU1);

    return 0;
}

BOOL CPangolinListCtrl::Create( CWnd* pParent, UINT nID )
{
    int iRet = CListCtrl::Create( WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT/*|LVS_OWNERDRAWFIXED*/, CRect(0,0,0,0), pParent, nID);
    return (BOOL)iRet;
}

BOOL CPangolinListCtrl::Insert( TASK_ITEM *item )
{
    int iIndex = GetItemCount();
    CString csTemp;
    csTemp.Format(_T("%d"), item->thread_id);
    InsertItem(iIndex, csTemp);
    Edit(iIndex,item);
    return TRUE;
}

BOOL CPangolinListCtrl::Edit( int iIndex, const TASK_ITEM *item, BOOL bSel /*= FALSE*/ )
{
    CString csTemp;
    CString csFilename = *(item->name);
    CString csUrl = *(item->url);
    SetItemText(iIndex, 1, csFilename);
    SetItemText(iIndex, 2, csUrl);
    SetItemText(iIndex, 3, L"Waiting...");

    return TRUE;
}

BOOL CPangolinListCtrl::UpdatePercent( DWORD tid, int percent )
{
     int nIndex = FindItemByTID(tid);
     if (nIndex != -1)
     {
         CString csPercent;
         csPercent.Format(L"%d%%", percent);
         SetItemText(nIndex, 3, csPercent);

         return TRUE;
     }
     return FALSE;
}

int CPangolinListCtrl::FindItemByTID( DWORD tid )
{
    int nCount = GetItemCount();
    CString csTid;
    csTid.Format(L"%d", tid);
    for (int i=0; i<nCount; i++)  
    {
        CString strItemText = GetItemText(i, 0);   //subItem是你要  
        //比较的子项。  
        if( strItemText==csTid)  
        {  
            return i;
        }  
    }
    return -1;
}

BOOL CPangolinListCtrl::Remove( DWORD tid )
{
    int nIndex = FindItemByTID(tid);
    if (nIndex != -1)
    {
        DeleteItem(nIndex);
        return TRUE;
    }
    return FALSE;
}

void CPangolinListCtrl::SelItemByIndex(int nIndex)
{
    POSITION pos = GetFirstSelectedItemPosition();
    CString csWord;
    while (pos)
    {
        SetItemState(GetNextSelectedItem(pos), 0, LVIS_SELECTED);
    }

    SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
    EnsureVisible(nIndex, TRUE);
    SetFocus();

}

void CPangolinListCtrl::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
    
    LPNMLISTVIEW temp = (LPNMLISTVIEW)pNMHDR;
    int nIndex = temp->iItem;
    CPoint point;
    GetCursorPos(&point);
    DWORD dwMenuStyle = MF_STRING;
     
    if (nIndex >= 0)
    {
        SelItemByIndex(nIndex);
        m_Menu.GetSubMenu(0)-> TrackPopupMenu( 
            TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
    }
    
}
void CPangolinListCtrl::OnPopupCopyurl()
{

    CString url,tid;
    int nIndex = GetCurSelItem(tid,url);
    if (nIndex >=0 && url.GetLength() >0 )
    {
        PutStringToClipboard(url);
        MessageBox(L"Copied Successfully!",L"Pangolin",MB_ICONINFORMATION);
    }
    // TODO: Add your command handler code here
}

int CPangolinListCtrl::GetCurSelItem(CString &tid, CString& url )
{
    UINT iSelCount = GetSelectedCount();
    if (iSelCount!=1)
    {
        return -1;
    }
    int iIndex = 0;
    POSITION pos = GetFirstSelectedItemPosition();
    if (!pos)
    {
        return -1;
    }
    iIndex = GetNextSelectedItem(pos);
    url = GetItemText(iIndex, 2);
    tid = GetItemText(iIndex, 0);
    return iIndex;
}

BOOL CPangolinListCtrl::PutStringToClipboard( CString &csText )
{
    HGLOBAL hGlobal=GlobalAlloc(GHND,csText.GetLength()*2+2); 
    wchar_t *pGlobal=(wchar_t*)GlobalLock(hGlobal); 
    lstrcpy(pGlobal, csText);
    GlobalUnlock(hGlobal);     
    BOOL b = OpenClipboard(); 
    EmptyClipboard(); 
    SetClipboardData(CF_UNICODETEXT,hGlobal);
    ::CloseClipboard();
    return TRUE;
}
