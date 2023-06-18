
#include "param_demo.h"
#include <string.h>
#include <stdio.h>


int main()
{
    InitParam(true);

    ShowAllParam();

    g_test = 80;
    g_test_3 = -20.5;

    SaveParam(false);
    ReloadParam(false);

    int ret = PARAM_DAT_SET_NEW_VALUE(g_test_6, 50, PARAM_DEF);
    printf("ret = %d, g_test_6 = %d\n", ret, g_test_6);

    ret = PARAM_DAT_SET_NEW_VALUE(g_test_6, 101, PARAM_DEF);
    printf("ret = %d, g_test_6 = %d\n", ret, g_test_6);

    ret = PARAM_DAT_SET_NEW_VALUE(g_test_6, 101, PARAM_NONE);
    printf("ret = %d, g_test_6 = %d\n", ret, g_test_6);

    ret = PARAM_DAT_SET_NEW_VALUE(g_test_6, 101, PARAM_MIN_MAX);
    printf("ret = %d, g_test_6 = %d\n", ret, g_test_6);

    ret = PARAM_DAT_SET_NEW_VALUE(g_test_6, 1, PARAM_MIN_MAX);
    printf("ret = %d, g_test_6 = %d\n", ret, g_test_6);

    PARAM_STR_SET_NEW_VALUE(g_test_str, "123aa9", PARAM_NONE);
    printf("ret = %d, g_test_str = %s\n", ret, g_test_str);

    PARAM_STR_SET_NEW_VALUE(g_test_str, "123aa1245249", PARAM_DEF);
    printf("ret = %d, g_test_str = %s\n", ret, g_test_str);

    PARAM_STR_SET_NEW_VALUE(g_test_str, "123aa945457", PARAM_MIN_MAX);
    printf("ret = %d, g_test_str = %s\n", ret, g_test_str);

    ShowAllParam();

    return 0;
}
