#include "stdafx.h"
#include "StringUtil.h"


CStringUtil::CStringUtil()
{
}


CStringUtil::~CStringUtil()
{
}


void CStringUtil::Big5ToCurrentCP(const char * strBig5, char* pszBuf)
{
    if (strBig5 == nullptr)
    {
        return;
    }

    // 1. Big5 -> UTF-16 (宽字符)
    int wlen = MultiByteToWideChar(950, 0, strBig5, -1, NULL, 0);

    if (wlen == 0) {
        return;
    }

    wchar_t* wbuf = (wchar_t*)malloc(wlen * sizeof(wchar_t));

    if (wbuf == NULL) {
        return;
    }

    if (MultiByteToWideChar(950, 0, strBig5, -1, wbuf, wlen) == 0) {
        free(wbuf);
        return;
    }

    // 2. 获取当前控制台的输出代码页（例如简体中文系统下通常是 936 GBK）
    UINT cp = GetConsoleOutputCP();

    // 3. UTF-16 -> 当前代码页的窄字符串
    int mblen = WideCharToMultiByte(cp, 0, wbuf, -1, NULL, 0, NULL, NULL);
    if (mblen == 0) {
        free(wbuf);
        return;
    }

    if (WideCharToMultiByte(cp, 0, wbuf, -1, pszBuf, mblen, NULL, NULL) == 0)
    {
        free(wbuf);

        return;
    }

    free(wbuf);

    return ;
}
