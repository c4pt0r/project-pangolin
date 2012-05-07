#include "StdAfx.h"
#include "TimeOutSocket.h"

BOOL CTimeOutSocket::OnMessagePending()
{
    MSG msg;
    if(::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE))
    {
        if (msg.wParam == (UINT) m_nTimerID)
        {
            // Remove the message and call CancelBlockingCall.
            ::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
            CancelBlockingCall();
            return FALSE;  // No need for idle time processing.
        };
    };

    return CSocket::OnMessagePending();
}

BOOL CTimeOutSocket::SetTimeOut(UINT uTimeOut)
{
    m_nTimerID = SetTimer(NULL,0,uTimeOut,NULL);
    return m_nTimerID;
}

BOOL CTimeOutSocket::KillTimeOut()
{
    return KillTimer(NULL,m_nTimerID);
}