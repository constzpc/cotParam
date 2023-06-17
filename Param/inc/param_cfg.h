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

/** 参数保存时是否采用键值对方式保存
 * 1,采用键值对, 该方式在参数表更改时可以向下兼容
 * 0,禁止, 该方式在参数表更改时可以不一定兼容, 删除/添加表中参数时无法兼容, 即使不使用也不能删除, 只有在参数表末尾不断添加才能保证向下兼容
 *  */
#define PARAM_USE_KEY_VALUE         1

/**
  * @brief  使用字符串参数类型
  * 0,禁止; 1,使能
  */
#define PARAM_USE_STRING_TYPE       1

/** 参数名字最大定义长度（包括结束符'\0'）, 因此小于或等于1则禁用参数名字字符串相关功能 */
#define PARAM_NAME_MAX_LENGTH       15

#if PARAM_USE_STRING_TYPE
/** 字符串类型的参数取值最大定义长度（包括结束符） */
#define PARAM_STRING_MAX_LENGTH     15
#endif

#if PARAM_USE_KEY_VALUE

/** 最多支持多少个参数 */
#define PARAM_SUPPORT_NUM           PARAM_SUPPORT_16
#define PARAM_SUPPORT_16            1
#define PARAM_SUPPORT_256           2
#define PARAM_SUPPORT_4096          3

#endif

#if PARAM_USE_KEY_VALUE
    #if (PARAM_SUPPORT_NUM == PARAM_SUPPORT_16)
        #if PARAM_STRING_MAX_LENGTH >= 16
            #error "Param: The maximum length of a string can not over 15"
        #endif
    #elif (PARAM_SUPPORT_NUM == PARAM_SUPPORT_256)
        #if PARAM_STRING_MAX_LENGTH >= 256
            #error "Param: The maximum length of a string can not over 256"
        #endif
    #elif (PARAM_SUPPORT_NUM == PARAM_SUPPORT_4096)
        #if PARAM_STRING_MAX_LENGTH >= 4096
            #error "Param: The maximum length of a string can not over 4096"
        #endif
    #endif
#endif


#endif
