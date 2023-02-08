#include "param.h"
#include <string.h>
#include <stdio.h>

PARAM_EXTERN(test, PARAM_INT16);
PARAM_EXTERN(test_2, PARAM_UINT16);
PARAM_EXTERN(test_3, PARAM_DOUBLE);
PARAM_STR_EXTERN(test_str, 10);

const ParamInfo_t sg_tParamList[]=
{
    PARAM_REG(1, test, PARAM_INT16),
    PARAM_REG(2, test_2, PARAM_UINT16),
    PARAM_REG(3, test_3, PARAM_DOUBLE),
    PARAM_STR_REG(4, test_str),
};

static ParamTable_t sg_tParamTable = {sg_tParamList, sizeof(sg_tParamList) / sizeof(sg_tParamList[0])};


uint8_t sg_saveBuf[200];

void ShowAllParam(void)
{
    for (int i = 0; i < sg_tParamTable.num; i++)
    {
        switch (sg_tParamTable.pTab[i].type)
        {
        case PARAM_INT8:
            printf ("%-10s : %d\n", sg_tParamTable.pTab[i].pszName, (PARAM_INT8_T)(*(PARAM_INT8_T *)sg_tParamTable.pTab[i].pCurValue));
            break;
        case PARAM_INT16:
            printf ("%-10s : %d\n", sg_tParamTable.pTab[i].pszName, (PARAM_INT16_T)(*(PARAM_INT16_T *)sg_tParamTable.pTab[i].pCurValue));
            break;
        case PARAM_INT32:
            printf ("%-10s : %d\n", sg_tParamTable.pTab[i].pszName, (PARAM_INT32_T)(*(PARAM_INT32_T *)sg_tParamTable.pTab[i].pCurValue));
            break;
        case PARAM_INT64:
            printf ("%-10s : %d\n", sg_tParamTable.pTab[i].pszName, (PARAM_INT64_T)(*(PARAM_INT64_T *)sg_tParamTable.pTab[i].pCurValue));
            break;
        case PARAM_UINT8:
            printf ("%-10s : %u\n", sg_tParamTable.pTab[i].pszName, (PARAM_UINT8_T)(*(PARAM_UINT8_T *)sg_tParamTable.pTab[i].pCurValue));
            break;
        case PARAM_UINT16:
            printf ("%-10s : %u\n", sg_tParamTable.pTab[i].pszName, (PARAM_UINT16_T)(*(PARAM_UINT16_T *)sg_tParamTable.pTab[i].pCurValue));
            break;
        case PARAM_UINT32:
            printf ("%-10s : %u\n", sg_tParamTable.pTab[i].pszName, (PARAM_UINT32_T)(*(PARAM_UINT32_T *)sg_tParamTable.pTab[i].pCurValue));
            break;
        case PARAM_UINT64:
            printf ("%-10s : %u\n", sg_tParamTable.pTab[i].pszName, (PARAM_UINT64_T)(*(PARAM_UINT64_T *)sg_tParamTable.pTab[i].pCurValue));
            break;
        case PARAM_FLOAT:
            printf ("%-10s : %f\n", sg_tParamTable.pTab[i].pszName, (PARAM_FLOAT_T)(*(PARAM_FLOAT_T *)sg_tParamTable.pTab[i].pCurValue));
            break;
        case PARAM_DOUBLE:
            printf ("%-10s : %f\n", sg_tParamTable.pTab[i].pszName, (PARAM_DOUBLE_T)(*(PARAM_DOUBLE_T *)sg_tParamTable.pTab[i].pCurValue));
            break;
        case PARAM_STARING:
            printf ("%-10s : %s\n", sg_tParamTable.pTab[i].pszName, sg_tParamTable.pTab[i].pCurValue);
            break;
        
        default:
            break;
        }
    }
}

void SaveAllParam(void)
{
    size_t bufzise;
    uint8_t buf[200];

    bufzise = Param_Serialize(&sg_tParamTable, buf);

    printf("Save: ");

    for (int i = 0; i < bufzise; i++)
    {
        printf("%02x ", buf[i]);
    }

    printf("\n");

    sg_saveBuf[0] = 0x55;

    memcpy(&sg_saveBuf[1], buf, bufzise);
}

void InitAllParam(void)
{
    if (sg_saveBuf[0] != 0x55)
    {
        printf("need frist save\n");
        SaveAllParam();
    }

    Param_Parse(&sg_tParamTable, &sg_saveBuf[1]);
}

int ModifyByName(const char *pszName, const void *pNewData)
{
    return Param_ModifyByName(&sg_tParamTable, pszName, pNewData);
}

int ResetParam(void)
{
    return Param_Reset(&sg_tParamTable);
}