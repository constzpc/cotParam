/**
  **********************************************************************************************************************
  * @file    cot_param.h
  * @brief   该文件提供参数管理框架所有函数原型
  * @author  const_zpc    any question please send mail to const_zpc@163.com
  **********************************************************************************************************************
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/

#ifndef _COT_PARAM_H_
#define _COT_PARAM_H_

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "cot_param_type.h"

#ifdef __cplusplus
 extern "C" {
#endif 

/**
  * @brief  添加参数(以变量名为参数名)
  * 
  * @note   COT_PARAM_ITEM_BIND(id, name, type, attr, defVal, minVal, maxVal, checkFun)
  * @param  id   参数表中的唯一ID
  * @param  variable 变量
  * @param  type 参数类型 @enum ParamType_e 取值
  * @param  attr 属性
  * @param  defVal 默认值（可选）
  * @param  minVal 最小值（可选）
  * @param  maxVal 最大值（可选）
  * @param  checkFun 特殊校验回调函数(返回0表示校验成功)（可选）
  */
#define COT_PARAM_ITEM_BIND(id, variable, type, attr, defVal...) COT_PARAM_ITEM_##type##_IMPL(id, #variable, variable, type, attr, ##defVal)

/**
  * @brief  添加参数
  * 
  * @note   COT_PARAM_ITEM_BIND(id, name, type, attr, defVal, minVal, maxVal, checkFun)
  * @param  id   参数表中的唯一ID
  * @param  name 参数名
  * @param  variable 变量
  * @param  type 参数类型 @enum ParamType_e 取值
  * @param  attr 属性
  * @param  defVal 默认值（可选）
  * @param  minVal 最小值（可选）
  * @param  maxVal 最大值（可选）
  * @param  checkFun 特殊校验回调函数(返回0表示校验成功)（可选）
  */
#define COT_PARAM_ITEM_BIND_WITH_NAME(id, name, variable, type, attr, defVal...) COT_PARAM_ITEM_##type##_IMPL(id, name, variable, type, attr, ##defVal)

#define COT_PARAM_ITEM_BIND_NUM(_id, _name, _variable, _type, _attr) \
                                COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, &_variable, _type, sizeof(_variable), (_attr), \
                                NULL, NULL, NULL, NULL)
#define COT_PARAM_ITEM_BIND_NUM_DEF(_id, _name, _variable, _type, _attr, _defVal) \
                                COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, &_variable, _type, sizeof(_variable), (_attr | COT_PARAM_ATTR_RESET), \
                                (void *)&(_type##_T){_defVal}, NULL, NULL, NULL)
#define COT_PARAM_ITEM_BIND_NUM_DEF_RANGE(_id, _name, _variable, _type, _attr, _defVal, _minVal, _maxVal) \
                                COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, &_variable, _type, sizeof(_variable), (_attr | COT_PARAM_ATTR_RESET | COT_PARAM_ATTR_RANGE), \
                                (void *)&(_type##_T){_defVal}, (void *)&(_type##_T){_minVal}, (void *)&(_type##_T){_maxVal}, NULL)
#define COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_CHECK(_id, _name, _variable, _type, _attr, _defVal, _minVal, _maxVal, _check) \
                                COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, &_variable, _type, sizeof(_variable), (_attr | COT_PARAM_ATTR_RESET | COT_PARAM_ATTR_RANGE), \
                                (void *)&(_type##_T){_defVal}, (void *)&(_type##_T){_minVal}, (void *)&(_type##_T){_maxVal}, _check)

#define COT_PARAM_ITEM_BIND_STR(_id, _name, _variable, _type, _attr) \
                                COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, _variable, _type, sizeof(_variable), (_attr), \
                                NULL, NULL, NULL, NULL)
#define COT_PARAM_ITEM_BIND_STR_DEF(_id, _name, _variable, _type, _attr, _defVal) \
                                COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, _variable, _type, sizeof(_variable), (_attr | COT_PARAM_ATTR_RESET), \
                                (void *)_defVal, NULL, NULL, NULL)
#define COT_PARAM_ITEM_BIND_STR_DEF_RANGE(_id, _name, _variable, _type, _attr, _defVal, _minVal, _maxVal) \
                                COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, _variable, _type, sizeof(_variable), (_attr | COT_PARAM_ATTR_RESET | COT_PARAM_ATTR_RANGE), \
                                (void *)_defVal, (void *)&(param_size_t){_minVal}, (void *)&(param_size_t){_maxVal}, NULL)
#define COT_PARAM_ITEM_BIND_STR_DEF_RANGE_CHECK(_id, _name, _variable, _type, _attr, _defVal, _minVal, _maxVal, _check) \
                                COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, _variable, _type, sizeof(_variable), (_attr | COT_PARAM_ATTR_RESET | COT_PARAM_ATTR_RANGE), \
                                (void *)_defVal, (void *)&(param_size_t){_minVal}, (void *)&(param_size_t){_maxVal}, _check)

#if COT_PARAM_USE_CUSTOM_CHECK
 #if (COT_PARAM_NAME_MAX_LENGTH > 1)
#define COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, _variable, _type, _length, _attr, _defVal, _minVal, _maxVal, _check) { \
                                          .pszName = _name, .id = _id, .type = _type, .length = _length, .attr = _attr, \
                                          .unCurValuePtr.pVoid = _variable, .unDefValuePtr.pVoid = _defVal, \
                                          .unMinValuePtr.pVoid = _minVal, .unMaxValuePtr.pVoid = _maxVal, .pfnParamCheck = _check}
 #else
#define COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, _variable, _type, _length, _attr, _defVal, _minVal, _maxVal, _check) { \
                                          .id = _id, .type = _type, .length = _length, .attr = _attr, \
                                          .unCurValuePtr.pVoid = _variable, .unDefValuePtr.pVoid = _defVal, \
                                          .unMinValuePtr.pVoid = _minVal, .unMaxValuePtr.pVoid = _maxVal, .pfnParamCheck = _check}
                                          
 #endif
#else
 #if (COT_PARAM_NAME_MAX_LENGTH > 1)
#define COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, _variable, _type, _length, _attr, _defVal, _minVal, _maxVal, _check) { \
                                          .pszName = _name, .id = _id, .type = _type, .length = _length, .attr = _attr, \
                                          .unCurValuePtr.pVoid = _variable, .unDefValuePtr.pVoid = _defVal, \
                                          .unMinValuePtr.pVoid = _minVal, .unMaxValuePtr.pVoid = _maxVal}
 #else
#define COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_IMPL(_id, _name, _variable, _type, _length, _attr, _defVal, _minVal, _maxVal, _check) { \
                                          .id = _id, .type = _type, .length = _length, .attr = _attr, \
                                          .unCurValuePtr.pVoid = _variable, .unDefValuePtr.pVoid = _defVal, \
                                          .unMinValuePtr.pVoid = _minVal, .unMaxValuePtr.pVoid = _maxVal}
 #endif
#endif

#define COT_PARAM_ITEM_COT_PARAM_INT8_IMPL(id, name, variable, type, attr, defVal...)     COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, ##defVal, \
                                      COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_CHECK, COT_PARAM_ITEM_BIND_NUM_DEF_RANGE, , COT_PARAM_ITEM_BIND_NUM_DEF, COT_PARAM_ITEM_BIND_NUM)(id, name, variable, type, attr, ##defVal)
#define COT_PARAM_ITEM_COT_PARAM_INT16_IMPL(id, name, variable, type, attr, defVal...)    COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, ##defVal, \
                                      COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_CHECK, COT_PARAM_ITEM_BIND_NUM_DEF_RANGE, , COT_PARAM_ITEM_BIND_NUM_DEF, COT_PARAM_ITEM_BIND_NUM)(id, name, variable, type, attr, ##defVal)
#define COT_PARAM_ITEM_COT_PARAM_INT32_IMPL(id, name, variable, type, attr, defVal...)    COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, ##defVal, \
                                      COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_CHECK, COT_PARAM_ITEM_BIND_NUM_DEF_RANGE, , COT_PARAM_ITEM_BIND_NUM_DEF, COT_PARAM_ITEM_BIND_NUM)(id, name, variable, type, attr, ##defVal)
#define COT_PARAM_ITEM_COT_PARAM_INT64_IMPL(id, name, variable, type, attr, defVal...)    COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, ##defVal, \
                                      COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_CHECK, COT_PARAM_ITEM_BIND_NUM_DEF_RANGE, , COT_PARAM_ITEM_BIND_NUM_DEF, COT_PARAM_ITEM_BIND_NUM)(id, name, variable, type, attr, ##defVal)
#define COT_PARAM_ITEM_COT_PARAM_UINT8_IMPL(id, name, variable, type, attr, defVal...)    COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, ##defVal, \
                                      COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_CHECK, COT_PARAM_ITEM_BIND_NUM_DEF_RANGE, , COT_PARAM_ITEM_BIND_NUM_DEF, COT_PARAM_ITEM_BIND_NUM)(id, name, variable, type, attr, ##defVal)
#define COT_PARAM_ITEM_COT_PARAM_UINT16_IMPL(id, name, variable, type, attr, defVal...)   COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, ##defVal, \
                                      COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_CHECK, COT_PARAM_ITEM_BIND_NUM_DEF_RANGE, , COT_PARAM_ITEM_BIND_NUM_DEF, COT_PARAM_ITEM_BIND_NUM)(id, name, variable, type, attr, ##defVal)
#define COT_PARAM_ITEM_COT_PARAM_UINT32_IMPL(id, name, variable, type, attr, defVal...)   COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, ##defVal, \
                                      COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_CHECK, COT_PARAM_ITEM_BIND_NUM_DEF_RANGE, , COT_PARAM_ITEM_BIND_NUM_DEF, COT_PARAM_ITEM_BIND_NUM)(id, name, variable, type, attr, ##defVal)
#define COT_PARAM_ITEM_COT_PARAM_UINT64_IMPL(id, name, variable, type, attr, defVal...)   COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, ##defVal, \
                                      COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_CHECK, COT_PARAM_ITEM_BIND_NUM_DEF_RANGE, , COT_PARAM_ITEM_BIND_NUM_DEF, COT_PARAM_ITEM_BIND_NUM)(id, name, variable, type, attr, ##defVal)
#define COT_PARAM_ITEM_COT_PARAM_FLOAT_IMPL(id, name, variable, type, attr, defVal...)    COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, ##defVal, \
                                      COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_CHECK, COT_PARAM_ITEM_BIND_NUM_DEF_RANGE, , COT_PARAM_ITEM_BIND_NUM_DEF, COT_PARAM_ITEM_BIND_NUM)(id, name, variable, type, attr, ##defVal)
#define COT_PARAM_ITEM_COT_PARAM_DOUBLE_IMPL(id, name, variable, type, attr, defVal...)   COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, ##defVal, \
                                      COT_PARAM_ITEM_BIND_NUM_DEF_RANGE_CHECK, COT_PARAM_ITEM_BIND_NUM_DEF_RANGE, , COT_PARAM_ITEM_BIND_NUM_DEF, COT_PARAM_ITEM_BIND_NUM)(id, name, variable, type, attr, ##defVal)
#define COT_PARAM_ITEM_COT_PARAM_STRING_IMPL(id, name, variable, type, attr, defVal...)   COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, ##defVal, \
                                      COT_PARAM_ITEM_BIND_STR_DEF_RANGE_CHECK, COT_PARAM_ITEM_BIND_STR_DEF_RANGE, , COT_PARAM_ITEM_BIND_STR_DEF, COT_PARAM_ITEM_BIND_STR)(id, name, variable, type, attr, ##defVal)

#define COT_PARAM_ITEM_FUN_IMPL(id, name, variable, type, attr, _1, _2, _3, _4, FUNC, ...)  FUNC

// 得到参数表的元素数目
#define COT_PARAM_TABLE_SIZE(tab)   (sizeof(tab) / sizeof(cotParamInfo_t))


extern int cotParam_Init(cotParamManager_t* pManager, cotParamInfo_t* pParamTable, uint16_t count);
extern int cotParam_ResetDefault(const cotParamManager_t *pManager);
extern int cotParam_Check(const cotParamManager_t* pManager, cotParamError_f pfnCheckError);

extern int cotParam_Load(const cotParamManager_t* pManager, cotParamLoad_f pfnLoadCallback);
extern int cotParam_Save(const cotParamManager_t* pManager, cotParamSave_f pfnSaveCallback);

extern uint32_t cotParam_GetSerializeSize(const cotParamManager_t* pManager);
extern uint32_t cotParam_Serialize(const cotParamManager_t* pManager, uint8_t *pbuf);
extern int cotParam_Deserialization(const cotParamManager_t* pManager, const uint8_t *pbuf, uint32_t length);

extern cotParamInfo_t *cotParam_IterateList(const cotParamManager_t *pManager, size_t *psIdx);

extern const cotParamInfo_t *cotParam_FindParamByName(const cotParamManager_t *pManager, const char *pszName);
extern const cotParamInfo_t *cotParam_FindParamByID(const cotParamManager_t* pManager, uint16_t id);
extern const cotParamInfo_t *cotParam_FindParamByParamPtr(const cotParamManager_t* pManager, const void *pCurParam);

extern int cotParam_SingleParamCheckProcess(const cotParamInfo_t *pParam, cotParamResetOpt_e eResetOpt);
extern int cotParam_SingleParamUpdate(const cotParamInfo_t *pParam, const void *pNewValue, cotParamResetOpt_e eResetOpt);

extern int cotParam_SingleParamCheckInput(const cotParamInfo_t *pParam, const void *pValue, cotParamCheckRet_e *peCheckResult);
extern int cotParam_SingleParamSelfCheck(const cotParamInfo_t *pParam, cotParamCheckRet_e *peCheckResult);
extern int cotParam_SingleParamResetDefValue(const cotParamInfo_t *pParam);
extern int cotParam_SingleParamResetMinValue(const cotParamInfo_t *pParam);
extern int cotParam_SingleParamResetMaxValue(const cotParamInfo_t *pParam);

#ifdef __cplusplus
 }
#endif

#endif // !_PARAM_H_
