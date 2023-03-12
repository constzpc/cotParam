/**
  **********************************************************************************************************************
  * @file    param.h
  * @brief   该文件提供参数管理框架所有函数原型
  * @author  const_zpc    any question please send mail to const_zpc@163.com
  * @version V0.1
  * @date    2023-02-07
  **********************************************************************************************************************
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/

#ifndef _PARAM_H_
#define _PARAM_H_

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "param_type.h"

#ifdef __cplusplus
 extern "C" {
#endif 


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
    typedef type##_T type##_##name##_T;  \
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
#define PARAM_REG(id, name, type, attr)  { #name, id, type, sizeof(name), attr, &name, (void *)&def_##name, (void *)&min_##name, (void *)&max_##name }

/**
  * @brief  字符串参数注册
  * 
  * @param  id   参数唯一 ID
  * @param  name 参数名（变量名）
  */
#define PARAM_STR_REG(id, name, attr)  { #name, id, PARAM_STARING, sizeof(name), attr, name, (void *)def_##name, NULL, NULL }

/**
  * @brief  参数声明
  * 
  * @param  name 参数名（变量名）
  * @param  type 参数类型 @enum ParamType_e 取值
  */
#define PARAM_EXTERN(name, type)  \
    typedef type##_T type##_##name##_T;  \
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


extern size_t Param_MultiSerialize(uint8_t *pBuf, const ParamInfo_t *paramTable, uint16_t num, uint8_t opt);
extern uint16_t Param_MultiParse(uint8_t *pBuf, size_t length, ParamInfo_t *paramTable, uint16_t num);


extern size_t Param_Serialize(uint8_t *pBuf, const ParamInfo_t *paramTable, uint16_t num, uint8_t isAddKey, uint8_t isUseEncoded);
extern uint16_t Param_Parse(uint8_t *pBuf, size_t length, ParamInfo_t *paramTable, uint16_t num, uint8_t isAddKey, uint8_t isUseEncoded);

#ifdef __cplusplus
 }
#endif

#endif // !_PARAM_H_
