// Marbles.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Marbles.h"
#include "DlgAbout.h"
#include <math.h>
#include <time.h>
#include <vector>
using namespace std;
#define MAX_LOADSTRING  100

// 全局变量: 
LONG g_framePerSec = 40, g_num = 10;
FLOAT g_v_g = 3.0f, g_v_f = 1.0f;
HINSTANCE g_hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
vector<LOGFONT*> g_vecLogFont;
LOGFONT* g_curLogFont;
DWORD g_mainDlgWidth = 0, g_mainDlgHeight = 0;
SIZE g_szMax;
TCHAR g_szFontInfo[][256] = 
{
	TEXT("FaceName: %s"),
	TEXT("Height: %d"),
	TEXT("Width: %d"),
	TEXT("Escapement: %d"),
	TEXT("Orientation: %d"),
	TEXT("Weight: %d"),
	TEXT("Italic: %d"),
	TEXT("Underline: %d"),
	TEXT("StrikeOut: %d"),
	TEXT("CharSet: %d"),
	TEXT("OutPrecision: %d"),
	TEXT("ClipPrecision: %d"),
	TEXT("Quality: %d"),
	TEXT("PitchAndFamily: %d")
};
enum Shape;
typedef struct DynamicRect
{
	RECT	rc;
	LONG	w, h;
	double	h_deltaMove, v_deltaMove;
	double	h_at, h_v0, h_vt, h_f, v_at, v_v0, v_vt, v_f;
	BOOL	h_sign, v_sign;
	HRGN	hRgn, hRgnNew, hRgnOld;
	DWORD	rgb;
	Shape	shape;
}DYRECT,*PDYRECT;
DYRECT *sDyRects;
//HWND /*hwndCanvas, */hwndMenu;
//HWND hwndFrameRate, hwndGravity, hwndResistance, hwndNum, hwndShape, hwndFontType, hwndOK;

DWORD deltaDuration = 0, lastTimePoint = 0;
BOOL g_pause = TRUE;
BOOL g_bFirst = TRUE;
enum Shape {
	Circle, Rect
};
struct ShapeVar
{
	Shape shape;
	TCHAR name[32];
} g_shapes[] = {
	{ Circle, TEXT("圆形") },
	{ Rect, TEXT("矩形") }
};

Shape FindShape(TCHAR *szShape)
{
	for (auto& shape : g_shapes)
	{
		if (_tcscmp(shape.name, szShape) == 0)
			return shape.shape;
	}
	return Shape::Circle;
}

LOGFONT* FindLogFont(TCHAR *szFaceName)
{
	for (auto& lf : g_vecLogFont)
	{
		if (_tcscmp(lf->lfFaceName, szFaceName) == 0)
			return lf;
	}
	return nullptr;
}

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK ProcDlgMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
SIZE CalculateFontInfo(HDC hdc, LOGFONT *lf);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	
    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MARBLES, szWindowClass, MAX_LOADSTRING);
    //MyRegisterClass(hInstance);
	g_hInst = hInstance;
    // 执行应用程序初始化: 
//     if (!InitInstance (hInstance, nCmdShow))
//     {
//         return FALSE;
//     }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MARBLES));
	
    MSG msg;
	CreateDialogParam(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, ProcDlgMain, 0);
	
    // 主消息循环: 
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

static INT_PTR OnExit(HWND hWnd);

static INT_PTR OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	/*HCURSOR hCursor = LoadCursor(NULL, IDC_IBEAM);
	SetCursor(hCursor);*/
	return TRUE;
}

static INT_PTR OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) { return TRUE; }
static INT_PTR OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) { return TRUE; }

int CALLBACK EnumFontFamExProc(
	const LOGFONT    *lpelfe,
	const TEXTMETRIC *lpntme,
	DWORD      FontType,
	LPARAM     lParam
)
{
	HWND hWnd = (HWND)lParam;
	ComboBox_AddString(GetDlgItem(hWnd, IDC_CBBOX_FONT_TYPE), lpelfe->lfFaceName);
	LOGFONT *lf = new LOGFONT;
	RtlCopyMemory(lf, lpelfe, sizeof(LOGFONT));
	g_vecLogFont.push_back(lf);
	return 1;
}


//void ResizeWindow(HWND hWnd)
//{
//	//图形 + 字体栏 + 菜单
//	g_mainDlgWidth = X_FONT_INFO + 10 + g_szMax.cx + 10 + 400;
//	g_mainDlgHeight = max(500, 10 + g_szMax.cy * 16);
//
//	RECT rc;
//	GetWindowRect(hWnd, &rc);
//	MoveWindow(hWnd, rc.left, rc.top, g_mainDlgWidth, g_mainDlgHeight, TRUE);
//
//	//InvalidateRect(hWnd, nullptr, TRUE);
//}

void GenerateShapes(int num, Shape shape, int x, int y, int w, int h)
{
	if (sDyRects)
	{
		delete sDyRects;
		sDyRects = nullptr;
	}

	sDyRects = new DYRECT[num];
	ZeroMemory(sDyRects, sizeof(sDyRects[0]) * num);
	//随机生成形状
	for (int i = 0; i < num; ++i)
	{
		DYRECT& dyRect = sDyRects[i];
		//  随机生成
		dyRect.w = dyRect.h = 100;
		dyRect.v_at = 0.0;
		dyRect.v_f = g_v_f;
		dyRect.h_v0 = dyRect.h_vt = 2.0;
		dyRect.h_sign = dyRect.v_sign = TRUE;
		dyRect.shape = shape;

		dyRect.rc.left = rand() % ((x + w) - dyRect.w);
		dyRect.rc.top = rand() % ((y + h) - dyRect.h);
		dyRect.rc.right = dyRect.rc.left + dyRect.w;
		dyRect.rc.bottom = dyRect.rc.top + dyRect.h;
		dyRect.rgb = rand() % DWORD(-1);
	}
}

static INT_PTR OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	TCHAR szBufferPlay[64];
	LoadString(g_hInst, IDS_BTN_PLAY, szBufferPlay, ARRAYSIZE(szBufferPlay));
	SetDlgItemText(hWnd, IDC_BTN_PAUSE, szBufferPlay);

	//帧率
	TCHAR szBuffer[64];
	_stprintf_s(szBuffer, TEXT("%d"), g_framePerSec);
	SetDlgItemText(hWnd, IDC_EDT_FRAME_RATE, szBuffer);

	//重力，阻力
	_stprintf_s(szBuffer, TEXT("%.2f"), g_v_g);
	SetDlgItemText(hWnd, IDC_EDT_GRAVITY, szBuffer);
	_stprintf_s(szBuffer, TEXT("%.2f"), g_v_f);
	SetDlgItemText(hWnd, IDC_EDT_RESISTANCE, szBuffer);

	//数量
	_stprintf_s(szBuffer, TEXT("%d"), g_num);
	SetDlgItemText(hWnd, IDC_EDT_NUM, szBuffer);

	//形状
	for (int i = 0; i < ARRAYSIZE(g_shapes); ++i)
	{
		ComboBox_AddString(GetDlgItem(hWnd, IDC_CBBOX_SHAPE), g_shapes[i].name);
	}
	ComboBox_SetCurSel(GetDlgItem(hWnd, IDC_CBBOX_SHAPE), 0);
	ComboBox_GetText(GetDlgItem(hWnd, IDC_CBBOX_SHAPE), szBuffer, sizeof(szBuffer));
	Shape shape = FindShape(szBuffer);
	

	
	//字体
	HDC hdc = GetDC(hWnd);
	LOGFONT logFont;
	ZeroMemory(&logFont, sizeof(logFont));
	logFont.lfFaceName[0] = TEXT('\0');
	logFont.lfCharSet = DEFAULT_CHARSET;
	//EnumFontFamiliesEx(hdc, &logFont, EnumFontFamExProc, (LPARAM)hWnd, 0);
	//EnumFontFamilies(hdc, NULL, EnumFontFamExProc, (LPARAM)hWnd);
	EnumFontFamilies(hdc, NULL, EnumFontFamExProc, (LPARAM)hWnd);
	//EnumFonts(hdc, NULL, EnumFontFamExProc, (LPARAM)hWnd);
	ComboBox_SetCurSel(GetDlgItem(hWnd, IDC_CBBOX_FONT_TYPE), 0);
	ComboBox_GetText(GetDlgItem(hWnd, IDC_CBBOX_FONT_TYPE), szBuffer, sizeof(szBuffer));
	g_curLogFont = FindLogFont(szBuffer);
	DeleteObject(SelectObject(hdc, CreateFontIndirect(g_curLogFont)));
	g_szMax = CalculateFontInfo(hdc, g_curLogFont);
	ReleaseDC(hWnd, hdc);

	/*
	设置计时器
	*/
	SetTimer(hWnd, 1, (UINT)(1000.0f / g_framePerSec), nullptr);
	//lastTimePoint = GetTickCount();

	g_mainDlgWidth = X_FONT_INFO + 10 + g_szMax.cx + 10 + 400;
	g_mainDlgHeight = max(500, 10 + g_szMax.cy * 16);
	/*
	初始化方块
	*/
	srand((unsigned int)time(nullptr));	// 生成随机数种子
	GenerateShapes(g_num, shape, 0, 0, X_FONT_INFO, g_mainDlgHeight);

	MoveWindow(hWnd, X_MAIN_DLG, Y_MAIN_DLG, g_mainDlgWidth, g_mainDlgHeight, TRUE);

	ShowWindow(hWnd, SW_SHOW);

	return TRUE;
}

static INT_PTR OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	EndDialog(hWnd, 0);
	OnExit(hWnd);
	return TRUE;
}

static INT_PTR OnQueryEndSession(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	SetDlgMsgResult(hWnd, WM_QUERYENDSESSION, TRUE);
	return TRUE;
}

static INT_PTR OnEndSession(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	OnExit(hWnd);
	return TRUE;
}

static INT_PTR OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// Message Source		wParam(high word)					wParam(low word)				lParam
	// Menu					0									Menu identifier(IDM_*)			0
	// Accelerator			1									Accelerator identifier(IDM_*)	0
	// Control				Control-defined notification code	Control identifier				Handle to the control window
	
	if (LOWORD(wParam) == IDM_EXIT)
	{
		OnExit(hWnd);
	}
	else if (LOWORD(wParam) == IDM_ABOUT)
	{
		DialogBoxParam(g_hInst, MAKEINTRESOURCE(DLG_ABOUT), hWnd, ProcDlgAbout, 0);
		//DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
	}
	else if (LOWORD(wParam) == IDC_BTN_PAUSE)
	{
		TCHAR szBuffer[64], szBufferPlay[64], szBufferPause[64];
		GetDlgItemText(hWnd, IDC_BTN_PAUSE, szBuffer, ARRAYSIZE(szBuffer));
		LoadString(g_hInst, IDS_BTN_CONTINUE, szBufferPlay, ARRAYSIZE(szBufferPlay));
		LoadString(g_hInst, IDS_BTN_PAUSE, szBufferPause, ARRAYSIZE(szBufferPause));
		if (_tccmp(szBuffer, szBufferPlay) == 0)
		{
			SetDlgItemText(hWnd, IDC_BTN_PAUSE, szBufferPause);
			g_pause = FALSE;
		}
		else
		{
			SetDlgItemText(hWnd, IDC_BTN_PAUSE, szBufferPlay);
			g_pause = TRUE;
		}
	}
	else if (LOWORD(wParam) == IDC_CBBOX_SHAPE)
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			/*TCHAR szBuffer[64];
			HWND hwndDlgShape = (HWND)lParam;
			ComboBox_GetText(hwndDlgShape, szBuffer, sizeof(szBuffer));
			Shape shape = FindShape(szBuffer);
			for (int i = 0; i < g_num; ++i)
			{
				DYRECT& dyRect = sDyRects[i];
				dyRect.shape = shape;
			}
			InvalidateRect(hWnd, nullptr, TRUE);*/
		}
	}
	else if (LOWORD(wParam) == IDC_CBBOX_FONT_TYPE)
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			//TCHAR szBuffer[64];
			//HWND hwndDlgFontType = (HWND)lParam;
			//ComboBox_GetText(hwndDlgFontType, szBuffer, sizeof(szBuffer));
			//g_curLogFont = FindLogFont(szBuffer);
			////SendMessage(GetDlgItem(hWnd, DLG_MAIN), WM_SETFONT, (WPARAM)CreateFontIndirect(lf), TRUE);
			////UpdateWindow(hWnd);
			//HDC hdc = GetDC(hWnd);
			//DeleteObject(SelectObject(hdc, CreateFontIndirect(g_curLogFont)));
			//g_szMax = CalculateFontInfo(hdc, g_curLogFont);
			//ReleaseDC(hWnd, hdc);
			//ResizeWindow(hWnd);
		}
	}
	else if (LOWORD(wParam) == IDC_EDT_FRAME_RATE)
	{
		/*if (HIWORD(wParam) == EN_CHANGE)
		{
			g_framePerSec = GetDlgItemInt(hWnd, LOWORD(wParam), nullptr, FALSE);
			SetTimer(hWnd, 1, (UINT)(1000.0f / g_framePerSec), nullptr);
		}*/
	}
	else if (LOWORD(wParam) == IDC_BTN_OK)
	{
		if (HIWORD(wParam) == BN_CLICKED)
		{
			TCHAR szBuffer[64];
			g_framePerSec = GetDlgItemInt(hWnd, IDC_EDT_FRAME_RATE, nullptr, FALSE);
			GetDlgItemText(hWnd, IDC_EDT_GRAVITY, szBuffer, sizeof(szBuffer));
			g_v_g = _tcstof(szBuffer, nullptr);
			GetDlgItemText(hWnd, IDC_EDT_RESISTANCE, szBuffer, sizeof(szBuffer));
			g_v_f = _tcstof(szBuffer, nullptr);
			int newNum = GetDlgItemInt(hWnd, IDC_EDT_NUM, nullptr, FALSE);

			ComboBox_GetText(GetDlgItem(hWnd, IDC_CBBOX_SHAPE), szBuffer, sizeof(szBuffer));
			Shape shape = FindShape(szBuffer);
			for (int i = 0; i < g_num; ++i)
			{
				DYRECT& dyRect = sDyRects[i];
				dyRect.shape = shape;
			}

			ComboBox_GetText(GetDlgItem(hWnd, IDC_CBBOX_FONT_TYPE), szBuffer, sizeof(szBuffer));
			g_curLogFont = FindLogFont(szBuffer);
			//SendMessage(GetDlgItem(hWnd, DLG_MAIN), WM_SETFONT, (WPARAM)CreateFontIndirect(lf), TRUE);
			//UpdateWindow(hWnd);
			HDC hdc = GetDC(hWnd);
			DeleteObject(SelectObject(hdc, CreateFontIndirect(g_curLogFont)));
			g_szMax = CalculateFontInfo(hdc, g_curLogFont);
			ReleaseDC(hWnd, hdc);

			/*ResizeWindow(hWnd);*/
			//图形 + 字体栏 + 菜单
			g_mainDlgWidth = X_FONT_INFO + 10 + g_szMax.cx + 10 + 400;
			g_mainDlgHeight = max(500, 10 + g_szMax.cy * 16);

			if (newNum != g_num)
			{
				g_num = newNum;
				GenerateShapes(g_num, shape, 0, 0, X_FONT_INFO, g_mainDlgHeight);
			}

			RECT rc;
			GetWindowRect(hWnd, &rc);
			MoveWindow(hWnd, rc.left, rc.top, g_mainDlgWidth, g_mainDlgHeight, TRUE);
			InvalidateRect(hWnd, nullptr, TRUE);
		}
	}
	return TRUE;
}

SIZE CalculateFontInfo(HDC hdc, LOGFONT *lf)
{
	TCHAR szBuffer[256];
	SIZE szMax, szTmp;
	g_szMax = { 0,0 };
	auto sz_max = [](SIZE &s1, SIZE &s2)->SIZE {
		SIZE tmp;
		tmp.cx = s1.cx > s2.cx ? s1.cx : s2.cx;
		tmp.cy = s1.cy > s2.cy ? s1.cy : s2.cy;
		return tmp;
	};

	_stprintf_s(szBuffer, g_szFontInfo[0], lf->lfFaceName);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = szTmp;

	_stprintf_s(szBuffer, g_szFontInfo[1], lf->lfHeight);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[2], lf->lfWidth);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[3], lf->lfEscapement);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[4], lf->lfOrientation);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[5], lf->lfWeight);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[6], lf->lfItalic);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[7], lf->lfUnderline);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[8], lf->lfStrikeOut);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[9], lf->lfCharSet);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[10], lf->lfOutPrecision);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[11], lf->lfClipPrecision);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[12], lf->lfQuality);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[13], lf->lfPitchAndFamily);
	GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);
	
	g_szMax = szMax;
	g_szMax.cy = max(g_curLogFont->lfHeight, g_szMax.cy);
	return g_szMax;
}

void TextOutFontInfo(HDC hdc, LOGFONT *lf)
{
	DWORD fontH = g_szMax.cy;
	TCHAR szBuffer[256];

	_stprintf_s(szBuffer, g_szFontInfo[0], lf->lfFaceName);
	TextOut(hdc, X_FONT_INFO + 10, 10, szBuffer, _tcslen(szBuffer));
	/*GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);*/
	
	_stprintf_s(szBuffer, g_szFontInfo[1], lf->lfHeight);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 1 * fontH, szBuffer, _tcslen(szBuffer));
	/*GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);*/
	
	_stprintf_s(szBuffer, g_szFontInfo[2], lf->lfWidth);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 2 * fontH, szBuffer, _tcslen(szBuffer));
	/*GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);*/

	_stprintf_s(szBuffer, g_szFontInfo[3], lf->lfEscapement);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 3 * fontH, szBuffer, _tcslen(szBuffer));
	/*GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);*/

	_stprintf_s(szBuffer, g_szFontInfo[4], lf->lfOrientation);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 4 * fontH, szBuffer, _tcslen(szBuffer));
	/*GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);*/

	_stprintf_s(szBuffer, g_szFontInfo[5], lf->lfWeight);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 5 * fontH, szBuffer, _tcslen(szBuffer));
	/*GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);*/

	_stprintf_s(szBuffer, g_szFontInfo[6], lf->lfItalic);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 6 * fontH, szBuffer, _tcslen(szBuffer));
	/*GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);*/

	_stprintf_s(szBuffer, g_szFontInfo[7], lf->lfUnderline);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 7 * fontH, szBuffer, _tcslen(szBuffer));
	/*GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);*/

	_stprintf_s(szBuffer, g_szFontInfo[8], lf->lfStrikeOut);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 8 * fontH, szBuffer, _tcslen(szBuffer));
	/*szMax = sz_max(szMax, szTmp);*/

	_stprintf_s(szBuffer, g_szFontInfo[9], lf->lfCharSet);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 9 * fontH, szBuffer, _tcslen(szBuffer));
	/*GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);*/

	_stprintf_s(szBuffer, g_szFontInfo[10],	lf->lfOutPrecision);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 10 * fontH, szBuffer, _tcslen(szBuffer));
	/*GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	szMax = sz_max(szMax, szTmp);*/

	_stprintf_s(szBuffer, g_szFontInfo[11], lf->lfClipPrecision);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 11 * fontH, szBuffer, _tcslen(szBuffer));
	//GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	//szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[12], lf->lfQuality);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 12 * fontH, szBuffer, _tcslen(szBuffer));
	//GetTextExtentPoint32(hdc, szBuffer, _tcslen(szBuffer), &szTmp);
	//szMax = sz_max(szMax, szTmp);

	_stprintf_s(szBuffer, g_szFontInfo[13], lf->lfPitchAndFamily);
	TextOut(hdc, X_FONT_INFO + 10, 10 + 13 * fontH, szBuffer, _tcslen(szBuffer));

	//return szMax;
}

static INT_PTR OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	BeginPaint(hWnd, &ps);
	DeleteObject(SelectObject(ps.hdc, GetStockObject(WHITE_BRUSH)));
	Rectangle(ps.hdc, rcClient.left - 1, rcClient.top - 1, rcClient.right + 1, rcClient.bottom + 1);
	DeleteObject(SelectObject(ps.hdc, GetStockObject(DC_BRUSH)));
	DeleteObject(SelectObject(ps.hdc, CreateFontIndirect(g_curLogFont)));
	SetBkColor(ps.hdc, RGB(0xff, 0xff, 0xff));
	SetBkMode(ps.hdc, OPAQUE);
	/*DWORD begX = 0, begY = 0, fontH = g_curLogFont->lfHeight;
	_stprintf_s(szBuffer, TEXT("这是中文，用于字体测试"));
	TextOut(ps.hdc, 10, 10, szBuffer, _tcslen(szBuffer));

	_stprintf_s(szBuffer, TEXT("Hello world, what the fuck!"));
	TextOut(ps.hdc, 10, 30, szBuffer, _tcslen(szBuffer));*/
	

// 	SetMapMode(hdc, MM_ISOTROPIC);
// 	SetWindowExtEx(hdc, cxClient, cyClient, nullptr);
// 	SetWindowOrgEx(hdc, 0, 0, nullptr);
// 	SetViewportExtEx(hdc, cxClient, cyClient, nullptr);
// 	SetViewportOrgEx(hdc, 0, 0, nullptr);
	for (int i = 0; i < g_num; ++i)
	{
		DYRECT& dyRect = sDyRects[i];
		SetDCBrushColor(ps.hdc, dyRect.rgb);
		switch (dyRect.shape)
		{
		case Circle:
			Ellipse(ps.hdc, dyRect.rc.left, dyRect.rc.top, dyRect.rc.right, dyRect.rc.bottom);
			break;
		case Rect:
			Rectangle(ps.hdc, dyRect.rc.left, dyRect.rc.top, dyRect.rc.right, dyRect.rc.bottom);
			break;
		default:
			break;
		}
	}
	HPEN penVertical = CreatePen(PS_SOLID, 2, RGB(0xC4, 0xC4, 0xC4));
	HGDIOBJ oldPen = SelectObject(ps.hdc, penVertical);
	DeleteObject(SelectObject(ps.hdc, oldPen));
	g_szMax = CalculateFontInfo(ps.hdc, g_curLogFont);
	TextOutFontInfo(ps.hdc, g_curLogFont);
	MoveToEx(ps.hdc, X_FONT_INFO, 0, nullptr);
	LineTo(ps.hdc, X_FONT_INFO, g_mainDlgHeight);
	MoveToEx(ps.hdc, X_FONT_INFO + 10 + g_szMax.cx + 10, 0, nullptr);
	LineTo(ps.hdc, X_FONT_INFO + 10 + g_szMax.cx + 10, g_mainDlgHeight);
	EndPaint(hWnd, &ps);
	return TRUE;
}

static INT_PTR OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{	
	//菜单窗口的位置为相对于最顶层窗口的位置
	//MoveWindow(hwndMenu, rcCanvas.right, 0, cxClient - rcCanvas.right, cyClient, TRUE);
	//GetClientRect(hwndMenu, &rcMenu);

	DWORD xBeg = X_FONT_INFO + 10 + g_szMax.cx + 10 + 30;
	
	MoveWindow(GetDlgItem(hWnd, IDC_LB_FRAME_RATE), xBeg,			32,	27, 18, TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_EDT_FRAME_RATE), xBeg + 74,	32, 50, 24, TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_LB_FRAME_RATE2), xBeg + 128,	32, 42, 18, TRUE);
	
	MoveWindow(GetDlgItem(hWnd, IDC_LB_GRAVITY),	xBeg,			54 + 10, 61, 18, TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_EDT_GRAVITY),	xBeg + 74,		51 + 10, 50, 24, TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_LB_GRAVITY2),	xBeg + 128,	51 + 10, 22, 18, TRUE);
	
	MoveWindow(GetDlgItem(hWnd, IDC_LB_RESISTANCE), xBeg,			74 + 20, 27, 18, TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_EDT_RESISTANCE), xBeg + 74,	71 + 20, 50, 24, TRUE);
	
	MoveWindow(GetDlgItem(hWnd, IDC_LB_NUM),		xBeg,			93 + 30, 27, 18, TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_EDT_NUM),		xBeg + 74,		90 + 30, 50, 24, TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_LB_NUM2),		xBeg + 128,	92 + 30, 19, 18, TRUE);
	
	MoveWindow(GetDlgItem(hWnd, IDC_LB_SHAPE),		xBeg,			111 + 40, 27, 18, TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_CBBOX_SHAPE),	xBeg + 74,		108 + 40, 150, 200, TRUE);
	
	MoveWindow(GetDlgItem(hWnd, IDC_LB_FONT_TYPE),	xBeg,			133 + 50, 27, 18, TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_CBBOX_FONT_TYPE), xBeg + 74,	131 + 50, 150, 200, TRUE);

	MoveWindow(GetDlgItem(hWnd, IDC_BTN_PAUSE),		xBeg + 10,		154 + 60, 60, 24, TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_BTN_OK),		xBeg + 100,	154 + 60, 60, 24, TRUE);

	

	return TRUE;
}

static INT_PTR OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam) { return TRUE; }

static INT_PTR OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

static INT_PTR OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (g_pause)
	{
		g_bFirst = TRUE;
		return TRUE;
	}

	DWORD currentTimePoint = GetTickCount();

	if (g_bFirst)
	{
		g_bFirst = FALSE;
		lastTimePoint = currentTimePoint;
	}

	// △t
	deltaDuration = (currentTimePoint - lastTimePoint) / 10;
	lastTimePoint = currentTimePoint;

	// 进行计算
	for (int i = 0; i < g_num; ++i)
	{
		DYRECT& dyRect = sDyRects[i];
		if (dyRect.v_sign)
		{
			// v,g方向相同，v增加
			dyRect.v_at = g_v_g - dyRect.v_f;
			dyRect.v_vt = dyRect.v_v0 + dyRect.v_at             * (deltaDuration);
			dyRect.v_deltaMove = (pow(dyRect.v_vt, 2.0f) - pow(dyRect.v_v0, 2.0f)) / (2.0f * dyRect.v_at);
			dyRect.v_v0 = dyRect.v_vt;
		}
		else
		{
			// v,g方向相反，v减少
			dyRect.v_at = g_v_g + dyRect.v_f;
			dyRect.v_vt = dyRect.v_v0 - dyRect.v_at             * (deltaDuration);
			dyRect.v_deltaMove = (pow(dyRect.v_vt, 2.0f) - pow(dyRect.v_v0, 2.0f)) / (2.0f * dyRect.v_at);
			dyRect.v_v0 = dyRect.v_vt;
		}

		if (dyRect.h_sign)
		{
			dyRect.h_deltaMove = dyRect.h_vt * (deltaDuration);
		}
		else
		{
			dyRect.h_deltaMove = -(dyRect.h_vt * (deltaDuration));
		}

		// 计算新位置
		RECT rcNew;
		rcNew.left = dyRect.rc.left + (LONG)dyRect.h_deltaMove;
		rcNew.right = rcNew.left + (LONG)dyRect.w;
		rcNew.top = dyRect.rc.top + (LONG)dyRect.v_deltaMove;
		rcNew.bottom = rcNew.top + (LONG)dyRect.h;

		// 是否到达上下下边界
		if (rcNew.bottom >= g_mainDlgHeight)
		{
			dyRect.v_sign = !dyRect.v_sign;
			rcNew.bottom = 2 * g_mainDlgHeight - rcNew.bottom;
			rcNew.top = rcNew.bottom - dyRect.h;
		}
		else if (dyRect.v_v0 < 0.0f)
		{
			dyRect.v_sign = !dyRect.v_sign;
			dyRect.v_v0 = 0.0f;
		}

		// 是否到达左右边界
		if (rcNew.left < 10 || rcNew.right >= X_FONT_INFO - 10)
		{
			dyRect.h_sign = !dyRect.h_sign;
		}

		// 合并区域
		dyRect.hRgnOld = CreateRectRgn(dyRect.rc.left, dyRect.rc.top, dyRect.rc.right, dyRect.rc.bottom);
		dyRect.hRgnNew = CreateRectRgn(rcNew.left, rcNew.top, rcNew.right, rcNew.bottom);
		CombineRgn(dyRect.hRgn, dyRect.hRgnOld, dyRect.hRgnNew, RGN_OR);

		dyRect.rc = rcNew;

		// 擦出之前的图形
		InvalidateRgn(hWnd, dyRect.hRgnOld, TRUE);
		InvalidateRgn(hWnd, dyRect.hRgnNew, TRUE);
	}

	return TRUE;
}

static INT_PTR Exit(HWND hWnd, bool restart)
{
	// Exit
	DestroyWindow(hWnd);
	PostQuitMessage(0);

	// stop timer
	KillTimer(hWnd, 1);

	// Release memory
	for (auto& var : g_vecLogFont)
	{
		delete var;
	}
	g_vecLogFont.clear();
	return TRUE;
}

static INT_PTR OnExit(HWND hWnd)
{
	Exit(hWnd, false);
	return TRUE;
}

static INT_PTR OnCtlColorStatic(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//HDC hdcStatic = (HDC)wParam;
	//SetBkColor(hdcStatic, RGB(0xff, 0xff, 0xff));
	return (INT_PTR)GetStockObject(WHITE_BRUSH);
}

//static void OnSetFont(HWND hWnd, WPARAM wParam, LPARAM lParam)
//{
//	HFONT font = (HFONT)wParam;
//
//}

///----------------------------------------------------------------------------------------------//
///                                    Main Dialog Proc                                          //
///----------------------------------------------------------------------------------------------//
INT_PTR CALLBACK ProcDlgMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#define PROCESS_MSG(MSG, HANDLER) if(uMsg == MSG) { return HANDLER(hWnd, wParam, lParam); }

	PROCESS_MSG(WM_MOUSEMOVE, OnMouseMove);
	PROCESS_MSG(WM_LBUTTONDOWN, OnLButtonDown);
	PROCESS_MSG(WM_LBUTTONUP, OnLButtonUp);
	PROCESS_MSG(WM_INITDIALOG, OnInitDialog);     // Init
	PROCESS_MSG(WM_CLOSE, OnClose);
	PROCESS_MSG(WM_QUERYENDSESSION, OnQueryEndSession);
	PROCESS_MSG(WM_ENDSESSION, OnEndSession);
	PROCESS_MSG(WM_COMMAND, OnCommand);
		//PROCESS_MSG(WM_USER_TRAY, OnUserTray)        // Tray icon messages
		//PROCESS_MSG(WM_RECONNECT, OnReconnect)       // Resume from hibernation
	PROCESS_MSG(WM_PAINT, OnPaint);
	PROCESS_MSG(WM_SIZE, OnSize);         // Resize controls
	PROCESS_MSG(WM_GETMINMAXINFO, OnGetMinMaxInfo);  // Set Window's minimun size
	PROCESS_MSG(WM_NOTIFY, OnNotify);
	PROCESS_MSG(WM_TIMER, OnTimer);
	PROCESS_MSG(WM_CTLCOLORSTATIC, OnCtlColorStatic);
		//PROCESS_MSG(WM_CLEAR_DB_AND_RESTART, OnClearAndRestart)
		//PROCESS_MSG(WM_RESTART, OnRestart)
		//PROCESS_MSG(WM_CTLCOLORDLG, OnCtlColorDlg)
	//PROCESS_MSG(WM_SETFONT, OnSetFont);
#undef PROCESS_MSG
	return FALSE;
}

// “关于”框的消息处理程序。
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
