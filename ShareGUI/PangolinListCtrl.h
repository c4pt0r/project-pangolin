#pragma once

#include "ShareGUI.h"
// CPangolinListCtrl

class CPangolinListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CPangolinListCtrl)

public:
	CPangolinListCtrl();
	virtual ~CPangolinListCtrl();
    
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    BOOL Create(CWnd* pParent, UINT nID);
    BOOL Insert(TASK_ITEM *item);
    BOOL Edit(int iIndex, const TASK_ITEM *item, BOOL bSel = FALSE);
    BOOL Remove(DWORD tid);

    BOOL UpdatePercent(DWORD tid, int percent);

    int FindItemByTID(DWORD tid);
    afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
    void SelItemByIndex(int nIndex);
    int GetCurSelItem(CString& tid,CString& url);
private:
    CImageList  m_imgList;

    CMenu m_Menu; 
public:

    BOOL PutStringToClipboard( CString &csText );
    afx_msg void OnPopupCopyurl();
};


