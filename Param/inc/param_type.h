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
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
 extern "C" {
#endif 

/** 参数权限属性 */

#define PARAM_ATTR_ALL     0XFF // 所有权限
#define PARAM_ATTR_READ    0X01 // 可读权限
#define PARAM_ATTR_WRITE   0X02 // 可写权限
#define PARAM_ATTR_RESET   0X04 // 可重置权限

/** 参数序列化选项 */

#define PARAM_PACK_ALL      0x7F // 所有信息都序列化
#define PARAM_PACK_NAME     0x01 // 参数名字信息序列化
#define PARAM_PACK_ATTR     0x02 // 参数属性信息序列化
#define PARAM_PACK_DEF_VAL  0x04 // 参数默认值信息序列化
#define PARAM_PACK_MIN_VAL  0x08 // 参数最小值信息序列化
#define PARAM_PACK_MAX_VAL  0x10 // 参数最大值信息序列化

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
}ParamType_e;

typedef struct
{
    char *pszName;            /*!< 名称 */
    uint16_t id;              /*!< 唯一ID */
    uint8_t type;             /*!< 类型, @enum ParamType_e */
    uint16_t length;          /*!< 长度 */
    uint8_t attr;             /*!< 属性 */
    void *pCurValue;          /*!< 当前值指针 */
    void *pDefValue;          /*!< 默认值指针 */
    void *pMinValue;          /*!< 最小值指针 */
    void *pMaxValue;          /*!< 最大值指针 */
} ParamInfo_t;

#ifdef __cplusplus
 }
#endif

#endif
