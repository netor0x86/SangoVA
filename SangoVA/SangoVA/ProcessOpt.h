#pragma once

/* 游戏内存读写类 */
class CProcessOpt
{
public:
    CProcessOpt();
    ~CProcessOpt();

private:
    /* 游戏的窗口句柄 */
    HWND m_hSangoWnd;
    /* 游戏进程ID */
    DWORD m_dwSangoProcessId;
    /* 游戏的进程句柄 */
    HANDLE m_hSango;

private:
    /* 初始化 */
    void Init();
    /* 获取游戏的进程ID */
    void GetSangoProcessId();
    /* 获取游戏进程的句柄 */
    void OpenSangoProcess();

public:
    /* 读取游戏进程内存中的数据 */
    BOOL ReadGameProcessMemory(
        LPCVOID lpBaseAddress,
        LPVOID lpBuffer,
        SIZE_T nSize,
        SIZE_T * lpNumberOfBytesRead = NULL);
    /* 写入数据到游戏进程内存中 */
    BOOL WriteGameProcessMemory(
        LPVOID lpBaseAddress,
        LPCVOID lpBuffer,
        SIZE_T nSize,
        SIZE_T * lpNumberOfBytesWritten = NULL);
};

