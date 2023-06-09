#include "param_demo.h"
#include <string.h>
#include <stdio.h>

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
PARAM_DEFINE_STR_RANGE (g_test_str, 10, "abcdef", 5, 10);
PARAM_DEFINE_DAT_RANGE (g_test_4, PARAM_INT8, 8, -10, 10);
PARAM_DEFINE_DAT_RANGE (g_test_5, PARAM_UINT32, 620, 500, 10000);
PARAM_DEFINE_DAT_RANGE (g_test_6, PARAM_UINT8, 45, 5, 100);
PARAM_DEFINE_DAT_RANGE (g_test_7, PARAM_INT64, 5, -542, 5450);

ParamInfo_t sg_ParamTable[] = {
    PARAM_ITEM_DAT(1, g_test, PARAM_ATTR_READ | PARAM_ATTR_WRITE),
    PARAM_ITEM_DAT_DEF(2, g_test_2, PARAM_ATTR_READ),
    PARAM_ITEM_DAT_RANGE(3, g_test_3, PARAM_ATTR_READ | PARAM_ATTR_WRITE),
    PARAM_ITEM_STR_RANGE(4, g_test_str, PARAM_ATTR_READ | PARAM_ATTR_WRITE),
    PARAM_ITEM_DAT_RANGE(5, g_test_4, PARAM_ATTR_READ),
    PARAM_ITEM_DAT_RANGE(6, g_test_5, PARAM_ATTR_READ),
    PARAM_ITEM_DAT_RANGE(7, g_test_6, PARAM_ATTR_READ),
    PARAM_ITEM_DAT_RANGE(8, g_test_7, PARAM_ATTR_READ | PARAM_ATTR_WRITE),
};


static ParamManager_t sg_tParamManager;

// 数据校验出错时恢复默认处理
int OnCheckErrorResetHandle(const ParamInfo_t *pParamInfo)
{
    char outbuf[1024];
    Param_Sprintf(outbuf, pParamInfo);
    printf("--error: \n%s\n", outbuf);
    Param_ResetParamValue(pParamInfo);
    return 0;
}

void InitParam(bool isReset)
{
    Param_Init(&sg_tParamManager, sg_ParamTable, PARAM_TABLE_SIZE(sg_ParamTable));

    Param_SetCallBackFun(&sg_tParamManager, OnLoadCallback, OnSaveCallback);

    if (sg_length == 0) // 储存设备中没有储存过参数则首次进行储存
    {
        printf("frist save param\n");
        Param_Save(&sg_tParamManager, NULL); // 初次储存可以认为数据都是默认值，无需数据校验出错时恢复默认处理
    }

    ReloadParam(isReset);
}

void ReloadParam(bool isReset)
{
    printf("load param\n");

    if (isReset)
    {
        Param_Load(&sg_tParamManager, OnCheckErrorResetHandle);
    }
    else
    {
        Param_Load(&sg_tParamManager, NULL);
    }
}

void ResetParam(void)
{
    printf("reset param\n");
    Param_ResetParam(&sg_tParamManager);
}

void SaveParam(void)
{
    printf("save param\n");
    Param_Save(&sg_tParamManager, OnCheckErrorResetHandle);
}

const ParamInfo_t *FindParamByName(const char *pszName)
{
    return Param_FindParamByName(&sg_tParamManager, pszName);
}

const ParamInfo_t* FindParamByID(uint16_t id)
{
    return Param_FindParamByID(&sg_tParamManager, id);
}

const bool SetParamNewValue(const void *curParamPtr, const void *newValue)
{
    return Param_SetParamValue(Param_FindParamByParamPtr(&sg_tParamManager, curParamPtr), newValue);
}
