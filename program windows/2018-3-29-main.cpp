// 
#include <windows.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	static TCHAR szAppName[] = TEXT("SysMets1");
	HWND		hwnd;
	MSG			msg;
	WNDCLASS	wndclass;

	wndclass.style 		       = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc       = WndProc;
	wndclass.cbClsExtra        = 0;
	wndclass.cbWndExtra        = 0;
	wndclass.hInstance         = hInstance;
	wndclass.hIcon             = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hCursor           = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hbrBackground     = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName      = nullptr;
	wndclass.lpszClassName     = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(nullptr, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Get System Metrics No. 1"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxChar, cyChar, cxCaps;
	HDC hdc;
	int i;
	PAINTSTRUCT ps;
	TCHAR szBuffer[10];
	TEXTMETRIC tm;

	switch (message)
	{
	case WM_CREATE:
		hdc     = GetDC(hwnd);

		/*
		 *	tm.tmPitchAndFamily: 1变宽字体，0等宽字体
		 */
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxChar = tm.tmAveCharWidth;


		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (i = 0; i < NUMLINES; i++)
		{
			TextOut(hdc, 0, cyChar * i, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));

			TextOut(hdc, 22 * cxCaps, cyChar * i, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);

			TextOut(hdc, 22 * cxCaps + 40 * cxChar, cyChar * i, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));

			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}