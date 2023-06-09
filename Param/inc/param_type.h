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

#define PARAM_ATTR_WR      (PARAM_ATTR_READ | PARAM_ATTR_WRITE)
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
typedef char        PARAM_STARING_T;

typedef uint16_t    param_size_t;

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
    PARAM_STARING,
} ParamType_e;

typedef union
{
    uint8_t u8val;
    uint16_t u16val;
    uint32_t u32val;
    uint64_t u64val;
    int8_t s8val;
    int16_t s16val;
    int32_t s32val;
    int64_t s64val;
    float  fval;
    double dval;
    char szVal[PARAM_STRING_MAX_LENGTH];
}ParamVal_u;

#define DEFINE_PARAM_STRUCT(_type)\
            typedef struct\
            {\
                char szName[PARAM_NAME_MAX_LENGTH];\
                uint16_t id;\
                uint8_t type; \
                uint16_t length;\
                uint8_t attr;\
                _type##_T curValue;\
                _type##_T defValue;\
                _type##_T minValue;\
                _type##_T maxValue;\
            } Param##_type##_t;

DEFINE_PARAM_STRUCT(PARAM_INT8);

#define DEFINE_PARAM_STRUCT_VARIABLE(_type, name)  Param##_type##_t name;

/**
  * @brief 定义有内存的参数结构体信息
  * 
  */
typedef struct
{
    char szName[PARAM_NAME_MAX_LENGTH];/*!< 名称 */
    uint16_t id;                    /*!< 唯一ID */
    uint8_t type;                   /*!< 类型, @enum ParamType_e */
    uint16_t length;                /*!< 长度（字节数） */
    uint8_t attr;                   /*!< 属性 */
    ParamVal_u curValue;            /*!< 当前值指针 */
    ParamVal_u defValue;            /*!< 默认值指针 */
    ParamVal_u minValue;            /*!< 最小值指针 */
    ParamVal_u maxValue;            /*!< 最大值指针 */
} ParamMemInfo_t;

/**
  * @brief  定义无内存的参数结构体信息
  * 
  */
typedef struct stuParamInfo
{
    const char *pszName;            /*!< 名称 */
    uint16_t id;                    /*!< 唯一ID */
    uint8_t type;                   /*!< 类型, @enum ParamType_e */
    uint16_t length;                /*!< 长度（字节数） */
    uint8_t attr;                   /*!< 属性 */
    void *pCurValue;                /*!< 当前值指针 */
    const void *pDefValue;          /*!< 默认值指针 */
    const void *pMinValue;          /*!< 最小值指针(字符串参数限制最小长度时需要 param_size_t 定义) */
    const void *pMaxValue;          /*!< 最大值指针(字符串参数限制最大长度时需要 param_size_t 定义) */
    void (*pfnCallback)(const struct stuParamInfo *); // 参数修改后需要触发的回调函数指针
} ParamInfo_t;


typedef int (*pfnCheckError_cb)(const ParamInfo_t *pParamInfo);
typedef int (*pfnLoad_cb)(uint8_t *pBuf, uint16_t *len, bool *pisFinish);
typedef int (*pfnSave_cb)(const uint8_t *pBuf, uint16_t len, bool isFinish);

// 定义参数管理结构体
typedef struct
{
    ParamInfo_t *pParamTable;       /*!< 参数信息数组表 */
    uint16_t count;                 /*!< 参数信息数组元素个数 */
    pfnLoad_cb pfnLoadCallback;     /*!< 参数加载时需要触发的回调函数指针 */
    pfnSave_cb pfnSaveCallback;     /*!< 参数保存时需要触发的回调函数指针 */
} ParamManager_t;


#ifdef __cplusplus
}
#endif

#endif
