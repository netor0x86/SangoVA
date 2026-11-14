#pragma once

#pragma pack(push, 1)
/* 城市结构体 */
// 城市结构体是各双链表
typedef struct _CITY
{
    // 当前城市地址, 并非内存中的数据
    // 读内存时从该结构偏移4的位置开始
    DWORD dwCurCityAddr;                 

    // 游戏中城市结构体的数据
    /* 00 */ unsigned char unknown1[4];
    // 城市名字
    /* 04 */ unsigned char szCityName[5];
    /* 09 */unsigned char unknown2[7];
    // 0x10 => 16 君主
    /* 16 */char *pszBoss;
    // 太守
    /* 20 */char *pszPrefect;
    // 军师名字
    /* 24 */char *pszStrategist;
    /* 28 */unsigned char unknown3[8];

    // 36 => 0x24
    /* 36 */char *p1;
    // 40 = > 0x28
    /* 40 */char *p2;

    /* 44 */unsigned char unknown33[4];
    // 0x30 => 48 城市武将数量
    /* 48 */WORD wHeroNum;
    /* 50 */unsigned char unknown4[18];
    // 0x44 => 68 城市武将数量上限
    /* 68 */WORD wMaxHeroNum;
    // 连通的六个城市
    // 0x46 => 70
    /* 70 */DWORD dwOtherCity[6];
    /* 94 */unsigned char unknown13[12];
    // 0x6a => 106 内政自治 搜索1 建设2 平均3 方式不限4 取消自治0
    /* 106 */BYTE bStatus;
    /* 107 */unsigned char unknown12[7];      // 107
    // 坐标
    // 114 => 0x72
    /* 114 */WORD x1;
    // 116 => 0x74
    /* 116 */WORD y1;
    // 118 => 0x76
    /* 118 */WORD x2;
    // 120 => 0x78
    /* 120 */WORD y2;
   
    /* 122 */unsigned char unknown8[8];
    // // 0x82 => 130 预备兵上限
    /* 130 */WORD wMaxReservist;
    /* 132 */unsigned char unknown5[2];
    // 0x86 => 134 预备兵
    /* 134 */WORD wReservist;
    /* 136 */unsigned char unknown6[6];
    // 0x8e => 142 防御
    /* 142 */WORD wDenfense;
    /* 144 */unsigned char unknown11[2];
    // 0x92 => 146 金钱
    /* 146 */WORD wMoney;
    /* 148 */unsigned char unknown7[14];
    // 162 上一个城市的地址
    /* 162 */DWORD dwPreCityAddr;
    // 0xA6 => 166 下一个城市的地址
    /* 166 */DWORD dwNextCityAddr;
} CITY, *PCITY;
#pragma pack(pop)