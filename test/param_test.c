#include <stdio.h>
#include <string.h>
#include <float.h>
#include "unity.h"
#include "param.h"

PARAM_DEFINE_DAT (g_test_1, PARAM_INT16, 10);
PARAM_DEFINE_DAT_DEF (g_test_2, PARAM_UINT16, 20);
PARAM_DEFINE_DAT_RANGE (g_test_float, PARAM_FLOAT, 3.15, -10, 10);
PARAM_DEFINE_DAT_RANGE (g_test_double, PARAM_DOUBLE, 3.15, -10, 10);
#if PARAM_USE_STRING_TYPE
PARAM_DEFINE_STR_RANGE (g_test_str, 10, "abcdef", 5);
#endif
PARAM_DEFINE_DAT_RANGE (g_test_s8, PARAM_INT8, 10, -10, 15);
PARAM_DEFINE_DAT_RANGE (g_test_s16, PARAM_INT16, 100, -100, 3000);
PARAM_DEFINE_DAT_RANGE (g_test_s32, PARAM_INT32, 1000, -900, 10000);
PARAM_DEFINE_DAT_RANGE (g_test_s64, PARAM_INT64, 8000, -100, 1000000);
PARAM_DEFINE_DAT_RANGE (g_test_u8, PARAM_UINT8, 10, 5, 15);
PARAM_DEFINE_DAT_RANGE (g_test_u16, PARAM_UINT16, 100, 100, 3000);
PARAM_DEFINE_DAT_RANGE (g_test_u32, PARAM_UINT32, 1000, 900, 10000);
PARAM_DEFINE_DAT_RANGE (g_test_u64, PARAM_UINT64, 8000, 100, 1000000);


ParamInfo_t sg_ParamTable[] = {
    PARAM_ITEM_DAT(1, g_test_1, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_DEF(2, g_test_2, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(3, g_test_float, PARAM_ATTR_READ),
    PARAM_ITEM_DAT_RANGE(4, g_test_double, PARAM_ATTR_WR),
#if PARAM_USE_STRING_TYPE
    PARAM_ITEM_STR_RANGE(5, g_test_str, PARAM_ATTR_WR),
#endif
    PARAM_ITEM_DAT_RANGE(6, g_test_s8, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(7, g_test_s16, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(8, g_test_s32, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(9, g_test_s64, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(10, g_test_u8, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(11, g_test_u16, PARAM_ATTR_WR),
    PARAM_ITEM_DAT_RANGE(12, g_test_u32, PARAM_ATTR_NONE),
    PARAM_ITEM_DAT_RANGE(13, g_test_u64, PARAM_ATTR_WR),
};

static ParamManager_t sg_tParamManager;

PARAM_EXTERN_DAT(g_test_1, PARAM_INT16);
PARAM_EXTERN_DAT(g_test_2, PARAM_UINT16);
PARAM_EXTERN_DAT(g_test_float, PARAM_FLOAT);
PARAM_EXTERN_DAT(g_test_double, PARAM_DOUBLE);
PARAM_EXTERN_STR(g_test_str, 10);
PARAM_EXTERN_DAT(g_test_s8, PARAM_INT8);
PARAM_EXTERN_DAT(g_test_s16, PARAM_INT16);
PARAM_EXTERN_DAT(g_test_s32, PARAM_INT32);
PARAM_EXTERN_DAT(g_test_s64, PARAM_INT64);
PARAM_EXTERN_DAT(g_test_u8, PARAM_UINT8);
PARAM_EXTERN_DAT(g_test_u16, PARAM_UINT16);
PARAM_EXTERN_DAT(g_test_u32, PARAM_UINT32);
PARAM_EXTERN_DAT(g_test_u64, PARAM_UINT64);

void setUp(void)
{
    // Set up any necessary initialization before each test
}

void tearDown(void)
{
    // Clean up any resources after each test
}

void test_ParamInit(void)
{
    Param_Init(&sg_tParamManager, sg_ParamTable, PARAM_TABLE_SIZE(sg_ParamTable));

    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable, sg_ParamTable);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.count, PARAM_TABLE_SIZE(sg_ParamTable));

    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[0].id, 1);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[0].length, 2);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[0].type, PARAM_INT16);
    TEST_ASSERT_EQUAL_HEX8(sg_tParamManager.pParamTable[0].attr, PARAM_ATTR_WR);
    TEST_ASSERT_EQUAL_STRING(sg_tParamManager.pParamTable[0].pszName, "g_test_1");
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[0].unCurValuePtr.pVoid , &PARAM_DAT_CUR_VALUE(g_test_1));
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[0].unDefValuePtr.pVoid , NULL);
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[0].unMinValuePtr.pVoid , NULL);
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[0].unMaxValuePtr.pVoid , NULL);

    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[1].id, 2);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[1].length, 2);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[1].type, PARAM_UINT16);
    TEST_ASSERT_EQUAL_HEX8(sg_tParamManager.pParamTable[1].attr, PARAM_ATTR_WR | PARAM_ATTR_RESET);
    TEST_ASSERT_EQUAL_STRING(sg_tParamManager.pParamTable[1].pszName, "g_test_2");
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[1].unCurValuePtr.pVoid , &PARAM_DAT_CUR_VALUE(g_test_2));
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[1].unDefValuePtr.pVoid , &PARAM_DAT_DEF_VALUE(g_test_2));
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[1].unMinValuePtr.pVoid , NULL);
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[1].unMaxValuePtr.pVoid , NULL);

    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[2].id, 3);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[2].length, 4);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[2].type, PARAM_FLOAT);
    TEST_ASSERT_EQUAL_HEX8(sg_tParamManager.pParamTable[2].attr, PARAM_ATTR_READ | PARAM_ATTR_RESET | PARAM_ATTR_RANGE);
    TEST_ASSERT_EQUAL_STRING(sg_tParamManager.pParamTable[2].pszName, "g_test_float");
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[2].unCurValuePtr.pVoid , &PARAM_DAT_CUR_VALUE(g_test_float));
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[2].unDefValuePtr.pVoid , &PARAM_DAT_DEF_VALUE(g_test_float));
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[2].unMinValuePtr.pVoid , &PARAM_DAT_MIN_VALUE(g_test_float));
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[2].unMaxValuePtr.pVoid , &PARAM_DAT_MAX_VALUE(g_test_float));

#if PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[4].id, 5);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[4].length, 10);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[4].type, PARAM_STRING);
    TEST_ASSERT_EQUAL_HEX8(sg_tParamManager.pParamTable[4].attr, PARAM_ATTR_WR | PARAM_ATTR_RESET | PARAM_ATTR_RANGE);
    TEST_ASSERT_EQUAL_STRING(sg_tParamManager.pParamTable[4].pszName, "g_test_str");
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[4].unCurValuePtr.pVoid , PARAM_STR_CUR_VALUE(g_test_str));
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[4].unDefValuePtr.pVoid , PARAM_STR_DEF_VALUE(g_test_str));
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[4].unMinValuePtr.pVoid , &PARAM_STR_MIN_LENGTH(g_test_str));
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[4].unMaxValuePtr.pVoid , &PARAM_STR_MAX_LENGTH(g_test_str));
#endif
}

void test_ResetParam(void)
{
    g_test_1 = 80;
    g_test_2 = 50;
    g_test_float = 1;
#if PARAM_USE_STRING_TYPE
    sprintf(g_test_str, "ABCDFS");
#endif
    Param_ResetParam(&sg_tParamManager);

    TEST_ASSERT_EQUAL_INT(g_test_1, 80);
    TEST_ASSERT_EQUAL_UINT(g_test_2, PARAM_DAT_DEF_VALUE(g_test_2));
    TEST_ASSERT_EQUAL_FLOAT(g_test_float, PARAM_DAT_DEF_VALUE(g_test_float));
#if PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_STRING(g_test_str, PARAM_STR_DEF_VALUE(g_test_str));
#endif
}



// 储存空间定义
static uint8_t sg_buf[500];
static uint32_t sg_length = 0;

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
        s_offset = 0;
        return 0;
    }

    memcpy(&sg_buf[s_offset], pBuf, len);
    s_offset += len;
    sg_length = s_offset;

    return 0;
}

int OnCheckErrorResetHandle(const ParamInfo_t *pParamInfo)
{
    Param_ResetDefaultValue(pParamInfo);
    return 0;
}

void test_SaveAndLoadParam(void)
{
    g_test_1 = 80;
    g_test_2 = 50;
    g_test_float = -20;
#if PARAM_USE_STRING_TYPE
    sprintf(g_test_str, "12a");
#endif
    Param_Save(&sg_tParamManager, OnSaveCallback, NULL);
    TEST_ASSERT_EQUAL_INT(g_test_1, 80);
    TEST_ASSERT_EQUAL_UINT(g_test_2, 50);
    TEST_ASSERT_EQUAL_FLOAT(g_test_float, -20);
#if PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_STRING(g_test_str, "12a");
#endif
    Param_Save(&sg_tParamManager, OnSaveCallback, OnCheckErrorResetHandle);

    TEST_ASSERT_EQUAL_INT(g_test_1, 80);
    TEST_ASSERT_EQUAL_UINT(g_test_2, 50);
    TEST_ASSERT_EQUAL_FLOAT(g_test_float, PARAM_DAT_DEF_VALUE(g_test_float));
#if PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_STRING(g_test_str, PARAM_STR_DEF_VALUE(g_test_str));
#endif
    g_test_1 = 60;
    g_test_2 = 40;
    g_test_float = -20;
    g_test_double = 5.36;
#if PARAM_USE_STRING_TYPE
    sprintf(g_test_str, "12a123");
#endif
    g_test_s8 = 20;
    g_test_s16 = 1000;

    Param_Load(&sg_tParamManager, OnLoadCallback, OnCheckErrorResetHandle);

    TEST_ASSERT_EQUAL_INT(g_test_1, 80);
    TEST_ASSERT_EQUAL_UINT(g_test_2, 50);
    TEST_ASSERT_EQUAL_FLOAT(g_test_float, PARAM_DAT_DEF_VALUE(g_test_float));
#if PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_STRING(g_test_str, PARAM_STR_DEF_VALUE(g_test_str));
#endif
    TEST_ASSERT_EQUAL_INT(PARAM_DAT_DEF_VALUE(g_test_s8), g_test_s8);
    TEST_ASSERT_EQUAL_INT(PARAM_DAT_DEF_VALUE(g_test_s16), g_test_s16);
    TEST_ASSERT_EQUAL_INT(PARAM_DAT_DEF_VALUE(g_test_u16), g_test_u16);
    TEST_ASSERT_EQUAL_INT(PARAM_DAT_DEF_VALUE(g_test_u32), g_test_u32);
}

void test_IterateParam(void)
{
    size_t idx = 0;
    ParamInfo_t *paramInfo;
    size_t cnt = 0;

    do
    {
        paramInfo = Param_IterateList(&sg_tParamManager, &idx);

        if (paramInfo != NULL)
        {
            cnt++;
        }
        
    } while (paramInfo != NULL);

    TEST_ASSERT_EQUAL_UINT((PARAM_TABLE_SIZE(sg_ParamTable) - 1), cnt);
}


void test_FindParam(void)
{
    const ParamInfo_t *paramInfo;

    paramInfo = Param_FindParamByName(&sg_tParamManager, "g_test_s16");
#if PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_PTR(&sg_ParamTable[6], paramInfo);
#else
    TEST_ASSERT_EQUAL_PTR(&sg_ParamTable[5], paramInfo);
#endif
    

    paramInfo = Param_FindParamByID(&sg_tParamManager, 3);
    TEST_ASSERT_EQUAL_PTR(&sg_ParamTable[2], paramInfo);

    paramInfo = Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16);
#if PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_PTR(&sg_ParamTable[10], paramInfo);
#else
    TEST_ASSERT_EQUAL_PTR(&sg_ParamTable[9], paramInfo);
#endif

    paramInfo = Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u32);
    TEST_ASSERT_EQUAL_PTR(NULL, paramInfo);
}

void test_CheckRange(void)
{
    g_test_u16 = 60;
    TEST_ASSERT_EQUAL_INT(1, Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), PARAM_NONE));
    g_test_u16 = 120;
    TEST_ASSERT_EQUAL_INT(0, Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), PARAM_NONE));
    g_test_u16 = 4000;
    TEST_ASSERT_EQUAL_INT(2, Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), PARAM_NONE));

    g_test_u16 = 60;
    TEST_ASSERT_EQUAL_INT(1, Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), PARAM_DEF));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));
    g_test_u16 = 4000;
    TEST_ASSERT_EQUAL_INT(2, Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), PARAM_DEF));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));

    g_test_u16 = 60;
    TEST_ASSERT_EQUAL_INT(1, Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));
    g_test_u16 = 4000;
    TEST_ASSERT_EQUAL_INT(2, Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_UINT(3000, PARAM_DAT_CUR_VALUE(g_test_u16));

#if PARAM_USE_STRING_TYPE
    snprintf(g_test_str, sizeof(g_test_str), "ABCDEF");
    TEST_ASSERT_EQUAL_INT(0, Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, g_test_str), PARAM_NONE));
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);
    snprintf(g_test_str, sizeof(g_test_str), "AB");
    TEST_ASSERT_EQUAL_INT(1, Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, g_test_str), PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_STRING("AB", g_test_str);
    snprintf(g_test_str, sizeof(g_test_str), "abcdefg123456");
    TEST_ASSERT_EQUAL_INT(0, Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, g_test_str), PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_STRING("abcdefg12", g_test_str);
    snprintf(g_test_str, sizeof(g_test_str), "AB");
    TEST_ASSERT_EQUAL_INT(1, Param_CheckRange(Param_FindParamByParamPtr(&sg_tParamManager, g_test_str), PARAM_DEF));
    TEST_ASSERT_EQUAL_STRING("abcdef", g_test_str);
#endif
}

void test_CheckRangeMacroDefine(void)
{
    g_test_u16 = 60;
    TEST_ASSERT_EQUAL_INT(1, PARAM_DAT_CHECK_RANGE(g_test_u16, PARAM_NONE));
    g_test_u16 = 120;
    TEST_ASSERT_EQUAL_INT(0, PARAM_DAT_CHECK_RANGE(g_test_u16, PARAM_NONE));
    g_test_u16 = 4000;
    TEST_ASSERT_EQUAL_INT(2, PARAM_DAT_CHECK_RANGE(g_test_u16, PARAM_NONE));

    g_test_u16 = 60;
    TEST_ASSERT_EQUAL_INT(1, PARAM_DAT_CHECK_RANGE(g_test_u16, PARAM_DEF));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));
    g_test_u16 = 4000;
    TEST_ASSERT_EQUAL_INT(2, PARAM_DAT_CHECK_RANGE(g_test_u16, PARAM_DEF));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));

    g_test_u16 = 60;
    TEST_ASSERT_EQUAL_INT(1, PARAM_DAT_CHECK_RANGE(g_test_u16, PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));
    g_test_u16 = 4000;
    TEST_ASSERT_EQUAL_INT(2, PARAM_DAT_CHECK_RANGE(g_test_u16, PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_UINT(3000, PARAM_DAT_CUR_VALUE(g_test_u16));

#if PARAM_USE_STRING_TYPE
    snprintf(g_test_str, sizeof(g_test_str), "ABCDEF");
    TEST_ASSERT_EQUAL_INT(0, PARAM_STR_CHECK_RANGE(g_test_str, PARAM_NONE));
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);
    snprintf(g_test_str, sizeof(g_test_str), "AB");
    TEST_ASSERT_EQUAL_INT(1, PARAM_STR_CHECK_RANGE(g_test_str, PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_STRING("AB", g_test_str);
    snprintf(g_test_str, sizeof(g_test_str), "abcdefg123456");
    TEST_ASSERT_EQUAL_INT(0, PARAM_STR_CHECK_RANGE(g_test_str, PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_STRING("abcdefg12", g_test_str);
    snprintf(g_test_str, sizeof(g_test_str), "AB");
    TEST_ASSERT_EQUAL_INT(1, PARAM_STR_CHECK_RANGE(g_test_str, PARAM_DEF));
    TEST_ASSERT_EQUAL_STRING("abcdef", g_test_str);
#endif
}

void test_SetNewValue(void)
{
    PARAM_UINT16_T tmp = 60;
    tmp = 60;
    TEST_ASSERT_EQUAL_INT(1, Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, PARAM_NONE));
    tmp = 120;
    TEST_ASSERT_EQUAL_INT(0, Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, PARAM_NONE));
    tmp = 4000;
    TEST_ASSERT_EQUAL_INT(2, Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, PARAM_NONE));

    tmp = 60;
    TEST_ASSERT_EQUAL_INT(1, Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, PARAM_DEF));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));
    tmp = 4000;
    TEST_ASSERT_EQUAL_INT(2, Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, PARAM_DEF));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));

    tmp = 60;
    TEST_ASSERT_EQUAL_INT(1, Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));
    tmp = 4000;
    TEST_ASSERT_EQUAL_INT(2, Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_UINT(3000, PARAM_DAT_CUR_VALUE(g_test_u16));

#if PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_INT(0, Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, g_test_str), "ABCDEF", PARAM_NONE));
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);
    TEST_ASSERT_EQUAL_INT(1, Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, g_test_str), "AB", PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);
    TEST_ASSERT_EQUAL_INT(2, Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, g_test_str), "abcdefg123456", PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);
    TEST_ASSERT_EQUAL_INT(2, Param_SetNewValue(Param_FindParamByParamPtr(&sg_tParamManager, g_test_str), "abcdefg123456", PARAM_DEF));
    TEST_ASSERT_EQUAL_STRING("abcdef", g_test_str);
#endif
}

void test_SetNewValueMacroDefine(void)
{
    PARAM_UINT16_T tmp = 60;
    tmp = 60;
    TEST_ASSERT_EQUAL_INT(1, PARAM_DAT_SET_NEW_VALUE(g_test_u16, tmp, PARAM_NONE));
    tmp = 120;
    TEST_ASSERT_EQUAL_INT(0, PARAM_DAT_SET_NEW_VALUE(g_test_u16, tmp, PARAM_NONE));
    tmp = 4000;
    TEST_ASSERT_EQUAL_INT(2, PARAM_DAT_SET_NEW_VALUE(g_test_u16, tmp, PARAM_NONE));

    tmp = 60;
    TEST_ASSERT_EQUAL_INT(1, PARAM_DAT_SET_NEW_VALUE(g_test_u16, tmp, PARAM_DEF));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));
    tmp = 4000;
    TEST_ASSERT_EQUAL_INT(2, PARAM_DAT_SET_NEW_VALUE(g_test_u16, tmp, PARAM_DEF));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));

    tmp = 60;
    TEST_ASSERT_EQUAL_INT(1, PARAM_DAT_SET_NEW_VALUE(g_test_u16, tmp, PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_UINT(100, PARAM_DAT_CUR_VALUE(g_test_u16));
    tmp = 4000;
    TEST_ASSERT_EQUAL_INT(2, PARAM_DAT_SET_NEW_VALUE(g_test_u16, tmp, PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_UINT(3000, PARAM_DAT_CUR_VALUE(g_test_u16));

#if PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_INT(0, PARAM_STR_SET_NEW_VALUE(g_test_str, "ABCDEF", PARAM_NONE));
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);
    TEST_ASSERT_EQUAL_INT(1, PARAM_STR_SET_NEW_VALUE(g_test_str, "AB", PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);
    TEST_ASSERT_EQUAL_INT(2, PARAM_STR_SET_NEW_VALUE(g_test_str, "abcdefg123456", PARAM_MIN_MAX));
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);
    TEST_ASSERT_EQUAL_INT(2, PARAM_STR_SET_NEW_VALUE(g_test_str, "abcdefg123456", PARAM_DEF));
    TEST_ASSERT_EQUAL_STRING("abcdef", g_test_str);
#endif
}

void test_ResetValue(void)
{
    g_test_u16 = 500;

    TEST_ASSERT_TRUE(Param_ResetDefaultValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16)));
    TEST_ASSERT_EQUAL_UINT(100, g_test_u16);

    TEST_ASSERT_TRUE(Param_ResetMinValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16)));
    TEST_ASSERT_EQUAL_UINT(100, g_test_u16);

    TEST_ASSERT_TRUE(Param_ResetMaxValue(Param_FindParamByParamPtr(&sg_tParamManager, &g_test_u16)));
    TEST_ASSERT_EQUAL_UINT(3000, g_test_u16);
}

// Run the test suite
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ParamInit);
    RUN_TEST(test_ResetParam);
    RUN_TEST(test_SaveAndLoadParam);
    RUN_TEST(test_IterateParam);
    RUN_TEST(test_FindParam);
    RUN_TEST(test_CheckRange);
    RUN_TEST(test_CheckRangeMacroDefine);
    RUN_TEST(test_SetNewValue);
    RUN_TEST(test_SetNewValueMacroDefine);
    RUN_TEST(test_ResetValue);

    UNITY_END();

    return 0;
}