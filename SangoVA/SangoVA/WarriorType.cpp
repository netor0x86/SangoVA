#include "stdafx.h"
#include "WarriorType.h"


/* 場宎趙條笱 */
std::map<int, std::string> CSoldierType::m_mapSoldierType = {
    { 1,    "は絮條" },
    { 2,    "酗Л條" },
    { 4,    "湮絮條" },
    { 8,    "僮璋條" },
    { 16,   "蟈晴條" },
    { 32,   "滄絮條" },
    { 64,   "挕須條" },
    { 128,  "雙逜條" },
    { 256,  "沺晴條" },
    { 512,  "枘樅條" },
    { 1024, "酴踫條" },
    { 2048, "殢條" },
    { 4096, "躓條" },
};

CSoldierType::CSoldierType()
{
}


CSoldierType::~CSoldierType()
{
}


string CSoldierType::GetwSoldierTypeName(WORD wSoldier)
{
    map<int, string>::iterator iter = m_mapSoldierType.find(wSoldier);

    return iter->second.c_str();
}
