#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "ProcessOpt.h"
#include "City.h"
#include "Warrior.h"

using namespace std;

/* 游戏城市类 */
class CGame
{
public:
    CGame();
    ~CGame();

private:
    /* 保存城市列表 */
    vector<CITY> m_vecCity;

public:
    /* 读取城市列表 */
    void ReadCityList();
    CProcessOpt m_ProcessOpt;
    /* 获取城市列表 */
    vector<CITY>& GetCityList();
    /* 获取城市的坐标 */
    void GetCityPoint(DWORD dwCityAddrRef, POINT* pt1, POINT* pt2);
    /* 判断鼠标单击是否在城市内，并返回城市的内存地址 */
    DWORD CGame::GetCityInfoForPt(const POINT &pt);
    /* 读取城市的信息 */
    void ReadCityInfo(LPCVOID dwCityAddr, CITY *CityInfo, DWORD dwSize);
    /* 获取城市中的武将列表 */
    void CGame::GetWarriorListForCity(const CITY& City, vector<WARRIOR> &vecWarriorList);
private:
    // 主公
    unordered_map<char *, COLORREF> m_boss;
public:
    COLORREF m_clrCityColor[20];
    // 获取主公对应的城市颜色
    COLORREF GetCityColor(char* pBossAddr);
    // 获取主公列表
    unordered_map<char *, COLORREF>& GetBossList();
    // 获取武将信息
    bool GetWarrior(char* pWarriorAddr, WARRIOR& Warrior);
};
