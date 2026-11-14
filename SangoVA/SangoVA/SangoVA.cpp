// SangoVA.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SangoVA.h"

#include <windowsx.h>

#include <map>
#include "Game.h"
#include "StringUtil.h"

#include "WarriorType.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 游戏类
CGame g_Game;

// 弹出窗口句柄
HWND g_hPopup = NULL;

// 全局变量，在地图上单击以后获得城市的地址，在弹出窗口中进行读取城市数据
DWORD g_dwCityAddr = 0;

LRESULT CALLBACK PopupWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 设置红色文字
        SetTextColor(hdc, RGB(255, 0, 0));
        // 背景已在WM_ERASEBKGND消息中绘制设为灰色，这里只画文字
        RECT rcClient;

        GetClientRect(hWnd, &rcClient);

        // 读取城市信息
        CITY CityInfo = { 0 };
        g_Game.ReadCityInfo((LPCVOID)g_dwCityAddr, &CityInfo, sizeof(CITY));

        char szBuf[MAXBYTE * 10] = { 0 };
        // 格式化武将信息
        wsprintf(szBuf, "武将数：%d/%d\r\n预备兵：%d/%d\r\n", CityInfo.wHeroNum, CityInfo.wMaxHeroNum, CityInfo.wReservist, CityInfo.wMaxReservist);

        vector<WARRIOR> vecWarriorList;
        g_Game.GetWarriorListForCity(CityInfo, vecWarriorList);

        wsprintf(szBuf, "%s名字|等级|武力|智力|带兵数|兵种", szBuf);
        for (int i = 0; i < CityInfo.wHeroNum; i ++)
        {
            char szWarriorName[10] = { 0 };
            CStringUtil::Big5ToCurrentCP((const char *)vecWarriorList[i].szWarriorName, szWarriorName);

           string strSoldier = CSoldierType::GetwSoldierTypeName(vecWarriorList[i].wType);

            wsprintf(szBuf, "%s\r\n%s|%d|%d|%d|%d|%s", 
                                szBuf, szWarriorName, 
                                vecWarriorList[i].wLevel, 
                                vecWarriorList[i].wCombat, 
                                vecWarriorList[i].wIntelligence,
                                vecWarriorList[i].wSoldierNum,
                                strSoldier.c_str());
        }

        DrawText(hdc, szBuf, -1, &rcClient, DT_WORDBREAK);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_ERASEBKGND:
    {
        // 绘制灰色背景
        HDC hdc = (HDC)wParam;
        RECT rcClient;
        GetClientRect(hWnd, &rcClient);
        HBRUSH hBrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        FillRect(hdc, &rcClient, hBrush);
        DeleteObject(hBrush);
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

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
    LoadStringW(hInstance, IDC_SANGOVA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 注册弹出窗口类
    WNDCLASS popWc = { 0 };
    popWc.lpfnWndProc = PopupWndProc;
    popWc.hInstance = hInstance;
    popWc.hbrBackground = NULL; // 背景在WM_ERASEBKGND中绘制
    popWc.lpszClassName = "PopupClass";
    RegisterClass(&popWc);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SANGOVA));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SANGOVA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SANGOVA);
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

struct MSGMAP_ENTRY
{
    UINT nMessage;
    LONG (*PFN)(HWND, UINT, WPARAM, LPARAM);
};
#define dim(x) (sizeof(x) / sizeof(x[0]))

LRESULT OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    g_Game.ReadCityList();
    return 0;
}

void DrawBossColorDesc(HDC hDc)
{
    int x1 = 15;
    int y1 = 15;
    int x2 = 25;
    int y2 = 25;

    for (auto iter = g_Game.GetBossList().begin(); iter != g_Game.GetBossList().end(); ++ iter) 
    {
        WARRIOR Warrior = { 0 };
        if (g_Game.GetWarrior(iter->first, Warrior))
        {
            HBRUSH hBrush = CreateSolidBrush(g_Game.GetCityColor(iter->first));
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hDc, hBrush);

            // 画城市
            Rectangle(hDc, x1, y1, x2, y2);

            SelectObject(hDc, hOldBrush);
            DeleteObject(hBrush);

            char szWarriorName[10] = { 0 };
            CStringUtil::Big5ToCurrentCP((const char *)Warrior.szWarriorName, szWarriorName);
            TextOut(hDc, x1 + 15, y1 - 3, szWarriorName, strlen(szWarriorName));

            x1 += 80;
            x2 += 80;
        }
    }
}

/* 绘制地图 */
void DrawMap(HDC hDc)
{
    vector<CITY>::iterator iter;

    for (iter = g_Game.GetCityList().begin(); iter != g_Game.GetCityList().end(); iter++)
    {
        HBRUSH hBrush = CreateSolidBrush(g_Game.GetCityColor(iter->pszBoss));
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hDc, hBrush);

        // 画城市
        Rectangle(hDc, iter->x1, iter->y1, iter->x2, iter->y2);

        SelectObject(hDc, hOldBrush);
        DeleteObject(hBrush);

        // 显示城市名称
        char szBuf[MAXBYTE] = { 0 };
        CStringUtil::Big5ToCurrentCP((const char *)iter->szCityName, szBuf);
        TextOut(hDc, iter->x1 + 5, iter->y1 + 5, szBuf, lstrlen(szBuf));

        // 画和其他城市的连线
        for (int i = 0; i < 6; i++)
        {
            POINT pt1 = { 0, 0 };
            POINT pt2 = { 0, 0 };

            // 获取城市的坐标
            g_Game.GetCityPoint(iter->dwOtherCity[i], &pt1, &pt2);

            if (pt1.x == 0 || pt1.y == 0 || pt2.x == 0 || pt2.y == 0)
            {
                continue;
            }

            // 连线
            DWORD x1 = (iter->x1 + iter->x2) / 2;
            DWORD y1 = (iter->y1 + iter->y2) / 2;
            DWORD x2 = (pt1.x + pt2.x) / 2;
            DWORD y2 = (pt1.y + pt2.y) / 2;

            MoveToEx(hDc, x1, y1, NULL);
            LineTo(hDc, x2, y2);
        }
    }
}

LRESULT OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    DrawMap(hdc);
    DrawBossColorDesc(hdc);

    EndPaint(hWnd, &ps);

    return 0;
}

LRESULT OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

    return 0;
}

LRESULT OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);

    return 0;
}

LRESULT OnLButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 获取鼠标点击的未知
    POINT pt;
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);

    // 获取城市的地址
    g_dwCityAddr = g_Game.GetCityInfoForPt(pt);

    // 检查是否点击在城市区域内
    if (g_dwCityAddr != 0)
    {
        // 如果已经打开，则关闭
        if (g_hPopup != NULL)
        {
            DestroyWindow(g_hPopup);
            g_hPopup = NULL;
        }
        // 创建弹出窗口
        if (g_hPopup == NULL)
        {
            // 创建无边框弹出窗口，位于点击位置偏移处
            POINT ptTmp;
            ptTmp = pt;
            ClientToScreen(hWnd, &ptTmp);
            int popX = ptTmp.x;
            int popY = ptTmp.y;
            int popW = 260;
            int popH = 400;
            g_hPopup = CreateWindow(
                "PopupClass", NULL,
                WS_POPUP,   // 无边框弹出
                popX, popY, popW, popH,
                hWnd, NULL,
                GetModuleHandle(NULL), NULL
            );
            if (g_hPopup)
            {
                ShowWindow(g_hPopup, SW_SHOW);
                UpdateWindow(g_hPopup);
            }
        }
    } 
    else
    {
        // 点击其他位置，销毁弹出窗口
        if (g_hPopup)
        {
            DestroyWindow(g_hPopup);
            g_hPopup = NULL;
        }
    }

    InvalidateRect(hWnd, NULL, TRUE);

    return 0;
}

struct MSGMAP_ENTRY _messageEntries[] = 
{
     WM_CREATE, OnCreate,
     WM_PAINT, OnPaint,
     WM_COMMAND, OnCommand,
     WM_DESTROY, OnDestroy,
     WM_LBUTTONDOWN, OnLButtonDown
};

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
    for (int i = 0; i < dim(_messageEntries); i ++)
    {
        if (message == _messageEntries[i].nMessage)
        {
            return ((*_messageEntries[i].PFN)(hWnd, message, wParam, lParam));
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
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
