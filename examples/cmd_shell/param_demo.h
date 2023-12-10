#ifndef _PARAM_DEMO_H_
#define _PARAM_DEMO_H_

#include "cot_param.h"

typedef struct
{
    uint16_t usValue;
    uint8_t ucValue;
    uint32_t uiValue;
    float fValue;
    char szString_1[12];
    double dValue;
    int16_t sValue;
    int8_t cValue;
    int32_t iValue;
    char szString_2[10];
}ParamDemo_t;

extern ParamDemo_t g_tTestVal;
extern int8_t g_cTest;
extern char g_szString[10];


void InitParam(bool isReset);
void ReloadParam(bool isReset);

void ResetParam(void);
void SaveParam(bool isReset);

int SingleParamCheckProcess(const void *pCurParam, cotParamResetOpt_e eResetOpt);

cotParamCheckRet_e SingleParamCheck(const void *pCurParam, const void *pCheckValue);

cotParamCheckRet_e SingleParamSelfCheck(const void *pCurParam);

void SingleParamResetResetDefValue(const void *pCurParam);

void ShowAllParam(void);
#endif
