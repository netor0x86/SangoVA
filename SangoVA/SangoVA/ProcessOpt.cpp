#include "stdafx.h"
#include "ProcessOpt.h"
#include <windows.h>
#include "Global.h"

CProcessOpt::CProcessOpt()
{
    m_dwSangoProcessId = -1;
    m_hSangoWnd = NULL;
    m_hSango = INVALID_HANDLE_VALUE;

    Init();
}

CProcessOpt::~CProcessOpt()
{
    // 关闭游戏的进程句柄
    if (m_hSango != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hSango);
        m_hSango = INVALID_HANDLE_VALUE;
    }
}

void CProcessOpt::Init()
{
    GetSangoProcessId();
    OpenSangoProcess();
}

void CProcessOpt::GetSangoProcessId()
{
    // 获取游戏窗口句柄
    m_hSangoWnd = FindWindow(NULL, GAME_WINDOW_NAME);
    // 获取游戏的进程ID
    GetWindowThreadProcessId(m_hSangoWnd, &m_dwSangoProcessId);
}

void CProcessOpt::OpenSangoProcess()
{
    m_hSango = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwSangoProcessId);
}

BOOL CProcessOpt::ReadGameProcessMemory(
    LPCVOID lpBaseAddress,
    LPVOID lpBuffer,
    SIZE_T nSize,
    SIZE_T * lpNumberOfBytesRead)
{
    return ReadProcessMemory(m_hSango, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

BOOL CProcessOpt::WriteGameProcessMemory(
    LPVOID lpBaseAddress,
    LPCVOID lpBuffer,
    SIZE_T nSize,
    SIZE_T * lpNumberOfBytesWritten)
{
    return WriteProcessMemory(m_hSango, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
}
