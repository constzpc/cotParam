/**
  **********************************************************************************************************************
  * @file    param_type.h
  * @brief   该文件提供参数管理框架所有函数原型
  * @author  const_zpc    any question please send mail to const_zpc@163.com
  * @version V0.1
  * @date    2023-02-07
  **********************************************************************************************************************
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/

#ifndef _PARAM_TYPE_H_
#define _PARAM_TYPE_H_

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "param_cfg.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 参数权限属性 */

#define PARAM_ATTR_WR      (PARAM_ATTR_READ | PARAM_ATTR_WRITE) // 读写权限
#define PARAM_ATTR_NONE    0X00 // 无读写权限（不使用键值对模式储存参数则用于删除参数的属性）
#define PARAM_ATTR_READ    0X01 // 可读权限属性
#define PARAM_ATTR_WRITE   0X02 // 可写权限属性
#define PARAM_ATTR_RESET   0X04 // 可重置权限属性
#define PARAM_ATTR_RANGE   0X10 // 支持最大/最小值校验


typedef int8_t      PARAM_INT8_T;
typedef int16_t     PARAM_INT16_T;
typedef int32_t     PARAM_INT32_T;
typedef int64_t     PARAM_INT64_T;
typedef uint8_t     PARAM_UINT8_T;
typedef uint16_t    PARAM_UINT16_T;
typedef uint32_t    PARAM_UINT32_T;
typedef uint64_t    PARAM_UINT64_T;
typedef float       PARAM_FLOAT_T;
typedef double      PARAM_DOUBLE_T;
typedef char        PARAM_STRING_T;

#if PARAM_STRING_MAX_LENGTH >= 256
typedef uint16_t    param_size_t;
#else
typedef uint8_t     param_size_t;
#endif

typedef enum
{
    PARAM_INT8 = 0,
    PARAM_INT16,
    PARAM_INT32,
    PARAM_INT64,
    PARAM_UINT8,
    PARAM_UINT16,
    PARAM_UINT32,
    PARAM_UINT64,
    PARAM_FLOAT,
    PARAM_DOUBLE,
    PARAM_STRING,
} ParamType_e;

typedef union {
    PARAM_INT8_T *pInt8;
    PARAM_INT16_T *pInt16;
    PARAM_INT32_T *pInt32;
    PARAM_INT64_T *pInt64;
    PARAM_UINT8_T *pUint8;
    PARAM_UINT16_T *pUint16;
    PARAM_UINT32_T *pUint32;
    PARAM_UINT64_T *pUint64;
    PARAM_FLOAT_T *pFloat;
    PARAM_DOUBLE_T *pDouble;
    PARAM_STRING_T *pString;
    param_size_t *pStringLength;
    void *pVoid;
} ParamTypePtr_u;

/**
  * @brief  定义无内存的参数结构体信息
  * 
  */
typedef struct stuParamInfo
{
#if (PARAM_NAME_MAX_LENGTH > 1)
    const char *pszName;            /*!< 名称 */
#endif
    uint16_t id;                    /*!< 唯一ID */
    uint8_t type;                   /*!< 类型, @enum ParamType_e */
    uint16_t length;                /*!< 长度（字节数） */
    uint8_t attr;                   /*!< 属性 */
    ParamTypePtr_u unCurValuePtr;    /*!< 当前值指针 */
    const ParamTypePtr_u unDefValuePtr;   /*!< 默认值指针 */
    const ParamTypePtr_u unMinValuePtr;   /*!< 最小值指针(字符串参数限制最小长度时需要 param_size_t 定义) */
    const ParamTypePtr_u unMaxValuePtr;   /*!< 最大值指针(字符串参数限制最大长度时需要 param_size_t 定义) */
} ParamInfo_t;

typedef int (*pfnCheckError_cb)(const ParamInfo_t *pParamInfo);
typedef int (*pfnLoad_cb)(uint8_t *pBuf, uint16_t *len, bool *pisFinish);
typedef int (*pfnSave_cb)(const uint8_t *pBuf, uint16_t len, bool isFinish);

// 定义参数管理结构体
typedef struct
{
    ParamInfo_t *pParamTable;       /*!< 参数信息数组表 */
    uint16_t count;                 /*!< 参数信息数组元素个数 */
} ParamManager_t;


#ifdef __cplusplus
}
#endif

#endif
