#include "stdafx.h"
#include "Game.h"
#include "Global.h"
#include "StringUtil.h"

CGame::CGame() : m_clrCityColor{
    RGB(31, 119, 180), RGB(255, 127,  14), RGB(44, 160,  44), RGB(214,  39,  40),
    RGB(148, 103, 189), RGB(140,  86,  75), RGB(227, 119, 194), RGB(127, 127, 127),
    RGB(188, 189,  34), RGB(23, 190, 207), RGB(174, 199, 232), RGB(255, 187, 120),
    RGB(152, 223, 138), RGB(255, 152, 150), RGB(197, 176, 213), RGB(196, 156, 148),
    RGB(247, 182, 210), RGB(199, 199, 199), RGB(219, 219, 141), RGB(158, 218, 229)
}
{
}


CGame::~CGame()
{
}


/* 读取城市列表 */
void CGame::ReadCityList()
{
    /* 城市的地址 */
    DWORD dwCityAddr = 0;

    m_ProcessOpt.ReadGameProcessMemory(
        (LPCVOID)CITY_BASE_ADDR,
        &dwCityAddr,
        sizeof(DWORD));

    int nCityColor = 0;
    while (dwCityAddr)
    {
        CITY city = { 0 };

        city.dwCurCityAddr = dwCityAddr;
        // 读取城市的信息
        m_ProcessOpt.ReadGameProcessMemory(
            (LPCVOID)dwCityAddr,
            &city.unknown1,
            sizeof(city) - sizeof(DWORD));

        // 保存城市信息
        m_vecCity.push_back(city);

        if (m_boss.find(city.pszBoss) == m_boss.end())
        {
            m_boss[city.pszBoss] = m_clrCityColor[nCityColor];
            nCityColor++;
        }

        // 一个城市的地址
        dwCityAddr = city.dwNextCityAddr;
    }
}

/* 获取城市列表 */
vector<CITY>& CGame::GetCityList()
{
    return m_vecCity;
}

/**
函数名：GetCityPoint
函数说明：获取城市的坐标
参数：
dwCtyAddrRef：指向城市的地址
pt1：OUT 城市左上角的坐标
pt2：OUT 城市右下角的坐标
*/
void CGame::GetCityPoint(DWORD dwCityAddrRef, POINT* pt1, POINT* pt2)
{
    DWORD dwCityAddr = 0;
    // 读取城市地址
    m_ProcessOpt.ReadGameProcessMemory(
        (LPCVOID)dwCityAddrRef,
        &dwCityAddr,
        sizeof(DWORD));

    // 根据城市地址找到城市对应的信息
    auto it = std::find_if(m_vecCity.begin(), m_vecCity.end(),
        [dwCityAddr](const CITY& c) { return c.dwCurCityAddr == dwCityAddr;  });

    // 返回城市坐标
    if (it != m_vecCity.end())
    {
        pt1->x = it->x1;
        pt1->y = it->y1;
        pt2->x = it->x2;
        pt2->y = it->y2;
    }
}


/* 判断鼠标单击是否在城市内，并返回城市的内存地址 */
// 此处需要返回地址，因为有些数据不是固定的，需要再次读取
// 名字、坐标、连接的城市是固定的
DWORD CGame::GetCityInfoForPt(const POINT &pt)
{
    // 城市地址
    DWORD dwCityAddr = 0;

    vector<CITY>::iterator iter;

    for (iter = m_vecCity.begin(); iter != m_vecCity.end(); iter++)
    {
        // 鼠标点击在城市的范围内
        if (pt.x >= iter->x1 && pt.y >= iter->y1
            && pt.x <= iter->x2 && pt.y <= iter->y2)
        {
            dwCityAddr = iter->dwCurCityAddr;
            break;
        }
    }

    return dwCityAddr;
}

/* 读取城市的信息 */
void CGame::ReadCityInfo(LPCVOID dwCityAddr, CITY *CityInfo, DWORD dwSize)
{
    m_ProcessOpt.ReadGameProcessMemory(dwCityAddr, CityInfo->unknown1, dwSize - 4);
    CityInfo->dwCurCityAddr = (DWORD)dwCityAddr;
}



/* 获取城市中的武将列表 */
void CGame::GetWarriorListForCity(const CITY& City, vector<WARRIOR> &vecWarriorList)
{
    DWORD per = (DWORD)City.p1;

    for (int i = 0; i < City.wHeroNum; i++)
    {
        WARRIOR Warrior = { 0 };
        m_ProcessOpt.ReadGameProcessMemory((LPCVOID)per, &Warrior, sizeof(WARRIOR));
        vecWarriorList.push_back(Warrior);

        per = Warrior.dwNextWarrior;
    }
}


// 获取主公对应的城市颜色
COLORREF CGame::GetCityColor(char* pBossAddr)
{
    return m_boss[pBossAddr];
}

// 获取主公列表
unordered_map<char *, COLORREF>& CGame::GetBossList()
{
    // TODO: 在此处插入 return 语句
    return m_boss;
}

// 获取武将信息
bool CGame::GetWarrior(char* pWarriorAddr, WARRIOR& Warrior)
{
    return m_ProcessOpt.ReadGameProcessMemory((LPCVOID)pWarriorAddr, &Warrior, sizeof(WARRIOR));;
}
