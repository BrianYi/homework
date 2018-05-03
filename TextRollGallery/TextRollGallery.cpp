// TextRollGallery.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TextRollGallery.h"
#include "SysMets.h"
#include <strsafe.h>
#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TEXTROLLGALLERY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEXTROLLGALLERY));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEXTROLLGALLERY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TEXTROLLGALLERY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static		int cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth;
	static		int DeltaMove = 5, xBeg;
	HDC			hdc;
	int			i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd;
	PAINTSTRUCT ps;
	SCROLLINFO  si;
	TCHAR       szBuffer[10];
	TEXTMETRIC  tm;
	ULONG		ulScrollLines;
   switch  (message)
   {
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		xBeg = -cxClient;
		return 0;

	case WM_CREATE:
		SetTimer(hWnd, 1, 10, nullptr);
		hdc = GetDC(hWnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hWnd, hdc);
		iMaxWidth = 40 * cxChar + 22 * cxCaps;

   case WM_COMMAND:
       {
           int wmId = LOWORD(wParam);
           // �����˵�ѡ��: 
           switch (wmId)
           {
           case IDM_ABOUT:
               DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
               break;
           case IDM_EXIT:
               DestroyWindow(hWnd);
               break;
           default:
               return DefWindowProc(hWnd, message, wParam, lParam);
           }
       }
       break;
   case WM_PAINT:
       {
           PAINTSTRUCT ps;
           HDC hdc = BeginPaint(hWnd, &ps);
           // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
			iVertPos = 0;

			iPaintBeg = 0;
			iPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / cyChar);

			for (i = iPaintBeg; i <= iPaintEnd; ++i)
			{
				x = xBeg;
				y = cyChar * (i - iVertPos);
				TextOut(hdc, x, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
				TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
				SetTextAlign(hdc, TA_RIGHT | TA_TOP);
				TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));
				SetTextAlign(hdc, TA_LEFT | TA_TOP);
			}
			
           EndPaint(hWnd, &ps);
       }
       break;
	case WM_TIMER:
		xBeg += DeltaMove;
		if (xBeg >= cxClient)
			xBeg = -cxClient;
		ScrollWindow(hWnd, DeltaMove, 0, nullptr, nullptr);
		//UpdateWindow(hWnd);
		//InvalidateRect(hWnd, nullptr, TRUE);
		break;
   case WM_DESTROY:
		KillTimer(hWnd, 1);
       PostQuitMessage(0);
       break;

   default:
       return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

//LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	HDC hdc;
//	PAINTSTRUCT ps;
//	TEXTMETRIC tm;
//	SCROLLINFO si;
//
//	// These variables are required to display text. 
//	static int xClient;     // width of client area 
//	static int yClient;     // height of client area 
//	static int xClientMax;  // maximum width of client area 
//
//	static int xChar;       // horizontal scrolling unit 
//	static int yChar;       // vertical scrolling unit 
//	static int xUpper;      // average width of uppercase letters 
//
//	static int xPos;        // current horizontal scrolling position 
//	static int yPos;        // current vertical scrolling position 
//
//	int i;                  // loop counter 
//	int x, y;               // horizontal and vertical coordinates
//
//	int FirstLine;          // first line in the invalidated area 
//	int LastLine;           // last line in the invalidated area 
//	HRESULT hr;
//	size_t abcLength;        // length of an abc[] item 
//
//							 // Create an array of lines to display. 
//#define LINES 28 
//	static TCHAR *abc[] = {
//		TEXT("anteater"),  TEXT("bear"),      TEXT("cougar"),
//		TEXT("dingo"),     TEXT("elephant"),  TEXT("falcon"),
//		TEXT("gazelle"),   TEXT("hyena"),     TEXT("iguana"),
//		TEXT("jackal"),    TEXT("kangaroo"),  TEXT("llama"),
//		TEXT("moose"),     TEXT("newt"),      TEXT("octopus"),
//		TEXT("penguin"),   TEXT("quail"),     TEXT("rat"),
//		TEXT("squid"),     TEXT("tortoise"),  TEXT("urus"),
//		TEXT("vole"),      TEXT("walrus"),    TEXT("xylophone"),
//		TEXT("yak"),       TEXT("zebra"),
//		TEXT("This line contains words, but no character. Go figure."),
//		TEXT("")
//	};
//
//	switch (uMsg)
//	{
//	case WM_CREATE:
//		// Get the handle to the client area's device context. 
//		hdc = GetDC(hwnd);
//
//		// Extract font dimensions from the text metrics. 
//		GetTextMetrics(hdc, &tm);
//		xChar = tm.tmAveCharWidth;
//		xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * xChar / 2;
//		yChar = tm.tmHeight + tm.tmExternalLeading;
//
//		// Free the device context. 
//		ReleaseDC(hwnd, hdc);
//
//		// Set an arbitrary maximum width for client area. 
//		// (xClientMax is the sum of the widths of 48 average 
//		// lowercase letters and 12 uppercase letters.) 
//		xClientMax = 48 * xChar + 12 * xUpper;
//
//		return 0;
//
//	case WM_SIZE:
//
//		// Retrieve the dimensions of the client area. 
//		yClient = HIWORD(lParam);
//		xClient = LOWORD(lParam);
//
//		// Set the vertical scrolling range and page size
//		si.cbSize = sizeof(si);
//		si.fMask = SIF_RANGE | SIF_PAGE;
//		si.nMin = 0;
//		si.nMax = LINES - 1;
//		si.nPage = yClient / yChar;
//		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
//
//		// Set the horizontal scrolling range and page size. 
//		si.cbSize = sizeof(si);
//		si.fMask = SIF_RANGE | SIF_PAGE;
//		si.nMin = 0;
//		si.nMax = 2 + xClientMax / xChar;
//		si.nPage = xClient / xChar;
//		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
//
//		return 0;
//	case WM_HSCROLL:
//		// Get all the vertial scroll bar information.
//		si.cbSize = sizeof(si);
//		si.fMask = SIF_ALL;
//
//		// Save the position for comparison later on.
//		GetScrollInfo(hwnd, SB_HORZ, &si);
//		xPos = si.nPos;
//		switch (LOWORD(wParam))
//		{
//			// User clicked the left arrow.
//		case SB_LINELEFT:
//			si.nPos -= 1;
//			break;
//
//			// User clicked the right arrow.
//		case SB_LINERIGHT:
//			si.nPos += 1;
//			break;
//
//			// User clicked the scroll bar shaft left of the scroll box.
//		case SB_PAGELEFT:
//			si.nPos -= si.nPage;
//			break;
//
//			// User clicked the scroll bar shaft right of the scroll box.
//		case SB_PAGERIGHT:
//			si.nPos += si.nPage;
//			break;
//
//			// User dragged the scroll box.
//		case SB_THUMBTRACK:
//			si.nPos = si.nTrackPos;
//			break;
//
//		default:
//			break;
//		}
//
//		// Set the position and then retrieve it.  Due to adjustments
//		// by Windows it may not be the same as the value set.
//		si.fMask = SIF_POS;
//		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
//		GetScrollInfo(hwnd, SB_HORZ, &si);
//
//		// If the position has changed, scroll the window.
//		if (si.nPos != xPos)
//		{
//			ScrollWindow(hwnd, xChar * (xPos - si.nPos), 0, NULL, NULL);
//		}
//
//		return 0;
//
//	case WM_VSCROLL:
//		// Get all the vertial scroll bar information.
//		si.cbSize = sizeof(si);
//		si.fMask = SIF_ALL;
//		GetScrollInfo(hwnd, SB_VERT, &si);
//
//		// Save the position for comparison later on.
//		yPos = si.nPos;
//		switch (LOWORD(wParam))
//		{
//
//			// User clicked the HOME keyboard key.
//		case SB_TOP:
//			si.nPos = si.nMin;
//			break;
//
//			// User clicked the END keyboard key.
//		case SB_BOTTOM:
//			si.nPos = si.nMax;
//			break;
//
//			// User clicked the top arrow.
//		case SB_LINEUP:
//			si.nPos -= 1;
//			break;
//
//			// User clicked the bottom arrow.
//		case SB_LINEDOWN:
//			si.nPos += 1;
//			break;
//
//			// User clicked the scroll bar shaft above the scroll box.
//		case SB_PAGEUP:
//			si.nPos -= si.nPage;
//			break;
//
//			// User clicked the scroll bar shaft below the scroll box.
//		case SB_PAGEDOWN:
//			si.nPos += si.nPage;
//			break;
//
//			// User dragged the scroll box.
//		case SB_THUMBTRACK:
//			si.nPos = si.nTrackPos;
//			break;
//
//		default:
//			break;
//		}
//
//		// Set the position and then retrieve it.  Due to adjustments
//		// by Windows it may not be the same as the value set.
//		si.fMask = SIF_POS;
//		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
//		GetScrollInfo(hwnd, SB_VERT, &si);
//
//		// If the position has changed, scroll window and update it.
//		if (si.nPos != yPos)
//		{
//			ScrollWindow(hwnd, 0, yChar * (yPos - si.nPos), NULL, NULL);
//			UpdateWindow(hwnd);
//		}
//
//		return 0;
//
//	case WM_PAINT:
//		// Prepare the window for painting.
//		hdc = BeginPaint(hwnd, &ps);
//
//		// Get vertical scroll bar position.
//		si.cbSize = sizeof(si);
//		si.fMask = SIF_POS;
//		GetScrollInfo(hwnd, SB_VERT, &si);
//		yPos = si.nPos;
//
//		// Get horizontal scroll bar position.
//		GetScrollInfo(hwnd, SB_HORZ, &si);
//		xPos = si.nPos;
//
//		// Find painting limits.
//		FirstLine = max(0, yPos + ps.rcPaint.top / yChar);
//		LastLine = min(LINES - 1, yPos + ps.rcPaint.bottom / yChar);
//
//		for (i = FirstLine; i <= LastLine; i++)
//		{
//			x = xChar * (1 - xPos);
//			y = yChar * (i - yPos);
//
//			// Note that "55" in the following depends on the 
//			// maximum size of an abc[] item. Also, you must include
//			// strsafe.h to use the StringCchLength function.
//			hr = StringCchLength(abc[i], 55, &abcLength);
//			if ((FAILED(hr)) | (abcLength == NULL))
//			{
//				//
//				// TODO: write error handler
//				//
//			}
//
//			// Write a line of text to the client area.
//			TextOut(hdc, x, y, abc[i], abcLength);
//		}
//
//		// Indicate that painting is finished.
//		EndPaint(hwnd, &ps);
//		return 0;
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
