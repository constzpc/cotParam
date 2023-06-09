
#include "param_demo.h"
#include <string.h>
#include <stdio.h>

void ShowAllParam(void)
{
    printf("param list:\n");
    printf("\tg_test:     %d\n", g_test);
    printf("\tg_test_2:   %d\n", g_test_2);
    printf("\tg_test_3:   %f\n", g_test_3);
    printf("\tg_test_str: %s\n", g_test_str);
    printf("\tg_test_4:   %d\n", g_test_4);
    printf("\tg_test_5:   %d\n", g_test_5);
    printf("\tg_test_6:   %d\n", g_test_6);
    printf("\tg_test_7:   %d\n", g_test_7);

}

int main()
{
    InitParam(true);

    ShowAllParam();

    g_test = 80;
    sprintf(g_test_str, "g_test_6");

    

    g_test_6 = 200;

    ParamVal_u test_6;

    test_6.u8val = 200;

    // bool ret;

    // PARAM_DECLTYPE(g_test_6) tmp = 20;


    // PARAM_UPDATE(g_test_str, "999x", ret);

    // if (!ret)
    // {
    //     printf("change %s fail\n", "g_test_str");
    // }

    // PARAM_UPDATE(g_test_6, 200, ret);

    // if (!ret)
    // {
    //     printf("change %s fail\n", "g_test_6");
    // }

    ShowAllParam();

    SaveParam();



    // ParamManager_t tParamManager;
    // ParamManager_t tParamManager2;

    // Param_Init(&tParamManager, sg_ParamTable, PARAM_TABLE_SIZE(sg_ParamTable));

    // Param_Init(&tParamManager2, sg_ParamTable2, PARAM_TABLE_SIZE(sg_ParamTable2));

    // Param_SetCallBackFun(&tParamManager, OnLoadCallback, OnSaveCallback);
    // Param_SetCallBackFun(&tParamManager2, OnLoadCallback, OnSaveCallback);

    // for (int i = 0; i < tParamManager.count; i++)
    // {
    //     char outbuf[1024];
    //     Param_Sprintf(outbuf, &tParamManager.pParamTable[i]);
    //     printf("%s\n", outbuf);
    // }

    // g_test_5 = 300;  // 设置超出范围

    // Param_Save(&tParamManager, NULL);
    // Param_Load(&tParamManager, OnCheckErrorResetHandle);

    // printf("%s\n", "outbuf");

    // for (int i = 0; i < tParamManager.count; i++)
    // {
    //     char outbuf[1024];
    //     Param_Sprintf(outbuf, &tParamManager.pParamTable[i]);
    //     printf("%s\n", outbuf);
    // }

    // g_test_5 = 400;  // 设置超出范围

    // Param_Save(&tParamManager, NULL);
    // Param_Load(&tParamManager2, OnCheckErrorHandle);

    // for (int i = 0; i < tParamManager2.count; i++)
    // {
    //     char outbuf[1024];
    //     Param_Sprintf(outbuf, &tParamManager2.pParamTable[i]);
    //     printf("%s\n", outbuf);
    // }

    return 0;
}
