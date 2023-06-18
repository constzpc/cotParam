#ifndef _PARAM_DEMO_H_
#define _PARAM_DEMO_H_

#include "param.h"

PARAM_EXTERN_DAT(g_test, PARAM_INT16);
PARAM_EXTERN_DAT(g_test_2, PARAM_UINT16);
PARAM_EXTERN_DAT(g_test_3, PARAM_DOUBLE);
PARAM_EXTERN_STR(g_test_str, 10);
PARAM_EXTERN_DAT(g_test_4, PARAM_INT8);
PARAM_EXTERN_DAT(g_test_5, PARAM_UINT32);
PARAM_EXTERN_DAT(g_test_6, PARAM_UINT8);
PARAM_EXTERN_DAT(g_test_7, PARAM_INT64);
PARAM_EXTERN_BIND_DAT(sg_tTest_test1, PARAM_UINT16);
PARAM_EXTERN_BIND_DAT(sg_tTest_test2, PARAM_FLOAT);
PARAM_EXTERN_BIND_STR(sg_tTest_str, 20);


#define PARAM_UPDATE(name, val, ret)   {\
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
