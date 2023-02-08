/**
  * @file       param.h
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

#ifndef _PARAM_H_
#define _PARAM_H_

#include <stdint.h>
#include <stddef.h>

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
    PARAM_INT8,
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
    const char *pszName;            /*!< 名称 */
    const uint32_t id;              /*!< 唯一ID */
    const ParamType_e type;         /*!< 类型 */
    const uint16_t length;          /*!< 长度 */
    void *pCurValue;                /*!< 当前值指针 */
    const void *pDefValue;          /*!< 默认值指针 */
    const void *pMinValue;          /*!< 最小值指针 */
    const void *pMaxValue;          /*!< 最大值指针 */
} ParamInfo_t;

typedef struct
{
    const ParamInfo_t *pTab;
    uint32_t num;
} ParamTable_t;

/**
  * @brief  定义参数
  * 
  * @param  name 参数名（变量名）
  * @param  type 参数类型 @enum ParamType_e 取值
  * @param  defVal 默认值
  * @param  minVal 最小值
  * @param  maxVal 最大值
  */
#define PARAM_DEFINE(name, type, defVal, minVal, maxVal)   \
    const ParamType_e type_##name = type;\
    type##_T name = defVal;\
    const type##_T def_##name = defVal;\
    const type##_T min_##name = minVal;\
    const type##_T max_##name = maxVal;

/**
  * @brief  定义字符串参数
  * 
  * @param  name 参数名（变量名）
  * @param  length 字符串预留最大长度
  * @param  defVal 默认值
  */
#define PARAM_DEFINE_STR(name, length, defVal)   \
    char name[length] = {defVal};\
    const char def_##name[] = {defVal};

/**
  * @brief  参数注册
  * 
  * @param  id   参数唯一 ID
  * @param  name 参数名（变量名）
  * @param  type 参数类型 @enum ParamType_e 取值
  */
#define PARAM_REG(id, name, type)  { #name, id, type, sizeof(name), &name, &def_##name, &min_##name, &max_##name }

/**
  * @brief  字符串参数注册
  * 
  * @param  id   参数唯一 ID
  * @param  name 参数名（变量名）
  */
#define PARAM_STR_REG(id, name)  { #name, id, PARAM_STARING, sizeof(name), name, def_##name, NULL, NULL }

/**
  * @brief  参数声明
  * 
  * @param  name 参数名（变量名）
  * @param  type 参数类型 @enum ParamType_e 取值
  */
#define PARAM_EXTERN(name, type)  \
    extern type##_T name;\
    extern type##_T def_##name;\
    extern type##_T min_##name;\
    extern type##_T max_##name;

/**
  * @brief  字符串参数声明
  * 
  * @param  name 参数名（变量名）
  * @param  length 字符串预留最大长度
  */
#define PARAM_STR_EXTERN(name, length)  \
    extern char name[length];\
    extern char def_##name[];

extern int Param_Reset(ParamTable_t *pParamTable);
extern int Param_ModifyById(ParamTable_t *pParamTable, uint32_t id, const void *pNewData);
extern int Param_ModifyByName(ParamTable_t *pParamTable, const char *pszName, const void *pNewData);
extern size_t Param_Serialize(ParamTable_t *pParamTable, uint8_t *pBuf);
extern void Param_Parse(ParamTable_t *pParamTable, const uint8_t *pBuf);

#endif // !_PARAM_H_
