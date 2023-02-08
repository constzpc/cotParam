/**
  * @file       param.c
  * @author     pczhou (pczhou@streamax.com)
  * @brief      
  * @version    0.1
  * @date       2023-02-07
  * 
  * @copyright Copyright (c) 2023  锐明技术股份有限公司
  * 
  * @par 修改日志:
  * <table>
  * <tr><th>Date       <th>Version <th>Author  <th>Description
  * <tr><td>2023-02-07 <td>1.0     <td>pczhou     <td>内容
  * </table>
  */


#include "param.h"
#include <string.h>



// const uint32_t sg_tParamTableNum = sizeof(sg_tParamTable) / sizeof(sg_tParamTable[0]);

#define CASE_CHANGE(type) \
case type:\
{\
    if ((*(type##_T *)pNewData) <= (*(type##_T *)pParamTable->pTab[i].pMaxValue) &&\
        (*(type##_T *)pNewData) >= (*(type##_T *)pParamTable->pTab[i].pMinValue))\
    {\
        *(type##_T *)pParamTable->pTab[i].pCurValue = *(type##_T *)pNewData;\
    }\
}\
break;

#define CASE_CHECK(type) \
case type:\
{\
    if ((*(type##_T *)pParamTable->pTab[i].pCurValue) > (*(type##_T *)pParamTable->pTab[i].pMaxValue) ||\
        (*(type##_T *)pParamTable->pTab[i].pCurValue) < (*(type##_T *)pParamTable->pTab[i].pMinValue))\
    {\
        *(type##_T *)pParamTable->pTab[i].pCurValue = *(type##_T *)pParamTable->pTab[i].pDefValue;\
    }\
}\
break;

int Param_Reset(ParamTable_t *pParamTable)
{
    if (pParamTable == NULL)
    {
        return -1;
    }

    for (int i = 0; i < pParamTable->num; i++)
    {
        memcpy(pParamTable->pTab[i].pCurValue, pParamTable->pTab[i].pDefValue, pParamTable->pTab[i].length);
    }

    return 0;
}

/**
  * @brief      根据参数ID进行修改参数
  * 
  * @param[in]  pParamTable 参数表指针
  * @param[in]  id 参数ID
  * @param[in]  pNewData 新数据指针
  * @return     -1,失败 0,成功
  */
int Param_ModifyById(ParamTable_t *pParamTable, uint32_t id, const void *pNewData)
{
    if (pParamTable == NULL)
    {
        return -1;
    }

    for (int i = 0; i < pParamTable->num; i++)
    {
        if (pParamTable->pTab[i].id == id)
        {
            switch (pParamTable->pTab[i].type)
            {
                CASE_CHANGE(PARAM_INT8);
                CASE_CHANGE(PARAM_INT16);
                CASE_CHANGE(PARAM_INT32);
                CASE_CHANGE(PARAM_INT64);
                CASE_CHANGE(PARAM_UINT8);
                CASE_CHANGE(PARAM_UINT16);
                CASE_CHANGE(PARAM_UINT32);
                CASE_CHANGE(PARAM_UINT64);
                CASE_CHANGE(PARAM_FLOAT);
                CASE_CHANGE(PARAM_DOUBLE);
            case PARAM_STARING:
                if (strlen((char *)pNewData) < pParamTable->pTab[i].length)
                {
                    strcpy((char *)pParamTable->pTab[i].pCurValue, (char *)pNewData);
                }
                break;
            default:
                break;
            }
        }
    }

    return 0;
}

/**
  * @brief      根据参数名进行修改参数
  * 
  * @param[in]  pParamTable 参数表指针
  * @param[in]  pszName 参数名
  * @param[in]  pNewData 新数据指针
  * @return     -1,失败 0,成功
  */
int Param_ModifyByName(ParamTable_t *pParamTable, const char *pszName, const void *pNewData)
{

    for (int i = 0; i < pParamTable->num; i++)
    {
        if (strcmp(pParamTable->pTab[i].pszName, pszName) == 0)
        {
            switch (pParamTable->pTab[i].type)
            {
                CASE_CHANGE(PARAM_INT8);
                CASE_CHANGE(PARAM_INT16);
                CASE_CHANGE(PARAM_INT32);
                CASE_CHANGE(PARAM_INT64);
                CASE_CHANGE(PARAM_UINT8);
                CASE_CHANGE(PARAM_UINT16);
                CASE_CHANGE(PARAM_UINT32);
                CASE_CHANGE(PARAM_UINT64);
                CASE_CHANGE(PARAM_FLOAT);
                CASE_CHANGE(PARAM_DOUBLE);
            case PARAM_STARING:
                if (strlen((char *)pNewData) < pParamTable->pTab[i].length)
                {
                    strcpy((char *)pParamTable->pTab[i].pCurValue, (char *)pNewData);
                }
                break;
            default:
                break;
            }
        }
    }

    return 0;
}

/**
  * @brief      将参数表序列化
  * 
  * @param[in]  pParamTable 参数表指针
  * @param[out] pBuf    序列化后的数据buf
  * @return     序列化后的数据buf长度
  */
size_t Param_Serialize(ParamTable_t *pParamTable, uint8_t *pBuf)
{
    size_t idx = 0;
    
    for (int i = 0; i < pParamTable->num; i++)
    {
        memcpy(&pBuf[idx], pParamTable->pTab[i].pCurValue, pParamTable->pTab[i].length);
        idx += pParamTable->pTab[i].length;
    }
    
    return idx;
}

/**
  * @brief      将序列化的数据转为参数表信息
  * 
  * @param[in]  pParamTable 参数表指针
  * @param[in] pBuf    序列化前的数据buf
  */
void Param_Parse(ParamTable_t *pParamTable, const uint8_t *pBuf)
{
    size_t idx = 0;
    
    for (int i = 0; i < pParamTable->num; i++)
    {
        memcpy(pParamTable->pTab[i].pCurValue, &pBuf[idx], pParamTable->pTab[i].length);
        idx += pParamTable->pTab[i].length;
    }
    
    for (int i = 0; i < pParamTable->num; i++)
    {
        switch (pParamTable->pTab[i].type)
        {
            CASE_CHECK(PARAM_INT8);
            CASE_CHECK(PARAM_INT16);
            CASE_CHECK(PARAM_INT32);
            CASE_CHECK(PARAM_INT64);
            CASE_CHECK(PARAM_UINT8);
            CASE_CHECK(PARAM_UINT16);
            CASE_CHECK(PARAM_UINT32);
            CASE_CHECK(PARAM_UINT64);
            CASE_CHECK(PARAM_FLOAT);
            CASE_CHECK(PARAM_DOUBLE);
        case PARAM_STARING:
            if (strlen((char *)pParamTable->pTab[i].pCurValue) >= pParamTable->pTab[i].length)
            {
                strcpy((char *)pParamTable->pTab[i].pCurValue, (char *)pParamTable->pTab[i].pDefValue);
            }
            break;
        default:
            break;
        }
    }
}

