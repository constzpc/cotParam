/**
  **********************************************************************************************************************
  * @file    param.h
  * @brief   该文件提供参数管理框架所有函数原型
  * @author  const_zpc    any question please send mail to const_zpc@163.com
  * @version V1.0
  * @date    2023-06-09
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
  * @param  initVal 初始值
  */
#define PARAM_DEFINE_DAT(name, type, initVal)   \
    enum {PARAM_TYPE_##name = type};\
    enum {PARAM_INIT_ATTR_##name = 0};\
    type##_T name = initVal;

/**
  * @brief  定义普通参数, 具有默认值
  * 
  * @param  name 参数名（变量名）
  * @param  type 参数类型 @enum ParamType_e 取值
  * @param  defVal 默认值/初始值
  */
#define PARAM_DEFINE_DAT_DEF(name, type, defVal)   \
    enum {PARAM_TYPE_##name = type};\
    enum {PARAM_INIT_ATTR_##name = (PARAM_ATTR_RESET)};\
    type##_T name = defVal;\
    const type##_T def_##name = defVal;

/**
  * @brief  定义普通参数, 具有默认值和范围校验
  * 
  * @param  name 参数名（变量名）
  * @param  type 参数类型 @enum ParamType_e 取值
  * @param  defVal 默认值/初始值
  * @param  minVal 最小值
  * @param  maxVal 最大值
  */
#define PARAM_DEFINE_DAT_RANGE(name, type, defVal, minVal, maxVal)   \
    enum {PARAM_TYPE_##name = type};\
    enum {PARAM_INIT_ATTR_##name = (PARAM_ATTR_RANGE | PARAM_ATTR_RESET)};\
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
    enum {PARAM_INIT_ATTR_##name = 0};\
    char name[length] = {defVal};\
    const char def_##name[] = {defVal};

#define PARAM_DEFINE_STR_DEF(name, length, defVal)   \
    enum {PARAM_INIT_ATTR_##name = (PARAM_ATTR_RESET)};\
    char name[length] = {defVal};\
    const char def_##name[] = {defVal};
    
#define PARAM_DEFINE_STR_RANGE(name, length, defVal, minLength, maxLength)   \
    enum {PARAM_INIT_ATTR_##name = (PARAM_ATTR_RANGE | PARAM_ATTR_RESET)};\
    char name[length] = {defVal};\
    const char def_##name[] = {defVal};\
    const param_size_t min_##name = minLength;\
    const param_size_t max_##name = maxLength;

/**
  * @brief  普通参数注册
  * 
  * @param  _id   参数唯一 ID
  * @param  _name 参数名（变量名）
  * @param  _attr 参数读/写属性组合（PARAM_ATTR_READ/PARAM_ATTR_WRITE）
  */
#define PARAM_ITEM_DAT(_id, _name, _attr)  { .pszName = #_name, \
                                          .id = _id, \
                                          .type = PARAM_TYPE_##_name, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .pCurValue = &_name, \
                                          .pDefValue = NULL, \
                                          .pMinValue = NULL, \
                                          .pMaxValue = NULL}

#define PARAM_ITEM_DAT_DEF(_id, _name, _attr)  { .pszName = #_name, \
                                          .id = _id, \
                                          .type = PARAM_TYPE_##_name, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .pCurValue = &_name, \
                                          .pDefValue = (void *)&def_##_name, \
                                          .pMinValue = NULL, \
                                          .pMaxValue = NULL}

#define PARAM_ITEM_DAT_RANGE(_id, _name, _attr)  { .pszName = #_name, \
                                          .id = _id, \
                                          .type = PARAM_TYPE_##_name, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .pCurValue = &_name, \
                                          .pDefValue = (void *)&def_##_name, \
                                          .pMinValue = (void *)&min_##_name, \
                                          .pMaxValue = (void *)&max_##_name}

/**
  * @brief  字符串参数注册
  * 
  * @param  _id   参数唯一 ID
  * @param  _name 参数名（变量名）
  * @param  _attr 参数读/写属性组合（PARAM_ATTR_READ/PARAM_ATTR_WRITE）
  */
#define PARAM_ITEM_STR(_id, _name, _attr)  { .pszName = #_name, \
                                          .id = _id, \
                                          .type = PARAM_STARING, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .pCurValue = &_name, \
                                          .pDefValue = NULL, \
                                          .pMinValue = NULL, \
                                          .pMaxValue = NULL}

#define PARAM_ITEM_STR_DEF(_id, _name, _attr)  { .pszName = #_name, \
                                          .id = _id, \
                                          .type = PARAM_STARING, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .pCurValue = &_name, \
                                          .pDefValue = (void *)&def_##_name, \
                                          .pMinValue = NULL, \
                                          .pMaxValue = NULL}

#define PARAM_ITEM_STR_RANGE(_id, _name, _attr)  { .pszName = #_name, \
                                          .id = _id, \
                                          .type = PARAM_STARING, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .pCurValue = &_name, \
                                          .pDefValue = (void *)&def_##_name, \
                                          .pMinValue = (void *)&min_##_name, \
                                          .pMaxValue = (void *)&max_##_name}

/**
  * @brief  参数声明
  * 
  * @param  name 参数名（变量名）
  * @param  type 参数类型 @enum ParamType_e 取值
  */
#define PARAM_EXTERN_DAT(name, type) typedef type##_T param_##name##_t; extern type##_T name;

/**
  * @brief  字符串参数声明
  * 
  * @param  name 参数名（变量名）
  * @param  length 字符串预留最大长度
  */
#define PARAM_EXTERN_STR(name, length) typedef char* param_##name##_t; extern char name[length];

// 根据参数名获取该参数的类型定义
#define PARAM_DECLTYPE(name)        param_##name##_t

// 得到参数表的元素数目
#define PARAM_TABLE_SIZE(tab)   (sizeof(tab) / sizeof(ParamInfo_t))


extern int Param_Init(ParamManager_t* manager, ParamInfo_t* pParamTable, uint16_t count);
extern int Param_SetCallBackFun(ParamManager_t* manager, pfnLoad_cb pfnLoadCallback, pfnSave_cb pfnSaveCallback);
extern int Param_ResetParam(const ParamManager_t *manager);
extern bool Param_SetParam(ParamManager_t *manager, const void *curParamPtr, const void *newValue);

extern int Param_Load(ParamManager_t* manager, pfnCheckError_cb pfnCheckError);
extern int Param_Save(ParamManager_t* manager, pfnCheckError_cb pfnCheckError);

extern const ParamInfo_t *Param_FindParamByName(ParamManager_t *manager, const char *pszName);
extern const ParamInfo_t *Param_FindParamByID(ParamManager_t* manager, uint16_t id);
extern const ParamInfo_t *Param_FindParamByParamPtr(ParamManager_t* manager, const void *curParamPtr);

extern const void *Param_GetParamValue(const ParamInfo_t *param);
extern bool Param_SetParamValue(const ParamInfo_t *param, const void *value);
extern bool Param_ResetParamValue(const ParamInfo_t *param);
extern int Param_Sprintf(char *pout, const ParamInfo_t *param);




#ifdef __cplusplus
 }
#endif

#endif // !_PARAM_H_
