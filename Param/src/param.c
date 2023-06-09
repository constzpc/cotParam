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


#define CASE_CHECK_RANGE(type, cur_ptr, min_ptr, max_ptr)   \
    case type:\
    {\
        type##_T cur = *(const type##_T *)cur_ptr;\
        type##_T min = min_ptr != NULL ? *(const type##_T *)min_ptr : 0;\
        type##_T max = max_ptr != NULL ? *(const type##_T *)max_ptr : 0;\
        ret = (cur < min || cur > max) ? false : true;\
        break;\
    }

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
    if (param != NULL && param->attr & PARAM_ATTR_RESET)
    {
        if (param->type != PARAM_STARING)
        {
            memcpy(param->pCurValue, param->pDefValue, param->length);
        }
        else
        {
            strcpy(param->pCurValue, param->pDefValue);
        }

        if (param->pfnCallback != NULL)
        {
            param->pfnCallback(param);
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
        if (pParamTable[i].pMaxValue == NULL || pParamTable[i].pMinValue == NULL)
        {
            pParamTable[i].attr &= ~PARAM_ATTR_RANGE;
        }

        if (!(pParamTable[i].attr & PARAM_ATTR_READ))
        {
            pParamTable[i].attr &= ~PARAM_ATTR_WRITE;
        }

        if (pParamTable[i].pDefValue == NULL)
        {
            pParamTable[i].attr &= ~PARAM_ATTR_RESET;
        }

        if (pParamTable[i].pCurValue == NULL)
        {
            return -1;
        }
    }

    manager->pParamTable = pParamTable;
    manager->count = count;
    return 0;
}

/**
  * @brief      设置参数加载和保存时调用的回调函数
  *
  * @param      manager 参数表管理句柄
  * @param      pfnLoadCallback 加载回调函数
  * @param      pfnSaveCallback 保存回调函数
  * @return     0,成功; -1,失败
  */
int Param_SetCallBackFun(ParamManager_t *manager, pfnLoad_cb pfnLoadCallback, pfnSave_cb pfnSaveCallback)
{
    if (manager == NULL)
    {
        return -1;
    }

    manager->pfnLoadCallback = pfnLoadCallback;
    manager->pfnSaveCallback = pfnSaveCallback;
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
        if (manager->pParamTable[i].pCurValue == curParamPtr)
        {
            return &manager->pParamTable[i];
        }
    }

    return NULL;
}

extern bool Param_SetParam(ParamManager_t *manager, const void *curParamPtr, const void *newValue)
{
    ParamInfo_t *pInfo;

    if (manager != NULL)
    {
        pInfo = FindParamByParamPtr(manager, curParamPtr);

        if (pInfo->attr & PARAM_ATTR_WRITE)
        {
            if (pInfo->type != PARAM_STARING)
            {
                memcpy(pInfo->pCurValue, newValue, pInfo->length);
            }
            else
            {
                strcpy(pInfo->pCurValue, newValue);
            }

            return true;
        }
    }

    return false;
}

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
    ParamInfo_t *pInfo;

    if (manager != NULL)
    {
        pInfo = FindParamByName(manager, pszName);

        if (pInfo->attr & PARAM_ATTR_READ)
        {
            return pInfo;
        }
    }

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

/**
  * @brief      获取参数的当前值
  *
  * @param      param 参数信息
  * @return     当前值的地址指针
  */
const void *Param_GetParamValue(const ParamInfo_t *param)
{
    if (param == NULL)
    {
        return NULL;
    }

    return (const void *)param->pCurValue;
}

static void ParamInfoToParamMemInfo(ParamMemInfo_t *pParamMem, const ParamInfo_t *param)
{
    memset(pParamMem, 0, sizeof(ParamMemInfo_t));
    pParamMem->id = param->id;
    pParamMem->type = param->type;
    pParamMem->attr = param->attr;
    pParamMem->length = param->length;

    if (param->pszName != NULL)
    {
        size_t len = strlen(param->pszName) > param->length ? param->length - 1 : strlen(param->pszName);

        memcpy(pParamMem->szName, param->pszName, len);
    }

    switch (param->type)
    {
    case PARAM_INT8:
        pParamMem->curValue.s8val = *(const int8_t *)param->pCurValue;
        pParamMem->minValue.s8val = param->pMinValue != NULL ? *(const int8_t *)param->pMinValue : 0;
        pParamMem->maxValue.s8val = param->pMaxValue != NULL ? *(const int8_t *)param->pMaxValue : 0;
        pParamMem->defValue.s8val = param->pDefValue != NULL ? *(const int8_t *)param->pDefValue : 0;
        break;

    case PARAM_INT16:
        pParamMem->curValue.s16val = *(const int16_t *)param->pCurValue;
        pParamMem->minValue.s16val = param->pMinValue != NULL ? *(const int16_t *)param->pMinValue : 0;
        pParamMem->maxValue.s16val = param->pMaxValue != NULL ? *(const int16_t *)param->pMaxValue : 0;
        pParamMem->defValue.s16val = param->pDefValue != NULL ? *(const int16_t *)param->pDefValue : 0;
        break;

    case PARAM_INT32:
        pParamMem->curValue.s32val = *(const int32_t *)param->pCurValue;
        pParamMem->minValue.s32val = param->pMinValue != NULL ? *(const int32_t *)param->pMinValue : 0;
        pParamMem->maxValue.s32val = param->pMaxValue != NULL ? *(const int32_t *)param->pMaxValue : 0;
        pParamMem->defValue.s32val = param->pDefValue != NULL ? *(const int32_t *)param->pDefValue : 0;
        break;

    case PARAM_INT64:
        pParamMem->curValue.s64val = *(const int64_t *)param->pCurValue;
        pParamMem->minValue.s64val = param->pMinValue != NULL ? *(const int64_t *)param->pMinValue : 0;
        pParamMem->maxValue.s64val = param->pMaxValue != NULL ? *(const int64_t *)param->pMaxValue : 0;
        pParamMem->defValue.s64val = param->pDefValue != NULL ? *(const int64_t *)param->pDefValue : 0;
        break;

    case PARAM_UINT8:
        pParamMem->curValue.u8val = *(const uint8_t *)param->pCurValue;
        pParamMem->minValue.u8val = param->pMinValue != NULL ? *(const uint8_t *)param->pMinValue : 0;
        pParamMem->maxValue.u8val = param->pMaxValue != NULL ? *(const uint8_t *)param->pMaxValue : 0;
        pParamMem->defValue.u8val = param->pDefValue != NULL ? *(const uint8_t *)param->pDefValue : 0;
        break;

    case PARAM_UINT16:
        pParamMem->curValue.u16val = *(const uint16_t *)param->pCurValue;
        pParamMem->minValue.u16val = param->pMinValue != NULL ? *(const uint16_t *)param->pMinValue : 0;
        pParamMem->maxValue.u16val = param->pMaxValue != NULL ? *(const uint16_t *)param->pMaxValue : 0;
        pParamMem->defValue.u16val = param->pDefValue != NULL ? *(const uint16_t *)param->pDefValue : 0;
        break;

    case PARAM_UINT32:
        pParamMem->curValue.u32val = *(const uint32_t *)param->pCurValue;
        pParamMem->minValue.u32val = param->pMinValue != NULL ? *(const uint32_t *)param->pMinValue : 0;
        pParamMem->maxValue.u32val = param->pMaxValue != NULL ? *(const uint32_t *)param->pMaxValue : 0;
        pParamMem->defValue.u32val = param->pDefValue != NULL ? *(const uint32_t *)param->pDefValue : 0;
        break;

    case PARAM_UINT64:
        pParamMem->curValue.u64val = *(const uint64_t *)param->pCurValue;
        pParamMem->minValue.u64val = param->pMinValue != NULL ? *(const uint64_t *)param->pMinValue : 0;
        pParamMem->maxValue.u64val = param->pMaxValue != NULL ? *(const uint64_t *)param->pMaxValue : 0;
        pParamMem->defValue.u64val = param->pDefValue != NULL ? *(const uint64_t *)param->pDefValue : 0;
        break;

    case PARAM_FLOAT:
        pParamMem->curValue.fval = *(const PARAM_FLOAT_T *)param->pCurValue;
        pParamMem->minValue.fval = param->pMinValue != NULL ? *(const PARAM_FLOAT_T *)param->pMinValue : 0;
        pParamMem->maxValue.fval = param->pMaxValue != NULL ? *(const PARAM_FLOAT_T *)param->pMaxValue : 0;
        pParamMem->defValue.fval = param->pDefValue != NULL ? *(const PARAM_FLOAT_T *)param->pDefValue : 0;
        break;

    case PARAM_DOUBLE:
        pParamMem->curValue.dval = *(const PARAM_DOUBLE_T *)param->pCurValue;
        pParamMem->minValue.dval = param->pMinValue != NULL ? *(const PARAM_DOUBLE_T *)param->pMinValue : 0;
        pParamMem->maxValue.dval = param->pMaxValue != NULL ? *(const PARAM_DOUBLE_T *)param->pMaxValue : 0;
        pParamMem->defValue.dval = param->pDefValue != NULL ? *(const PARAM_DOUBLE_T *)param->pDefValue : 0;
        break;

    case PARAM_STARING:
        memcpy(pParamMem->curValue.szVal, param->pCurValue, strlen(param->pCurValue) >= param->length ? param->length - 1 : strlen(param->pCurValue));

        pParamMem->minValue.u64val = param->pMinValue != NULL ? *(const param_size_t *)param->pMinValue : 0;
        pParamMem->maxValue.u64val = param->pMaxValue != NULL ? *(const param_size_t *)param->pMaxValue : 0;

        if (param->pDefValue != NULL)
        {
            memcpy(pParamMem->defValue.szVal, param->pDefValue, strlen(param->pDefValue) >= param->length ? param->length - 1 : strlen(param->pDefValue));
        }

        break;

    default:
        break;
    }
}

static void ParamInfoFromParamMemInfo(ParamInfo_t *param, const ParamMemInfo_t *pParamMem)
{
    // param->id = pParamMem->id;
    // param->attr = pParamMem->attr;
    param->type = pParamMem->type;
    // param->length = pParamMem->length;

    switch (pParamMem->type)
    {
    case PARAM_INT8:
        *(int8_t *)param->pCurValue = pParamMem->curValue.s8val;
        break;

    case PARAM_INT16:
        *(int16_t *)param->pCurValue = pParamMem->curValue.s16val;
        break;

    case PARAM_INT32:
        *(int32_t *)param->pCurValue = pParamMem->curValue.s32val;
        break;

    case PARAM_INT64:
        *(int64_t *)param->pCurValue = pParamMem->curValue.s64val;
        break;

    case PARAM_UINT8:
        *(uint8_t *)param->pCurValue = pParamMem->curValue.u8val;
        break;

    case PARAM_UINT16:
        *(uint16_t *)param->pCurValue = pParamMem->curValue.u16val;
        break;

    case PARAM_UINT32:
        *(uint32_t *)param->pCurValue = pParamMem->curValue.u32val;
        break;

    case PARAM_UINT64:
        *(uint64_t *)param->pCurValue = pParamMem->curValue.u64val;
        break;

    case PARAM_FLOAT:
        *(PARAM_FLOAT_T *)param->pCurValue = pParamMem->curValue.fval;
        break;

    case PARAM_DOUBLE:
        *(PARAM_DOUBLE_T *)param->pCurValue = pParamMem->curValue.dval;
        break;

    case PARAM_STARING:
        if (strlen(pParamMem->curValue.szVal) >= param->length)
        {
            memcpy(param->pCurValue, pParamMem->curValue.szVal, param->length - 1);
        }
        else
        {
            memcpy(param->pCurValue, pParamMem->curValue.szVal, strlen(pParamMem->curValue.szVal));
        }

        break;

    default:
        break;
    }
}


// 验证参数是否在指定范围内
bool ValidateIntRange(const ParamInfo_t *param, const void *pCurValue)
{
    bool ret = false;

    if (!(param->attr & PARAM_ATTR_RANGE))
    {
        return true;
    }

    switch (param->type)
    {
        CASE_CHECK_RANGE(PARAM_INT8, pCurValue, param->pMinValue, param->pMaxValue);
        CASE_CHECK_RANGE(PARAM_INT16, pCurValue, param->pMinValue, param->pMaxValue);
        CASE_CHECK_RANGE(PARAM_INT32, pCurValue, param->pMinValue, param->pMaxValue);
        CASE_CHECK_RANGE(PARAM_INT64, pCurValue, param->pMinValue, param->pMaxValue);
        CASE_CHECK_RANGE(PARAM_UINT8, pCurValue, param->pMinValue, param->pMaxValue);
        CASE_CHECK_RANGE(PARAM_UINT16, pCurValue, param->pMinValue, param->pMaxValue);
        CASE_CHECK_RANGE(PARAM_UINT32, pCurValue, param->pMinValue, param->pMaxValue);
        CASE_CHECK_RANGE(PARAM_UINT64, pCurValue, param->pMinValue, param->pMaxValue);
        CASE_CHECK_RANGE(PARAM_FLOAT, pCurValue, param->pMinValue, param->pMaxValue);
        CASE_CHECK_RANGE(PARAM_DOUBLE, pCurValue, param->pMinValue, param->pMaxValue);

    case PARAM_STARING:
        if (strlen(pCurValue) < param->length)
        {
            ret = true;
        }

        break;

    default:
        return false; // 不支持的参数类型
    }

    return ret;
}

/**
  * @brief      设置参数的当前值
  *
  * @param      param   参数信息
  * @param      value   当前值
  * @return     true   成功
  * @return     false  失败
  */
bool Param_SetParamValue(const ParamInfo_t *param, const void *value)
{
    if (param == NULL || !ValidateIntRange(param, value))
    {
        return false; // 参数验证失败
    }

    if (param->type != PARAM_STARING)
    {
        memcpy(param->pCurValue, value, param->length);
    }
    else
    {
        strcpy(param->pCurValue, value);
    }

    if (param->pfnCallback != NULL)
    {
        param->pfnCallback(param);
    }

    return true;
}

/**
  * @brief      重置参数恢复位缺省值
  *
  * @param      param 参数信息
  * @return     true   成功
  * @return     false  失败
  */
bool Param_ResetParamValue(const ParamInfo_t *param)
{
    if (param != NULL && (param->attr & PARAM_ATTR_RESET))
    {
        if (param->type != PARAM_STARING)
        {
            memcpy(param->pCurValue, param->pDefValue, param->length);
        }
        else
        {
            strcpy(param->pCurValue, param->pDefValue);
        }

        if (param->pfnCallback != NULL)
        {
            param->pfnCallback(param);
        }

        return true;
    }

    return false;
}

static int UintParamPrintf(char *pout, uint8_t attr, uint64_t data, uint64_t def, uint64_t min, uint64_t max)
{
    char *p = pout;
    p += sprintf(p, "\tcur:%u\n", data);

    if (attr & PARAM_ATTR_RESET)
    {
        p += sprintf(p, "\tdef:%u\n", def);
    }

    if (attr & PARAM_ATTR_RANGE)
    {
        p += sprintf(p, "\tmin:%u\n", min);
        p += sprintf(p, "\tmax:%u\n", max);
    }

    return p - pout;
}

static int IntParamPrintf(char *pout, uint8_t attr, int64_t data, int64_t def, int64_t min, int64_t max)
{
    char *p = pout;
    p += sprintf(p, "\tcur:%d\n", data);

    if (attr & PARAM_ATTR_RESET)
    {
        p += sprintf(p, "\tdef:%d\n", def);
    }

    if (attr & PARAM_ATTR_RANGE)
    {
        p += sprintf(p, "\tmin:%d\n", min);
        p += sprintf(p, "\tmax:%d\n", max);
    }

    return p - pout;
}

static int DoubleParamPrintf(char *pout, uint8_t attr, double data, double def, double min, double max)
{
    char *p = pout;
    p += sprintf(p, "\tcur:%f\n", data);

    if (attr & PARAM_ATTR_RESET)
    {
        p += sprintf(p, "\tdef:%f\n", def);
    }

    if (attr & PARAM_ATTR_RANGE)
    {
        p += sprintf(p, "\tmin:%f\n", min);
        p += sprintf(p, "\tmax:%f\n", max);
    }

    return p - pout;
}

/**
  * @brief      参数序列化为字符串
  *
  * @param[out] pout      输出buf
  * @param[in]  paramInfo 参数信息
  * @return     0,成功; -1,失败
  */
int Param_Sprintf(char *pout, const ParamInfo_t *paramInfo)
{
    char *p = pout;
    ParamMemInfo_t pParamMem;

    if (pout == NULL || paramInfo == NULL)
    {
        return -1;
    }

    p += sprintf(p, "[%d]:\n", paramInfo->id);
    p += sprintf(p, "\tname:%s\n", paramInfo->pszName);
    p += sprintf(p, "\ttype:%d\n", paramInfo->type);
    p += sprintf(p, "\tlength:%d\n", paramInfo->length);
    p += sprintf(p, "\tattr:");

    if (paramInfo->attr & PARAM_ATTR_WRITE)
    {
        p += sprintf(p, "w");
    }

    if (paramInfo->attr & PARAM_ATTR_READ)
    {
        p += sprintf(p, "r");
    }

    if (paramInfo->attr & PARAM_ATTR_RESET)
    {
        p += sprintf(p, "s");
    }

    if (paramInfo->attr & PARAM_ATTR_RANGE)
    {
        p += sprintf(p, "m");
    }

    p += sprintf(p, "\n");

    ParamInfoToParamMemInfo(&pParamMem, paramInfo);

    switch (pParamMem.type)
    {
    case PARAM_INT8:
        p += IntParamPrintf(p, paramInfo->attr, pParamMem.curValue.s8val, pParamMem.defValue.s8val, pParamMem.minValue.s8val, pParamMem.maxValue.s8val);
        break;

    case PARAM_INT16:
        p += IntParamPrintf(p, paramInfo->attr, pParamMem.curValue.s16val, pParamMem.defValue.s16val, pParamMem.minValue.s16val, pParamMem.maxValue.s16val);
        break;

    case PARAM_INT32:
        p += IntParamPrintf(p, paramInfo->attr, pParamMem.curValue.s32val, pParamMem.defValue.s32val, pParamMem.minValue.s32val, pParamMem.maxValue.s32val);
        break;

    case PARAM_INT64:
        p += IntParamPrintf(p, paramInfo->attr, pParamMem.curValue.s64val, pParamMem.defValue.s64val, pParamMem.minValue.s64val, pParamMem.maxValue.s64val);
        break;

    case PARAM_UINT8:
        p += UintParamPrintf(p, paramInfo->attr, pParamMem.curValue.u8val, pParamMem.defValue.u8val, pParamMem.minValue.u8val, pParamMem.maxValue.u8val);
        break;

    case PARAM_UINT16:
        p += UintParamPrintf(p, paramInfo->attr, pParamMem.curValue.u16val, pParamMem.defValue.u16val, pParamMem.minValue.u16val, pParamMem.maxValue.u16val);
        break;

    case PARAM_UINT32:
        p += UintParamPrintf(p, paramInfo->attr, pParamMem.curValue.u32val, pParamMem.defValue.u32val, pParamMem.minValue.u32val, pParamMem.maxValue.u32val);
        break;

    case PARAM_UINT64:
        p += UintParamPrintf(p, paramInfo->attr, pParamMem.curValue.u64val, pParamMem.defValue.u64val, pParamMem.minValue.u64val, pParamMem.maxValue.u64val);
        break;

    case PARAM_FLOAT:
        p += DoubleParamPrintf(p, paramInfo->attr, pParamMem.curValue.fval, pParamMem.defValue.fval, pParamMem.minValue.fval, pParamMem.maxValue.fval);
        break;

    case PARAM_DOUBLE:
        p += DoubleParamPrintf(p, paramInfo->attr, pParamMem.curValue.dval, pParamMem.defValue.dval, pParamMem.minValue.dval, pParamMem.maxValue.dval);
        break;

    case PARAM_STARING:
        p += sprintf(p, "\tcur:%s\n", pParamMem.curValue.szVal);

        if (paramInfo->attr & PARAM_ATTR_RESET)
        {
            p += sprintf(p, "\tdef:%s\n", pParamMem.defValue.szVal);
        }

        if (paramInfo->attr & PARAM_ATTR_RANGE)
        {
            p += sprintf(p, "\tmin:%d\n", pParamMem.minValue.u64val);
            p += sprintf(p, "\tmax:%d\n", pParamMem.maxValue.u64val);
        }

        break;

    default:
        break;
    }

    return 0;
}

static uint16_t ParamInfoFormStream(ParamInfo_t *param, const uint8_t *pbuf)
{
    switch (param->type)
    {
    case PARAM_INT8:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, param->length);
        *(PARAM_INT8_T *)param->pCurValue = (PARAM_INT8_T)val;
    }
    break;

    case PARAM_INT16:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, param->length);
        *(PARAM_INT16_T *)param->pCurValue = (PARAM_INT16_T)val;
    }
    break;

    case PARAM_INT32:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, param->length);
        *(PARAM_INT32_T *)param->pCurValue = (PARAM_INT32_T)val;
    }
    break;

    case PARAM_INT64:
    {
        int64_t val = 0;
        pbuf = UnSerializeInt(pbuf, &val, param->length);
        *(PARAM_INT64_T *)param->pCurValue = (PARAM_INT64_T)val;
    }
    break;

    case PARAM_UINT8:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, param->length);
        *(PARAM_UINT8_T *)param->pCurValue = (PARAM_UINT8_T)val;
    }
    break;

    case PARAM_UINT16:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, param->length);
        *(PARAM_UINT16_T *)param->pCurValue = (PARAM_UINT16_T)val;
    }
    break;

    case PARAM_UINT32:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, param->length);
        *(PARAM_UINT32_T *)param->pCurValue = (PARAM_UINT32_T)val;
    }
    break;

    case PARAM_UINT64:
    {
        uint64_t val = 0;
        pbuf = UnSerializeUint(pbuf, &val, param->length);
        *(PARAM_UINT64_T *)param->pCurValue = (PARAM_UINT64_T)val;
    }
    break;

    case PARAM_FLOAT:
        pbuf = UnSerializeFloat(pbuf, (PARAM_FLOAT_T *)param->pCurValue);
        break;

    case PARAM_DOUBLE:
        pbuf = UnSerializeDouble(pbuf, (PARAM_DOUBLE_T *)param->pCurValue);
        break;

    case PARAM_STARING:
        memcpy(param->pCurValue, &pbuf[0], PARAM_STRING_MAX_LENGTH);
        break;

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
  * @attention  使用前确保函数 Param_SetCallBackFun 设置了加载回调处理函数
  * @note       该操作会校验参数的合法性（取值范围）
  * @param      manager 参数表管理句柄
  * @param      pfnCheckError   参数不合法时触发回调函数
  * @return     0,成功; -1,失败
  */
int Param_Load(ParamManager_t *manager, pfnCheckError_cb pfnCheckError)
{
    uint8_t buf[sizeof(ParamVal_u) + 5];
    uint8_t *ptr = buf;

    if (manager == NULL || manager->pfnSaveCallback == NULL)
    {
        return -1;
    }

    bool isFinish = false;
    uint16_t length = 0;
    uint16_t paramLength = 0;
    uint16_t id = 0;
    ParamInfo_t *pParamInfo;
#if PARAM_USE_KEY_VALUE
    uint64_t key = 0;
#endif

    do
    {
        length = sizeof(buf) - (ptr - buf);

        if (manager->pfnLoadCallback(ptr, &length, &isFinish) != 0)
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

        while (length >= 3)
        {
            ptr = UnSerializeUint(ptr, &key, PARAM_SUPPORT_NUM);
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

            if (length < (paramLength + PARAM_SUPPORT_NUM))
            {
                break;
            }

            pParamInfo = (ParamInfo_t *)FindParamByID(manager, id);

            if (pParamInfo == NULL)
            {
                ptr += paramLength;
                length -= paramLength;
                continue;
            }

            ParamInfoFormStream(pParamInfo, ptr);
            ptr += paramLength;
            length -= paramLength;

            if (!ValidateIntRange(pParamInfo, pParamInfo->pCurValue))
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

            if (!ValidateIntRange(pParamInfo, pParamInfo->pCurValue))
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
    } while (!isFinish);

    return 0;
}

static uint16_t ParamInfoToStream(uint8_t *pbuf, ParamInfo_t *param)
{
    switch (param->type)
    {
    case PARAM_INT8:
        pbuf = SerializeInt(pbuf, *(PARAM_INT8_T *)param->pCurValue, param->length);
        break;

    case PARAM_INT16:
        pbuf = SerializeInt(pbuf, *(PARAM_INT16_T *)param->pCurValue, param->length);
        break;

    case PARAM_INT32:
        pbuf = SerializeInt(pbuf, *(PARAM_INT32_T *)param->pCurValue, param->length);
        break;

    case PARAM_INT64:
        pbuf = SerializeInt(pbuf, *(PARAM_INT64_T *)param->pCurValue, param->length);
        break;

    case PARAM_UINT8:
        pbuf = SerializeUint(pbuf, *(PARAM_UINT8_T *)param->pCurValue, param->length);
        break;

    case PARAM_UINT16:
        pbuf = SerializeUint(pbuf, *(PARAM_UINT16_T *)param->pCurValue, param->length);
        break;

    case PARAM_UINT32:
        pbuf = SerializeUint(pbuf, *(PARAM_UINT32_T *)param->pCurValue, param->length);
        break;

    case PARAM_UINT64:
        pbuf = SerializeUint(pbuf, *(PARAM_UINT64_T *)param->pCurValue, param->length);
        break;

    case PARAM_FLOAT:
        pbuf = SerializeFloat(pbuf, *(PARAM_FLOAT_T *)param->pCurValue);
        break;

    case PARAM_DOUBLE:
        pbuf = SerializeDouble(pbuf, *(PARAM_DOUBLE_T *)param->pCurValue);
        break;

    case PARAM_STARING:
        memcpy(&pbuf[0], param->pCurValue, PARAM_STRING_MAX_LENGTH);
        break;

    default:
        return 0; // 不支持的参数类型
    }

    return param->length;
}

/**
  * @brief      保存数据
  *
  * @attention  使用前确保函数 Param_SetCallBackFun 设置了保存回调处理函数
  * @note       参数在保存时会对参数校验合法性（取值范围）
  * @param      manager 参数表管理句柄
  * @param      pfnCheckError   参数不合法时触发回调函数
  * @return     0,成功; -1,失败
  */
int Param_Save(ParamManager_t *manager, pfnCheckError_cb pfnCheckError)
{
    uint8_t buf[sizeof(ParamVal_u) + 8];
    uint8_t *ptr = buf;
    uint16_t length = 0;
#if PARAM_USE_KEY_VALUE
    uint64_t key = 0;
#endif

    if (manager == NULL || manager->pfnSaveCallback == NULL)
    {
        return -1;
    }

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

        if (!ValidateIntRange(&manager->pParamTable[i], manager->pParamTable[i].pCurValue))
        {
            if (pfnCheckError != NULL)
            {
                pfnCheckError(&manager->pParamTable[i]);
            }
        }

        length = ParamInfoToStream(&buf[length], &manager->pParamTable[i]);
        ptr += length;

        if (manager->pfnSaveCallback(buf, (ptr - buf), false) != 0)
        {
            return -2;
        }
    }

    if (manager->pfnSaveCallback(buf, 0, true) != 0)
    {
        return -2;
    }

    return 0;
}




