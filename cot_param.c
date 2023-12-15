/**
  **********************************************************************************************************************
  * @file    cot_param.c
  * @brief   该文件提供参数管理框架功能
  * @author  const_zpc  any question please send mail to const_zpc@163.com
  * @version V2.0
  * @date    2023-12-10
  *
  * @details  功能详细说明：
  *           + 参数修改和重置管理
  *           + 参数序列化和反序列化
  * 数据序列化格式：
  * | 支持数目(1) | 数据1的长度和ID(0.5+0.5) | 数据1(n) | 数据2的长度和ID(1) | 数据2(n) | 数据3的长度和ID(1) | 数据3(n) | 
  * | 支持数目(1) | 数据1的长度和ID(1.0+1.0) | 数据1(n) | 数据2的长度和ID(2) | 数据2(n) | 数据3的长度和ID(2) | 数据3(n) | 
  * | 支持数目(1) | 数据1的长度和ID(1.5+1.5) | 数据1(n) | 数据2的长度和ID(3) | 数据2(n) | 数据3的长度和ID(3) | 数据3(n) | 
  *
  **********************************************************************************************************************
  * 源码路径：https://gitee.com/cot_package/cot_param.git 具体问题及建议可在该网址填写 Issue
  *
  *
  **********************************************************************************************************************
  */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "cot_param.h"
#include <string.h>
#include <stdio.h>


typedef union
{
    uint64_t u64val;
    int64_t s64val;
    double fVal;
#if COT_PARAM_USE_STRING_TYPE
    char str[COT_PARAM_STRING_MAX_LENGTH + 2];
#endif
} Value_u;

static cotParamCheckRet_e ValidateRangeByVoid(const cotParamInfo_t *pParam, const void *pval);

static uint8_t *SerializeUint(uint8_t *ptr, uint64_t value, uint8_t len)
{
    for (int i = 0; i < len; i++)
    {
        *ptr++ = value >> (i * 8);
    }

    return ptr;
}

static uint8_t *SerializeInt(uint8_t *ptr, int64_t value, uint8_t len)
{
    for (int i = 0; i < len; i++)
    {
        *ptr++ = value >> (i * 8);
    }

    return ptr;
}

static uint8_t *SerializeFloat(uint8_t *ptr, float value)
{
    uint32_t ivalue = *((uint32_t *)&value);

    for (int i = 0; i < sizeof(ivalue); i++)
    {
        *ptr++ = ivalue >> (i * 8);
    }

    return ptr;
}

static uint8_t *SerializeDouble(uint8_t *ptr, double value)
{
    uint64_t ivalue = *((uint64_t *)&value);

    for (int i = 0; i < sizeof(ivalue); i++)
    {
        *ptr++ = ivalue >> (i * 8);
    }

    return ptr;
}

static uint8_t *UnSerializeUint(const uint8_t *ptr, uint64_t *value, uint8_t len)
{
    *value = 0;

    for (int i = 0; i < len; i++)
    {
        *value |= (uint64_t)(*ptr++) << (i * 8);
    }

    return (uint8_t *)ptr;
}

static uint8_t *UnSerializeInt(const uint8_t *ptr, int64_t *value, uint8_t len)
{
    *value = 0;

    for (int i = 0; i < len; i++)
    {
        *value |= (int64_t)(*ptr++) << (i * 8);
    }

    return (uint8_t *)ptr;
}

static uint8_t *UnSerializeFloat(const uint8_t *ptr, float *value)
{
    uint32_t ivalue = 0;

    for (int i = 0; i < sizeof(ivalue); i++)
    {
        ivalue |= (uint32_t)(*ptr++) << (i * 8);
    }

    *value = *((float *)&ivalue);

    return (uint8_t *)ptr;
}

static uint8_t *UnSerializeDouble(const uint8_t *ptr, double *value)
{
    uint64_t ivalue = 0;

    for (int i = 0; i < sizeof(ivalue); i++)
    {
        ivalue |= (uint64_t)(*ptr++) << (i * 8);
    }

    *value = *((double *)&ivalue);

    return (uint8_t *)ptr;
}

static bool ResetParamValue(const cotParamInfo_t *pParam)
{
    if (pParam != NULL && (pParam->attr & COT_PARAM_ATTR_RESET))
    {
#if COT_PARAM_USE_STRING_TYPE
        if (pParam->type != COT_PARAM_STRING)
        {
            memcpy(pParam->unCurValuePtr.pVoid, pParam->unDefValuePtr.pVoid, pParam->length);
        }
        else
        {
            strcpy(pParam->unCurValuePtr.pString, pParam->unDefValuePtr.pString);
        }
#else
        memcpy(pParam->unCurValuePtr.pVoid, pParam->unDefValuePtr.pVoid, pParam->length);
#endif
        return true;
    }

    return false;
}

static bool ResetParamMinValue(const cotParamInfo_t *pParam)
{
    if (pParam != NULL && (pParam->attr & COT_PARAM_ATTR_RANGE))
    {
#if COT_PARAM_USE_STRING_TYPE
        if (pParam->type != COT_PARAM_STRING)
#endif
        {
            memcpy(pParam->unCurValuePtr.pVoid, pParam->unMinValuePtr.pVoid, pParam->length);
        }

        return true;
    }

    return false;
}

static bool ResetParamMaxValue(const cotParamInfo_t *pParam)
{
    if (pParam != NULL && (pParam->attr & COT_PARAM_ATTR_RANGE))
    {
#if COT_PARAM_USE_STRING_TYPE
        if (pParam->type == COT_PARAM_STRING)
        {
            pParam->unCurValuePtr.pString[*pParam->unMaxValuePtr.pStringLength] = 0;
        }
        else
#endif
        {
            memcpy(pParam->unCurValuePtr.pVoid, pParam->unMaxValuePtr.pVoid, pParam->length);
        }

        return true;
    }

    return false;
}

/**
  * @brief      参数表初始化
  *
  * @param      pManager     参数表管理句柄
  * @param      pParamTable 参数表
  * @param      count       参数表元素数目, 可以通过宏 COT_PARAM_TABLE_SIZE 获取
  * @return     0,成功; -1,失败
  */
int cotParam_Init(cotParamManager_t *pManager, cotParamInfo_t *pParamTable, uint16_t count)
{
    if (pManager == NULL || pParamTable == NULL)
    {
        return -1;
    }

    for (uint16_t i = 0; i < count; i++)
    {
        if (pParamTable[i].unMaxValuePtr.pVoid == NULL || pParamTable[i].unMinValuePtr.pVoid == NULL)
        {
            pParamTable[i].attr &= ~COT_PARAM_ATTR_RANGE;
        }

        if (!(pParamTable[i].attr & COT_PARAM_ATTR_READ))
        {
            pParamTable[i].attr &= ~COT_PARAM_ATTR_WRITE;
        }

        if (pParamTable[i].unDefValuePtr.pVoid == NULL)
        {
            pParamTable[i].attr &= ~COT_PARAM_ATTR_RESET;
        }

        if (pParamTable[i].unCurValuePtr.pVoid == NULL)
        {
            return -1;
        }
    }

    pManager->pParamTable = pParamTable;
    pManager->count = count;
    return 0;
}

/**
  * @brief      重置所有参数，恢复为缺省值
  *
  * @attention  无可重置权限的参数不能恢复为缺省值
  * @param      pManager 参数表管理句柄
  * @return     0,成功; -1,失败
  */
int cotParam_ResetDefault(const cotParamManager_t *pManager)
{
    if (pManager == NULL)
    {
        return -1;
    }

    for (uint16_t i = 0; i < pManager->count; ++i)
    {
        ResetParamValue(&pManager->pParamTable[i]);
    }

    return 0;
}

/**
  * @brief      检查所有参数当前值的范围并对超出范围时进行处理
  * 
  * @attention  如果参数有自定义校验方式，则只有满足范围校验成功的前提下才会执行自定义校验
  * @param      param 参数信息
  * @param      pfnCheckError 参数超出范围时的处理函数
  * @return     0,成功; -1,失败
  */
int cotParam_Check(const cotParamManager_t* pManager, cotParamError_f pfnCheckError)
{
    cotParamCheckRet_e eCheckResult;

    if (pManager == NULL)
    {
        return -1;
    }

    for (int id = 0; id < pManager->count; id++)
    {
        eCheckResult = ValidateRangeByVoid(&pManager->pParamTable[id], pManager->pParamTable[id].unCurValuePtr.pVoid);

        if (COT_PARAM_CHECK_OK != eCheckResult)
        {
            if (pfnCheckError != NULL)
            {
                pfnCheckError(&pManager->pParamTable[id], eCheckResult);
            }
        }
        else
        {
#if COT_PARAM_USE_CUSTOM_CHECK
            if (pManager->pParamTable[id].pfnParamCheck != NULL)
            {
                if (pManager->pParamTable[id].pfnParamCheck(pManager->pParamTable[id].unCurValuePtr.pVoid) != 0)
                {
                    if (pfnCheckError != NULL)
                    {
                        pfnCheckError(&pManager->pParamTable[id], COT_PARAM_CHECK_OTHER_ERR);
                    }
                }
            }
#endif
        }
    }

    return 0;
}

static cotParamInfo_t *FindParamByParamPtr(const cotParamManager_t *pManager, const void *curParamPtr)
{
    for (uint16_t i = 0; i < pManager->count; ++i)
    {
        if (pManager->pParamTable[i].unCurValuePtr.pVoid == curParamPtr)
        {
            return &pManager->pParamTable[i];
        }
    }

    return NULL;
}

/**
  * @brief      参数列表迭代器
  * 
  * @attention  无读取权限的参数会自动跳过
  * @param      pManager 参数表管理句柄
  * @param      psIdx   参数表起始索引
  * @return     参数信息
  */
cotParamInfo_t *cotParam_IterateList(const cotParamManager_t *pManager, size_t *psIdx)
{
    cotParamInfo_t *p = NULL;

    if (pManager == NULL || psIdx == NULL || *psIdx > pManager->count)
    {
        return NULL;
    }

    while (*psIdx < pManager->count)
    {
        if (pManager->pParamTable[*psIdx].attr & COT_PARAM_ATTR_READ)
        {
            p = &pManager->pParamTable[*psIdx];
            (*psIdx)++;
            break;
        }

        (*psIdx)++;
    }

    return p;
}

#if ( COT_PARAM_NAME_MAX_LENGTH > 1)
static cotParamInfo_t *FindParamByName(const cotParamManager_t *pManager, const char *pszName)
{
    for (uint16_t i = 0; i < pManager->count; ++i)
    {
        if (strcmp(pManager->pParamTable[i].pszName, pszName) == 0)
        {
            return &pManager->pParamTable[i];
        }
    }

    return NULL;
}
#endif

/**
  * @brief      根据参数名称查找参数信息
  *
  * @attention  无可读权限时会查找失败
  * @param      pManager   参数表管理句柄
  * @param      pszName   参数名称
  * @return     参数信息
  */
const cotParamInfo_t *cotParam_FindParamByName(const cotParamManager_t *pManager, const char *pszName)
{
#if ( COT_PARAM_NAME_MAX_LENGTH > 1)
    cotParamInfo_t *pInfo;

    if (pManager != NULL)
    {
        pInfo = FindParamByName(pManager, pszName);

        if (pInfo->attr & COT_PARAM_ATTR_READ)
        {
            return pInfo;
        }
    }

#endif
    return NULL;
}


static cotParamInfo_t *FindParamByID(const cotParamManager_t *pManager, uint16_t id)
{
    for (uint16_t i = 0; i < pManager->count; ++i)
    {
        if (pManager->pParamTable[i].id == id)
        {
            return &pManager->pParamTable[i];
        }
    }

    return NULL;
}

/**
  * @brief      根据参数ID查找参数信息
  *
  * @attention  无可读权限时会查找失败
  * @param      pManager   参数表管理句柄
  * @param      id   参数ID
  * @return     参数信息
  */
const cotParamInfo_t *cotParam_FindParamByID(const cotParamManager_t *pManager, uint16_t id)
{
    cotParamInfo_t *pInfo;

    if (pManager != NULL)
    {
        pInfo = FindParamByID(pManager, id);

        if (pInfo->attr & COT_PARAM_ATTR_READ)
        {
            return pInfo;
        }
    }

    return NULL;
}

/**
  * @brief      根据当前参数数据地址指针查找参数信息
  *
  * @attention  无可读权限时会查找失败
  * @param      pManager   参数表管理句柄
  * @param      pCurParam  当前参数数据指针
  * @return     参数信息
  */
const cotParamInfo_t *cotParam_FindParamByParamPtr(const cotParamManager_t *pManager, const void *pCurParam)
{
    cotParamInfo_t *pInfo;

    if (pManager != NULL || pCurParam != NULL)
    {
        pInfo = FindParamByParamPtr(pManager, pCurParam);

        if (pInfo->attr & COT_PARAM_ATTR_READ)
        {
            return pInfo;
        }
    }

    return NULL;
}


// 验证参数是否在指定范围内
static cotParamCheckRet_e ValidateRange(const cotParamInfo_t *pParam, const Value_u *pval)
{
    if (!(pParam->attr & COT_PARAM_ATTR_RANGE))
    {
        return COT_PARAM_CHECK_OK;
    }

    switch (pParam->type)
    {
    case COT_PARAM_INT8:
        if (pval->s64val < *pParam->unMinValuePtr.pInt8)
        {
            return COT_PARAM_CHECK_OVER_MIN;
        }
        else if (pval->s64val > *pParam->unMaxValuePtr.pInt8)
        {
            return COT_PARAM_CHECK_OVER_MAX;
        }
        break;

    case COT_PARAM_INT16:
        if (pval->s64val < *pParam->unMinValuePtr.pInt16)
        {
            return COT_PARAM_CHECK_OVER_MIN;
        }
        else if (pval->s64val > *pParam->unMaxValuePtr.pInt16)
        {
            return COT_PARAM_CHECK_OVER_MAX;
        }
        break;

    case COT_PARAM_INT32:
        if (pval->s64val < *pParam->unMinValuePtr.pInt32)
        {
            return COT_PARAM_CHECK_OVER_MIN;
        }
        else if (pval->s64val > *pParam->unMaxValuePtr.pInt32)
        {
            return COT_PARAM_CHECK_OVER_MAX;
        }
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_INT64:
        if (pval->s64val < *pParam->unMinValuePtr.pInt64)
        {
            return COT_PARAM_CHECK_OVER_MIN;
        }
        else if (pval->s64val > *pParam->unMaxValuePtr.pInt64)
        {
            return COT_PARAM_CHECK_OVER_MAX;
        }
        break;
#endif
    case COT_PARAM_UINT8:
        if (pval->u64val < *pParam->unMinValuePtr.pUint8)
        {
            return COT_PARAM_CHECK_OVER_MIN;
        }
        else if (pval->u64val > *pParam->unMaxValuePtr.pUint8)
        {
            return COT_PARAM_CHECK_OVER_MAX;
        }
        break;

    case COT_PARAM_UINT16:
        if (pval->u64val < *pParam->unMinValuePtr.pUint16)
        {
            return COT_PARAM_CHECK_OVER_MIN;
        }
        else if (pval->u64val > *pParam->unMaxValuePtr.pUint16)
        {
            return COT_PARAM_CHECK_OVER_MAX;
        }
        break;

    case COT_PARAM_UINT32:
        if (pval->u64val < *pParam->unMinValuePtr.pUint32)
        {
            return COT_PARAM_CHECK_OVER_MIN;
        }
        else if (pval->u64val > *pParam->unMaxValuePtr.pUint32)
        {
            return COT_PARAM_CHECK_OVER_MAX;
        }
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_UINT64:
        if (pval->u64val < *pParam->unMinValuePtr.pUint64)
        {
            return COT_PARAM_CHECK_OVER_MIN;
        }
        else if (pval->u64val > *pParam->unMaxValuePtr.pUint64)
        {
            return COT_PARAM_CHECK_OVER_MAX;
        }
        break;
#endif
    case COT_PARAM_FLOAT:
        if (pval->fVal < *pParam->unMinValuePtr.pFloat)
        {
            return COT_PARAM_CHECK_OVER_MIN;
        }
        else if (pval->fVal > *pParam->unMaxValuePtr.pFloat)
        {
            return COT_PARAM_CHECK_OVER_MAX;
        }
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_DOUBLE:
        if (pval->fVal < *pParam->unMinValuePtr.pDouble)
        {
            return COT_PARAM_CHECK_OVER_MIN;
        }
        else if (pval->fVal > *pParam->unMaxValuePtr.pDouble)
        {
            return COT_PARAM_CHECK_OVER_MAX;
        }
        break;
#endif
#if COT_PARAM_USE_STRING_TYPE
    case COT_PARAM_STRING:
        if (strlen(pval->str) < *pParam->unMinValuePtr.pStringLength)
        {
            return COT_PARAM_CHECK_OVER_MIN;
        }
        else if (strlen(pval->str) > *pParam->unMaxValuePtr.pStringLength)
        {
            return COT_PARAM_CHECK_OVER_MAX;
        }
        break;
#endif
    default:
        break;
    }

    return COT_PARAM_CHECK_OK;
}

static cotParamCheckRet_e ValidateRangeByVoid(const cotParamInfo_t *pParam, const void *pval)
{
    Value_u uValue;

    switch (pParam->type)
    {
    case COT_PARAM_INT8:
        uValue.s64val = *(COT_PARAM_INT8_T *)pval;
        break;

    case COT_PARAM_INT16:
        uValue.s64val = *(COT_PARAM_INT16_T *)pval;
        break;

    case COT_PARAM_INT32:
        uValue.s64val = *(COT_PARAM_INT32_T *)pval;
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_INT64:
        uValue.s64val = *(COT_PARAM_INT64_T *)pval;
        break;
#endif
    case COT_PARAM_UINT8:
        uValue.s64val = *(COT_PARAM_UINT8_T *)pval;
        break;

    case COT_PARAM_UINT16:
        uValue.s64val = *(COT_PARAM_UINT16_T *)pval;
        break;

    case COT_PARAM_UINT32:
        uValue.s64val = *(COT_PARAM_UINT32_T *)pval;
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_UINT64:
        uValue.s64val = *(COT_PARAM_UINT64_T *)pval;
        break;
#endif
    case COT_PARAM_FLOAT:
        uValue.fVal = *(COT_PARAM_FLOAT_T *)pval;
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_DOUBLE:
        uValue.fVal = *(COT_PARAM_DOUBLE_T *)pval;
        break;
#endif
#if COT_PARAM_USE_STRING_TYPE
    case COT_PARAM_STRING:
        memcpy(uValue.str, pval, strlen(pval) > COT_PARAM_STRING_MAX_LENGTH ? COT_PARAM_STRING_MAX_LENGTH + 2 : strlen(pval) + 1);
        uValue.str[COT_PARAM_STRING_MAX_LENGTH + 2] = '\0';
        break;
#endif
    default:
        return COT_PARAM_CHECK_OK;
    }

    return ValidateRange(pParam, &uValue);
}

/**
  * @brief      校验当前参数值并进行处理
  *
  * @attention  字符串类型参数若设置了最长的长度，超长则会进行截断，但是小于最小长度时则不做处理
  * @attention  该函数不会执行自定义校验
  * @param      pParam   参数信息
  * @param      eResetOpt 参数值超出范围的处理选项 @enum cotParamResetOpt_e
  * @return     0,成功; -1,失败
  */
int cotParam_SingleParamCheckProcess(const cotParamInfo_t *pParam, cotParamResetOpt_e eResetOpt)
{
    cotParamCheckRet_e eCheckResult;

    if (pParam == NULL)
    {
        return -1;
    }

    eCheckResult = ValidateRangeByVoid(pParam, pParam->unCurValuePtr.pVoid);

    if (eCheckResult != COT_PARAM_CHECK_OK)
    {
        if (eResetOpt == COT_PARAM_RESET_DEF)
        {
            ResetParamValue(pParam);
        }
        else if (eResetOpt == COT_PARAM_RESET_MIN || (eResetOpt == COT_PARAM_RESET_MIN_MAX && eCheckResult == COT_PARAM_CHECK_OVER_MIN))
        {
            ResetParamMinValue(pParam);
        }
        else if (eResetOpt == COT_PARAM_RESET_MAX || (eResetOpt == COT_PARAM_RESET_MIN_MAX && eCheckResult == COT_PARAM_CHECK_OVER_MAX))
        {
            ResetParamMaxValue(pParam);
        }
    }

    return 0;
}

/**
  * @brief      更新参数值同时进行校验处理
  *
  * @attention  该函数不会执行自定义校验
  * @param      pParam    参数信息
  * @param      pNewValue 新的参数值
  * @param      eResetOpt 新的参数值超出范围的处理选项 @enum cotParamResetOpt_e
  * @return     0,成功; -1,失败
  */
int cotParam_SingleParamUpdate(const cotParamInfo_t *pParam, const void *pNewValue, cotParamResetOpt_e eResetOpt)
{
    cotParamCheckRet_e eCheckResult;

    if (pParam == NULL)
    {
        return -1;
    }

    eCheckResult = ValidateRangeByVoid(pParam, pNewValue);

    if (eCheckResult != COT_PARAM_CHECK_OK)
    {
        if (eResetOpt == COT_PARAM_RESET_DEF)
        {
            ResetParamValue(pParam);
        }
        else if (eResetOpt == COT_PARAM_RESET_MIN || (eResetOpt == COT_PARAM_RESET_MIN_MAX && eCheckResult == COT_PARAM_CHECK_OVER_MIN))
        {
            ResetParamMinValue(pParam);
        }
        else if (eResetOpt == COT_PARAM_RESET_MAX || (eResetOpt == COT_PARAM_RESET_MIN_MAX && eCheckResult == COT_PARAM_CHECK_OVER_MAX))
        {
            ResetParamMaxValue(pParam);
        }
    }
    else
    {
#if COT_PARAM_USE_STRING_TYPE
        if (pParam->type != COT_PARAM_STRING)
        {
            memcpy(pParam->unCurValuePtr.pVoid, pNewValue, pParam->length);
        }
        else
        {
            strcpy(pParam->unCurValuePtr.pString, pNewValue);
        }
#else
        memcpy(pParam->unCurValuePtr.pVoid, pNewValue, pParam->length);
#endif
    }

    return 0;
}

/**
 * @brief      根据参数信息校验输入的值
 * 
 * @attention  如果参数有自定义校验方式，则只有满足范围校验成功的前提下才会执行自定义校验
 * @param[in]  pParam    参数信息
 * @param[in]  pValue    输入的参数值，该变量的类型需要和该参数的类型保持一致
 * @param[out] peCheckResult 校验结果
 * @return     0,成功; -1,失败
 */
int cotParam_SingleParamCheckInput(const cotParamInfo_t *pParam, const void *pValue, cotParamCheckRet_e *peCheckResult)
{
    if (pParam == NULL || pValue == NULL || peCheckResult == NULL)
    {
        return -1;
    }

    *peCheckResult = ValidateRangeByVoid(pParam, pValue);

#if COT_PARAM_USE_CUSTOM_CHECK
    if (*peCheckResult == COT_PARAM_CHECK_OK)
    {
        if (pParam->pfnParamCheck != NULL)
        {
            if (pParam->pfnParamCheck(pValue) != 0)
            {
                *peCheckResult = COT_PARAM_CHECK_OTHER_ERR;
            }
        }
    }
#endif
    return 0;
}

/**
 * @brief      校验当前参数值
 * 
 * @attention  如果参数有自定义校验方式，则只有满足范围校验成功的前提下才会执行自定义校验
 * @param[in]  pParam    参数信息
 * @param[out] peCheckResult 校验结果
 * @return     0,成功; -1,失败
 */
int cotParam_SingleParamSelfCheck(const cotParamInfo_t *pParam, cotParamCheckRet_e *peCheckResult)
{
    return cotParam_SingleParamCheckInput(pParam, pParam->unCurValuePtr.pVoid, peCheckResult);
}

/**
  * @brief      重置参数为缺省值
  *
  * @attention  无缺省值的参数会失败
  * @param      pParam 参数信息
  * @return     0,成功; -1,失败
  */
int cotParam_SingleParamResetDefValue(const cotParamInfo_t *pParam)
{
    if (pParam == NULL)
    {
        return -1;
    }

    if (!ResetParamValue(pParam))
    {
        return -1;
    }

    return 0;
}

/**
  * @brief      重置参数为最小值
  * 
  * @attention  字符串类型参数该功能无效, 无最大最小值的参数也会失败
  * @param      pParam 参数信息
  * @return     0,成功; -1,失败
  */
int cotParam_SingleParamResetMinValue(const cotParamInfo_t *pParam)
{
    if (pParam == NULL)
    {
        return -1;
    }

    if (!ResetParamMinValue(pParam))
    {
        return -1;
    }

    return 0;
}

/**
  * @brief      重置参数为最大值
  * 
  * @attention  字符串类型参数该功能无效, 无最大最小值的参数也会失败
  * @param      pParam 参数信息
  * @return     0,成功; -1,失败
  */
int cotParam_SingleParamResetMaxValue(const cotParamInfo_t *pParam)
{
    if (pParam == NULL)
    {
        return -1;
    }

    if (!ResetParamMaxValue(pParam))
    {
        return -1;
    }

    return 0;
}

static uint16_t ParamInfoFormStream(cotParamInfo_t *pParam, const uint8_t *pbuf)
{
    switch (pParam->type)
    {
    case COT_PARAM_INT8:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, pParam->length);
        *pParam->unCurValuePtr.pInt8 = (COT_PARAM_INT8_T)val;
    }
    break;

    case COT_PARAM_INT16:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, pParam->length);
        *pParam->unCurValuePtr.pInt16 = (COT_PARAM_INT16_T)val;
    }
    break;

    case COT_PARAM_INT32:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, pParam->length);
        *pParam->unCurValuePtr.pInt32 = (COT_PARAM_INT32_T)val;
    }
    break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_INT64:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, pParam->length);
        *pParam->unCurValuePtr.pInt64 = (COT_PARAM_INT64_T)val;
    }
    break;
#endif
    case COT_PARAM_UINT8:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, pParam->length);
        *pParam->unCurValuePtr.pUint8 = (COT_PARAM_UINT8_T)val;
    }
    break;

    case COT_PARAM_UINT16:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, pParam->length);
        *pParam->unCurValuePtr.pUint16 = (COT_PARAM_UINT16_T)val;
    }
    break;

    case COT_PARAM_UINT32:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, pParam->length);
        *pParam->unCurValuePtr.pUint32= (COT_PARAM_UINT32_T)val;
    }
    break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_UINT64:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, pParam->length);
        *pParam->unCurValuePtr.pUint64 = (COT_PARAM_UINT64_T)val;
    }
    break;
#endif
    case COT_PARAM_FLOAT:
        pbuf = UnSerializeFloat(pbuf, pParam->unCurValuePtr.pFloat);
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_DOUBLE:
        pbuf = UnSerializeDouble(pbuf, pParam->unCurValuePtr.pDouble);
        break;
#endif
#if COT_PARAM_USE_STRING_TYPE
    case COT_PARAM_STRING:
        memcpy(pParam->unCurValuePtr.pString, &pbuf[0], pParam->length);
        break;
#endif
    default:
        return 0; // 不支持的参数类型
    }

    return pParam->length;
}

static uint8_t *MoveBufToBase(uint8_t *pbuf, uint32_t length)
{
    memmove(pbuf - length, pbuf, length);
    return (pbuf - length);
}

/**
  * @brief      加载数据
  *
  * @param      pManager 参数表管理句柄
  * @param      pfnLoadCallback 加载回调函数
  * @return     0,成功; -1,失败; -2,加载回调函数返回失败
  */
int cotParam_Load(const cotParamManager_t *pManager, cotParamLoad_f pfnLoadCallback)
{
#if COT_PARAM_USE_STRING_TYPE
    uint8_t buf[sizeof(cotParamInfo_t) + COT_PARAM_STRING_MAX_LENGTH];
#else
    uint8_t buf[sizeof(cotParamInfo_t)];
#endif
    uint8_t *ptr = buf;

    if (pManager == NULL || pfnLoadCallback == NULL)
    {
        return -1;
    }

    uint16_t length = 0;
    uint16_t paramLength = 0;
    uint16_t id = 0;
    cotParamInfo_t *pParamInfo;
    uint8_t keyLength = 0;
#if COT_PARAM_USE_KEY_VALUE
    uint64_t key = 0;
#endif

    do
    {
        length = sizeof(buf) - (ptr - buf);

        if (pfnLoadCallback(ptr, length, &length) != 0)
        {
            return -2;
        }

        if (length == 0)
        {
            break;
        }

        length += (ptr - buf);
        ptr = buf;

#if COT_PARAM_USE_KEY_VALUE
        if (keyLength == 0)
        {
            keyLength = ptr[0];
            ptr++;
            length -= 1;
        }

        while (length > keyLength)
        {
            UnSerializeUint(ptr, &key, keyLength);
            
#if COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_16
            id = (key >> 4) & 0x0F;
            paramLength = key & 0x0F;
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_256
            id = (key >> 8) & 0xFF;
            paramLength = key & 0xFF;
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_4096
            id = (key >> 12) & 0xFFF;
            paramLength = key & 0xFFF;
#endif

            if (length < (paramLength + keyLength))
            {
                break;
            }

            ptr += keyLength;
            length -= keyLength;

            pParamInfo = (cotParamInfo_t *)FindParamByID(pManager, id);

            if (pParamInfo == NULL || paramLength != pParamInfo->length)
            {
                ptr += paramLength;
                length -= paramLength;
                continue;
            }

            ParamInfoFormStream(pParamInfo, ptr);
            ptr += paramLength;
            length -= paramLength;
        }

#else

        while (length >= 1)
        {
            pParamInfo = &pManager->pParamTable[id];

            if (length < pParamInfo->length)
            {
                break;
            }

            ParamInfoFormStream(pParamInfo, ptr);
            ptr += pParamInfo->length;
            length -= pParamInfo->length;
            id++;
        }

#endif
        ptr = MoveBufToBase(ptr, ptr - buf);
        ptr += length;
    } while (1);

    return 0;
}

static uint16_t ParamInfoToStream(uint8_t *pbuf, cotParamInfo_t *pParam)
{
    switch (pParam->type)
    {
    case COT_PARAM_INT8:
        pbuf = SerializeInt(pbuf, *(COT_PARAM_INT8_T *)pParam->unCurValuePtr.pVoid, pParam->length);
        break;

    case COT_PARAM_INT16:
        pbuf = SerializeInt(pbuf, *(COT_PARAM_INT16_T *)pParam->unCurValuePtr.pVoid, pParam->length);
        break;

    case COT_PARAM_INT32:
        pbuf = SerializeInt(pbuf, *(COT_PARAM_INT32_T *)pParam->unCurValuePtr.pVoid, pParam->length);
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_INT64:
        pbuf = SerializeInt(pbuf, *(COT_PARAM_INT64_T *)pParam->unCurValuePtr.pVoid, pParam->length);
        break;
#endif
    case COT_PARAM_UINT8:
        pbuf = SerializeUint(pbuf, *(COT_PARAM_UINT8_T *)pParam->unCurValuePtr.pVoid, pParam->length);
        break;

    case COT_PARAM_UINT16:
        pbuf = SerializeUint(pbuf, *(COT_PARAM_UINT16_T *)pParam->unCurValuePtr.pVoid, pParam->length);
        break;

    case COT_PARAM_UINT32:
        pbuf = SerializeUint(pbuf, *(COT_PARAM_UINT32_T *)pParam->unCurValuePtr.pVoid, pParam->length);
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_UINT64:
        pbuf = SerializeUint(pbuf, *(COT_PARAM_UINT64_T *)pParam->unCurValuePtr.pVoid, pParam->length);
        break;
#endif
    case COT_PARAM_FLOAT:
        pbuf = SerializeFloat(pbuf, *(COT_PARAM_FLOAT_T *)pParam->unCurValuePtr.pVoid);
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_DOUBLE:
        pbuf = SerializeDouble(pbuf, *(COT_PARAM_DOUBLE_T *)pParam->unCurValuePtr.pVoid);
        break;
#endif
#if COT_PARAM_USE_STRING_TYPE
    case COT_PARAM_STRING:
        memcpy(&pbuf[0], pParam->unCurValuePtr.pString, pParam->length);
        break;
#endif
    default:
        return 0; // 不支持的参数类型
    }

    return pParam->length;
}

/**
  * @brief      保存数据
  *
  * @param      pManager 参数表管理句柄
  * @param      pfnSaveCallback 保存回调函数
  * @return     0,成功; -1,失败; -2,保存回调函数返回失败
  */
int cotParam_Save(const cotParamManager_t *pManager, cotParamSave_f pfnSaveCallback)
{
#if COT_PARAM_USE_STRING_TYPE
    uint8_t buf[sizeof(cotParamInfo_t) + COT_PARAM_STRING_MAX_LENGTH];
#else
    uint8_t buf[sizeof(cotParamInfo_t)];
#endif
    uint8_t *ptr = buf;
    uint16_t length = 0;
#if COT_PARAM_USE_KEY_VALUE
    uint64_t key = 0;
#endif

    if (pManager == NULL || pfnSaveCallback == NULL)
    {
        return -1;
    }

#if COT_PARAM_USE_KEY_VALUE
    buf[0] = COT_PARAM_SUPPORT_NUM;

    if (pfnSaveCallback(buf, 1) != 0)
    {
        return -2;
    }
#endif

    for (int i = 0; i < pManager->count; i++)
    {
        ptr = buf;
        length = 0;
#if COT_PARAM_USE_KEY_VALUE
#if COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_16
        key = (pManager->pParamTable[i].id << 4) | (pManager->pParamTable[i].length & 0x0F);
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_256
        key = (pManager->pParamTable[i].id << 8) | (pManager->pParamTable[i].length & 0xFF);
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_4096
        key = (pManager->pParamTable[i].id << 12) | (pManager->pParamTable[i].length & 0xFFF);
#endif
        ptr = SerializeUint(ptr, key, COT_PARAM_SUPPORT_NUM);
        length += COT_PARAM_SUPPORT_NUM;
#endif
        length = ParamInfoToStream(&buf[length], &pManager->pParamTable[i]);
        ptr += length;

        if (pfnSaveCallback(buf, (ptr - buf)) != 0)
        {
            return -2;
        }
    }

    if (pfnSaveCallback(buf, 0) != 0)
    {
        return -2;
    }

    return 0;
}

/**
 * @brief      获取参数序列化所需要的内存大小
 * 
 * @param[in]  pManager 参数表管理句柄
 * @return     参数序列化所需要的内存大小
 */
uint32_t cotParam_GetSerializeSize(const cotParamManager_t* pManager)
{
    uint32_t length = 1;
    uint16_t idx = 0;

    if (pManager == NULL)
    {
        return 0;
    }

    while (idx < pManager->count)
    {
#if COT_PARAM_USE_KEY_VALUE
        length += (pManager->pParamTable[idx++].length + COT_PARAM_SUPPORT_NUM);
#else
        length += pManager->pParamTable[idx++].length;
#endif
    }

    return length;
}

/**
 * @brief      获取参数序列化后的数据
 * 
 * @param[in]  pManager 参数表管理句柄
 * @param[out] pbuf    存参数序列化后的数据
 * @return     0,成功; -1,失败 
 */
uint32_t cotParam_Serialize(const cotParamManager_t* pManager, uint8_t *pbuf)
{
    if (pManager == NULL)
    {
        return 0;
    }

    uint8_t *ptr = pbuf;
#if COT_PARAM_USE_KEY_VALUE
    uint64_t key = 0;
#endif

#if COT_PARAM_USE_KEY_VALUE
    *ptr = COT_PARAM_SUPPORT_NUM;
    ptr++;
#endif

    for (int i = 0; i < pManager->count; i++)
    {
#if COT_PARAM_USE_KEY_VALUE
#if COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_16
        key = (pManager->pParamTable[i].id << 4) | (pManager->pParamTable[i].length & 0x0F);
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_256
        key = (pManager->pParamTable[i].id << 8) | (pManager->pParamTable[i].length & 0xFF);
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_4096
        key = (pManager->pParamTable[i].id << 12) | (pManager->pParamTable[i].length & 0xFFF);
#endif
        ptr = SerializeUint(ptr, key, COT_PARAM_SUPPORT_NUM);
#endif
        ptr += ParamInfoToStream(ptr, &pManager->pParamTable[i]);
    }

    return ptr - pbuf;
}

/**
 * @brief      参数反序列化
 * 
 * @param[in]  pManager  参数表管理句柄
 * @param[in]  pbuf     参数需要反序列化的数据
 * @param[in]  length   反序列化的数据长度
 * @return     0,成功; -1,失败 
 */
int cotParam_Deserialization(const cotParamManager_t* pManager, const uint8_t *pbuf, uint32_t length)
{
    if (pManager == NULL)
    {
        return -1;
    }

    const uint8_t *ptr = pbuf;

    uint16_t id = 0;
    cotParamInfo_t *pParamInfo;
    
#if COT_PARAM_USE_KEY_VALUE
    uint8_t keyLength = 0;
    uint16_t paramLength = 0;
    uint64_t key = 0;
#endif

#if COT_PARAM_USE_KEY_VALUE
    if (keyLength == 0)
    {
        keyLength = ptr[0];
        ptr++;
        length -= 1;
    }

    while (length > keyLength)
    {
        ptr = UnSerializeUint(ptr, &key, keyLength);
        
#if COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_16
        id = (key >> 4) & 0x0F;
        paramLength = key & 0x0F;
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_256
        id = (key >> 8) & 0xFF;
        paramLength = key & 0xFF;
#elif COT_PARAM_SUPPORT_NUM == COT_PARAM_SUPPORT_4096
        id = (key >> 12) & 0xFFF;
        paramLength = key & 0xFFF;
#endif

        if (length < (paramLength + keyLength))
        {
            break;
        }

        length -= (keyLength + paramLength);

        pParamInfo = (cotParamInfo_t *)FindParamByID(pManager, id);

        if (pParamInfo == NULL || paramLength != pParamInfo->length)
        {
            ptr += paramLength;
            continue;
        }

        ParamInfoFormStream(pParamInfo,  ptr);
        ptr += paramLength;
    }

#else
    while (length >= 1)
    {
        pParamInfo = &pManager->pParamTable[id];

        if (length < pParamInfo->length)
        {
            break;
        }

        ParamInfoFormStream(pParamInfo, ptr);
        ptr += pParamInfo->length;
        length -= pParamInfo->length;
        id++;
    }
#endif

    return 0;
}

/**
  * @brief      修改参数值，新值校验不通过则不修改
  * 
  * @note       该函数主要是方便进行二次函数封装，隐藏参数表管理句柄入参，可参考 cotParam_SingleParamChange 的实现
  * @param      pManager   参数表管理句柄
  * @param      pCurParam  当前参数数据指针
  * @param      paramList  可变参数列表
  * @return     0,成功; -1,失败; 
  */
int cotParam_SingleParamChangeImpl(const cotParamManager_t* pManager, const void *pCurParam, va_list paramList)
{
    const cotParamInfo_t *pParam = cotParam_FindParamByParamPtr(pManager, pCurParam);

    if (pParam == NULL)
    {
        return -1;
    }

    switch (pParam->type)
    {
    case COT_PARAM_INT8:
    {
        COT_PARAM_INT8_T val = (COT_PARAM_INT8_T)va_arg(paramList, COT_PARAM_INT32_T);
        cotParam_SingleParamUpdate(pParam, &val, COT_PARAM_RESET_NONE);
    }
        break;

    case COT_PARAM_INT16:
    {
        COT_PARAM_INT16_T val = (COT_PARAM_INT16_T)va_arg(paramList, COT_PARAM_INT32_T);
        cotParam_SingleParamUpdate(pParam, &val, COT_PARAM_RESET_NONE);
    }
        break;

    case COT_PARAM_INT32:
    {
        COT_PARAM_INT32_T val = (COT_PARAM_INT32_T)va_arg(paramList, COT_PARAM_INT32_T);
        cotParam_SingleParamUpdate(pParam, &val, COT_PARAM_RESET_NONE);
    }
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_INT64:
    {
        COT_PARAM_INT64_T val = (COT_PARAM_INT64_T)va_arg(paramList, COT_PARAM_INT64_T);
        cotParam_SingleParamUpdate(pParam, &val, COT_PARAM_RESET_NONE);
    }
        break;
#endif
    case COT_PARAM_UINT8:
    {
        COT_PARAM_UINT8_T val = (COT_PARAM_UINT8_T)va_arg(paramList, COT_PARAM_UINT32_T);
        cotParam_SingleParamUpdate(pParam, &val, COT_PARAM_RESET_NONE);
    }
        break;

    case COT_PARAM_UINT16:
    {
        COT_PARAM_UINT16_T val = (COT_PARAM_UINT16_T)va_arg(paramList, COT_PARAM_UINT32_T);
        cotParam_SingleParamUpdate(pParam, &val, COT_PARAM_RESET_NONE);
    }
        break;

    case COT_PARAM_UINT32:
    {
        COT_PARAM_UINT32_T val = (COT_PARAM_UINT32_T)va_arg(paramList, COT_PARAM_UINT32_T);
        cotParam_SingleParamUpdate(pParam, &val, COT_PARAM_RESET_NONE);
    }
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_UINT64:
    {
        COT_PARAM_UINT64_T val = (COT_PARAM_UINT64_T)va_arg(paramList, COT_PARAM_UINT64_T);
        cotParam_SingleParamUpdate(pParam, &val, COT_PARAM_RESET_NONE);
    }
        break;
#endif
    case COT_PARAM_FLOAT:
    {
        COT_PARAM_FLOAT_T val = (COT_PARAM_FLOAT_T)va_arg(paramList, COT_PARAM_DOUBLE_T);
        cotParam_SingleParamUpdate(pParam, &val, COT_PARAM_RESET_NONE);
    }
        break;
#if COT_PARAM_USE_64_BIT_LENGTH
    case COT_PARAM_DOUBLE:
    {
        COT_PARAM_DOUBLE_T val = (COT_PARAM_DOUBLE_T)va_arg(paramList, COT_PARAM_DOUBLE_T);
        cotParam_SingleParamUpdate(pParam, &val, COT_PARAM_RESET_NONE);
    }
        break;
#endif
#if COT_PARAM_USE_STRING_TYPE
    case COT_PARAM_STRING:
        {
            char *pszString = (char *)va_arg(paramList, char *);
            char szString[COT_PARAM_STRING_MAX_LENGTH] = {0};

            memcpy(szString, pszString, strlen(pszString) >= COT_PARAM_STRING_MAX_LENGTH ? 
                            (COT_PARAM_STRING_MAX_LENGTH - 1) : strlen(pszString));
            cotParam_SingleParamUpdate(pParam, szString, COT_PARAM_RESET_NONE);
        }
        break;
#endif
    default:
        return -1;
    }
    
    return 0;
}

/**
  * @brief      修改参数值，新值校验不通过则不修改
  * 
  * @code       如：cotParam_SingleParamChange(&sg_tParamManager, &g_test_u16, 60)
  * @param      pManager   参数表管理句柄
  * @param      pCurParam  当前参数数据指针
  * @param      ... 新值（只有一个参数）
  * @return     0,成功; -1,失败; 
  */
int cotParam_SingleParamChange(const cotParamManager_t* pManager, const void *pCurParam, ...)
{
    int ret = 0;
    va_list paramList;

    va_start(paramList, pCurParam);

    ret = cotParam_SingleParamChangeImpl(pManager, pCurParam, paramList);

    va_end(paramList);

    return ret;
}

