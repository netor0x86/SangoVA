#pragma once

/* 字符串工具类 */
class CStringUtil
{
public:
    CStringUtil();
    ~CStringUtil();
    /*
    strBig5 待转换的big5字符
    pszBuf 保存转换后字符的缓冲区指针
    */
    static void Big5ToCurrentCP(const char * strBig5, char* pszBuf);
};

