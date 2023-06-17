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



PARAM_DEFINE_DAT (g_test, PARAM_INT16, 10);
PARAM_DEFINE_DAT_DEF (g_test_2, PARAM_UINT16, 20);
PARAM_DEFINE_DAT_RANGE (g_test_3, PARAM_DOUBLE, 3.15, -10, 10);
PARAM_DEFINE_STR_RANGE (g_test_str, 10, "abcdef", 5);
PARAM_DEFINE_DAT_RANGE (g_test_4, PARAM_INT8, 8, -10, 10);
PARAM_DEFINE_DAT_RANGE (g_test_5, PARAM_UINT32, 620, 500, 10000);
PARAM_DEFINE_DAT_RANGE (g_test_6, PARAM_UINT8, 45, 5, 100);
PARAM_DEFINE_DAT_RANGE (g_test_7, PARAM_INT64, 5, -542, 5450);

ParamInfo_t sg_ParamTable[] = {
    PARAM_ITEM_DAT(1, g_test, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_DEF(2, g_test_2, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(3, g_test_3, PARAM_ATTR_WR),
    PARAM_ITEM_STR_RANGE(4, g_test_str, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(5, g_test_4, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(6, g_test_5, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(7, g_test_6, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(8, g_test_7, PARAM_ATTR_WR),
};


static ParamManager_t sg_tParamManager;

static void ShowSingleParam(const ParamInfo_t *paramInfo);

// 数据校验出错时恢复默认处理
int OnCheckErrorResetHandle(const ParamInfo_t *pParamInfo)
{
    printf("\n--------error list start ----------\n");
    printf("%4s    %-15s %-8s %-8s %-8s %-10s %-10s %-10s %-10s\n", "id", "name", "type", "length", "attr", "val", "def", "min", "max");
    ShowSingleParam(pParamInfo);
    printf("--------error list end ------------\n");
    Param_ResetDefaultValue(pParamInfo);
    return 0;
}

void InitParam(bool isReset)
{
    Param_Init(&sg_tParamManager, sg_ParamTable, PARAM_TABLE_SIZE(sg_ParamTable));

    if (sg_length == 0) // 储存设备中没有储存过参数则首次进行储存
    {
        printf("frist save param\n");
        Param_Save(&sg_tParamManager, OnSaveCallback, NULL); // 初次储存可以认为数据都是默认值，无需数据校验出错时恢复默认处理
    }

    ReloadParam(isReset);
}

void ReloadParam(bool isReset)
{
    printf("load param\n");

    if (isReset)
    {
        Param_Load(&sg_tParamManager, OnLoadCallback, OnCheckErrorResetHandle);
    }
    else
    {
        Param_Load(&sg_tParamManager, OnLoadCallback, NULL);
    }
}

void ResetParam(void)
{
    printf("reset param\n");
    Param_ResetParam(&sg_tParamManager);
}

void SaveParam(bool isReset)
{
    printf("save param\n");
    if (isReset)
    {
        Param_Save(&sg_tParamManager, OnSaveCallback, OnCheckErrorResetHandle);
    }
    else
    {
        Param_Save(&sg_tParamManager, OnSaveCallback, NULL);
    }
}

#define ATTR(x)   ((x & (PARAM_ATTR_READ | PARAM_ATTR_WRITE)) == (PARAM_ATTR_READ | PARAM_ATTR_WRITE) ? \
                    "wr" : (x & (PARAM_ATTR_READ) ? "r" : ((x & (PARAM_ATTR_WRITE) ? "w" : ""))))

char *Attr(uint8_t attr)
{
    static char buf[10];
    char *p = buf;

    if (attr & PARAM_ATTR_READ)
    {
        p += sprintf(p, "r");
    }

    if (attr & PARAM_ATTR_WRITE)
    {
        p += sprintf(p, "w");
    }

    if (attr & PARAM_ATTR_RESET)
    {
        p += sprintf(p, "s");
    }

    if (attr & PARAM_ATTR_RANGE)
    {
        p += sprintf(p, "m");
    }

    return buf;
}

void ShowSingleParam(const ParamInfo_t *paramInfo)
{
    if (paramInfo != NULL)
    {
        switch (paramInfo->type)
        {
        case PARAM_INT8:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->pszName, 
                "int8_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt8);

            if (paramInfo->attr & PARAM_ATTR_RESET)
                printf("%-10d ", *paramInfo->unDefValuePtr.pInt8);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & PARAM_ATTR_RANGE)
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
        case PARAM_INT16:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->pszName, 
                "int16_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt16);

            if (paramInfo->attr & PARAM_ATTR_RESET)
                printf("%-10d ", *paramInfo->unDefValuePtr.pInt16);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & PARAM_ATTR_RANGE)
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
        case PARAM_INT32:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->pszName, 
                "int32_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt32);

            if (paramInfo->attr & PARAM_ATTR_RESET)
                printf("%-10d ", *paramInfo->unDefValuePtr.pInt32);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & PARAM_ATTR_RANGE)
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
        case PARAM_INT64:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->pszName, 
                "int64_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt64);

            if (paramInfo->attr & PARAM_ATTR_RESET)
                printf("%-10d ", *paramInfo->unDefValuePtr.pInt64);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & PARAM_ATTR_RANGE)
            {
                printf("%-10d ", *paramInfo->unMinValuePtr.pInt64);
                printf("%-10d ", *paramInfo->unMaxValuePtr.pInt64);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case PARAM_UINT8:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->pszName, 
                "uint8_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint8);

            if (paramInfo->attr & PARAM_ATTR_RESET)
                printf("%-10u ", *paramInfo->unDefValuePtr.pUint8);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & PARAM_ATTR_RANGE)
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
        case PARAM_UINT16:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->pszName, 
                "uint16_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint16);

            if (paramInfo->attr & PARAM_ATTR_RESET)
                printf("%-10u ", *paramInfo->unDefValuePtr.pUint16);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & PARAM_ATTR_RANGE)
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
        case PARAM_UINT32:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->pszName, 
                "uint32_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint32);

            if (paramInfo->attr & PARAM_ATTR_RESET)
                printf("%-10u ", *paramInfo->unDefValuePtr.pUint32);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & PARAM_ATTR_RANGE)
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
        case PARAM_UINT64:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->pszName, 
                "uint64_t", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint64);

            if (paramInfo->attr & PARAM_ATTR_RESET)
                printf("%-10u ", *paramInfo->unDefValuePtr.pUint64);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & PARAM_ATTR_RANGE)
            {
                printf("%-10u ", *paramInfo->unMinValuePtr.pUint64);
                printf("%-10u ", *paramInfo->unMaxValuePtr.pUint64);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case PARAM_FLOAT:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10f ", paramInfo->id, paramInfo->pszName, 
                "float", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pFloat);

            if (paramInfo->attr & PARAM_ATTR_RESET)
                printf("%-10f ", *paramInfo->unDefValuePtr.pFloat);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & PARAM_ATTR_RANGE)
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
        case PARAM_DOUBLE:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10f ", paramInfo->id, paramInfo->pszName, 
                "double", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pDouble);

            if (paramInfo->attr & PARAM_ATTR_RESET)
                printf("%-10f ", *paramInfo->unDefValuePtr.pDouble);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & PARAM_ATTR_RANGE)
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
        case PARAM_STRING:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10s ", paramInfo->id, paramInfo->pszName, 
                "string", paramInfo->length, Attr(paramInfo->attr), paramInfo->unCurValuePtr.pString);

            if (paramInfo->attr & PARAM_ATTR_RESET)
                printf("%-10s ", paramInfo->unDefValuePtr.pString);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & PARAM_ATTR_RANGE)
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
        
        default:
            break;
        }

        printf("\n");

    }
}

void ShowAllParam(void)
{
    size_t idx = 0;
    ParamInfo_t *paramInfo;

    printf("%4s    %-15s %-8s %-8s %-8s %-10s %-10s %-10s %-10s\n", "id", "name", "type", "length", "attr", "val", "def", "min", "max");

    do
    {
        paramInfo = Param_IterateList(&sg_tParamManager, &idx);

        ShowSingleParam(paramInfo);
    } while (paramInfo != NULL);
}
