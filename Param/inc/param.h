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
  * @param  length 字符串预留最大长度（包括结束符'\0'）
  * @param  initVal 初始值
  */
#define PARAM_DEFINE_STR(name, length, defVal)   \
    enum {PARAM_INIT_ATTR_##name = 0};\
    char name[length] = {defVal};\

/**
  * @brief  定义字符串参数
  * 
  * @param  name 参数名（变量名）
  * @param  length 字符串预留最大长度（包括结束符'\0'）
  * @param  defVal 默认值
  */
#define PARAM_DEFINE_STR_DEF(name, length, defVal)   \
    enum {PARAM_INIT_ATTR_##name = (PARAM_ATTR_RESET)};\
    char name[length] = {defVal};\
    const char def_##name[] = {defVal};

/**
  * @brief  定义字符串参数, 具有默认值和长度校验
  * 
  * @param  name 参数名（变量名）
  * @param  length 字符串预留最大长度（包括结束符'\0'）
  * @param  defVal 默认值
  * @param  minLength 最小长度
  */
#define PARAM_DEFINE_STR_RANGE(name, length, defVal, minLength)   \
    enum {PARAM_INIT_ATTR_##name = (PARAM_ATTR_RANGE | PARAM_ATTR_RESET)};\
    char name[length] = {defVal};\
    const char def_##name[] = {defVal};\
    const param_size_t min_##name = minLength;\
    const param_size_t max_##name = length - 1;

#if (PARAM_NAME_MAX_LENGTH > 1)
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
                                          .unCurValuePtr.pVoid = &_name, \
                                          .unDefValuePtr.pVoid = NULL, \
                                          .unMinValuePtr.pVoid = NULL, \
                                          .unMaxValuePtr.pVoid = NULL}

#define PARAM_ITEM_DAT_DEF(_id, _name, _attr)  { .pszName = #_name, \
                                          .id = _id, \
                                          .type = PARAM_TYPE_##_name, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .unCurValuePtr.pVoid = &_name, \
                                          .unDefValuePtr.pVoid = (void *)&def_##_name, \
                                          .unMinValuePtr.pVoid = NULL, \
                                          .unMaxValuePtr.pVoid = NULL}

#define PARAM_ITEM_DAT_RANGE(_id, _name, _attr)  { .pszName = #_name, \
                                          .id = _id, \
                                          .type = PARAM_TYPE_##_name, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .unCurValuePtr.pVoid = &_name, \
                                          .unDefValuePtr.pVoid = (void *)&def_##_name, \
                                          .unMinValuePtr.pVoid = (void *)&min_##_name, \
                                          .unMaxValuePtr.pVoid = (void *)&max_##_name}

/**
  * @brief  字符串参数注册
  * 
  * @param  _id   参数唯一 ID
  * @param  _name 参数名（变量名）
  * @param  _attr 参数读/写属性组合（PARAM_ATTR_READ/PARAM_ATTR_WRITE）
  */
#define PARAM_ITEM_STR(_id, _name, _attr)  { .pszName = #_name, \
                                          .id = _id, \
                                          .type = PARAM_STRING, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .unCurValuePtr.pVoid = &_name, \
                                          .unDefValuePtr.pVoid = NULL, \
                                          .unMinValuePtr.pVoid = NULL, \
                                          .unMaxValuePtr.pVoid = NULL}

#define PARAM_ITEM_STR_DEF(_id, _name, _attr)  { .pszName = #_name, \
                                          .id = _id, \
                                          .type = PARAM_STRING, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .unCurValuePtr.pVoid = &_name, \
                                          .unDefValuePtr.pVoid = (void *)&def_##_name, \
                                          .unMinValuePtr.pVoid = NULL, \
                                          .unMaxValuePtr.pVoid = NULL}

#define PARAM_ITEM_STR_RANGE(_id, _name, _attr)  { .pszName = #_name, \
                                          .id = _id, \
                                          .type = PARAM_STRING, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .unCurValuePtr.pVoid = &_name, \
                                          .unDefValuePtr.pVoid = (void *)&def_##_name, \
                                          .unMinValuePtr.pVoid = (void *)&min_##_name, \
                                          .unMaxValuePtr.pVoid = (void *)&max_##_name}

#else

/**
  * @brief  普通参数注册
  * 
  * @param  _id   参数唯一 ID
  * @param  _name 参数名（变量名）
  * @param  _attr 参数读/写属性组合（PARAM_ATTR_READ/PARAM_ATTR_WRITE）
  */
#define PARAM_ITEM_DAT(_id, _name, _attr)  {\
                                          .id = _id, \
                                          .type = PARAM_TYPE_##_name, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .unCurValuePtr.pVoid = &_name, \
                                          .unDefValuePtr.pVoid = NULL, \
                                          .unMinValuePtr.pVoid = NULL, \
                                          .unMaxValuePtr.pVoid = NULL}

#define PARAM_ITEM_DAT_DEF(_id, _name, _attr)  {\
                                          .id = _id, \
                                          .type = PARAM_TYPE_##_name, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .unCurValuePtr.pVoid = &_name, \
                                          .unDefValuePtr.pVoid = (void *)&def_##_name, \
                                          .unMinValuePtr.pVoid = NULL, \
                                          .unMaxValuePtr.pVoid = NULL}

#define PARAM_ITEM_DAT_RANGE(_id, _name, _attr)  {\
                                          .id = _id, \
                                          .type = PARAM_TYPE_##_name, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .unCurValuePtr.pVoid = &_name, \
                                          .unDefValuePtr.pVoid = (void *)&def_##_name, \
                                          .unMinValuePtr.pVoid = (void *)&min_##_name, \
                                          .unMaxValuePtr.pVoid = (void *)&max_##_name}

/**
  * @brief  字符串参数注册
  * 
  * @param  _id   参数唯一 ID
  * @param  _name 参数名（变量名）
  * @param  _attr 参数读/写属性组合（PARAM_ATTR_READ/PARAM_ATTR_WRITE）
  */
#define PARAM_ITEM_STR(_id, _name, _attr)  {\
                                          .id = _id, \
                                          .type = PARAM_STRING, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .pCurValue = &_name, \
                                          .unDefValuePtr.pVoid = NULL, \
                                          .unMinValuePtr.pVoid = NULL, \
                                          .unMaxValuePtr.pVoid = NULL}

#define PARAM_ITEM_STR_DEF(_id, _name, _attr)  {\
                                          .id = _id, \
                                          .type = PARAM_STRING, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .pCurValue = &_name, \
                                          .unDefValuePtr.pVoid = (void *)&def_##_name, \
                                          .unMinValuePtr.pVoid = NULL, \
                                          .unMaxValuePtr.pVoid = NULL}

#define PARAM_ITEM_STR_RANGE(_id, _name, _attr)  {\
                                          .id = _id, \
                                          .type = PARAM_STRING, \
                                          .length = sizeof(_name), \
                                          .attr = (_attr | PARAM_INIT_ATTR_##_name), \
                                          .pCurValue = &_name, \
                                          .unDefValuePtr.pVoid = (void *)&def_##_name, \
                                          .unMinValuePtr.pVoid = (void *)&min_##_name, \
                                          .unMaxValuePtr.pVoid = (void *)&max_##_name}

#endif

/**
  * @brief  参数声明
  * 
  * @param  name 参数名（变量名）
  * @param  type 参数类型 @enum ParamType_e 取值
  */
#define PARAM_EXTERN_DAT(name, type)        \
                typedef type##_T param_##name##_t;\
                extern type##_T name;\
                const extern type##_T def_##name;\
                const extern type##_T min_##name;\
                const extern type##_T max_##name;

/**
  * @brief  字符串参数声明
  * 
  * @param  name 参数名（变量名）
  * @param  length 字符串预留最大长度
  */
#define PARAM_EXTERN_STR(name, length)      \
                typedef char* param_##name##_t;\
                extern char name[length];\
                const extern char def_##name[];\
                const extern param_size_t min_##name;\
                const extern param_size_t max_##name;

// 根据参数名获取该参数的类型定义
#define PARAM_DECLTYPE(name)        param_##name##_t

#define PARAM_DAT_CUR_VALUE(name)   name
#define PARAM_DAT_DEF_VALUE(name)   def_##name
#define PARAM_DAT_MIN_VALUE(name)   min_##name
#define PARAM_DAT_MAX_VALUE(name)   max_##name

#define PARAM_STR_CUR_VALUE(name)   name
#define PARAM_STR_DEF_VALUE(name)   def_##name
#define PARAM_STR_MIN_LENGTH(name)  min_##name
#define PARAM_STR_MAX_LENGTH(name)  max_##name


#define PARAM_NONE              0
#define PARAM_DEF               1
#define PARAM_MIN_MAX           2


#define _PARAM_DAT_PARAM_NONE(name, def, min_or_max)            (0)
#define _PARAM_DAT_PARAM_DEF(name, def, min_or_max)             (name = def##_##name)
#define _PARAM_DAT_PARAM_MIN_MAX(name, def, min_or_max)         (name = min_or_max##_##name)
#define _PARAM_STR_PARAM_NONE(name, def, min_or_max)            (0)
#define _PARAM_STR_PARAM_DEF(name, def, min_or_max)             (strcpy(name, def##_##name))
#define _PARAM_STR_PARAM_MIN_MAX(name, def, min_or_max)         (0)

/**
  * @brief 使用 PARAM_DEFINE_DAT_RANGE 定义的参数可以通过该宏修改
  * 
  * @param  name  参数名
  * @param  val   参数新的值
  * @param  opt   超出范围的处理：PARAM_NONE,参数不变；PARAM_DEF,参数恢复默认；PARAM_MIN_MAX,参数取最大最小值
  * @return 0,修改成功；-1,新的值小于最小限制值；1,新的值大于最大限制值
  */
#define PARAM_DAT_SET_NEW_VALUE(name, val, opt)  \
            (val < PARAM_DAT_MIN_VALUE(name) ? (_PARAM_DAT_##opt(name, def, min), -1) : \
             val > PARAM_DAT_MAX_VALUE(name) ? (_PARAM_DAT_##opt(name, def, max), 1) : \
                (PARAM_DAT_CUR_VALUE(name) = val, 0))

/**
  * @brief 使用 PARAM_DEFINE_STR_RANGE 定义的参数可以通过该宏修改
  * 
  * @param  name  参数名
  * @param  val   参数新的字符串
  * @param  opt   超出范围的处理：PARAM_NONE 或 PARAM_MIN_MAX,, 参数不变；PARAM_DEF,参数恢复默认
  * @return 0,修改成功；-1,新的字符串长度小于最小限制值；1,新的字符串长度大于最大限制值
  */
#define PARAM_STR_SET_NEW_VALUE(name, str, opt)    \
            (strlen(str) < PARAM_STR_MIN_LENGTH(name) ? (_PARAM_STR_##opt(name, def, min), -1) : \
             strlen(str) > PARAM_STR_MAX_LENGTH(name) ? (_PARAM_STR_##opt(name, def, max), 1) : \
                (strcpy(PARAM_STR_CUR_VALUE(name), str), 0))

// 得到参数表的元素数目
#define PARAM_TABLE_SIZE(tab)   (sizeof(tab) / sizeof(ParamInfo_t))


extern int Param_Init(ParamManager_t* manager, ParamInfo_t* pParamTable, uint16_t count);
extern int Param_ResetParam(const ParamManager_t *manager);

extern int Param_Load(ParamManager_t* manager, pfnLoad_cb pfnLoadCallback, pfnCheckError_cb pfnCheckError);
extern int Param_Save(ParamManager_t* manager, pfnSave_cb pfnSaveCallback, pfnCheckError_cb pfnCheckError);

extern ParamInfo_t *Param_IterateList(ParamManager_t *manager, size_t *psIdx);

extern const ParamInfo_t *Param_FindParamByName(ParamManager_t *manager, const char *pszName);
extern const ParamInfo_t *Param_FindParamByID(ParamManager_t* manager, uint16_t id);
extern const ParamInfo_t *Param_FindParamByParamPtr(ParamManager_t* manager, const void *curParamPtr);

extern int Param_SetNewValue(const ParamInfo_t *param, const void *value, uint8_t opt);
extern bool Param_ResetDefaultValue(const ParamInfo_t *param);
extern bool Param_ResetMinValue(const ParamInfo_t *param);
extern bool Param_ResetMaxValue(const ParamInfo_t *param);

#ifdef __cplusplus
 }
#endif

#endif // !_PARAM_H_
