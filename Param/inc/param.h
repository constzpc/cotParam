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

// typedef struct
// {
//     uint16_t id;
//     uint16_t length;
//     char szName[32];
//     uint8_t type;
//     uint8_t attr;
//     uint8_t curVal[8];
//     uint8_t defVal[8];
//     uint8_t minVal[8];
//     uint8_t maxVal[8];
// } ParamData_t;



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
#define PARAM_REG(id, name, type, attr)  { #name, id, type, sizeof(name), attr, &name, &def_##name, &min_##name, &max_##name }

/**
  * @brief  字符串参数注册
  * 
  * @param  id   参数唯一 ID
  * @param  name 参数名（变量名）
  */
#define PARAM_STR_REG(id, name, attr)  { #name, id, PARAM_STARING, sizeof(name), attr, name, def_##name, NULL, NULL }

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

#define DEFINE_PARAM(type)  \
struct param\
{\
    char *pszName;\
    uint32_t id;\
    ParamType_e type;\
    uint16_t length;\
    uint8_t attr;\
    type##_T val;\
    type##_T def;\
    type##_T min;\
    type##_T max;\
};


extern int Param_Init(const ParamInfo_t *paramTable, uint16_t num);

extern int Param_Load(void);
extern int Param_Save(void);

extern size_t Param_Read(uint16_t id, uint8_t *pBuf, uint8_t opt);
extern size_t Param_Reads(uint16_t id, uint16_t num, uint8_t *pBuf, uint8_t opt);


// extern int Param_Reset(ParamTable_t *pParamTable);
// extern int Param_ModifyById(ParamTable_t *pParamTable, uint32_t id, const void *pNewData);
// extern int Param_ModifyByName(ParamTable_t *pParamTable, const char *pszName, const void *pNewData);


// extern size_t Param_Serialize(ParamTable_t *pParamTable, uint8_t *pBuf);
// extern void Param_Parse(ParamTable_t *pParamTable, const uint8_t *pBuf);

#ifdef __cplusplus
 }
#endif

#endif // !_PARAM_H_
