// Marbles.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Marbles.h"
#include <math.h>
#include <time.h>
#define MAX_LOADSTRING 100
#define MAX_DYRECT 10
// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
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
    LoadStringW(hInstance, IDC_MARBLES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MARBLES));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MARBLES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MARBLES);
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
	//static RECT rcTarget[4] = { {0,0,100,100}, {100,0,200,100}, {300,0,400,100 }, {400,0,500,100} };
	static LONG cxClient = 0, cyClient = 0;
	//static double h_deltaMove = 0.0f, v_deltaMove = 0.0f;
	//static double v_g = 9.8f, v_v0  = 0.0f, v_vt = 0.0f, v_f = 1.5f, v_at = 0.0f;
	//static double h_vt = 5.0f;
	static DWORD deltaDuration = 0, lastTimePoint = 0;
	//static HRGN hRgn1, hRgn2, hRgnClip;
	//static BOOL h_fSign = TRUE, v_fSign = TRUE;
	//RECT rcNewTarget;
    switch (message)
    {
	case WM_CREATE:
		/*
			��ʼ������
		*/
		srand(time(nullptr));	// �������������
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
			���ü�ʱ��
		*/
		SetTimer(hWnd, 1, 10, nullptr);
		lastTimePoint = GetTickCount();
		break;
	case WM_SIZE:
		cxClient      = LOWORD(lParam);
		cyClient      = HIWORD(lParam);

		for (auto& dyRect : sDyRects)
		{
			                   //  �������
			dyRect.rc.left     = rand() % (cxClient    - dyRect.w);
			dyRect.rc.top      = rand() % (cyClient    - dyRect.h);
			dyRect.rc.right    = dyRect.rc.left        + dyRect.w;
			dyRect.rc.bottom   = dyRect.rc.top         - dyRect.h;
			dyRect.rgb         = rand() % DWORD(-1);
		}
		break;
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
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			
			SetMapMode(hdc, MM_ISOTROPIC);
			SetWindowExtEx(hdc, cxClient, cyClient, nullptr);
			SetWindowOrgEx(hdc, 0, 0, nullptr);
			SetViewportExtEx(hdc, cxClient, cyClient, nullptr);
			SetViewportOrgEx(hdc, 0, 0, nullptr);
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
		static double v_g = 4.0f;

		// ��t
		deltaDuration = (currentTimePoint - lastTimePoint) / 10;
		lastTimePoint = currentTimePoint;

		// ���м���
		for (auto& dyRect : sDyRects)
		{
			if (dyRect.v_sign)
			{
				                   // v,g������ͬ��v����
				dyRect.v_at        = v_g                       - dyRect.v_f;
				dyRect.v_vt        = dyRect.v_v0               + dyRect.v_at             * (deltaDuration);
				dyRect.v_deltaMove = (pow(dyRect.v_vt, 2.0f)   - pow(dyRect.v_v0, 2.0f)) / (2.0f * dyRect.v_at);
				dyRect.v_v0        = dyRect.v_vt;
			}
			else
			{
				                   // v,g�����෴��v����
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

			             // ������λ��
			RECT rcNew;
			rcNew.left   = dyRect.rc.left + dyRect.h_deltaMove;
			rcNew.right  = rcNew.left     + dyRect.w;
			rcNew.top    = dyRect.rc.top  + dyRect.v_deltaMove;
			rcNew.bottom = rcNew.top      + dyRect.h;

			// �Ƿ񵽴������±߽�
			if (rcNew.bottom >= cyClient)
			{
				dyRect.v_sign = !dyRect.v_sign;
				rcNew.bottom  = 2            * cyClient - rcNew.bottom;
				rcNew.top     = rcNew.bottom - dyRect.h;
			}
			else if (dyRect.v_v0 < 0.0f)
			{
				dyRect.v_sign = !dyRect.v_sign;
				dyRect.v_v0   = 0.0f;
			}

			// �Ƿ񵽴����ұ߽�
			if (rcNew.left < 0.0f || rcNew.right >= cxClient)
			{
				dyRect.h_sign = !dyRect.h_sign;
			}

			// �ϲ�����
			dyRect.hRgnOld = CreateRectRgn(dyRect.rc.left, dyRect.rc.top, dyRect.rc.right, dyRect.rc.bottom);
			dyRect.hRgnNew = CreateRectRgn(rcNew.left, rcNew.top, rcNew.right, rcNew.bottom);
			//CombineRgn(dyRect.hRgn, dyRect.hRgnOld, dyRect.hRgnNew, RGN_OR);

			dyRect.rc = rcNew;

			// ����֮ǰ��ͼ��
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
