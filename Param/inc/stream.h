/**
  **********************************************************************************************************************
  * @file    param.h
  * @brief   该文件提供参数序列化所有函数原型
  * @author  const_zpc    any question please send mail to const_zpc@163.com
  * @version V0.1
  * @date    2023-02-07
  **********************************************************************************************************************
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef _PARAM_STREAM_H_
#define _PARAM_STREAM_H_

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "param_type.h"

#ifdef __cplusplus
 extern "C" {
#endif 

extern uint32_t StreamToBin(uint8_t *pbuf, const ParamInfo_t *pParamTab, uint16_t num);
extern void StreamFromBin(uint8_t *pbuf, uint32_t length, ParamInfo_t *pParamTab, uint16_t num);

#ifdef __cplusplus
 }
#endif

#endif
