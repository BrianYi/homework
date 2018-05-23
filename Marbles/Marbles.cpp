// Marbles.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Marbles.h"
#include "DlgAbout.h"
#include <math.h>
#include <time.h>
#include <vector>
using namespace std;
#define MAX_LOADSTRING  100
#define MAX_DYRECT      10

#define WIDTH_RATIO		7
#define HEIGHT_RATIO	7

#define ID_MENU			0
#define ID_FRAME_RATE	1
#define ID_GRAVITY		2
#define ID_RESISTANCE	3
#define ID_NUM			4
#define ID_SHAPE		5
#define ID_FONT_TYPE	6
#define ID_OK			7

// ȫ�ֱ���: 
LONG g_framePerSec = 25, g_num = 10;
FLOAT g_v_g = 7.0f, g_v_f = 1.0f;
HINSTANCE g_hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
vector<LOGFONT*> g_vecLogFont;
LOGFONT* g_curLogFont;
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

RECT rcCanvas, rcOption;
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
	{ Circle, TEXT("Բ��") },
	{ Rect, TEXT("����") }
};

Shape FindShape(TCHAR *szShape)
{
	for (auto& shape : g_shapes)
	{
		if (_tccmp(shape.name, szShape) == 0)
			return shape.shape;
	}
	return Shape::Circle;
}

LOGFONT* FindLogFont(TCHAR *szFaceName)
{
	for (auto& lf : g_vecLogFont)
	{
		if (_tccmp(lf->lfFaceName, szFaceName) == 0)
			return lf;
	}
	return nullptr;
}

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK ProcDlgMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


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
    LoadStringW(hInstance, IDC_MARBLES, szWindowClass, MAX_LOADSTRING);
    //MyRegisterClass(hInstance);
	g_hInst = hInstance;
    // ִ��Ӧ�ó����ʼ��: 
//     if (!InitInstance (hInstance, nCmdShow))
//     {
//         return FALSE;
//     }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MARBLES));

    MSG msg;
	CreateDialogParam(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, ProcDlgMain, 0);
	
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

static void OnExit(HWND hWnd);

static void OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	/*HCURSOR hCursor = LoadCursor(NULL, IDC_IBEAM);
	SetCursor(hCursor);*/
}

static void OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {}
static void OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {}

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

static void InitUI(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	TCHAR szBufferPlay[64];
	LoadString(g_hInst, IDS_BTN_PLAY, szBufferPlay, ARRAYSIZE(szBufferPlay));
	SetDlgItemText(hWnd, IDC_BTN_PAUSE, szBufferPlay);

	//֡��
	TCHAR szBuffer[64];
	_stprintf_s(szBuffer, TEXT("%d"), g_framePerSec);
	SetDlgItemText(hWnd, IDC_EDT_FRAME_RATE, szBuffer);

	//����������
	_stprintf_s(szBuffer, TEXT("%.2f"), g_v_g);
	SetDlgItemText(hWnd, IDC_EDT_GRAVITY, szBuffer);
	_stprintf_s(szBuffer, TEXT("%.2f"), g_v_f);
	SetDlgItemText(hWnd, IDC_EDT_RESISTANCE, szBuffer);

	//����
	_stprintf_s(szBuffer, TEXT("%d"), g_num);
	SetDlgItemText(hWnd, IDC_EDT_NUM, szBuffer);

	//��״
	for (int i = 0; i < ARRAYSIZE(g_shapes); ++i)
	{
		ComboBox_AddString(GetDlgItem(hWnd, IDC_CBBOX_SHAPE), g_shapes[i].name);
	}
	ComboBox_SetCurSel(GetDlgItem(hWnd, IDC_CBBOX_SHAPE), 0);
	
	//����
	HDC hdc = GetDC(hWnd);
	LOGFONT logFont;
	ZeroMemory(&logFont, sizeof(logFont));
	logFont.lfFaceName[0] = TEXT('\0');
	logFont.lfCharSet = DEFAULT_CHARSET;
	//EnumFontFamiliesEx(hdc, &logFont, EnumFontFamExProc, (LPARAM)hWnd, 0);
	//EnumFontFamilies(hdc, NULL, EnumFontFamExProc, (LPARAM)hWnd);
	EnumFontFamilies(hdc, NULL, EnumFontFamExProc, (LPARAM)hWnd);
	//EnumFonts(hdc, NULL, EnumFontFamExProc, (LPARAM)hWnd);
	ReleaseDC(hWnd, hdc);
	ComboBox_SetCurSel(GetDlgItem(hWnd, IDC_CBBOX_FONT_TYPE), 0);
	ComboBox_GetText(GetDlgItem(hWnd, IDC_CBBOX_FONT_TYPE), szBuffer, sizeof(szBuffer));
	g_curLogFont = FindLogFont(szBuffer);
}

static void OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	// ��ʼ��UI
	InitUI(hWnd, wParam, lParam);

	/*
	��ʼ������
	*/
	srand((unsigned int)time(nullptr));	// �������������
	sDyRects = new DYRECT[g_num];
	ZeroMemory(sDyRects, sizeof(sDyRects[0]) * g_num);
	for (int i = 0; i < g_num; ++i)
	{
		DYRECT& dyRect = sDyRects[i];
		dyRect.w = dyRect.h = 100;
		dyRect.v_at = 0.0;
		dyRect.v_f = g_v_f;
		dyRect.h_v0 = dyRect.h_vt = 2.0;
		dyRect.h_sign = dyRect.v_sign = TRUE;
		dyRect.shape = Circle;
	}

	/*
	���ü�ʱ��
	*/
	SetTimer(hWnd, 400, (UINT)(1000.0f / g_framePerSec), nullptr);
	//lastTimePoint = GetTickCount();

	UINT uMainDlgWidth = 900, uMainDlgHeight = 500;
	MoveWindow(hWnd, X_MAIN_DLG, Y_MAIN_DLG, uMainDlgWidth, uMainDlgHeight, FALSE);

	ShowWindow(hWnd, SW_SHOW);
}

static void OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	EndDialog(hWnd, 0);
	OnExit(hWnd);
}

static void OnQueryEndSession(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	SetDlgMsgResult(hWnd, WM_QUERYENDSESSION, TRUE);
}

static void OnEndSession(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	OnExit(hWnd);
}

static void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) 
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
			TCHAR szBuffer[64];
			HWND hwndDlgShape = (HWND)lParam;
			ComboBox_GetText(hwndDlgShape, szBuffer, sizeof(szBuffer));
			Shape shape = FindShape(szBuffer);
			for (int i = 0; i < g_num; ++i)
			{
				DYRECT& dyRect = sDyRects[i];
				dyRect.shape = shape;
			}
			InvalidateRect(hWnd, nullptr, TRUE);
		}
	}
	else if (LOWORD(wParam) == IDC_CBBOX_FONT_TYPE)
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			TCHAR szBuffer[64];
			HWND hwndDlgFontType = (HWND)lParam;
			ComboBox_GetText(hwndDlgFontType, szBuffer, sizeof(szBuffer));
			g_curLogFont = FindLogFont(szBuffer);
			//SendMessage(GetDlgItem(hWnd, DLG_MAIN), WM_SETFONT, (WPARAM)CreateFontIndirect(lf), TRUE);
			//UpdateWindow(hWnd);
			InvalidateRect(hWnd, nullptr, TRUE);
		}
	}
}

static void OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	PAINTSTRUCT ps;
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	BeginPaint(hWnd, &ps);
	DeleteObject(SelectObject(ps.hdc, GetStockObject(WHITE_BRUSH)));
	Rectangle(ps.hdc, rcClient.left - 1, rcClient.top - 1, rcClient.right + 1, rcClient.bottom + 1);
	DeleteObject(SelectObject(ps.hdc, GetStockObject(DC_BRUSH)));
	DeleteObject(SelectObject(ps.hdc, CreateFontIndirect(g_curLogFont)));
	TCHAR szBuffer[64];
	_stprintf_s(szBuffer, TEXT("�������ģ������������"));
	TextOut(ps.hdc, 10, 10, szBuffer, _tcslen(szBuffer));
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
	EndPaint(hWnd, &ps);
}

static void OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	//��ȡ�ͻ�����С
	LONG cxClient = 0, cyClient = 0;
	cxClient = (LOWORD(lParam));
	cyClient = (HIWORD(lParam));

	//���û���λ��
	GetClientRect(hWnd, &rcCanvas);
	rcCanvas.right = (cxClient / WIDTH_RATIO) * (WIDTH_RATIO - 2);

	//�˵����ڵ�λ��Ϊ�������㴰�ڵ�λ��
	//MoveWindow(hwndMenu, rcCanvas.right, 0, cxClient - rcCanvas.right, cyClient, TRUE);
	//GetClientRect(hwndMenu, &rcMenu);
	rcOption.left = rcCanvas.right + 30;
	rcOption.top = 0;
	rcOption.right = cxClient;
	rcOption.bottom = cyClient;
	
	//���ÿؼ�λ��,�ؼ���λ��Ϊ����ڸ����ڵ�λ��
	RECT rcTmp[17];
	GetWindowRect(GetDlgItem(hWnd, IDC_LB_FRAME_RATE), &rcTmp[0]);
	GetWindowRect(GetDlgItem(hWnd, IDC_LB_FRAME_RATE2), &rcTmp[13]);
	GetWindowRect(GetDlgItem(hWnd, IDC_LB_GRAVITY), &rcTmp[1]);
	GetWindowRect(GetDlgItem(hWnd, IDC_LB_GRAVITY2), &rcTmp[14]);
	GetWindowRect(GetDlgItem(hWnd, IDC_LB_RESISTANCE), &rcTmp[2]);
	GetWindowRect(GetDlgItem(hWnd, IDC_LB_NUM), &rcTmp[3]);
	GetWindowRect(GetDlgItem(hWnd, IDC_LB_NUM2), &rcTmp[15]);
	GetWindowRect(GetDlgItem(hWnd, IDC_LB_SHAPE), &rcTmp[4]);
	GetWindowRect(GetDlgItem(hWnd, IDC_LB_FONT_TYPE), &rcTmp[5]);

	GetWindowRect(GetDlgItem(hWnd, IDC_EDT_FRAME_RATE), &rcTmp[6]);
	GetWindowRect(GetDlgItem(hWnd, IDC_EDT_GRAVITY), &rcTmp[7]);
	GetWindowRect(GetDlgItem(hWnd, IDC_EDT_RESISTANCE), &rcTmp[8]);
	GetWindowRect(GetDlgItem(hWnd, IDC_EDT_NUM), &rcTmp[9]);
	GetWindowRect(GetDlgItem(hWnd, IDC_CBBOX_SHAPE), &rcTmp[10]);
	GetWindowRect(GetDlgItem(hWnd, IDC_CBBOX_FONT_TYPE), &rcTmp[11]);
	GetWindowRect(GetDlgItem(hWnd, IDC_BTN_PAUSE), &rcTmp[16]);
	GetWindowRect(GetDlgItem(hWnd, IDC_BTN_OK), &rcTmp[12]);

	MoveWindow(GetDlgItem(hWnd, IDC_LB_FRAME_RATE), rcOption.left, rcTmp[0].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[0]), RC_HEIGHT(rcTmp[0]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_LB_FRAME_RATE2), rcOption.left + (rcTmp[13].left - rcTmp[0].left), rcTmp[13].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[13]), RC_HEIGHT(rcTmp[13]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_LB_GRAVITY), rcOption.left, rcTmp[1].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[1]), RC_HEIGHT(rcTmp[1]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_LB_GRAVITY2), rcOption.left + (rcTmp[14].left - rcTmp[1].left), rcTmp[14].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[14]), RC_HEIGHT(rcTmp[14]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_LB_RESISTANCE), rcOption.left, rcTmp[2].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[2]), RC_HEIGHT(rcTmp[2]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_LB_NUM), rcOption.left, rcTmp[3].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[3]), RC_HEIGHT(rcTmp[3]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_LB_NUM2), rcOption.left + (rcTmp[15].left - rcTmp[3].left), rcTmp[15].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[15]), RC_HEIGHT(rcTmp[15]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_LB_SHAPE), rcOption.left, rcTmp[4].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[4]), RC_HEIGHT(rcTmp[4]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_LB_FONT_TYPE), rcOption.left, rcTmp[5].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[5]), RC_HEIGHT(rcTmp[5]), TRUE);

	MoveWindow(GetDlgItem(hWnd, IDC_EDT_FRAME_RATE), rcOption.left + (rcTmp[6].left - rcTmp[0].left), rcTmp[6].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[6]), RC_HEIGHT(rcTmp[6]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_EDT_GRAVITY), rcOption.left + (rcTmp[7].left - rcTmp[1].left), rcTmp[7].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[7]), RC_HEIGHT(rcTmp[7]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_EDT_RESISTANCE), rcOption.left + (rcTmp[8].left - rcTmp[2].left), rcTmp[8].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[8]), RC_HEIGHT(rcTmp[8]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_EDT_NUM), rcOption.left + (rcTmp[9].left - rcTmp[3].left), rcTmp[9].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[9]), RC_HEIGHT(rcTmp[9]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_CBBOX_SHAPE), rcOption.left + (rcTmp[10].left - rcTmp[4].left), rcTmp[10].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[10]), RC_HEIGHT(rcTmp[10]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_CBBOX_FONT_TYPE), rcOption.left + (rcTmp[11].left - rcTmp[5].left), rcTmp[11].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[11]), RC_HEIGHT(rcTmp[11]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_BTN_PAUSE), rcOption.left, rcTmp[16].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[16]), RC_HEIGHT(rcTmp[16]), TRUE);
	MoveWindow(GetDlgItem(hWnd, IDC_BTN_OK), rcOption.left + (rcTmp[12].left - rcTmp[16].left), rcTmp[12].top - Y_MAIN_DLG, RC_WIDTH(rcTmp[12]), RC_HEIGHT(rcTmp[12]), TRUE);
	

	//���������״
	for (int i = 0; i < g_num; ++i)
	{
		DYRECT& dyRect = sDyRects[i];
		//  �������
		dyRect.rc.left = rand() % (rcCanvas.right - dyRect.w);
		dyRect.rc.top = rand() % (rcCanvas.bottom - dyRect.h);
		dyRect.rc.right = dyRect.rc.left + dyRect.w;
		dyRect.rc.bottom = dyRect.rc.top + dyRect.h;
		dyRect.rgb = rand() % DWORD(-1);
	}
}

static void OnGetMinMaxInfo(HWND hWnd, WPARAM wParam, LPARAM lParam) {}

static void OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	return;
}

static void OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (g_pause)
	{
		g_bFirst = TRUE;
		return;
	}

	DWORD currentTimePoint = GetTickCount();

	if (g_bFirst)
	{
		g_bFirst = FALSE;
		lastTimePoint = currentTimePoint;
	}

	// ��t
	deltaDuration = (currentTimePoint - lastTimePoint) / 10;
	lastTimePoint = currentTimePoint;

	// ���м���
	for (int i = 0; i < g_num; ++i)
	{
		DYRECT& dyRect = sDyRects[i];
		if (dyRect.v_sign)
		{
			// v,g������ͬ��v����
			dyRect.v_at = g_v_g - dyRect.v_f;
			dyRect.v_vt = dyRect.v_v0 + dyRect.v_at             * (deltaDuration);
			dyRect.v_deltaMove = (pow(dyRect.v_vt, 2.0f) - pow(dyRect.v_v0, 2.0f)) / (2.0f * dyRect.v_at);
			dyRect.v_v0 = dyRect.v_vt;
		}
		else
		{
			// v,g�����෴��v����
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

		// ������λ��
		RECT rcNew;
		rcNew.left = dyRect.rc.left + (LONG)dyRect.h_deltaMove;
		rcNew.right = rcNew.left + (LONG)dyRect.w;
		rcNew.top = dyRect.rc.top + (LONG)dyRect.v_deltaMove;
		rcNew.bottom = rcNew.top + (LONG)dyRect.h;

		// �Ƿ񵽴������±߽�
		if (rcNew.bottom >= rcCanvas.bottom)
		{
			dyRect.v_sign = !dyRect.v_sign;
			rcNew.bottom = 2 * rcCanvas.bottom - rcNew.bottom;
			rcNew.top = rcNew.bottom - dyRect.h;
		}
		else if (dyRect.v_v0 < 0.0f)
		{
			dyRect.v_sign = !dyRect.v_sign;
			dyRect.v_v0 = 0.0f;
		}

		// �Ƿ񵽴����ұ߽�
		if (rcNew.left < 0.0f || rcNew.right >= rcCanvas.right)
		{
			dyRect.h_sign = !dyRect.h_sign;
		}

		// �ϲ�����
		dyRect.hRgnOld = CreateRectRgn(dyRect.rc.left, dyRect.rc.top, dyRect.rc.right, dyRect.rc.bottom);
		dyRect.hRgnNew = CreateRectRgn(rcNew.left, rcNew.top, rcNew.right, rcNew.bottom);
		CombineRgn(dyRect.hRgn, dyRect.hRgnOld, dyRect.hRgnNew, RGN_OR);

		dyRect.rc = rcNew;

		// ����֮ǰ��ͼ��
		InvalidateRgn(hWnd, dyRect.hRgnOld, TRUE);
		InvalidateRgn(hWnd, dyRect.hRgnNew, TRUE);
	}
}

static void Exit(HWND hWnd, bool restart)
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
}

static void OnExit(HWND hWnd)
{
	Exit(hWnd, false);
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
#define PROCESS_MSG(MSG, HANDLER) if(uMsg == MSG) { HANDLER(hWnd, wParam, lParam); return TRUE; }

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
		//PROCESS_MSG(WM_CLEAR_DB_AND_RESTART, OnClearAndRestart)
		//PROCESS_MSG(WM_RESTART, OnRestart)
		//PROCESS_MSG(WM_CTLCOLORDLG, OnCtlColorDlg)
	//PROCESS_MSG(WM_SETFONT, OnSetFont);
#undef PROCESS_MSG
	return FALSE;
}

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
