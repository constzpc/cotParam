#include "param_demo.h"
#include <string.h>
#include <stdio.h>
#include<stdlib.h>

// 0 使用一次性完成参数保存/加载  1 使用分次完成参数保存/加载
#define USE_PARAM_SAVE_LOAD_API   0

/******************************************  模拟储存设备操作  ******************************************************/
// 储存空间定义
uint8_t sg_buf[500];
uint32_t sg_length = 0;

#define HEX_PRINTF(str, hex, len)  \
    printf("%s: [%d] -> ", str, len);\
    for (int i = 0; i < (len); i++){printf("%02x ", hex[i]);}\
    printf("\n");

// 从储存空间读取数据
int OnLoadCallback(uint8_t *pBuf, uint16_t bufSize, uint16_t *pLength)
{
    uint16_t length;
    static uint32_t s_offset = 0;

    if (s_offset < sg_length)
    {
        if (s_offset + bufSize <= sg_length)
        {
            length = bufSize;
            memcpy(pBuf, &sg_buf[s_offset], length);
            s_offset += length;
        }
        else
        {
            length = sg_length - s_offset;
            memcpy(pBuf, &sg_buf[s_offset], length);
            s_offset += length;
        }
    }
    else
    {
        length = 0;
        s_offset = 0;
    }

    if (length > 0)
    {
        printf("************************************************************************************************\n");
        HEX_PRINTF("\tread", pBuf, length);
        printf("************************************************************************************************\n");
    }

    *pLength = length;
    return 0;
}

// 写数据至储存空间
int OnSaveCallback(const uint8_t *pBuf, uint16_t len)
{
    static uint32_t s_offset = 0;

    if (len > 0)
    {
        memcpy(&sg_buf[s_offset], pBuf, len);
        HEX_PRINTF("\twrite", pBuf, len);
        s_offset += len;
        sg_length = s_offset;
    }
    else
    {
        printf("************************************************************************************************\n");
        HEX_PRINTF("\tSave", sg_buf, sg_length);
        printf("************************************************************************************************\n");
        s_offset = 0;
    }

    return 0;
}

/******************************************  模拟储存设备操作  ******************************************************/

ParamDemo_t g_tTestVal = {
    .usValue = 20,
    .ucValue = 10,
    .uiValue = 1000,
    .fValue = 3.14,
    .szString_1 = "abcd",
    .dValue = 5.12,
    .sValue = -100,
    .cValue = -2,
    .iValue = 300,
    .szString_2 = "12234",
};

int8_t g_cTest = 50;
char g_szString[10] = "qwer";

static int CheckSValue(const void *pCurParam);

cotParamInfo_t sg_ParamTable[] = {
    COT_PARAM_ITEM_BIND(1, g_tTestVal.usValue, COT_PARAM_UINT16, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND(2, g_tTestVal.ucValue, COT_PARAM_UINT8, COT_PARAM_ATTR_WR, 20),
    COT_PARAM_ITEM_BIND(3, g_tTestVal.uiValue, COT_PARAM_UINT32, COT_PARAM_ATTR_WR, 1000, 1000, 10000),
    COT_PARAM_ITEM_BIND(4, g_tTestVal.fValue, COT_PARAM_FLOAT, COT_PARAM_ATTR_WR, 10, -10.5, 10.5),
    COT_PARAM_ITEM_BIND(5, g_tTestVal.szString_1, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "abcd", 3, sizeof(g_tTestVal.szString_1)),
    COT_PARAM_ITEM_BIND(6, g_tTestVal.dValue, COT_PARAM_DOUBLE, COT_PARAM_ATTR_WR, 0, -90.10, 100.10),
    COT_PARAM_ITEM_BIND(7, g_tTestVal.sValue, COT_PARAM_INT16, COT_PARAM_ATTR_WR, 100, -200, 200, CheckSValue), // 添加自定义校验
    COT_PARAM_ITEM_BIND_WITH_NAME(8, "g_cTest", g_cTest, COT_PARAM_INT8, COT_PARAM_ATTR_WR, 50, -100, 100), // 另取参数名
    COT_PARAM_ITEM_BIND(9, g_szString, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "XXX", 3, 6),
};

static int CheckSValue(const void *pCurParam)
{
    const int16_t *p_sValue = (const int16_t *)pCurParam;

    if ((*p_sValue) % 2 != 0)
    {
        return -1;
    }

    return 0;
}


static cotParamManager_t sg_tParamManager;

static void ShowSingleParam(const cotParamInfo_t *paramInfo);

// 数据校验出错时恢复默认处理
int OnCheckErrorResetHandle(const cotParamInfo_t *pParamInfo, cotParamCheckRet_e eCheckResult)
{
    printf("\n--------error list start ----------\n");
    printf("%4s    %-24s %-8s %-8s %-8s %-10s %-10s %-10s %-10s\n", "id", "name", "type", "length", "attr", "val", "def", "min", "max");
    ShowSingleParam(pParamInfo);
    printf("--------error list end ------------\n");
    cotParam_SingleParamResetDefValue(pParamInfo);
    return 0;
}

void InitParam(bool isReset)
{
    cotParam_Init(&sg_tParamManager, sg_ParamTable, COT_PARAM_TABLE_SIZE(sg_ParamTable));

    if (sg_length == 0) // 储存设备中没有储存过参数则首次进行储存
    {
        printf("frist save param: %d\n", cotParam_GetSerializeSize(&sg_tParamManager));
        SaveParam(false);
    }

    ReloadParam(isReset);
}

void ReloadParam(bool isReset)
{
    uint8_t buf[300];
    uint16_t length;

    printf("\n============================ load param start ==========================\n");

    if (isReset)
    {
        cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
    }

#if USE_PARAM_SAVE_LOAD_API
    cotParam_Load(&sg_tParamManager, OnLoadCallback);
#else
    // OnLoadCallback(sg_buf, sg_length, true);
    cotParam_Deserialization(&sg_tParamManager, sg_buf, sg_length);
#endif
    printf("============================ load param end ============================\n\n");
}

void SaveParam(bool isReset)
{
    uint8_t buf[300];
    uint32_t bufLength = 0;
    printf("\n============================ save param start ==========================\n");
    if (isReset)
    {
        cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
    }

#if USE_PARAM_SAVE_LOAD_API
    cotParam_Save(&sg_tParamManager, OnSaveCallback);
#else
    bufLength = cotParam_Serialize(&sg_tParamManager, buf);
    OnSaveCallback(buf, bufLength);
    OnSaveCallback(buf, 0);
#endif
    printf("============================ save param end ============================\n\n");
}

void ResetParam(void)
{
    printf("reset param\n");
    cotParam_ResetDefault(&sg_tParamManager);
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
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->pszName, 
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
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->pszName, 
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
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->pszName, 
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
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10ld ", paramInfo->id, paramInfo->pszName, 
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
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->pszName, 
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
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->pszName, 
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
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->pszName, 
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
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10lu ", paramInfo->id, paramInfo->pszName, 
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
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10f ", paramInfo->id, paramInfo->pszName, 
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
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10f ", paramInfo->id, paramInfo->pszName, 
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
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10s ", paramInfo->id, paramInfo->pszName, 
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

int SingleParamCheckProcess(const void *pCurParam, cotParamResetOpt_e eResetOpt)
{
    return cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, pCurParam), eResetOpt);
}

cotParamCheckRet_e SingleParamCheck(const void *pCurParam, const void *pCheckValue)
{
    cotParamCheckRet_e eCheckResult;

    cotParam_SingleParamCheckInput(cotParam_FindParamByParamPtr(&sg_tParamManager, pCurParam), pCheckValue, &eCheckResult);

    return eCheckResult;
}

cotParamCheckRet_e SingleParamSelfCheck(const void *pCurParam)
{
    cotParamCheckRet_e eCheckResult;

    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, pCurParam), &eCheckResult);

    return eCheckResult;
}

void SingleParamResetResetDefValue(const void *pCurParam)
{
    cotParam_SingleParamResetDefValue(cotParam_FindParamByParamPtr(&sg_tParamManager, pCurParam));
}

void ShowAllParam(void)
{
    size_t idx = 0;
    cotParamInfo_t *paramInfo;

    printf("%4s    %-24s %-8s %-8s %-8s %-10s %-10s %-10s %-10s\n", "id", "name", "type", "length", "attr", "val", "def", "min", "max");

    do
    {
        paramInfo = cotParam_IterateList(&sg_tParamManager, &idx);

        ShowSingleParam(paramInfo);
    } while (paramInfo != NULL);
    printf("\n");
}

