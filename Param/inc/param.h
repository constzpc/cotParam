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
  * @brief  定义普通参数
  * 
  * @param  name 参数名（变量名）
  * @param  type 参数类型 @enum ParamType_e 取值
  * @param  defVal 默认值
  * @param  minVal 最小值
  * @param  maxVal 最大值
  */
#define PARAM_DEFINE_DAT(name, type, defVal, minVal, maxVal)   \
    enum {TYPE_##name = type};\
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
  * @brief  普通参数注册
  * 
  * @param  id   参数唯一 ID
  * @param  name 参数名（变量名）
  * @param  attr 参数属性
  */
#define PARAM_ITEM_DAT(id, name, attr)  { #name, id, TYPE_##name, sizeof(name), attr, &name, \
                                        (void *)&def_##name, (void *)&min_##name, (void *)&max_##name }

/**
  * @brief  字符串参数注册
  * 
  * @param  id   参数唯一 ID
  * @param  name 参数名（变量名）
  */
#define PARAM_ITEM_STR(id, name, attr)  { #name, id, PARAM_STARING, sizeof(name), attr, name, \
                                        (void *)def_##name, NULL, NULL }

/**
  * @brief  参数声明
  * 
  * @param  name 参数名（变量名）
  * @param  type 参数类型 @enum ParamType_e 取值
  */
#define PARAM_EXTERN_DAT(name, type) extern type##_T name;

/**
  * @brief  字符串参数声明
  * 
  * @param  name 参数名（变量名）
  * @param  length 字符串预留最大长度
  */
#define PARAM_EXTERN_STR(name, length) extern char name[length];


extern size_t Param_MultiSerialize(uint8_t *pBuf, const ParamInfo_t *paramTable, uint16_t num, uint8_t opt);
extern uint16_t Param_MultiParse(uint8_t *pBuf, size_t length, ParamInfo_t *paramTable, uint16_t num);


extern size_t Param_Serialize(uint8_t *pBuf, const ParamInfo_t *paramTable, uint16_t num, uint8_t isAddKey, uint8_t isUseEncoded);
extern uint16_t Param_Parse(uint8_t *pBuf, size_t length, ParamInfo_t *paramTable, uint16_t num, uint8_t isAddKey, uint8_t isUseEncoded);

#ifdef __cplusplus
 }
#endif

#endif // !_PARAM_H_
