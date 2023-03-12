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
#include "param_cfg.h"
#include "stream.h"
#include <string.h>

/**
  * @brief      将参数编码序列化，具备完整的解析信息
  * 
  * @note       序列化后的数据可支持跨设备传输解析
  * @param[out] pBuf        序列化后的数据
  * @param[in]  paramTable  需要进行序列化的参数表
  * @param[in]  num         参数表元素数目
  * @param[in]  opt         参数序列化选项 PARAM_PACK_NAME、PARAM_PACK_LENGTH 等, 可以将其他信息也进行序列化后传输解析
  * @return     序列化后的数据大小 
  */
size_t Param_MultiSerialize(uint8_t *pBuf, const ParamInfo_t *paramTable, uint16_t num, uint8_t opt)
{
    uint8_t *pPtr = pBuf;

    for (int i = 0; i < num; i++)
    {
        pPtr = PackTagAndOptAndParam(pPtr, &paramTable[i], 1, opt);
    }

    return pPtr - pBuf;
}

#if PARAM_PARSE_MAX_NUM != 0

static uint8_t sg_Buf[PARAM_PARSE_MAX_NUM * (PARAM_NAME_MAX_LENGTH + 32)];

/**
  * @brief      将数据反序列化得到参数表信息
  * 
  * @attention  必须是函数 Param_MultiSerialize 序列化后的数据
  * @param[in]  pBuf        需要反序列化后的数据
  * @param[in]  length      需要反序列化后的数据长度
  * @param[out] paramTable  反序列化得到参数表信息
  * @param[in]  num         参数表元素最大数目, 最大限制 PARAM_PARSE_MAX_NUM
  * @return     反序列化得到参数表元素数目
  */
uint16_t Param_MultiParse(uint8_t *pBuf, size_t length, ParamInfo_t *paramTable, uint16_t num)
{
    uint8_t *pPtr = pBuf;
    uint16_t idx = 0;

    num = num < PARAM_PARSE_MAX_NUM ? num : PARAM_PARSE_MAX_NUM;

    memset(paramTable, 0, num * sizeof(ParamInfo_t));

    while (pPtr - pBuf < length && idx < num)
    {
        paramTable[idx].pszName = &sg_Buf[idx * (PARAM_NAME_MAX_LENGTH + 32)];
        paramTable[idx].pCurValue = &sg_Buf[idx * (PARAM_NAME_MAX_LENGTH + 32) + PARAM_NAME_MAX_LENGTH];
        paramTable[idx].pDefValue = &sg_Buf[idx * (PARAM_NAME_MAX_LENGTH + 32) + PARAM_NAME_MAX_LENGTH + 8];
        paramTable[idx].pMinValue = &sg_Buf[idx * (PARAM_NAME_MAX_LENGTH + 32) + PARAM_NAME_MAX_LENGTH + 16];
        paramTable[idx].pMaxValue = &sg_Buf[idx * (PARAM_NAME_MAX_LENGTH + 32) + PARAM_NAME_MAX_LENGTH + 24];
        pPtr = UnPackTagAndOptAndParam(pPtr, &paramTable[idx]);
        idx++;
    }

    return idx;
}

#endif

/**
  * @brief      将参数编码序列化，不具备完整的解析信息
  * 
  * @attention  序列化时只对部分数据序列化，因此序列化后的数据更适合本机储存在储存芯片中，不适合跨设备传输
  * @param      pBuf        序列化后的数据
  * @param[in]  paramTable  需要进行序列化的参数表
  * @param[in]  num         参数表元素数目
  * @param      isAddKey    是否采用键值对（能向下兼容，即使删除一组参数也能正常反序列化）
  * @param      isUseEncoded 是否采用编码压缩（能够压缩数据，每个数值越小压缩效果越好）
  * @return     序列化后的数据大小
  */
size_t Param_Serialize(uint8_t *pBuf, const ParamInfo_t *paramTable, uint16_t num, uint8_t isAddKey, uint8_t isUseEncoded)
{
    uint8_t *pPtr = pBuf;

    if (isAddKey)
    {
        for (int i = 0; i < num; i++)
        {
            pPtr = PackTagAndParam(pPtr, &paramTable[i], isUseEncoded, 0);
        }
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            pPtr = PackParam(pPtr, &paramTable[i], isUseEncoded, 0);
        }
    }

    return pPtr - pBuf;
}

/**
  * @brief      将数据反序列化得到参数表信息
  * 
  * @attention  反序列化时需要定义和序列化前一样的完整参数表，才能完成解析，即函数 Param_Serialize 序列化后的数据
  * @param[in]  pBuf        需要反序列化后的数据
  * @param[in]  length      需要反序列化后的数据长度
  * @param[out] paramTable  反序列化得到参数表信息
  * @param[in]  num         参数表元素数目
  * @param      isHasKey    序列化的时候是否了采用键值对
  * @param      isUseDecode 序列化的时候是否了编码压缩
  * @return     参数表元素数目 
  */
uint16_t Param_Parse(uint8_t *pBuf, size_t length, ParamInfo_t *paramTable, uint16_t num, uint8_t isHasKey, uint8_t isUseDecode)
{
    uint8_t *pPtr = pBuf;
    uint8_t buf[100];
    ParamInfo_t tParam;

    if (isHasKey)
    {
        tParam.pCurValue = &buf[0];

        while (pPtr - pBuf < length)
        {
            pPtr = UnPackTagAndParam(pPtr, &tParam, isUseDecode, 0);

            for (int i = 0; i < num; i++)
            {
                if (tParam.id == paramTable[i].id && tParam.type == paramTable[i].type)
                {
                    memset(paramTable[i].pCurValue, 0, paramTable[i].length);
                    memcpy(paramTable[i].pCurValue, tParam.pCurValue, tParam.length);
                    break;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            UnPackParam(pBuf, &paramTable[i], isUseDecode, 0);
        }
    }

    return num;
}

