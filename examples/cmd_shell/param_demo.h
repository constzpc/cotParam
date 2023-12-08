#ifndef _PARAM_DEMO_H_
#define _PARAM_DEMO_H_

#include "cot_param.h"


extern COT_PARAM_INT16_T g_test;
extern COT_PARAM_UINT16_T g_test_2;
extern COT_PARAM_DOUBLE_T g_test_3;
extern COT_PARAM_INT8_T g_test_4;
extern COT_PARAM_UINT32_T g_test_5;
extern COT_PARAM_UINT8_T g_test_6;
extern COT_PARAM_INT64_T g_test_7;
extern COT_PARAM_INT8_T g_sd;
extern COT_PARAM_INT64_T g_test_88;
extern char g_test_str[15];
extern char g_str_des[13];
extern char g_str_des_2[15];
extern char g_str_des_3[15];

#define COT_PARAM_UPDATE(name, val, ret)   {\
    param_##name##_t t = val;\
    SetParamNewValue(&name, &t);\
}

void InitParam(bool isReset);
void ReloadParam(bool isReset);

void ResetParam(void);
void SaveParam(bool isReset);

extern const bool SetParamNewValue(const void *curParamPtr, const void *newValue);
void ShowAllParam(void);
#endif
