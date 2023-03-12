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



uint8_t *PackParam(uint8_t *pBuf, const ParamInfo_t *pParam, uint8_t isUseEncoded, uint8_t opt);
uint8_t *PackTagAndParam(uint8_t *pBuf, const ParamInfo_t *pParam, uint8_t isUseEncoded, uint8_t opt);
uint8_t *PackTagAndOptAndParam(uint8_t *pBuf, const ParamInfo_t *pParam, uint8_t isUseEncoded, uint8_t opt);

uint8_t *UnPackParam(uint8_t *pBuf, ParamInfo_t *pParam, uint8_t isUseDecode, uint8_t opt);
uint8_t *UnPackTagAndParam(uint8_t *pBuf, ParamInfo_t *pParam, uint8_t isUseDecode, uint8_t opt);
uint8_t *UnPackTagAndOptAndParam(uint8_t *pBuf, ParamInfo_t *pParam);

#ifdef __cplusplus
 }
#endif

#endif
