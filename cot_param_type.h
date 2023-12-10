/**
  **********************************************************************************************************************
  * @file    cot_param_type.h
  * @brief   该文件提供参数管理框架所有函数原型
  * @author  const_zpc    any question please send mail to const_zpc@163.com
  **********************************************************************************************************************
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/

#ifndef _COT_PARAM_TYPE_H_
#define _COT_PARAM_TYPE_H_

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "cot_param_cfg.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 参数权限属性 */

#define COT_PARAM_ATTR_WR      (COT_PARAM_ATTR_READ | COT_PARAM_ATTR_WRITE) // 读写权限
#define COT_PARAM_ATTR_NONE    0X00 // 无读写权限（不使用键值对模式储存参数则用于删除参数的属性）
#define COT_PARAM_ATTR_READ    0X01 // 可读权限属性
#define COT_PARAM_ATTR_WRITE   0X02 // 可写权限属性
#define COT_PARAM_ATTR_RESET   0X04 // 可重置权限属性
#define COT_PARAM_ATTR_RANGE   0X10 // 支持最大/最小值校验


typedef int8_t      COT_PARAM_INT8_T;
typedef int16_t     COT_PARAM_INT16_T;
typedef int32_t     COT_PARAM_INT32_T;
typedef int64_t     COT_PARAM_INT64_T;
typedef uint8_t     COT_PARAM_UINT8_T;
typedef uint16_t    COT_PARAM_UINT16_T;
typedef uint32_t    COT_PARAM_UINT32_T;
typedef uint64_t    COT_PARAM_UINT64_T;
typedef float       COT_PARAM_FLOAT_T;
typedef double      COT_PARAM_DOUBLE_T;
typedef char        COT_PARAM_STRING_T;

#if COT_PARAM_STRING_MAX_LENGTH >= 256
typedef uint16_t    param_size_t;
#else
typedef uint8_t     param_size_t;
#endif

typedef enum
{
    COT_PARAM_INT8 = 0,
    COT_PARAM_INT16,
    COT_PARAM_INT32,
#if COT_PARAM_USE_64_BIT_LENGTH
    COT_PARAM_INT64,
#endif
    COT_PARAM_UINT8,
    COT_PARAM_UINT16,
    COT_PARAM_UINT32,
#if COT_PARAM_USE_64_BIT_LENGTH
    COT_PARAM_UINT64,
#endif
    COT_PARAM_FLOAT,
#if COT_PARAM_USE_64_BIT_LENGTH
    COT_PARAM_DOUBLE,
#endif
#if COT_PARAM_USE_STRING_TYPE
    COT_PARAM_STRING,
#endif
} ParamType_e;

typedef enum
{
    COT_PARAM_CHECK_OK = 0,     // 校验成功 / 无最大最小值
    COT_PARAM_CHECK_OVER_MIN,   // 小于最小值 / 小于最小字符串长度
    COT_PARAM_CHECK_OVER_MAX,   // 大于最大值 / 大于最小字符串长度
    COT_PARAM_CHECK_OTHER_ERR   // 其他错误, 即自定义校验出错
} cotParamCheckRet_e;

typedef enum
{
    COT_PARAM_RESET_NONE = 0,   // 不做处理 / 不更新
    COT_PARAM_RESET_DEF,        // 恢复默认
    COT_PARAM_RESET_MIN,        // 恢复最小值
    COT_PARAM_RESET_MAX,        // 恢复最大值
    COT_PARAM_RESET_MIN_MAX     // 小于最小值则恢复最小值，大于最大值则恢复最大值
} cotParamResetOpt_e;

typedef union {
    COT_PARAM_INT8_T *pInt8;
    COT_PARAM_INT16_T *pInt16;
    COT_PARAM_INT32_T *pInt32;
#if COT_PARAM_USE_64_BIT_LENGTH
    COT_PARAM_INT64_T *pInt64;
#endif
    COT_PARAM_UINT8_T *pUint8;
    COT_PARAM_UINT16_T *pUint16;
    COT_PARAM_UINT32_T *pUint32;
#if COT_PARAM_USE_64_BIT_LENGTH
    COT_PARAM_UINT64_T *pUint64;
#endif
    COT_PARAM_FLOAT_T *pFloat;
#if COT_PARAM_USE_64_BIT_LENGTH
    COT_PARAM_DOUBLE_T *pDouble;
#endif
#if COT_PARAM_USE_STRING_TYPE
    COT_PARAM_STRING_T *pString;
    param_size_t *pStringLength;
#endif
    void *pVoid;
} cotParamTypePtr_u;

typedef int (*cotParamCheck_f)(const void *pCurParam);

/**
  * @brief  定义无内存的参数结构体信息
  * 
  */
typedef struct stuParamInfo
{
#if ( COT_PARAM_NAME_MAX_LENGTH > 1)
    const char *pszName;                    /*!< 名称 */
#endif
    uint16_t id;                            /*!< 唯一ID */
    uint8_t type;                           /*!< 类型, @enum ParamType_e */
    uint16_t length;                        /*!< 长度（字节数） */
    uint8_t attr;                           /*!< 属性 */
    cotParamTypePtr_u unCurValuePtr;        /*!< 当前值指针 */
    const cotParamTypePtr_u unDefValuePtr;  /*!< 默认值指针 */
    const cotParamTypePtr_u unMinValuePtr;  /*!< 最小值指针(字符串参数限制最小长度时需要 param_size_t 定义) */
    const cotParamTypePtr_u unMaxValuePtr;  /*!< 最大值指针(字符串参数限制最大长度时需要 param_size_t 定义) */
#if COT_PARAM_USE_CUSTOM_CHECK
    const cotParamCheck_f pfnParamCheck;    /*!< 自定义校验方式回调函数(返回0表示校验成功) */
#endif
} cotParamInfo_t;

typedef int (*cotParamError_f)(const cotParamInfo_t *pParamInfo, cotParamCheckRet_e eCheckResult);
typedef int (*cotParamLoad_f)(uint8_t *pBuf, uint16_t bufSize, uint16_t *pLength);
typedef int (*cotParamSave_f)(const uint8_t *pBuf, uint16_t len);

// 定义参数管理结构体
typedef struct
{
    cotParamInfo_t *pParamTable;            /*!< 参数信息数组表 */
    uint16_t count;                         /*!< 参数信息数组元素个数 */
} cotParamManager_t;


#ifdef __cplusplus
}
#endif

#endif
