
#include "param.h"
#include <stdio.h>

PARAM_DEFINE (test, PARAM_INT16, 10, -100, 100);
PARAM_DEFINE (test_2, PARAM_UINT16, 20, 0, 100);
PARAM_DEFINE (test_3, PARAM_DOUBLE, 3.15, -10, 10);
PARAM_DEFINE_STR (test_str, 10, "abcdef");
PARAM_DEFINE (test_4, PARAM_INT8, 8, -10, 10);
PARAM_DEFINE (test_5, PARAM_UINT32, 620, 500, 10000);
PARAM_DEFINE (test_6, PARAM_UINT8, 4, 5, 100);
PARAM_DEFINE (test_7, PARAM_INT64, 5, -542, 5450);

ParamInfo_t sg_ParamTable[] = {
    PARAM_REG(1, test, PARAM_INT16, PARAM_ATTR_ALL),
    PARAM_REG(2, test_2, PARAM_UINT16, PARAM_ATTR_READ),
    PARAM_REG(3, test_3, PARAM_DOUBLE, PARAM_ATTR_READ | PARAM_ATTR_WRITE),
    PARAM_STR_REG(4, test_str, PARAM_ATTR_READ | PARAM_ATTR_WRITE),
    PARAM_REG(5, test_4, PARAM_INT8, PARAM_ATTR_READ),
    PARAM_REG(6, test_5, PARAM_UINT32, PARAM_ATTR_READ),
    PARAM_REG(7, test_6, PARAM_UINT8, PARAM_ATTR_READ),
    PARAM_REG(8, test_7, PARAM_INT64, PARAM_ATTR_WRITE),
};

#define ATTR(x)   ((x & (PARAM_ATTR_READ | PARAM_ATTR_WRITE)) == (PARAM_ATTR_READ | PARAM_ATTR_WRITE) ? \
                    "wr" : (x & (PARAM_ATTR_READ) ? "r" : ((x & (PARAM_ATTR_WRITE) ? "w" : ""))))
#define VIEW(type, val) (type##_T)(*(type##_T *)val)

void ShowAllParam(const ParamInfo_t *paramTable, uint16_t num)
{
    printf("%4s    %-15s %-8s %-8s %-8s %-10s %-10s %-10s %-10s\n", "id", "name", "type", "length", "attr", "val", "def", "min", "max");
    
    for (int i = 0; i < num; i++)
    {
        switch (paramTable[i].type)
        {
        case PARAM_INT8:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10d %-10d %-10d %-10d\n", 
                paramTable[i].id, paramTable[i].pszName, "int8_t", paramTable[i].length, ATTR(paramTable[i].attr),
                VIEW(PARAM_INT8, paramTable[i].pCurValue), VIEW(PARAM_INT8, paramTable[i].pDefValue),
                VIEW(PARAM_INT8, paramTable[i].pMinValue), VIEW(PARAM_INT8, paramTable[i].pMaxValue));
            break;
        case PARAM_INT16:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10d %-10d %-10d %-10d\n", 
                paramTable[i].id, paramTable[i].pszName, "int16_t", paramTable[i].length, ATTR(paramTable[i].attr),
                VIEW(PARAM_INT16, paramTable[i].pCurValue), VIEW(PARAM_INT16, paramTable[i].pDefValue),
                VIEW(PARAM_INT16, paramTable[i].pMinValue), VIEW(PARAM_INT16, paramTable[i].pMaxValue));
            break;
        case PARAM_INT32:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10d %-10d %-10d %-10d\n", 
                paramTable[i].id, paramTable[i].pszName, "int32_t", paramTable[i].length, ATTR(paramTable[i].attr),
                VIEW(PARAM_INT32, paramTable[i].pCurValue), VIEW(PARAM_INT32, paramTable[i].pDefValue),
                VIEW(PARAM_INT32, paramTable[i].pMinValue), VIEW(PARAM_INT32, paramTable[i].pMaxValue));
            break;
        case PARAM_INT64:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10ld %-10ld %-10ld %-10ld\n", 
                paramTable[i].id, paramTable[i].pszName, "int64_t", paramTable[i].length, ATTR(paramTable[i].attr),
                VIEW(PARAM_INT64, paramTable[i].pCurValue), VIEW(PARAM_INT64, paramTable[i].pDefValue),
                VIEW(PARAM_INT64, paramTable[i].pMinValue), VIEW(PARAM_INT64, paramTable[i].pMaxValue));
            break;
        case PARAM_UINT8:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10u %-10u %-10u %-10u\n", 
                paramTable[i].id, paramTable[i].pszName, "uint8_t", paramTable[i].length, ATTR(paramTable[i].attr),
                VIEW(PARAM_UINT8, paramTable[i].pCurValue), VIEW(PARAM_UINT8, paramTable[i].pDefValue),
                VIEW(PARAM_UINT8, paramTable[i].pMinValue), VIEW(PARAM_UINT8, paramTable[i].pMaxValue));
            break;
        case PARAM_UINT16:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10u %-10u %-10u %-10u\n", 
                paramTable[i].id, paramTable[i].pszName, "uint16_t", paramTable[i].length, ATTR(paramTable[i].attr),
                VIEW(PARAM_UINT16, paramTable[i].pCurValue), VIEW(PARAM_UINT16, paramTable[i].pDefValue),
                VIEW(PARAM_UINT16, paramTable[i].pMinValue), VIEW(PARAM_UINT16, paramTable[i].pMaxValue));
            break;
        case PARAM_UINT32:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10u %-10u %-10u %-10u\n", 
                paramTable[i].id, paramTable[i].pszName, "uint32_t", paramTable[i].length, ATTR(paramTable[i].attr),
                VIEW(PARAM_UINT32, paramTable[i].pCurValue), VIEW(PARAM_UINT32, paramTable[i].pDefValue),
                VIEW(PARAM_UINT32, paramTable[i].pMinValue), VIEW(PARAM_UINT32, paramTable[i].pMaxValue));
            break;
        case PARAM_UINT64:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10lu %-10lu %-10lu %-10lu\n", 
                paramTable[i].id, paramTable[i].pszName, "uint64_t", paramTable[i].length, ATTR(paramTable[i].attr),
                VIEW(PARAM_UINT64, paramTable[i].pCurValue), VIEW(PARAM_UINT64, paramTable[i].pDefValue),
                VIEW(PARAM_UINT64, paramTable[i].pMinValue), VIEW(PARAM_UINT64, paramTable[i].pMaxValue));
            break;
        case PARAM_FLOAT:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10f %-10f %-10f %-10f\n", 
                paramTable[i].id, paramTable[i].pszName, "float", paramTable[i].length, ATTR(paramTable[i].attr),
                VIEW(PARAM_FLOAT, paramTable[i].pCurValue), VIEW(PARAM_FLOAT, paramTable[i].pDefValue),
                VIEW(PARAM_FLOAT, paramTable[i].pMinValue), VIEW(PARAM_FLOAT, paramTable[i].pMaxValue));
            break;
        case PARAM_DOUBLE:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10f %-10f %-10f %-10f\n", 
                paramTable[i].id, paramTable[i].pszName, "double", paramTable[i].length, ATTR(paramTable[i].attr),
                VIEW(PARAM_DOUBLE, paramTable[i].pCurValue), VIEW(PARAM_DOUBLE, paramTable[i].pDefValue),
                VIEW(PARAM_DOUBLE, paramTable[i].pMinValue), VIEW(PARAM_DOUBLE, paramTable[i].pMaxValue));
            break;
        case PARAM_STARING:
            printf(" %-4d   %-15s %-10s %-6d %-8s %-10s %-10s\n", 
                paramTable[i].id, paramTable[i].pszName, "string", paramTable[i].length, ATTR(paramTable[i].attr),
                (char *)paramTable[i].pCurValue, (char *)paramTable[i].pDefValue);
            break;
        
        default:
            break;
        }
    }
}

#define HEX_PRINTF(str, hex, len)  \
    printf("%s: [%d] -> ", str, len);\
    for (int i = 0; i < (len); i++){printf("%02x ", hex[i]);}\
    printf("\n");

uint8_t sg_buf[500];

int main()
{
    size_t length;

    printf("\nInit:\n");
    ShowAllParam(sg_ParamTable, sizeof(sg_ParamTable) / sizeof(ParamInfo_t));

    /* 展示多种序列化方式效果 */

    length = Param_Serialize(sg_buf, sg_ParamTable, sizeof(sg_ParamTable) / sizeof(ParamInfo_t), 0, 0); // 普通序列化
    HEX_PRINTF("\n1- Serialize", sg_buf, length);
    
    length = Param_Serialize(sg_buf, sg_ParamTable, sizeof(sg_ParamTable) / sizeof(ParamInfo_t), 0, 1); // 编码序列化
    HEX_PRINTF("2- Serialize", sg_buf, length);
    
    length = Param_Serialize(sg_buf, sg_ParamTable, sizeof(sg_ParamTable) / sizeof(ParamInfo_t), 1, 0); // 增加键值序列化
    HEX_PRINTF("3- Serialize", sg_buf, length);
    
    length = Param_Serialize(sg_buf, sg_ParamTable, sizeof(sg_ParamTable) / sizeof(ParamInfo_t), 1, 1); // 增加键值并编码序列化
    HEX_PRINTF("4- Serialize", sg_buf, length);
    
    /* 直接使用该值修改后显示 */
    test = -50;
    test_2 = 10;
    test_3 = 8.265;
    sprintf(test_str, "%s", "ertb");

    printf("\nchange:\n");
    ShowAllParam(sg_ParamTable, sizeof(sg_ParamTable) / sizeof(ParamInfo_t));
    
    
    /* 对修改之前的序列化后的数据反序列化（需要使用序列化前定义的参数表信息），得到的是修改前的数据 */
    Param_Parse(sg_buf, length, sg_ParamTable, sizeof(sg_ParamTable) / sizeof(ParamInfo_t), 1, 1);
    printf("\nParse:\n");
    ShowAllParam(sg_ParamTable, sizeof(sg_ParamTable) / sizeof(ParamInfo_t));
    
    /* 使用完整信息接口序列化数据（序列化选项拉满，可修改看效果） */
    length = Param_MultiSerialize(sg_buf, sg_ParamTable, sizeof(sg_ParamTable) / sizeof(ParamInfo_t), PARAM_PACK_ALL); // PARAM_PACK_ATTR | PARAM_PACK_NAME
    HEX_PRINTF("\nMultiSerialize", sg_buf, length);


    // 模拟跨设备使用该代码解析，定义一个空的数据参数表
    ParamInfo_t tParamTable[10]; 

    uint16_t num = Param_MultiParse(sg_buf, length, &tParamTable[0], 10); // 将完整信息接口序列化数据反序列化出来
    
    printf("\nMultiParse:\n");
    ShowAllParam(tParamTable, num);  // 展示信息
    
    return 0;
}
