#include "param_demo.h"
#include <string.h>
#include <stdio.h>

int main()
{
    InitParam(true);

    ShowAllParam();

    g_tTestVal.fValue = 20.05;

    g_tTestVal.uiValue = 50;
    SingleParamCheckProcess(&g_tTestVal.uiValue, COT_PARAM_RESET_MIN_MAX); // 修改后检查并处理：如果小于最小值则恢复最小值，大于最大值则恢复最大值

    g_tTestVal.uiValue = 50;
    if (SingleParamSelfCheck(&g_tTestVal.uiValue) != COT_PARAM_CHECK_OK) // 修改后检查
    {
        SingleParamResetResetDefValue(&g_tTestVal.uiValue); // 如果校验失败，则恢复为默认值
    }

    uint32_t tmp = 50;

    if (SingleParamCheck(&g_tTestVal.uiValue, &tmp) == COT_PARAM_CHECK_OK) // 修改前检查（参数和被检查变量值类型需要一样）
    {
        g_tTestVal.uiValue = tmp;// 如果校验成功，则修改
    }

    // g_test = 80;
    // g_test_3 = -20.5;
    sprintf(g_szString, "sd");

    SingleParamChange(&g_tTestVal.uiValue, 1500);

    SingleParamChange(&g_tTestVal.uiValue, 800); // 修改无效

    SingleParamChange(g_tTestVal.szString_1, "wwww.bau");
    SingleParamChange(g_tTestVal.szString_1, "ww");  // 修改无效

    SaveParam(true);
    ReloadParam(true);

    ShowAllParam();

    return 0;
}
