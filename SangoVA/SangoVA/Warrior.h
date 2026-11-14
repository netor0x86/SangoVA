#pragma once

#pragma pack(push, 1)
/* 武将结构体 */
typedef struct _WARRIOR
{
    /* 武将名字 */
    /* +00 */ unsigned char szWarriorName[7];
    /* +07 */ unsigned char unknown1[39];
    /* 等级 */
    /* +46 */ WORD wLevel;
    /* +48 */ unsigned char unknown2[4];
    /* 武力 */
    /* +52 */ WORD wCombat;
    /* 智力 */
    /* +54 */ WORD wIntelligence;
    /* +56 */ unsigned char unknown3[10];
    /* 带兵上限制 */
    /* +66 */ WORD wSoldierNumMax;
    /* 带兵数 */
    /* +68 */ WORD wSoldierNum;
    /* +70 */ unsigned char unknown4[69];
    /* 类型 */
    /* +139 */ WORD wType;
    /* +141 */ unsigned char unknown5[74];
    /* 所属城市的地址 */
    /* +215 */ DWORD dwCity;
    /* +219 */ unsigned char unknown6[8];
    /* 下一个武将的地址 */
    /* +227 */ DWORD dwNextWarrior;
} WARRIOR, *PWARRIOR;
#pragma pack(pop)