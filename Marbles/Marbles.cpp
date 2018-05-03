// Marbles.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Marbles.h"
#include <math.h>
#include <time.h>
#define MAX_LOADSTRING  100
#define MAX_DYRECT      10
#define FRAMES_PER_SEC  25
#define PAUSE		    TRUE
#define WITH_RATIO		5
#define HEIGHT_RATIO	7

#define ID_MENU			0
#define ID_FRAME_RATE	1
#define ID_GRAVITY		2
#define ID_RESISTANCE	3
#define ID_NUM			4
#define ID_SHAPE		5
#define ID_FONT_TYPE	6
#define ID_OK			7

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
typedef struct DynamicRect
{
	RECT	rc;
	LONG	w, h;
	double	h_deltaMove, v_deltaMove;
	double	h_at, h_v0, h_vt, h_f, v_at, v_v0, v_vt, v_f;
	BOOL	h_sign, v_sign;
	HRGN	hRgn, hRgnNew, hRgnOld;
	DWORD	rgb;
}DYRECT,*PDYRECT;
DYRECT sDyRects[MAX_DYRECT];
HWND /*hwndCanvas, */hwndMenu;
HWND hwndFrameRate, hwndGravity, hwndResistance, hwndNum, hwndShape, hwndFontType, hwndOK;

// 此代码模块中包含的函数的前向声明: 
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

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MARBLES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MARBLES));

    MSG msg;

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



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MARBLES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MARBLES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static LONG cxClient = 0, cyClient = 0, cxChar = 0, cyChar = 0, cxMenuClient = 0, cyMenuClient = 0;
	static RECT rcCanvas, rcMenu;
	static DWORD deltaDuration = 0, lastTimePoint = 0;
	HINSTANCE	hInstance;
    switch (message)
    {
	case WM_CREATE:
		//获取字体宽度和高度
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());

		//获取实例
		hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

		//创建左边画布窗口
// 		hwndCanvas = CreateWindow(TEXT("static"),
// 			NULL, WS_CHILD | WS_VISIBLE | SS_WHITERECT, 0, 0, 0, 0, hWnd, (HMENU)ID_CANVAS, hInstance, NULL);
		
		//创建右边菜单窗口
		hwndMenu = CreateWindow(TEXT("static"),
			NULL, WS_CHILD | WS_VISIBLE | SS_WHITERECT, 0, 0, 0, 0, hWnd, (HMENU)ID_MENU, hInstance, NULL);

		//创建控件
		hwndFrameRate = CreateWindow(TEXT("button"),
			TEXT("帧率"), WS_CHILD | WS_VISIBLE | BS_TEXT, 0, 0, 0, 0, hwndMenu, (HMENU)ID_FRAME_RATE, hInstance, NULL);
		hwndGravity = CreateWindow(TEXT("button"),
			TEXT("重力"), WS_CHILD | WS_VISIBLE | BS_TEXT, 0, 0, 0, 0, hwndMenu, (HMENU)ID_GRAVITY, hInstance, NULL);
		hwndResistance = CreateWindow(TEXT("button"),
			TEXT("阻力"), WS_CHILD | WS_VISIBLE | BS_TEXT, 0, 0, 0, 0, hwndMenu, (HMENU)ID_RESISTANCE, hInstance, NULL);
		hwndNum = CreateWindow(TEXT("button"),
			TEXT("数量"), WS_CHILD | WS_VISIBLE | BS_TEXT, 0, 0, 0, 0, hwndMenu, (HMENU)ID_NUM, hInstance, NULL);
		hwndShape = CreateWindow(TEXT("button"),
			TEXT("形状"), WS_CHILD | WS_VISIBLE | BS_TEXT, 0, 0, 0, 0, hwndMenu, (HMENU)ID_SHAPE, hInstance, NULL);
		hwndFontType = CreateWindow(TEXT("button"),
			TEXT("字体"), WS_CHILD | WS_VISIBLE | BS_TEXT, 0, 0, 0, 0, hwndMenu, (HMENU)ID_FONT_TYPE, hInstance, NULL);
		hwndOK = CreateWindow(TEXT("button"),
			TEXT("确定"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hwndMenu, (HMENU)ID_OK, hInstance, NULL);

		/*
			初始化方块
		*/
		srand((unsigned int)time(nullptr));	// 生成随机数种子
		ZeroMemory(&sDyRects, sizeof(sDyRects));
		for (auto& dyRect : sDyRects)
		{
			dyRect.w      = dyRect.h = 100;
			dyRect.v_at   = 0.0;
			dyRect.v_f    = 1.0;
			dyRect.h_v0   = dyRect.h_vt = 2.0; 
			dyRect.h_sign = dyRect.v_sign = TRUE;
		}

		/*
			设置计时器
		*/

		if (!PAUSE)
			SetTimer(hWnd, 1, (UINT)(1000.0f / FRAMES_PER_SEC), nullptr);
		lastTimePoint    = GetTickCount();
		break;
	case WM_SIZE:
	{
		//获取客户区大小
		cxClient = (LOWORD(lParam));
		cyClient = (HIWORD(lParam));

		//设置画布位置
		GetClientRect(hWnd, &rcCanvas);
		rcCanvas.right = (cxClient / WITH_RATIO) * (WITH_RATIO - 1);

		//菜单窗口的位置为相对于最顶层窗口的位置
		MoveWindow(hwndMenu, rcCanvas.right, 0, cxClient - rcCanvas.right, cyClient, TRUE);
		GetClientRect(hwndMenu, &rcMenu);
		cxMenuClient = rcMenu.right - rcMenu.left;
		cyMenuClient = rcMenu.bottom;

		//设置控件位置,控件的位置为相对于父窗口的位置
		LONG hUnit = 0;
		hUnit = cyMenuClient       / 7;
		MoveWindow(hwndFrameRate,  0, hUnit * 0, cxMenuClient, hUnit, TRUE);
		MoveWindow(hwndGravity,    0, hUnit * 1, cxMenuClient, hUnit, TRUE);
		MoveWindow(hwndResistance, 0, hUnit * 2, cxMenuClient, hUnit, TRUE);
		MoveWindow(hwndNum,        0, hUnit * 3, cxMenuClient, hUnit, TRUE);
		MoveWindow(hwndShape,      0, hUnit * 4, cxMenuClient, hUnit, TRUE);
		MoveWindow(hwndFontType,   0, hUnit * 5, cxMenuClient, hUnit, TRUE);
		MoveWindow(hwndOK,         0, hUnit * 6, cxMenuClient, hUnit, TRUE);

		//随机生成形状
		for (auto& dyRect : sDyRects)
		{
			//  随机生成
			SetRect(&dyRect.rc,
				rand() % (rcCanvas.right - dyRect.w),
				rand() % (rcCanvas.bottom - dyRect.h),
				dyRect.rc.left + dyRect.w,
				dyRect.rc.top - dyRect.h);
			dyRect.rgb = rand() % DWORD(-1);
		}
		break; 
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
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
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
<<<<<<< HEAD
			DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)));
			DeleteObject(SelectObject(hdc, GetStockObject(DC_BRUSH)));
			SetMapMode(hdc, MM_ISOTROPIC);
=======
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			
			SetMapMode(hdc, MM_ANISOTROPIC);
>>>>>>> 5c706bacc4533805091cc4685065ddc232c71ae5
			SetWindowExtEx(hdc, cxClient, cyClient, nullptr);
			SetWindowOrgEx(hdc, 0, 0, nullptr);
			SetViewportExtEx(hdc, cxClient, cyClient, nullptr);
			SetViewportOrgEx(hdc, 0, 0, nullptr);
			//Rectangle(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
			for (auto& dyRect : sDyRects)
			{
				SetDCBrushColor(hdc, dyRect.rgb);
				Ellipse(hdc, dyRect.rc.left, dyRect.rc.top, dyRect.rc.right, dyRect.rc.bottom);
			}
			//ValidateRect(hWnd, &rcTarget);

			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
	case WM_TIMER:
	{
		DWORD currentTimePoint = GetTickCount();
		static double v_g = 7.0f;

		// △t
		deltaDuration = (currentTimePoint - lastTimePoint) / 10;
		lastTimePoint = currentTimePoint;

		// 进行计算
		for (auto& dyRect : sDyRects)
		{
			if (dyRect.v_sign)
			{
				                   // v,g方向相同，v增加
				dyRect.v_at        = v_g                       - dyRect.v_f;
				dyRect.v_vt        = dyRect.v_v0               + dyRect.v_at             * (deltaDuration);
				dyRect.v_deltaMove = (pow(dyRect.v_vt, 2.0f)   - pow(dyRect.v_v0, 2.0f)) / (2.0f * dyRect.v_at);
				dyRect.v_v0        = dyRect.v_vt;
			}
			else
			{
				                   // v,g方向相反，v减少
				dyRect.v_at        = v_g                       + dyRect.v_f;
				dyRect.v_vt        = dyRect.v_v0               - dyRect.v_at             * (deltaDuration);
				dyRect.v_deltaMove = (pow(dyRect.v_vt, 2.0f)   - pow(dyRect.v_v0, 2.0f)) / (2.0f * dyRect.v_at);
				dyRect.v_v0        = dyRect.v_vt;
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
			rcNew.left   = dyRect.rc.left + (LONG)dyRect.h_deltaMove;
			rcNew.right  = rcNew.left     + (LONG)dyRect.w;
			rcNew.top    = dyRect.rc.top  + (LONG)dyRect.v_deltaMove;
			rcNew.bottom = rcNew.top      + (LONG)dyRect.h;

			// 是否到达上下下边界
			if (rcNew.bottom >= rcCanvas.bottom)
			{
				dyRect.v_sign = !dyRect.v_sign;
				rcNew.bottom  = 2            * rcCanvas.bottom - rcNew.bottom;
				rcNew.top     = rcNew.bottom - dyRect.h;
			}
			else if (dyRect.v_v0 < 0.0f)
			{
				dyRect.v_sign = !dyRect.v_sign;
				dyRect.v_v0   = 0.0f;
			}

			// 是否到达左右边界
			if (rcNew.left < 0.0f || rcNew.right >= rcCanvas.right)
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
		break;
	}
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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
