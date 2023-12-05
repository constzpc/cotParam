/**
  **********************************************************************************************************************
  * @file    param.c
  * @brief   该文件提供参数管理框架功能
  * @author  const_zpc  any question please send mail to const_zpc@163.com
  * @version V1.0
  * @date    2023-06-09
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
#include <string.h>
#include <stdio.h>


typedef union
{
    uint64_t u64val;
    int64_t s64val;
    double fVal;
#if PARAM_USE_STRING_TYPE
    char str[PARAM_STRING_MAX_LENGTH + 2];
#endif
} Value_u;

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

static bool ResetParamValue(const ParamInfo_t *param)
{
    if (param != NULL && (param->attr & PARAM_ATTR_RESET))
    {
#if PARAM_USE_STRING_TYPE
        if (param->type != PARAM_STRING)
        {
            memcpy(param->unCurValuePtr.pVoid, param->unDefValuePtr.pVoid, param->length);
        }
        else
        {
            strcpy(param->unCurValuePtr.pString, param->unDefValuePtr.pString);
        }
#else
        memcpy(param->unCurValuePtr.pVoid, param->unDefValuePtr.pVoid, param->length);
#endif
        return true;
    }

    return false;
}

static bool ResetParamMinValue(const ParamInfo_t *param)
{
    if (param != NULL && (param->attr & PARAM_ATTR_RANGE))
    {
#if PARAM_USE_STRING_TYPE
        if (param->type != PARAM_STRING)
#endif
        {
            memcpy(param->unCurValuePtr.pVoid, param->unMinValuePtr.pVoid, param->length);
        }

        return true;
    }

    return false;
}

static bool ResetParamMaxValue(const ParamInfo_t *param)
{
    if (param != NULL && (param->attr & PARAM_ATTR_RANGE))
    {
#if PARAM_USE_STRING_TYPE
        if (param->type != PARAM_STRING)
#endif
        {
            memcpy(param->unCurValuePtr.pVoid, param->unMaxValuePtr.pVoid, param->length);
        }

        return true;
    }

    return false;
}

/**
  * @brief      参数表初始化
  *
  * @param      manager     参数表管理句柄
  * @param      pParamTable 参数表
  * @param      count       参数表元素数目, 可以通过宏 PARAM_TABLE_SIZE 获取
  * @return     0,成功; -1,失败
  */
int Param_Init(ParamManager_t *manager, ParamInfo_t *pParamTable, uint16_t count)
{
    if (manager == NULL || pParamTable == NULL)
    {
        return -1;
    }

    for (uint16_t i = 0; i < count; i++)
    {
        if (pParamTable[i].unMaxValuePtr.pVoid == NULL || pParamTable[i].unMinValuePtr.pVoid == NULL)
        {
            pParamTable[i].attr &= ~PARAM_ATTR_RANGE;
        }

        if (!(pParamTable[i].attr & PARAM_ATTR_READ))
        {
            pParamTable[i].attr &= ~PARAM_ATTR_WRITE;
        }

        if (pParamTable[i].unDefValuePtr.pVoid == NULL)
        {
            pParamTable[i].attr &= ~PARAM_ATTR_RESET;
        }

        if (pParamTable[i].unCurValuePtr.pVoid == NULL)
        {
            return -1;
        }
    }

    manager->pParamTable = pParamTable;
    manager->count = count;
    return 0;
}

/**
  * @brief      重置所有参数，恢复为缺省值
  *
  * @attention  无可重置权限的参数不能恢复为缺省值
  * @param      manager 参数表管理句柄
  * @return     0,成功; -1,失败
  */
int Param_ResetParam(const ParamManager_t *manager)
{
    if (manager == NULL)
    {
        return -1;
    }

    for (uint16_t i = 0; i < manager->count; ++i)
    {
        ResetParamValue(&manager->pParamTable[i]);
    }

    return 0;
}

static ParamInfo_t *FindParamByParamPtr(ParamManager_t *manager, const void *curParamPtr)
{
    for (uint16_t i = 0; i < manager->count; ++i)
    {
        if (manager->pParamTable[i].unCurValuePtr.pVoid == curParamPtr)
        {
            return &manager->pParamTable[i];
        }
    }

    return NULL;
}

/**
  * @brief      参数列表迭代器
  * 
  * @attention  无读取权限的参数会自动跳过
  * @param      manager 参数表管理句柄
  * @param      psIdx   参数表起始索引
  * @return     参数信息
  */
ParamInfo_t *Param_IterateList(ParamManager_t *manager, size_t *psIdx)
{
    ParamInfo_t *p = NULL;

    if (manager == NULL || psIdx == NULL || *psIdx > manager->count)
    {
        return NULL;
    }

    while (*psIdx < manager->count)
    {
        if (manager->pParamTable[*psIdx].attr & PARAM_ATTR_READ)
        {
            p = &manager->pParamTable[*psIdx];
            (*psIdx)++;
            break;
        }

        (*psIdx)++;
    }

    return p;
}

#if (PARAM_NAME_MAX_LENGTH > 1)
static ParamInfo_t *FindParamByName(ParamManager_t *manager, const char *pszName)
{
    for (uint16_t i = 0; i < manager->count; ++i)
    {
        if (strcmp(manager->pParamTable[i].pszName, pszName) == 0)
        {
            return &manager->pParamTable[i];
        }
    }

    return NULL;
}
#endif

/**
  * @brief      根据参数名称查找参数信息
  *
  * @attention  无可读权限时会查找失败
  * @param      manager   参数表管理句柄
  * @param      pszName   参数名称
  * @return     参数信息
  */
const ParamInfo_t *Param_FindParamByName(ParamManager_t *manager, const char *pszName)
{
#if (PARAM_NAME_MAX_LENGTH > 1)
    ParamInfo_t *pInfo;

    if (manager != NULL)
    {
        pInfo = FindParamByName(manager, pszName);

        if (pInfo->attr & PARAM_ATTR_READ)
        {
            return pInfo;
        }
    }

#endif
    return NULL;
}


static ParamInfo_t *FindParamByID(ParamManager_t *manager, uint16_t id)
{
    for (uint16_t i = 0; i < manager->count; ++i)
    {
        if (manager->pParamTable[i].id == id)
        {
            return &manager->pParamTable[i];
        }
    }

    return NULL;
}

/**
  * @brief      根据参数ID查找参数信息
  *
  * @attention  无可读权限时会查找失败
  * @param      manager   参数表管理句柄
  * @param      id   参数ID
  * @return     参数信息
  */
const ParamInfo_t *Param_FindParamByID(ParamManager_t *manager, uint16_t id)
{
    ParamInfo_t *pInfo;

    if (manager != NULL)
    {
        pInfo = FindParamByID(manager, id);

        if (pInfo->attr & PARAM_ATTR_READ)
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
  * @param      manager   参数表管理句柄
  * @param      curParamPtr  当前参数数据地址指针
  * @return     参数信息
  */
const ParamInfo_t *Param_FindParamByParamPtr(ParamManager_t *manager, const void *curParamPtr)
{
    ParamInfo_t *pInfo;

    if (manager != NULL)
    {
        pInfo = FindParamByParamPtr(manager, curParamPtr);

        if (pInfo->attr & PARAM_ATTR_READ)
        {
            return pInfo;
        }
    }

    return NULL;
}


// 验证参数是否在指定范围内
static int ValidateRange(const ParamInfo_t *param, const Value_u *pval)
{
    if (!(param->attr & PARAM_ATTR_RANGE))
    {
        return 0;
    }

    switch (param->type)
    {
    case PARAM_INT8:
        if (pval->s64val < *param->unMinValuePtr.pInt8)
        {
            return 1;
        }
        else if (pval->s64val > *param->unMaxValuePtr.pInt8)
        {
            return 2;
        }
        break;

    case PARAM_INT16:
        if (pval->s64val < *param->unMinValuePtr.pInt16)
        {
            return 1;
        }
        else if (pval->s64val > *param->unMaxValuePtr.pInt16)
        {
            return 2;
        }
        break;

    case PARAM_INT32:
        if (pval->s64val < *param->unMinValuePtr.pInt32)
        {
            return 1;
        }
        else if (pval->s64val > *param->unMaxValuePtr.pInt32)
        {
            return 2;
        }
        break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_INT64:
        if (pval->s64val < *param->unMinValuePtr.pInt64)
        {
            return 1;
        }
        else if (pval->s64val > *param->unMaxValuePtr.pInt64)
        {
            return 2;
        }
        break;
#endif
    case PARAM_UINT8:
        if (pval->u64val < *param->unMinValuePtr.pUint8)
        {
            return 1;
        }
        else if (pval->u64val > *param->unMaxValuePtr.pUint8)
        {
            return 2;
        }
        break;

    case PARAM_UINT16:
        if (pval->u64val < *param->unMinValuePtr.pUint16)
        {
            return 1;
        }
        else if (pval->u64val > *param->unMaxValuePtr.pUint16)
        {
            return 2;
        }
        break;

    case PARAM_UINT32:
        if (pval->u64val < *param->unMinValuePtr.pUint32)
        {
            return 1;
        }
        else if (pval->u64val > *param->unMaxValuePtr.pUint32)
        {
            return 2;
        }
        break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_UINT64:
        if (pval->u64val < *param->unMinValuePtr.pUint64)
        {
            return 1;
        }
        else if (pval->u64val > *param->unMaxValuePtr.pUint64)
        {
            return 2;
        }
        break;
#endif
    case PARAM_FLOAT:
        if (pval->fVal < *param->unMinValuePtr.pFloat)
        {
            return 1;
        }
        else if (pval->fVal > *param->unMaxValuePtr.pFloat)
        {
            return 2;
        }
        break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_DOUBLE:
        if (pval->fVal < *param->unMinValuePtr.pDouble)
        {
            return 1;
        }
        else if (pval->fVal > *param->unMaxValuePtr.pDouble)
        {
            return 2;
        }
        break;
#endif
#if PARAM_USE_STRING_TYPE
    case PARAM_STRING:
        if (strlen(pval->str) < *param->unMinValuePtr.pStringLength)
        {
            return 1;
        }
        else if (strlen(pval->str) > *param->unMaxValuePtr.pStringLength)
        {
            return 2;
        }
        break;
#endif
    default:
        return -1;
    }

    return 0;
}

static int ValidateRangeByVoid(const ParamInfo_t *param, const void *pval)
{
    Value_u uValue;

    switch (param->type)
    {
    case PARAM_INT8:
        uValue.s64val = *(PARAM_INT8_T *)pval;
        break;

    case PARAM_INT16:
        uValue.s64val = *(PARAM_INT16_T *)pval;
        break;

    case PARAM_INT32:
        uValue.s64val = *(PARAM_INT32_T *)pval;
        break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_INT64:
        uValue.s64val = *(PARAM_INT64_T *)pval;
        break;
#endif
    case PARAM_UINT8:
        uValue.s64val = *(PARAM_UINT8_T *)pval;
        break;

    case PARAM_UINT16:
        uValue.s64val = *(PARAM_UINT16_T *)pval;
        break;

    case PARAM_UINT32:
        uValue.s64val = *(PARAM_UINT32_T *)pval;
        break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_UINT64:
        uValue.s64val = *(PARAM_UINT64_T *)pval;
        break;
#endif
    case PARAM_FLOAT:
        uValue.fVal = *(PARAM_FLOAT_T *)pval;
        break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_DOUBLE:
        uValue.fVal = *(PARAM_DOUBLE_T *)pval;
        break;
#endif
#if PARAM_USE_STRING_TYPE
    case PARAM_STRING:
        memcpy(uValue.str, pval, strlen(pval) > PARAM_STRING_MAX_LENGTH ? PARAM_STRING_MAX_LENGTH + 2 : strlen(pval) + 1);
        uValue.str[PARAM_STRING_MAX_LENGTH + 2] = '\0';
        break;
#endif
    default:
        return -1;
    }

    return ValidateRange(param, &uValue);
}

/**
  * @brief      检查参数最新值的范围并对超出范围时进行处理
  * 
  * @param      param 参数信息
  * @param      opt   超出范围的处理选项：
  *                   @arg PARAM_NONE, 参数不变, 即不做处理
  *                   @arg PARAM_DEF,  参数恢复默认
  *                   @arg PARAM_MIN_MAX, 参数小于最小值则为最小值, 参数大于最大值则为最大值; 但对字符串类型参数该选项无效, 即不做处理
  * @return     0,正常; 1,参数小于最小值(字符串长度小于最小限制长度); 2,参数大于最大值(字符串长度大于最大限制长度); <0,错误
  */
int Param_CheckRange(const ParamInfo_t *param, uint8_t opt)
{
    int ret;

    if (param == NULL)
    {
        return -1; // 参数验证失败
    }

    ret = ValidateRangeByVoid(param, param->unCurValuePtr.pVoid);

    if (ret != 0)
    {
        if (opt == PARAM_DEF)
        {
            ResetParamValue(param);
        }
        else if (opt == PARAM_MIN_MAX)
        {
            ret == 1 ? ResetParamMinValue(param) : ResetParamMaxValue(param);
        }

        return ret;
    }

    return 0;
}

/**
  * @brief      设置新的参数值
  *
  * @param      param   参数信息
  * @param      value   新的参数值
  * @param      opt   超出范围的处理选项：
  *                   @arg PARAM_NONE, 参数不变, 即不做处理
  *                   @arg PARAM_DEF,  参数恢复默认
  *                   @arg PARAM_MIN_MAX, 参数小于最小值则为最小值, 参数大于最大值则为最大值; 但对字符串类型参数该选项无效, 即不做处理
  * @return     0,正常; 1,参数小于最小值(字符串长度小于最小限制长度); 2,参数大于最大值(字符串长度大于最大限制长度); <0,错误
  */
int Param_SetNewValue(const ParamInfo_t *param, const void *value, uint8_t opt)
{
    int ret;

    if (param == NULL)
    {
        return -1;
    }

    ret = ValidateRangeByVoid(param, value);

    if (ret != 0)
    {
        if (opt == PARAM_DEF)
        {
            ResetParamValue(param);
        }
        else if (opt == PARAM_MIN_MAX)
        {
            ret == 1 ? ResetParamMinValue(param) : ResetParamMaxValue(param);
        }

        return ret;
    }
#if PARAM_USE_STRING_TYPE
    if (param->type != PARAM_STRING)
    {
        memcpy(param->unCurValuePtr.pVoid, value, param->length);
    }
    else
    {
        strcpy(param->unCurValuePtr.pString, value);
    }
#else
    memcpy(param->unCurValuePtr.pVoid, value, param->length);
#endif
    return 0;
}

/**
  * @brief      重置参数为缺省值
  *
  * @attention  无可重置权限的参数不能恢复为缺省值
  * @param      param 参数信息
  * @return     true   成功
  * @return     false  失败
  */
bool Param_ResetDefaultValue(const ParamInfo_t *param)
{
    return ResetParamValue(param);
}

/**
  * @brief      重置参数为最小值
  * 
  * @attention  字符串类型参数该功能无效
  * @param      param 参数信息
  * @return     true   成功
  * @return     false  失败
  */
bool Param_ResetMinValue(const ParamInfo_t *param)
{
    return ResetParamMinValue(param);
}

/**
  * @brief      重置参数为最大值
  * 
  * @attention  字符串类型参数该功能无效
  * @param      param 参数信息
  * @return     true   成功
  * @return     false  失败
  */
bool Param_ResetMaxValue(const ParamInfo_t *param)
{
    return ResetParamMaxValue(param);
}

static uint16_t ParamInfoFormStream(ParamInfo_t *param, const uint8_t *pbuf)
{
    switch (param->type)
    {
    case PARAM_INT8:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, param->length);
        *param->unCurValuePtr.pInt8 = (PARAM_INT8_T)val;
    }
    break;

    case PARAM_INT16:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, param->length);
        *param->unCurValuePtr.pInt16 = (PARAM_INT16_T)val;
    }
    break;

    case PARAM_INT32:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, param->length);
        *param->unCurValuePtr.pInt32 = (PARAM_INT32_T)val;
    }
    break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_INT64:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, param->length);
        *param->unCurValuePtr.pInt64 = (PARAM_INT64_T)val;
    }
    break;
#endif
    case PARAM_UINT8:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, param->length);
        *param->unCurValuePtr.pUint8 = (PARAM_UINT8_T)val;
    }
    break;

    case PARAM_UINT16:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, param->length);
        *param->unCurValuePtr.pUint16 = (PARAM_UINT16_T)val;
    }
    break;

    case PARAM_UINT32:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, param->length);
        *param->unCurValuePtr.pUint32= (PARAM_UINT32_T)val;
    }
    break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_UINT64:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, param->length);
        *param->unCurValuePtr.pUint64 = (PARAM_UINT64_T)val;
    }
    break;
#endif
    case PARAM_FLOAT:
        pbuf = UnSerializeFloat(pbuf, param->unCurValuePtr.pFloat);
        break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_DOUBLE:
        pbuf = UnSerializeDouble(pbuf, param->unCurValuePtr.pDouble);
        break;
#endif
#if PARAM_USE_STRING_TYPE
    case PARAM_STRING:
        memcpy(param->unCurValuePtr.pString, &pbuf[0], param->length);
        break;
#endif
    default:
        return 0; // 不支持的参数类型
    }

    return param->length;
}

static uint8_t *MoveBufToBase(uint8_t *pbuf, uint32_t length)
{
    memmove(pbuf - length, pbuf, length);
    return (pbuf - length);
}

/**
  * @brief      加载数据
  *
  * @note       该操作会校验参数的合法性（取值范围）
  * @param      manager 参数表管理句柄
  * @param      pfnLoadCallback 加载回调函数
  * @param      pfnCheckError   参数不合法时触发回调函数
  * @return     0,成功; -1,失败
  */
int Param_Load(ParamManager_t *manager, pfnLoad_cb pfnLoadCallback, pfnCheckError_cb pfnCheckError)
{
#if PARAM_USE_STRING_TYPE
    uint8_t buf[sizeof(ParamInfo_t) + PARAM_STRING_MAX_LENGTH];
#else
    uint8_t buf[sizeof(ParamInfo_t)];
#endif
    uint8_t *ptr = buf;

    if (manager == NULL || pfnLoadCallback == NULL)
    {
        return -1;
    }

    bool isFinish = false;
    uint16_t length = 0;
    uint16_t paramLength = 0;
    uint16_t id = 0;
    ParamInfo_t *pParamInfo;
    uint8_t keyLength = 0;
#if PARAM_USE_KEY_VALUE
    uint64_t key = 0;
#endif

    do
    {
        length = sizeof(buf) - (ptr - buf);

        if (pfnLoadCallback(ptr, &length, &isFinish) != 0)
        {
            return -2;
        }

        if (isFinish || length == 0)
        {
            continue;
        }

        length += (ptr - buf);
        ptr = buf;

#if PARAM_USE_KEY_VALUE
        if (keyLength == 0)
        {
            keyLength = ptr[0];
            ptr++;
            length -= 1;
        }

        while (length > keyLength)
        {
            UnSerializeUint(ptr, &key, keyLength);
            
#if PARAM_SUPPORT_NUM == PARAM_SUPPORT_16
            id = (key >> 4) & 0x0F;
            paramLength = key & 0x0F;
#elif PARAM_SUPPORT_NUM == PARAM_SUPPORT_256
            id = (key >> 8) & 0xFF;
            paramLength = key & 0xFF;
#elif PARAM_SUPPORT_NUM == PARAM_SUPPORT_4096
            id = (key >> 12) & 0xFFF;
            paramLength = key & 0xFFF;
#endif

            if (length < (paramLength + keyLength))
            {
                break;
            }

            ptr += keyLength;
            length -= keyLength;

            pParamInfo = (ParamInfo_t *)FindParamByID(manager, id);

            if (pParamInfo == NULL || paramLength != pParamInfo->length)
            {
                ptr += paramLength;
                length -= paramLength;
                continue;
            }

            ParamInfoFormStream(pParamInfo, ptr);
            ptr += paramLength;
            length -= paramLength;

            if (0 != ValidateRangeByVoid(pParamInfo, pParamInfo->unCurValuePtr.pVoid))
            {
                if (pfnCheckError != NULL)
                {
                    pfnCheckError(pParamInfo);
                }
            }
        }

#else

        while (length >= 1)
        {
            pParamInfo = &manager->pParamTable[id];

            if (length < pParamInfo->length)
            {
                break;
            }

            ParamInfoFormStream(pParamInfo, ptr);
            ptr += pParamInfo->length;

            if (0 != ValidateRangeByVoid(pParamInfo, pParamInfo->unCurValuePtr.pVoid))
            {
                if (pfnCheckError != NULL)
                {
                    pfnCheckError(pParamInfo);
                }
            }

            length -= pParamInfo->length;
            id++;
        }

#endif
        ptr = MoveBufToBase(ptr, ptr - buf);
        ptr += length;
    } while (!isFinish);

    return 0;
}

static uint16_t ParamInfoToStream(uint8_t *pbuf, ParamInfo_t *param)
{
    switch (param->type)
    {
    case PARAM_INT8:
        pbuf = SerializeInt(pbuf, *(PARAM_INT8_T *)param->unCurValuePtr.pVoid, param->length);
        break;

    case PARAM_INT16:
        pbuf = SerializeInt(pbuf, *(PARAM_INT16_T *)param->unCurValuePtr.pVoid, param->length);
        break;

    case PARAM_INT32:
        pbuf = SerializeInt(pbuf, *(PARAM_INT32_T *)param->unCurValuePtr.pVoid, param->length);
        break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_INT64:
        pbuf = SerializeInt(pbuf, *(PARAM_INT64_T *)param->unCurValuePtr.pVoid, param->length);
        break;
#endif
    case PARAM_UINT8:
        pbuf = SerializeUint(pbuf, *(PARAM_UINT8_T *)param->unCurValuePtr.pVoid, param->length);
        break;

    case PARAM_UINT16:
        pbuf = SerializeUint(pbuf, *(PARAM_UINT16_T *)param->unCurValuePtr.pVoid, param->length);
        break;

    case PARAM_UINT32:
        pbuf = SerializeUint(pbuf, *(PARAM_UINT32_T *)param->unCurValuePtr.pVoid, param->length);
        break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_UINT64:
        pbuf = SerializeUint(pbuf, *(PARAM_UINT64_T *)param->unCurValuePtr.pVoid, param->length);
        break;
#endif
    case PARAM_FLOAT:
        pbuf = SerializeFloat(pbuf, *(PARAM_FLOAT_T *)param->unCurValuePtr.pVoid);
        break;
#if PARAM_USE_64_BIT_LENGTH
    case PARAM_DOUBLE:
        pbuf = SerializeDouble(pbuf, *(PARAM_DOUBLE_T *)param->unCurValuePtr.pVoid);
        break;
#endif
#if PARAM_USE_STRING_TYPE
    case PARAM_STRING:
        memcpy(&pbuf[0], param->unCurValuePtr.pString, param->length);
        break;
#endif
    default:
        return 0; // 不支持的参数类型
    }

    return param->length;
}

/**
  * @brief      保存数据
  *
  * @note       参数在保存时会对参数校验合法性（取值范围）
  * @param      manager 参数表管理句柄
  * @param      pfnSaveCallback 保存回调函数
  * @param      pfnCheckError   参数不合法时触发回调函数
  * @return     0,成功; -1,失败
  */
int Param_Save(ParamManager_t *manager, pfnSave_cb pfnSaveCallback, pfnCheckError_cb pfnCheckError)
{
#if PARAM_USE_STRING_TYPE
    uint8_t buf[sizeof(ParamInfo_t) + PARAM_STRING_MAX_LENGTH];
#else
    uint8_t buf[sizeof(ParamInfo_t)];
#endif
    uint8_t *ptr = buf;
    uint16_t length = 0;
#if PARAM_USE_KEY_VALUE
    uint64_t key = 0;
#endif

    if (manager == NULL || pfnSaveCallback == NULL)
    {
        return -1;
    }

#if PARAM_USE_KEY_VALUE
    buf[0] = PARAM_SUPPORT_NUM;

    if (pfnSaveCallback(buf, 1, false) != 0)
    {
        return -2;
    }
#endif

    for (int i = 0; i < manager->count; i++)
    {
        ptr = buf;
        length = 0;
#if PARAM_USE_KEY_VALUE
#if PARAM_SUPPORT_NUM == PARAM_SUPPORT_16
        key = (manager->pParamTable[i].id << 4) | (manager->pParamTable[i].length & 0x0F);
#elif PARAM_SUPPORT_NUM == PARAM_SUPPORT_256
        key = (manager->pParamTable[i].id << 8) | (manager->pParamTable[i].length & 0xFF);
#elif PARAM_SUPPORT_NUM == PARAM_SUPPORT_4096
        key = (manager->pParamTable[i].id << 12) | (manager->pParamTable[i].length & 0xFFF);
#endif
        ptr = SerializeUint(ptr, key, PARAM_SUPPORT_NUM);
        length += PARAM_SUPPORT_NUM;
#endif
        if (0 != ValidateRangeByVoid(&manager->pParamTable[i], manager->pParamTable[i].unCurValuePtr.pVoid))
        {
            if (pfnCheckError != NULL)
            {
                pfnCheckError(&manager->pParamTable[i]);
            }
        }

        length = ParamInfoToStream(&buf[length], &manager->pParamTable[i]);
        ptr += length;

        if (pfnSaveCallback(buf, (ptr - buf), false) != 0)
        {
            return -2;
        }
    }

    if (pfnSaveCallback(buf, 0, true) != 0)
    {
        return -2;
    }

    return 0;
}




