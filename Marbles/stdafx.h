// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <shellapi.h>
#include <iphlpapi.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <uxtheme.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <math.h>
#include <time.h>

// we use "tstring" that works with TCHARs
#include <string>
#include <xstring>

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

#include <vector>
#include <map>

#include <tcpmib.h>


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#define RC_WIDTH(rc)	(rc.right - rc.left)
#define RC_HEIGHT(rc)	(rc.bottom - rc.top)
#define X_MAIN_DLG		100
#define Y_MAIN_DLG		100