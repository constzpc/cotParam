/**
  **********************************************************************************************************************
  * @file    param.c
  * @brief   该文件提供参数管理框架功能
  * @author  const_zpc  any question please send mail to const_zpc@163.com
  * @version V0.1
  * @date    2023-02-07
  *
  * @details  功能详细说明：
  *           + 参数修改和重置管理
  *           + 参数序列化和反序列化
  *
  **********************************************************************************************************************
  * 源码路径：https://gitee.com/const-zpc/param.git 具体问题及建议可在该网址填写 Issue
  *
  *
  **********************************************************************************************************************
  */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "param.h"
#include "stream.h"
#include <string.h>


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

static struct
{
    const ParamInfo_t *pTab;
    uint32_t num;
} sg_tParamTable;

// int Param_Reset(ParamTable_t *pParamTable)
// {
//     if (pParamTable == NULL)
//     {
//         return -1;
//     }

//     for (int i = 0; i < pParamTable->num; i++)
//     {
//         if (pParamTable->pTab[i].attr & PARAM_ATTR_RESET)
//         {
//             memcpy(pParamTable->pTab[i].pCurValue, pParamTable->pTab[i].pDefValue, pParamTable->pTab[i].length);
//         }
//     }

//     return 0;
// }

// /**
//   * @brief      根据参数ID进行修改参数
//   * 
//   * @param[in]  pParamTable 参数表指针
//   * @param[in]  id 参数ID
//   * @param[in]  pNewData 新数据指针
//   * @return     -1,失败 0,成功
//   */
// int Param_ModifyById(ParamTable_t *pParamTable, uint32_t id, const void *pNewData)
// {
//     if (pParamTable == NULL)
//     {
//         return -1;
//     }

//     for (int i = 0; i < pParamTable->num; i++)
//     {
//         if (pParamTable->pTab[i].id == id && (pParamTable->pTab[i].attr & PARAM_ATTR_WRITE))
//         {
//             switch (pParamTable->pTab[i].type)
//             {
//                 CASE_CHANGE(PARAM_INT8);
//                 CASE_CHANGE(PARAM_INT16);
//                 CASE_CHANGE(PARAM_INT32);
//                 CASE_CHANGE(PARAM_INT64);
//                 CASE_CHANGE(PARAM_UINT8);
//                 CASE_CHANGE(PARAM_UINT16);
//                 CASE_CHANGE(PARAM_UINT32);
//                 CASE_CHANGE(PARAM_UINT64);
//                 CASE_CHANGE(PARAM_FLOAT);
//                 CASE_CHANGE(PARAM_DOUBLE);
//             case PARAM_STARING:
//                 if (strlen((char *)pNewData) < pParamTable->pTab[i].length)
//                 {
//                     strcpy((char *)pParamTable->pTab[i].pCurValue, (char *)pNewData);
//                 }
//                 break;
//             default:
//                 break;
//             }
//         }
//     }

//     return 0;
// }

// /**
//   * @brief      根据参数名进行修改参数
//   * 
//   * @param[in]  pParamTable 参数表指针
//   * @param[in]  pszName 参数名
//   * @param[in]  pNewData 新数据指针
//   * @return     -1,失败 0,成功
//   */
// int Param_ModifyByName(ParamTable_t *pParamTable, const char *pszName, const void *pNewData)
// {

//     for (int i = 0; i < pParamTable->num; i++)
//     {
//         if (strcmp(pParamTable->pTab[i].pszName, pszName) == 0 && (pParamTable->pTab[i].attr & PARAM_ATTR_WRITE))
//         {
//             switch (pParamTable->pTab[i].type)
//             {
//                 CASE_CHANGE(PARAM_INT8);
//                 CASE_CHANGE(PARAM_INT16);
//                 CASE_CHANGE(PARAM_INT32);
//                 CASE_CHANGE(PARAM_INT64);
//                 CASE_CHANGE(PARAM_UINT8);
//                 CASE_CHANGE(PARAM_UINT16);
//                 CASE_CHANGE(PARAM_UINT32);
//                 CASE_CHANGE(PARAM_UINT64);
//                 CASE_CHANGE(PARAM_FLOAT);
//                 CASE_CHANGE(PARAM_DOUBLE);
//             case PARAM_STARING:
//                 if (strlen((char *)pNewData) < pParamTable->pTab[i].length)
//                 {
//                     strcpy((char *)pParamTable->pTab[i].pCurValue, (char *)pNewData);
//                 }
//                 break;
//             default:
//                 break;
//             }
//         }
//     }

//     return 0;
// }

// /**
//   * @brief      将参数表序列化
//   * 
//   * @param[in]  pParamTable 参数表指针
//   * @param[out] pBuf    序列化后的数据buf
//   * @return     序列化后的数据buf长度
//   */
// size_t Param_Serialize(ParamTable_t *pParamTable, uint8_t *pBuf)
// {
//     size_t idx = 0;
    
//     for (int i = 0; i < pParamTable->num; i++)
//     {
//         memcpy(&pBuf[idx], pParamTable->pTab[i].pCurValue, pParamTable->pTab[i].length);
//         idx += pParamTable->pTab[i].length;
//     }
    
//     return idx;
// }

// /**
//   * @brief      将序列化的数据转为参数表信息
//   * 
//   * @param[in]  pParamTable 参数表指针
//   * @param[in] pBuf    序列化前的数据buf
//   */
// void Param_Parse(ParamTable_t *pParamTable, const uint8_t *pBuf)
// {
//     size_t idx = 0;
    
//     for (int i = 0; i < pParamTable->num; i++)
//     {
//         memcpy(pParamTable->pTab[i].pCurValue, &pBuf[idx], pParamTable->pTab[i].length);
//         idx += pParamTable->pTab[i].length;
//     }
    
//     for (int i = 0; i < pParamTable->num; i++)
//     {
//         switch (pParamTable->pTab[i].type)
//         {
//             CASE_CHECK(PARAM_INT8);
//             CASE_CHECK(PARAM_INT16);
//             CASE_CHECK(PARAM_INT32);
//             CASE_CHECK(PARAM_INT64);
//             CASE_CHECK(PARAM_UINT8);
//             CASE_CHECK(PARAM_UINT16);
//             CASE_CHECK(PARAM_UINT32);
//             CASE_CHECK(PARAM_UINT64);
//             CASE_CHECK(PARAM_FLOAT);
//             CASE_CHECK(PARAM_DOUBLE);
//         case PARAM_STARING:
//             if (strlen((char *)pParamTable->pTab[i].pCurValue) >= pParamTable->pTab[i].length)
//             {
//                 strcpy((char *)pParamTable->pTab[i].pCurValue, (char *)pParamTable->pTab[i].pDefValue);
//             }
//             break;
//         default:
//             break;
//         }
//     }
// }

int Param_Init(const ParamInfo_t *paramTable, uint16_t num)
{
    sg_tParamTable.pTab = paramTable;
    sg_tParamTable.num = num;
}

uint8_t sg_buf[200];
uint32_t sg_saveLength;

int Param_Load(void)
{
    if (sg_tParamTable.pTab == NULL)
    {
        return -1;
    }

    StreamFromBin(sg_buf, sg_saveLength, (ParamInfo_t *)sg_tParamTable.pTab, sg_tParamTable.num);

    return 0;
}

int Param_Save(void)
{
    if (sg_tParamTable.pTab == NULL)
    {
        return -1;
    }

    sg_saveLength = StreamToBin(sg_buf, sg_tParamTable.pTab, sg_tParamTable.num);

    return 0;
}

static uint8_t *MemPack(uint8_t *ptr, uint64_t val, uint8_t size)
{
    for (int i = 0; i < size; i++)
    {
        *ptr++ = val >> (i * 8);
    }

    return ptr;
}

static uint8_t *Pack(ParamInfo_t *pParam, uint8_t *pBuf, uint8_t opt)
{
    pBuf = MemPack(pBuf, ( (pParam->id << 4) | pParam->type), 2);

    if (opt & PARAM_PACK_NAME)
    {
        memcpy(pBuf, pParam->pszName, strlen(pParam->pszName) + 1);
        pBuf += (strlen(pParam->pszName) + 1);
    }

    if (opt & PARAM_PACK_LENGTH)
    {
        pBuf = MemPack(pBuf, pParam->length, 2);
    }

    if (opt & PARAM_PACK_ATTR)
    {
        pBuf = MemPack(pBuf, pParam->attr, 1);
    }

    if (opt & PARAM_PACK_DEF_VAL)
    {
        pBuf = MemPack(pBuf, pParam->pDefValue, pParam->pCurValue);
    }

    if (opt & PARAM_PACK_MIN_VAL)
    {
        pBuf = MemPack(pBuf, pParam->attr, 1);
    }

    if (opt & PARAM_PACK_MAX_VAL)
    {
        pBuf = MemPack(pBuf, pParam->attr, 1);
    }
}

/**
  * @brief      按照序列化读取指定参数
  * 
  * @param      id 
  * @param      pBuf 
  * @param      opt 
  * @return     size_t 
  */
size_t Param_Read(uint16_t id, uint8_t *pBuf, uint8_t opt)
{
    uint8_t *ptr = pBuf;
    
    for (int i = 0; i < sg_tParamTable.num; i++)
    {
        if (sg_tParamTable.pTab[i].id == id)
        {
            ptr = Pack(&sg_tParamTable.pTab[i], ptr, opt);
        }
    }
    
    return ptr - pBuf;
}

size_t Param_Reads(uint16_t id, uint16_t num, uint8_t *pBuf, uint8_t opt)
{
    size_t pos = 0;

    for (int i = id; i < sg_tParamTable.num && i < id + num; i++)
    {
        pos += Param_Read(i, &pBuf[pos], opt);
    }

    return pos;
}

