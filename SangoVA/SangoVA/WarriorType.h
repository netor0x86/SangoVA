#pragma once

#include <map>
#include <string>

using namespace std;

/* ±øÖÖ */
class CSoldierType
{
public:
    CSoldierType();
    ~CSoldierType();
    static map<int, string> m_mapSoldierType;
    static string GetwSoldierTypeName(WORD wSoldierType);
};
