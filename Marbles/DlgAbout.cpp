#include "stdafx.h"
#include "Resource.h"
#include "DlgAbout.h"

extern HINSTANCE g_hInstance;

///----------------------------------------------------------------------------------------------// 
///                                    L1 Message Handlers                                       //
///----------------------------------------------------------------------------------------------//
static void OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	/*GetWindowRect(GetParent(hWnd), &rcParent);
	GetWindowRect(hWnd, &rcDlg);
	MoveWindow(hWnd, rcParent.left + 50, rcParent.top + 50, RC_WIDTH(rcDlg), RC_HEIGHT(rcDlg), TRUE);*/
	SetDlgItemText(hWnd, IDC_EDT_ABOUT,
		TEXT("版权所有，仿冒必究."));
}

static void OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	BeginPaint(hWnd, &ps);
	Rectangle(ps.hdc, rcClient.left - 1, rcClient.top - 1, rcClient.right + 1, rcClient.bottom + 1);
	EndPaint(hWnd, &ps);
}

static void OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	EndDialog(hWnd, 0);
}

static void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDOK)
	{
		EndDialog(hWnd, 0);
	}
}

//static void OnCtlColorDlg(HWND hWnd, WPARAM wParam, LPARAM lParam)
//{
//	if (wParam == WM_CTLCOLORDLG)
//	{
//		return (INT_PTR)GetStockObject(WHITE_BRUSH);
//	}
//}


INT_PTR CALLBACK ProcDlgAbout(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#define PROCESS_MSG(MSG, HANDLER) if(uMsg == MSG) { HANDLER(hWnd, wParam, lParam); return TRUE; }
		PROCESS_MSG(WM_INITDIALOG, OnInitDialog)
		PROCESS_MSG(WM_CLOSE, OnClose)
		PROCESS_MSG(WM_COMMAND, OnCommand)
		PROCESS_MSG(WM_PAINT, OnPaint)
		if (uMsg == WM_CTLCOLORSTATIC) return (INT_PTR)GetStockObject(WHITE_BRUSH);
		/*PROCESS_MSG(WM_CTLCOLORDLG, OnCtlColorDlg);*/
#undef PROCESS_MSG
		return FALSE;
}