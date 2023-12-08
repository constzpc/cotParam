#include "param_demo.h"
#include <string.h>
#include <stdio.h>

void ShowAllParam_1(void);

int main()
{
    InitParam(false);

    ShowAllParam();
    ShowAllParam_1();

    g_test = 80;
    g_test_3 = -20.5;
    sprintf(g_str_des, "sdx");

    printf("\n\n");

    SaveParam(false);
    ReloadParam(false);

    ShowAllParam();
    ShowAllParam_1();
    // int ret = COT_PARAM_DAT_SET_NEW_VALUE(g_test_6, 50, COT_PARAM_DEF);
    // printf("ret = %d, g_test_6 = %d\n", ret, g_test_6);

    // ret = COT_PARAM_DAT_SET_NEW_VALUE(g_test_6, 101, COT_PARAM_DEF);
    // printf("ret = %d, g_test_6 = %d\n", ret, g_test_6);

    // ret = COT_PARAM_DAT_SET_NEW_VALUE(g_test_6, 101, COT_PARAM_NONE);
    // printf("ret = %d, g_test_6 = %d\n", ret, g_test_6);

    // ret = COT_PARAM_DAT_SET_NEW_VALUE(g_test_6, 101, COT_PARAM_MIN_MAX);
    // printf("ret = %d, g_test_6 = %d\n", ret, g_test_6);

    // ret = COT_PARAM_DAT_SET_NEW_VALUE(g_test_6, 1, COT_PARAM_MIN_MAX);
    // printf("ret = %d, g_test_6 = %d\n", ret, g_test_6);

    // ret = COT_PARAM_DAT_SET_NEW_VALUE(g_test_6, 1, COT_PARAM_MIN_MAX);
    // printf("ret = %d, g_test_6 = %d\n", ret, g_test_6);

    // COT_PARAM_STR_SET_NEW_VALUE(g_test_str, "123aa9", COT_PARAM_NONE);
    // printf("ret = %d, g_test_str = %s\n", ret, g_test_str);

    // COT_PARAM_STR_SET_NEW_VALUE(g_test_str, "123aa1245249", COT_PARAM_DEF);
    // printf("ret = %d, g_test_str = %s\n", ret, g_test_str);

    // COT_PARAM_STR_SET_NEW_VALUE(g_test_str, "123aa945457", COT_PARAM_MIN_MAX);
    // printf("ret = %d, g_test_str = %s\n", ret, g_test_str);

    // ShowAllParam();

    return 0;
}
