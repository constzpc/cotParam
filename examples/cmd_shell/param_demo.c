#include "param_demo.h"
#include <string.h>
#include <stdio.h>
#include<stdlib.h>

/******************************************  模拟储存设备操作  ******************************************************/
// 储存空间定义
uint8_t sg_buf[500];
uint32_t sg_length = 0;

#define HEX_PRINTF(str, hex, len)  \
    printf("%s: [%d] -> ", str, len);\
    for (int i = 0; i < (len); i++){printf("%02x ", hex[i]);}\
    printf("\n");

// 从储存空间读取数据
int OnLoadCallback(uint8_t *pBuf, uint16_t *len, bool *pisFinish)
{
    uint16_t needReadLen = *len;
    static uint32_t s_offset = 0;

    if (sg_length == s_offset)
    {
        *len = 0;
        s_offset = 0;
        *pisFinish = true;
        return 0;
    }
    else
    {
        *pisFinish = false;
    }

    if (sg_length - s_offset < needReadLen)
    {
        needReadLen = sg_length - s_offset;
    }

    memcpy(pBuf, &sg_buf[s_offset], needReadLen);
    HEX_PRINTF("\tread", pBuf, needReadLen);
    *len = needReadLen;
    s_offset += needReadLen;

    return 0;
}

// 写数据至储存空间
int OnSaveCallback(const uint8_t *pBuf, uint16_t len, bool isFinish)
{
    static uint32_t s_offset = 0;

    if (isFinish)
    {
        printf("********\n");
        HEX_PRINTF("\tsave", sg_buf, s_offset + len);
        s_offset = 0;
        return 0;
    }

    memcpy(&sg_buf[s_offset], pBuf, len);
    HEX_PRINTF("\tsave", pBuf, len);
    s_offset += len;
    sg_length = s_offset;

    return 0;
}

/******************************************  模拟储存设备操作  ******************************************************/

typedef struct
{
    uint16_t test1;
    float test2;
    char str[12];
}ParamDemo_t;

static ParamDemo_t sg_tTest = {
    .str = "sdf",
    .test1 = 20,
    .test2 = 567.4
};

COT_PARAM_INT16_T g_test = 50;
COT_PARAM_UINT16_T g_test_2 = 20;
COT_PARAM_DOUBLE_T g_test_3 = 3.15;
COT_PARAM_INT8_T g_test_4 = 8;
COT_PARAM_UINT32_T g_test_5 = 620;
COT_PARAM_UINT8_T g_test_6 = 45;
COT_PARAM_INT64_T g_test_7 = 5;
COT_PARAM_INT8_T g_sd = 2;
COT_PARAM_INT64_T g_test_88 = 80;
char g_test_str[15] = "abcdef";
char g_str_des[13] = "EERR";
char g_str_des_2[15] = "sdRR";
char g_str_des_3[15] = "ewRR";

// int a[10];

cotParamInfo_t sg_ParamTable[] = {
    COT_PARAM_ITEM_BIND(1, g_test, COT_PARAM_INT16, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND_WITH_NAME(2, "ds", g_test_2, COT_PARAM_UINT16, COT_PARAM_ATTR_WR, 20),
    // COT_PARAM_ITEM_BIND(2, g_test_2, COT_PARAM_UINT16, COT_PARAM_ATTR_WR, 20),
    COT_PARAM_ITEM_BIND(3, g_test_3, COT_PARAM_DOUBLE, COT_PARAM_ATTR_WR, 3.15, -2.15, 5.12),
    COT_PARAM_ITEM_BIND(4, g_test_str, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "abcdef", 0, sizeof(g_test_str)),
    COT_PARAM_ITEM_BIND(5, g_test_4, COT_PARAM_INT8, COT_PARAM_ATTR_WR, 8, -10, 10),
    COT_PARAM_ITEM_BIND(6, g_test_5, COT_PARAM_UINT32, COT_PARAM_ATTR_WR, 620, 500, 10000),
    COT_PARAM_ITEM_BIND(7, g_test_6, COT_PARAM_UINT8, COT_PARAM_ATTR_WR, 45, 5, 100),
    COT_PARAM_ITEM_BIND(8, g_test_7, COT_PARAM_INT64, COT_PARAM_ATTR_WR, 5, -542, 5450),
    COT_PARAM_ITEM_BIND(9, sg_tTest.test1, COT_PARAM_UINT16, COT_PARAM_ATTR_WR, 20, 10, 2000),
    COT_PARAM_ITEM_BIND(10, sg_tTest.test2, COT_PARAM_FLOAT, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND(11, sg_tTest.str, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "const-zpc", 6, sizeof(sg_tTest.str)),
    COT_PARAM_ITEM_BIND(12, g_test_88, COT_PARAM_INT64, COT_PARAM_ATTR_WR, 5, -542, 5450),
    COT_PARAM_ITEM_BIND(13, g_str_des, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "WER45", 10, sizeof(g_str_des)),
    COT_PARAM_ITEM_BIND(14, g_str_des_2, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "WTG"),
    COT_PARAM_ITEM_BIND(15, g_str_des_3, COT_PARAM_STRING, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND(17, g_sd, COT_PARAM_INT8, COT_PARAM_ATTR_WR, 5),
};

COT_PARAM_UINT64_T g_test_55 = 62055;

cotParamInfo_t sg_ParamTable1[] = {
    COT_PARAM_ITEM_BIND(1, g_test, COT_PARAM_INT16, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND(2, g_test_2, COT_PARAM_UINT16, COT_PARAM_ATTR_WR, 20),
    // COT_PARAM_ITEM_BIND(3, g_test_3, COT_PARAM_DOUBLE, COT_PARAM_ATTR_WR, 3.15, -2.15, 5.12),
    // COT_PARAM_ITEM_BIND(4, g_test_str, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "abcdef", 0, sizeof(g_test_str)),
    // COT_PARAM_ITEM_BIND(5, g_test_4, COT_PARAM_INT8, COT_PARAM_ATTR_WR, 8, -10, 10),
    COT_PARAM_ITEM_BIND(6, g_test_55, COT_PARAM_UINT64, COT_PARAM_ATTR_WR, 62055, 500, 10000),
    COT_PARAM_ITEM_BIND(7, g_test_6, COT_PARAM_UINT8, COT_PARAM_ATTR_WR, 45, 5, 100),
    COT_PARAM_ITEM_BIND(8, g_test_7, COT_PARAM_INT64, COT_PARAM_ATTR_WR, 5, -542, 5450),
    COT_PARAM_ITEM_BIND(9, sg_tTest.test1, COT_PARAM_UINT16, COT_PARAM_ATTR_WR, 20, 10, 2000),
    // COT_PARAM_ITEM_BIND(10, sg_tTest.test2, COT_PARAM_FLOAT, COT_PARAM_ATTR_WR),
    // COT_PARAM_ITEM_BIND(11, sg_tTest.str, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "const-zpc", 6, sizeof(sg_tTest.str)),
    COT_PARAM_ITEM_BIND(12, g_test_88, COT_PARAM_INT64, COT_PARAM_ATTR_WR, 5, -542, 5450),
    COT_PARAM_ITEM_BIND(13, g_str_des, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "WER45", 10, sizeof(g_str_des)),
    COT_PARAM_ITEM_BIND(14, g_str_des_2, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "WTG"),
    COT_PARAM_ITEM_BIND(15, g_str_des_3, COT_PARAM_STRING, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND(16, g_sd, COT_PARAM_INT8, COT_PARAM_ATTR_WR, 5),
};


static cotParamManager_t sg_tParamManager;
static cotParamManager_t sg_tParamManager1;

static void ShowSingleParam(const cotParamInfo_t *paramInfo);

// 数据校验出错时恢复默认处理
int OnCheckErrorResetHandle(const cotParamInfo_t *pParamInfo, cotParamCheckRet_e eCheckResult)
{
    printf("\n--------error list start ----------\n");
    printf("%4s    %-15s %-8s %-8s %-8s %-10s %-10s %-10s %-10s\n", "id", "name", "type", "length", "attr", "val", "def", "min", "max");
    ShowSingleParam(pParamInfo);
    printf("--------error list end ------------\n");
    cotParam_SingleParamResetDefValue(pParamInfo);
    return 0;
}

void InitParam(bool isReset)
{
    cotParam_Init(&sg_tParamManager, sg_ParamTable, COT_PARAM_TABLE_SIZE(sg_ParamTable));
    cotParam_Init(&sg_tParamManager1, sg_ParamTable1, COT_PARAM_TABLE_SIZE(sg_ParamTable1));

    if (sg_length == 0) // 储存设备中没有储存过参数则首次进行储存
    {
        printf("frist save param: %d\n", cotParam_GetSerializeSize(&sg_tParamManager));
        SaveParam(false);
        
        // cotParam_Save(&sg_tParamManager, OnSaveCallback, NULL); // 初次储存可以认为数据都是默认值，无需数据校验出错时恢复默认处理
    }

    ReloadParam(isReset);
}

void ReloadParam(bool isReset)
{
    printf("load param\n");

    if (isReset)
    {
        cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
        cotParam_Load(&sg_tParamManager, OnLoadCallback);
        // cotParam_Deserialization(&sg_tParamManager1, sg_buf, sg_length);
        // cotParam_Load(&sg_tParamManager1, OnLoadCallback, NULL);
    }
    else
    {
        cotParam_Load(&sg_tParamManager, OnLoadCallback);
        // cotParam_Deserialization(&sg_tParamManager1, sg_buf, sg_length);
        // cotParam_Load(&sg_tParamManager1, OnLoadCallback, NULL);
    }
}

void ResetParam(void)
{
    printf("reset param\n");
    cotParam_ResetDefault(&sg_tParamManager);
}

void SaveParam(bool isReset)
{
    uint8_t buf[300];
    uint32_t bufLength = 0;
    printf("save param\n");
    if (isReset)
    {
        cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
        cotParam_Save(&sg_tParamManager, OnSaveCallback);
    }
    else
    {
        bufLength = cotParam_Serialize(&sg_tParamManager, buf);
        OnSaveCallback(buf, bufLength, false);
        OnSaveCallback(buf, 0, true);
        // cotParam_Save(&sg_tParamManager, OnSaveCallback);
    }
}

#define ATTR(x)   ((x & (PARAM_ATTR_READ | COT_PARAM_ATTR_WRITE)) == (PARAM_ATTR_READ | COT_PARAM_ATTR_WRITE) ? \
                    "wr" : (x & (PARAM_ATTR_READ) ? "r" : ((x & (PARAM_ATTR_WRITE) ? "w" : ""))))

char *Attr(uint8_t attr)
{
    static char buf[10];
    char *p = buf;

    if (attr & COT_PARAM_ATTR_READ)
    {
        p += sprintf(p, "r");
    }

    if (attr & COT_PARAM_ATTR_WRITE)
    {
        p += sprintf(p, "w");
    }

    if (attr & COT_PARAM_ATTR_RESET)
    {
        p += sprintf(p, "s");
    }

    if (attr & COT_PARAM_ATTR_RANGE)
    {
        p += sprintf(p, "m");
    }

    return buf;
}

void ShowSingleParam(const cotParamInfo_t *paramInfo)
{
    if (paramInfo != NULL)
    {
        switch (paramInfo->type)
        {
        case COT_PARAM_INT8:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->pszName, 
                "int8_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt8);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10d ", *paramInfo->unDefValuePtr.pInt8);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10d ", *paramInfo->unMinValuePtr.pInt8);
                printf("%-10d ", *paramInfo->unMaxValuePtr.pInt8);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_INT16:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->pszName, 
                "int16_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt16);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10d ", *paramInfo->unDefValuePtr.pInt16);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10d ", *paramInfo->unMinValuePtr.pInt16);
                printf("%-10d ", *paramInfo->unMaxValuePtr.pInt16);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_INT32:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->pszName, 
                "int32_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt32);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10d ", *paramInfo->unDefValuePtr.pInt32);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10d ", *paramInfo->unMinValuePtr.pInt32);
                printf("%-10d ", *paramInfo->unMaxValuePtr.pInt32);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_INT64:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10ld ", paramInfo->id, paramInfo->pszName, 
                "int64_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt64);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10ld ", *paramInfo->unDefValuePtr.pInt64);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10ld ", *paramInfo->unMinValuePtr.pInt64);
                printf("%-10ld ", *paramInfo->unMaxValuePtr.pInt64);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_UINT8:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->pszName, 
                "uint8_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint8);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10u ", *paramInfo->unDefValuePtr.pUint8);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10u ", *paramInfo->unMinValuePtr.pUint8);
                printf("%-10u ", *paramInfo->unMaxValuePtr.pUint8);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_UINT16:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->pszName, 
                "uint16_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint16);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10u ", *paramInfo->unDefValuePtr.pUint16);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10u ", *paramInfo->unMinValuePtr.pUint16);
                printf("%-10u ", *paramInfo->unMaxValuePtr.pUint16);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_UINT32:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->pszName, 
                "uint32_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint32);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10u ", *paramInfo->unDefValuePtr.pUint32);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10u ", *paramInfo->unMinValuePtr.pUint32);
                printf("%-10u ", *paramInfo->unMaxValuePtr.pUint32);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_UINT64:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10lu ", paramInfo->id, paramInfo->pszName, 
                "uint64_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint64);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10lu ", *paramInfo->unDefValuePtr.pUint64);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10lu ", *paramInfo->unMinValuePtr.pUint64);
                printf("%-10lu ", *paramInfo->unMaxValuePtr.pUint64);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_FLOAT:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10f ", paramInfo->id, paramInfo->pszName, 
                "float", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pFloat);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10f ", *paramInfo->unDefValuePtr.pFloat);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10f ", *paramInfo->unMinValuePtr.pFloat);
                printf("%-10f ", *paramInfo->unMaxValuePtr.pFloat);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_DOUBLE:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10f ", paramInfo->id, paramInfo->pszName, 
                "double", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pDouble);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10f ", *paramInfo->unDefValuePtr.pDouble);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10f ", *paramInfo->unMinValuePtr.pDouble);
                printf("%-10f ", *paramInfo->unMaxValuePtr.pDouble);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
#if COT_PARAM_USE_STRING_TYPE
        case COT_PARAM_STRING:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10s ", paramInfo->id, paramInfo->pszName, 
                "string", paramInfo->length, Attr(paramInfo->attr), paramInfo->unCurValuePtr.pString);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10s ", paramInfo->unDefValuePtr.pString);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10u ", *paramInfo->unMinValuePtr.pStringLength);
                printf("%-10u ", *paramInfo->unMaxValuePtr.pStringLength);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
#endif
        default:
            break;
        }

        printf("\n");

    }
}

void ShowAllParam(void)
{
    size_t idx = 0;
    cotParamInfo_t *paramInfo;

    printf("%4s    %-15s %-8s %-8s %-8s %-10s %-10s %-10s %-10s\n", "id", "name", "type", "length", "attr", "val", "def", "min", "max");

    do
    {
        paramInfo = cotParam_IterateList(&sg_tParamManager, &idx);

        ShowSingleParam(paramInfo);
    } while (paramInfo != NULL);
}

void ShowAllParam_1(void)
{
    size_t idx = 0;
    cotParamInfo_t *paramInfo;

    printf("%4s    %-15s %-8s %-8s %-8s %-10s %-10s %-10s %-10s\n", "id", "name", "type", "length", "attr", "val", "def", "min", "max");

    do
    {
        paramInfo = cotParam_IterateList(&sg_tParamManager1, &idx);

        ShowSingleParam(paramInfo);
    } while (paramInfo != NULL);
}
