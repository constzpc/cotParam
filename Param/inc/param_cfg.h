/**
  **********************************************************************************************************************
  * @file    param_cfg.h
  * @brief   该文件提供参数管理框架相关配置选项
  * @author  const_zpc    any question please send mail to const_zpc@163.com
  * @date    2023-03-11
  **********************************************************************************************************************
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/

#ifndef _PARAM_CFG_H_
#define _PARAM_CFG_H_

/* Includes ----------------------------------------------------------------------------------------------------------*/


#define PARAM_SERIALIZE_TAG_ENCODED 4 // 键值对 + 编码压缩序列化方式
#define PARAM_SERIALIZE_TAG         3 // 键值对序列化方式
#define PARAM_SERIALIZE_ENCODED     2 // 编码压缩序列化方式，按照ID顺序序列化
#define PARAM_SERIALIZE_STRUCT      1 // 常规序列化方式，按照ID顺序序列化

/** 参数序列化方式 */
#define PARAM_SERIALIZE_MODE        PARAM_SERIALIZE_STRUCT

/** 字符串参数长度需要使用的字节(1：最长可定义254长度的字符串参数, 2：最长可定义63335长度的字符串参数) */
#define PARAM_STRING_SIZE           1


#endif
