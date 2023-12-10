#include <stdio.h>
#include <string.h>
#include <float.h>
#include "unity.h"
#include "cot_param.h"


#if COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_16
const uint8_t kBuf[200] = {0x01, 0x12, 0x0a, 0x00, 0x22, 0x14, 0x00, 0x34, 0x9a, 0x99, 0x49, 
    0x40, 0x48, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x09, 0x40, 0x5d, 0x61, 0x62, 0x63, 0x64, 0x65, 
    0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x0a, 0x72, 0x64, 0x00, 0x84, 0xe8, 0x03, 
    0x00, 0x00, 0x98, 0x40, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa1, 0x0a, 0xb2, 0x64, 0x00, 0xc4, 
    0xe8, 0x03, 0x00, 0x00, 0xd8, 0x40, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe2, 0x00, 0x00, 0xf4, 
    0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_256
const uint8_t kBuf[200] = {0x02, 0x02, 0x01, 0x0a, 0x00, 0x02, 0x02, 0x14, 0x00, 0x04, 0x03, 0x9a, 0x99,
    0x49, 0x40, 0x08, 0x04, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x09, 0x40, 0x0d, 0x05, 0x61, 0x62, 0x63, 0x64, 
    0x65, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06, 0x0a, 0x02, 0x07, 0x64, 0x00, 0x04, 0x08, 
    0xe8, 0x03, 0x00, 0x00, 0x08, 0x09, 0x40, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0a, 0x0a, 0x02, 
    0x0b, 0x64, 0x00, 0x04, 0x0c, 0xe8, 0x03, 0x00, 0x00, 0x08, 0x0d, 0x40, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x02, 0x0e, 0x00, 0x00, 0x04, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x14, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_4096
const uint8_t kBuf[200] = {0x03, 0x02, 0x10, 0x00, 0x0a, 0x00, 0x02, 0x20, 0x00, 0x14, 0x00, 0x04, 0x30, 0x00, 
    0x9a, 0x99, 0x49, 0x40, 0x08, 0x40, 0x00, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x09, 0x40, 0x0d, 0x50, 0x00, 0x61, 
    0x62, 0x63, 0x64, 0x65, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x60, 0x00, 0x0a, 0x02, 0x70, 0x00, 
    0x64, 0x00, 0x04, 0x80, 0x00, 0xe8, 0x03, 0x00, 0x00, 0x08, 0x90, 0x00, 0x40, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0xa0, 0x00, 0x0a, 0x02, 0xb0, 0x00, 0x64, 0x00, 0x04, 0xc0, 0x00, 0xe8, 0x03, 0x00, 0x00, 0x08, 0xd0, 
    0x00, 0x40, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xe0, 0x00, 0x00, 0x00, 0x04, 0xf0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif

typedef struct
{
    uint16_t test1;
    float test2;
    char str[20];
}ParamDemo_t;

static ParamDemo_t sg_tTest;

COT_PARAM_INT16_T g_test_1 = 10;
COT_PARAM_UINT16_T g_test_2 = 20;
COT_PARAM_FLOAT_T g_test_float = 3.15;
#if COT_PARAM_USE_64_BIT_LENGTH
COT_PARAM_DOUBLE_T g_test_double = 3.15;
#endif
#if COT_PARAM_USE_STRING_TYPE
char g_test_str[13] = "abcdef";
#endif
COT_PARAM_INT8_T g_test_s8 = 10;
COT_PARAM_INT16_T g_test_s16 = 100;
COT_PARAM_INT32_T g_test_s32 = 1000;

#if COT_PARAM_USE_64_BIT_LENGTH
COT_PARAM_INT64_T g_test_s64 = 8000;
#endif
COT_PARAM_UINT8_T g_test_u8 = 10;
COT_PARAM_UINT16_T g_test_u16 = 100;
COT_PARAM_UINT32_T g_test_u32 = 1000;
#if COT_PARAM_USE_64_BIT_LENGTH
COT_PARAM_UINT64_T g_test_u64 = 8000;
#endif

static int CheckTestS16(const void *pCurParam)
{
    const int16_t *p_test_s16 = (const int16_t *)pCurParam;

    if ((*p_test_s16) % 2 != 0)
    {
        return -1;
    }

    return 0;
}

cotParamInfo_t sg_ParamTable[] = {
    COT_PARAM_ITEM_BIND(1, g_test_1, COT_PARAM_INT16, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND(2, g_test_2, COT_PARAM_UINT16, COT_PARAM_ATTR_WR, 20),
    COT_PARAM_ITEM_BIND(3, g_test_float, COT_PARAM_FLOAT, COT_PARAM_ATTR_READ, 3.15, -10, 10),
#if COT_PARAM_USE_64_BIT_LENGTH
    COT_PARAM_ITEM_BIND(4, g_test_double, COT_PARAM_DOUBLE, COT_PARAM_ATTR_WR, 3.15, -10, 10),
#endif
#if COT_PARAM_USE_STRING_TYPE
    COT_PARAM_ITEM_BIND(5, g_test_str, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "abcdef", 5, 10),
#endif
    COT_PARAM_ITEM_BIND(6, g_test_s8, COT_PARAM_INT8, COT_PARAM_ATTR_WR, 10, -10, 15),
    COT_PARAM_ITEM_BIND(7, g_test_s16, COT_PARAM_INT16, COT_PARAM_ATTR_WR, 100, -100, 3000, CheckTestS16),
    COT_PARAM_ITEM_BIND(8, g_test_s32, COT_PARAM_INT32, COT_PARAM_ATTR_WR, 1000, -900, 10000),
#if COT_PARAM_USE_64_BIT_LENGTH
    COT_PARAM_ITEM_BIND(9, g_test_s64, COT_PARAM_INT64, COT_PARAM_ATTR_WR, 8000, -100, 1000000),
#endif

    COT_PARAM_ITEM_BIND(10, g_test_u8, COT_PARAM_UINT8, COT_PARAM_ATTR_WR, 10, 5, 15),
    COT_PARAM_ITEM_BIND(11, g_test_u16, COT_PARAM_UINT16, COT_PARAM_ATTR_WR, 100, 90, 3000),
    COT_PARAM_ITEM_BIND(12, g_test_u32, COT_PARAM_UINT32, COT_PARAM_ATTR_NONE, 1000, 900, 10000),

#if COT_PARAM_USE_64_BIT_LENGTH
    COT_PARAM_ITEM_BIND(13, g_test_u64, COT_PARAM_UINT64, COT_PARAM_ATTR_WR, 8000, 100, 1000000),
#endif
    COT_PARAM_ITEM_BIND(14, sg_tTest.test1, COT_PARAM_UINT16, COT_PARAM_ATTR_WR, 20, 10, 2000),
    COT_PARAM_ITEM_BIND(15, sg_tTest.test2, COT_PARAM_FLOAT, COT_PARAM_ATTR_WR),
#if COT_PARAM_USE_STRING_TYPE
    COT_PARAM_ITEM_BIND(16, sg_tTest.str, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "const-zpc", 6, sizeof(sg_tTest.str)),
#endif
};

static cotParamManager_t sg_tParamManager;

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
    cotParam_Init(&sg_tParamManager, sg_ParamTable, COT_PARAM_TABLE_SIZE(sg_ParamTable));

    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable, sg_ParamTable);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.count, COT_PARAM_TABLE_SIZE(sg_ParamTable));

    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[0].id, 1);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[0].length, 2);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[0].type, COT_PARAM_INT16);
    TEST_ASSERT_EQUAL_HEX8(sg_tParamManager.pParamTable[0].attr, COT_PARAM_ATTR_WR);
    TEST_ASSERT_EQUAL_STRING(sg_tParamManager.pParamTable[0].pszName, "g_test_1");
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[0].unCurValuePtr.pVoid , &g_test_1);
    TEST_ASSERT_NULL(sg_tParamManager.pParamTable[0].unDefValuePtr.pVoid);
    TEST_ASSERT_NULL(sg_tParamManager.pParamTable[0].unMinValuePtr.pVoid);
    TEST_ASSERT_NULL(sg_tParamManager.pParamTable[0].unMaxValuePtr.pVoid);

    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[1].id, 2);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[1].length, 2);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[1].type, COT_PARAM_UINT16);
    TEST_ASSERT_EQUAL_HEX8(sg_tParamManager.pParamTable[1].attr, COT_PARAM_ATTR_WR | COT_PARAM_ATTR_RESET);
    TEST_ASSERT_EQUAL_STRING(sg_tParamManager.pParamTable[1].pszName, "g_test_2");
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[1].unCurValuePtr.pVoid , &g_test_2);
    TEST_ASSERT_NOT_NULL(sg_tParamManager.pParamTable[1].unDefValuePtr.pVoid);
    TEST_ASSERT_NULL(sg_tParamManager.pParamTable[1].unMinValuePtr.pVoid);
    TEST_ASSERT_NULL(sg_tParamManager.pParamTable[1].unMaxValuePtr.pVoid);

    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[2].id, 3);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[2].length, 4);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[2].type, COT_PARAM_FLOAT);
    TEST_ASSERT_EQUAL_HEX8(sg_tParamManager.pParamTable[2].attr, COT_PARAM_ATTR_READ | COT_PARAM_ATTR_RESET | COT_PARAM_ATTR_RANGE);
    TEST_ASSERT_EQUAL_STRING(sg_tParamManager.pParamTable[2].pszName, "g_test_float");
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[2].unCurValuePtr.pVoid , &g_test_float);
    TEST_ASSERT_NOT_NULL(sg_tParamManager.pParamTable[2].unDefValuePtr.pVoid);
    TEST_ASSERT_NOT_NULL(sg_tParamManager.pParamTable[2].unMinValuePtr.pVoid);
    TEST_ASSERT_NOT_NULL(sg_tParamManager.pParamTable[2].unMaxValuePtr.pVoid);

#if COT_PARAM_USE_STRING_TYPE
#if COT_PARAM_USE_64_BIT_LENGTH
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[4].id, 5);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[4].length, sizeof(g_test_str));
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[4].type, COT_PARAM_STRING);
    TEST_ASSERT_EQUAL_HEX8(sg_tParamManager.pParamTable[4].attr, COT_PARAM_ATTR_WR | COT_PARAM_ATTR_RESET | COT_PARAM_ATTR_RANGE);
    TEST_ASSERT_EQUAL_STRING(sg_tParamManager.pParamTable[4].pszName, "g_test_str");
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[4].unCurValuePtr.pVoid , g_test_str);
    TEST_ASSERT_NOT_NULL(sg_tParamManager.pParamTable[4].unDefValuePtr.pVoid);
    TEST_ASSERT_NOT_NULL(sg_tParamManager.pParamTable[4].unMinValuePtr.pVoid);
    TEST_ASSERT_NOT_NULL(sg_tParamManager.pParamTable[4].unMaxValuePtr.pVoid);
#else
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[3].id, 5);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[3].length, 10);
    TEST_ASSERT_EQUAL_UINT(sg_tParamManager.pParamTable[3].type, COT_PARAM_STRING);
    TEST_ASSERT_EQUAL_HEX8(sg_tParamManager.pParamTable[3].attr, COT_PARAM_ATTR_WR | COT_PARAM_ATTR_RESET | COT_PARAM_ATTR_RANGE);
    TEST_ASSERT_EQUAL_STRING(sg_tParamManager.pParamTable[3].pszName, "g_test_str");
    TEST_ASSERT_EQUAL_PTR(sg_tParamManager.pParamTable[3].unCurValuePtr.pVoid , COT_PARAM_STR_CUR_VALUE(g_test_str));
    TEST_ASSERT_NOT_NULL(sg_tParamManager.pParamTable[3].unDefValuePtr.pVoid);
    TEST_ASSERT_NOT_NULL(sg_tParamManager.pParamTable[3].unMinValuePtr.pVoid);
    TEST_ASSERT_NOT_NULL(sg_tParamManager.pParamTable[3].unMaxValuePtr.pVoid);
#endif

#endif
}

// #define HEX_PRINTF(str, hex, len)  \
//     printf("%s: [%d] -> ", str, len);\
//     for (int i = 0; i < (len); i++){printf("0x%02x, ", hex[i]);}\
//     printf("\n");

void test_ParamSerializeSize(void)
{
    uint32_t length;
    uint8_t buf[200];
#if COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_16
    TEST_ASSERT_EQUAL_UINT(102, cotParam_GetSerializeSize(&sg_tParamManager));
    length = cotParam_Serialize(&sg_tParamManager, buf);
    // HEX_PRINTF("", buf, length);
    TEST_ASSERT_EQUAL_UINT(102, length);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(kBuf, buf, length);
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_256
    TEST_ASSERT_EQUAL_UINT(118, cotParam_GetSerializeSize(&sg_tParamManager));
    length = cotParam_Serialize(&sg_tParamManager, buf);
    // HEX_PRINTF("", buf, length);
    TEST_ASSERT_EQUAL_UINT(118, length);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(kBuf, buf, length);
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_4096
    TEST_ASSERT_EQUAL_UINT(134, cotParam_GetSerializeSize(&sg_tParamManager));
    length = cotParam_Serialize(&sg_tParamManager, buf);
    // HEX_PRINTF("", buf, length);
    TEST_ASSERT_EQUAL_UINT(134, length);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(kBuf, buf, length);
#endif
}

void test_ResetParam(void)
{
    g_test_1 = 80;
    g_test_2 = 50;
    g_test_float = 1;
#if COT_PARAM_USE_STRING_TYPE
    sprintf(g_test_str, "ABCDFS");
#endif
    cotParam_ResetDefault(&sg_tParamManager);

    TEST_ASSERT_EQUAL_INT(80, g_test_1);
    TEST_ASSERT_EQUAL_UINT(20, g_test_2);
    TEST_ASSERT_EQUAL_FLOAT(3.15, g_test_float);
#if COT_PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_STRING("abcdef", g_test_str);
#endif
}



// 储存空间定义
static uint8_t sg_buf[500];
static uint32_t sg_length = 0;

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
        s_offset += len;
        sg_length = s_offset;
    }
    else
    {
        s_offset = 0;
    }

    return 0;
}

int OnCheckErrorResetHandle(const cotParamInfo_t *pParamInfo, cotParamCheckRet_e eCheckResult)
{
    cotParam_SingleParamResetDefValue(pParamInfo);
    return 0;
}

void test_SaveAndLoadParam(void)
{
    cotParam_Save(&sg_tParamManager, OnSaveCallback);
#if COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_16
    TEST_ASSERT_EQUAL_UINT(102, sg_length);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(kBuf, sg_buf, sg_length);
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_256
    TEST_ASSERT_EQUAL_UINT(118, sg_length);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(kBuf, sg_buf, sg_length);
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_4096
    TEST_ASSERT_EQUAL_UINT(134, sg_length);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(kBuf, sg_buf, sg_length);
#endif

    g_test_1 = 80;
    g_test_2 = 50;
    g_test_float = -20;
#if COT_PARAM_USE_STRING_TYPE
    sprintf(g_test_str, "12a");
#endif
    cotParam_Save(&sg_tParamManager, OnSaveCallback);
    TEST_ASSERT_EQUAL_INT(g_test_1, 80);
    TEST_ASSERT_EQUAL_UINT(g_test_2, 50);
    TEST_ASSERT_EQUAL_FLOAT(g_test_float, -20);
#if COT_PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_STRING(g_test_str, "12a");
#endif
    cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
    cotParam_Save(&sg_tParamManager, OnSaveCallback);

    TEST_ASSERT_EQUAL_INT(80, g_test_1);
    TEST_ASSERT_EQUAL_UINT(50, g_test_2);
    TEST_ASSERT_EQUAL_FLOAT(3.15, g_test_float);
#if COT_PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_STRING("abcdef", g_test_str);
#endif
    g_test_1 = 60;
    g_test_2 = 40;
    g_test_float = -20;
#if COT_PARAM_USE_64_BIT_LENGTH
    g_test_double = 5.36;
#endif
#if COT_PARAM_USE_STRING_TYPE
    sprintf(g_test_str, "12a123");
#endif
    g_test_s8 = 20;
    g_test_s16 = 1000;

    cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
    cotParam_Load(&sg_tParamManager, OnLoadCallback);

    TEST_ASSERT_EQUAL_INT(80, g_test_1);
    TEST_ASSERT_EQUAL_UINT(50, g_test_2);
    TEST_ASSERT_EQUAL_FLOAT(3.15, g_test_float);
#if COT_PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_STRING("abcdef", g_test_str);
#endif
    TEST_ASSERT_EQUAL_INT(10, g_test_s8);
    TEST_ASSERT_EQUAL_INT(100, g_test_s16);
    TEST_ASSERT_EQUAL_INT(100, g_test_u16);
    TEST_ASSERT_EQUAL_INT(1000, g_test_u32);
}

void test_IterateParam(void)
{
    size_t idx = 0;
    cotParamInfo_t *paramInfo;
    size_t cnt = 0;

    do
    {
        paramInfo = cotParam_IterateList(&sg_tParamManager, &idx);

        if (paramInfo != NULL)
        {
            cnt++;
        }
        
    } while (paramInfo != NULL);

    TEST_ASSERT_EQUAL_UINT((COT_PARAM_TABLE_SIZE(sg_ParamTable) - 1), cnt);
}


void test_FindParam(void)
{
    const cotParamInfo_t *paramInfo;
#if COT_PARAM_USE_64_BIT_LENGTH
    paramInfo = cotParam_FindParamByName(&sg_tParamManager, "g_test_double");
    TEST_ASSERT_EQUAL_PTR(&sg_ParamTable[3], paramInfo);
#endif
    paramInfo = cotParam_FindParamByID(&sg_tParamManager, 3);
    TEST_ASSERT_EQUAL_PTR(&sg_ParamTable[2], paramInfo);

    paramInfo = cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_float);
    TEST_ASSERT_EQUAL_PTR(&sg_ParamTable[2], paramInfo);

    paramInfo = cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u32);
    TEST_ASSERT_EQUAL_PTR(NULL, paramInfo);
}

void test_CheckRange(void)
{
    cotParamCheckRet_e eCheckResult;

    g_test_u16 = 60;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &eCheckResult);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MIN, eCheckResult);
    g_test_u16 = 120;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &eCheckResult);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OK, eCheckResult);
    g_test_u16 = 4000;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &eCheckResult);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MAX, eCheckResult);

    g_test_u16 = 60;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), COT_PARAM_RESET_DEF);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MIN, eCheckResult);
    TEST_ASSERT_EQUAL_UINT(100, g_test_u16);
    g_test_u16 = 4000;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), COT_PARAM_RESET_DEF);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MAX, eCheckResult);
    TEST_ASSERT_EQUAL_UINT(100, g_test_u16);

    g_test_u16 = 60;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), COT_PARAM_RESET_MIN);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MIN, eCheckResult);
    TEST_ASSERT_EQUAL_UINT(90, g_test_u16);
    g_test_u16 = 4000;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), COT_PARAM_RESET_MIN);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MAX, eCheckResult);
    TEST_ASSERT_EQUAL_UINT(90, g_test_u16);

    g_test_u16 = 60;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), COT_PARAM_RESET_MAX);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MIN, eCheckResult);
    TEST_ASSERT_EQUAL_UINT(3000, g_test_u16);
    g_test_u16 = 4000;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), COT_PARAM_RESET_MAX);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MAX, eCheckResult);
    TEST_ASSERT_EQUAL_UINT(3000, g_test_u16);

    g_test_u16 = 60;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), COT_PARAM_RESET_MIN_MAX);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MIN, eCheckResult);
    TEST_ASSERT_EQUAL_UINT(90, g_test_u16);
    g_test_u16 = 4000;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), COT_PARAM_RESET_MIN_MAX);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MAX, eCheckResult);
    TEST_ASSERT_EQUAL_UINT(3000, g_test_u16);

#if COT_PARAM_USE_STRING_TYPE
    snprintf(g_test_str, sizeof(g_test_str), "ABCDEF");
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, g_test_str), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, g_test_str), COT_PARAM_RESET_NONE);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OK, eCheckResult);
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);

    snprintf(g_test_str, sizeof(g_test_str), "AB");
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, g_test_str), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, g_test_str), COT_PARAM_RESET_MIN_MAX);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MIN, eCheckResult);
    TEST_ASSERT_EQUAL_STRING("AB", g_test_str);

    snprintf(g_test_str, sizeof(g_test_str), "ABCDEF123456");
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, g_test_str), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, g_test_str), COT_PARAM_RESET_MIN_MAX);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MAX, eCheckResult);
    TEST_ASSERT_EQUAL_STRING("ABCDEF1234", g_test_str);

    snprintf(g_test_str, sizeof(g_test_str), "AB");
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, g_test_str), &eCheckResult);
    cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, g_test_str), COT_PARAM_RESET_DEF);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OVER_MIN, eCheckResult);
    TEST_ASSERT_EQUAL_STRING("abcdef", g_test_str);
#endif
}

void test_CheckCustomWay(void)
{
    cotParamCheckRet_e eCheckResult;

    g_test_s16 = 200;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_s16), &eCheckResult);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OK, eCheckResult);

#if COT_PARAM_USE_CUSTOM_CHECK
    g_test_s16 = 201;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_s16), &eCheckResult);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OTHER_ERR, eCheckResult);
#endif

    g_test_s16 = 202;
    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_s16), &eCheckResult);
    TEST_ASSERT_EQUAL_INT(COT_PARAM_CHECK_OK, eCheckResult);
}

void test_SetNewValue(void)
{
    COT_PARAM_UINT16_T tmp = 60;

    g_test_u16 = 200;
    tmp = 60;
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, COT_PARAM_RESET_NONE));
    TEST_ASSERT_EQUAL_UINT(200, g_test_u16);
    tmp = 120;
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, COT_PARAM_RESET_NONE));
    TEST_ASSERT_EQUAL_UINT(120, g_test_u16);
    tmp = 4000;
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, COT_PARAM_RESET_NONE));
    TEST_ASSERT_EQUAL_UINT(120, g_test_u16);

    tmp = 60;
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, COT_PARAM_RESET_DEF));
    TEST_ASSERT_EQUAL_UINT(100, g_test_u16);
    tmp = 4000;
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, COT_PARAM_RESET_DEF));
    TEST_ASSERT_EQUAL_UINT(100, g_test_u16);

    tmp = 60;
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, COT_PARAM_RESET_MIN));
    TEST_ASSERT_EQUAL_UINT(90, g_test_u16);
    tmp = 4000;
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, COT_PARAM_RESET_MIN));
    TEST_ASSERT_EQUAL_UINT(90, g_test_u16);

    tmp = 60;
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, COT_PARAM_RESET_MAX));
    TEST_ASSERT_EQUAL_UINT(3000, g_test_u16);
    tmp = 4000;
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, COT_PARAM_RESET_MAX));
    TEST_ASSERT_EQUAL_UINT(3000, g_test_u16);

    tmp = 60;
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, COT_PARAM_RESET_MIN_MAX));
    TEST_ASSERT_EQUAL_UINT(90, g_test_u16);
    tmp = 4000;
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16), &tmp, COT_PARAM_RESET_MIN_MAX));
    TEST_ASSERT_EQUAL_UINT(3000, g_test_u16);

#if COT_PARAM_USE_STRING_TYPE
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_str), "ABCDEF", COT_PARAM_RESET_NONE));
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_str), "AB", COT_PARAM_RESET_MIN_MAX));
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_str), "abcdefg123456", COT_PARAM_RESET_MIN_MAX));
    TEST_ASSERT_EQUAL_STRING("ABCDEF", g_test_str);
    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamUpdate(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_str), "abcdefg123456", COT_PARAM_RESET_DEF));
    TEST_ASSERT_EQUAL_STRING("abcdef", g_test_str);
#endif
}

void test_ResetValue(void)
{
    g_test_u16 = 500;

    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamResetDefValue(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16)));
    TEST_ASSERT_EQUAL_UINT(100, g_test_u16);

    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamResetMinValue(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16)));
    TEST_ASSERT_EQUAL_UINT(90, g_test_u16);

    TEST_ASSERT_EQUAL_INT(0, cotParam_SingleParamResetMaxValue(cotParam_FindParamByParamPtr(&sg_tParamManager, &g_test_u16)));
    TEST_ASSERT_EQUAL_UINT(3000, g_test_u16);
}

// Run the test suite
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ParamInit);
    RUN_TEST(test_ParamSerializeSize);
    RUN_TEST(test_SaveAndLoadParam);
    RUN_TEST(test_ResetParam);
    RUN_TEST(test_IterateParam);
    RUN_TEST(test_FindParam);
    RUN_TEST(test_CheckRange);
    RUN_TEST(test_CheckCustomWay);
    RUN_TEST(test_SetNewValue);
    RUN_TEST(test_ResetValue);

    UNITY_END();

    return 0;
}