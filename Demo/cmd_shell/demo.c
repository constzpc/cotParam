
#include "param.h"
#include <string.h>
#include <stdio.h>

PARAM_DEFINE (test, PARAM_INT16, 10, -100, 100);
PARAM_DEFINE (test_2, PARAM_UINT16, 20, 0, 100);
PARAM_DEFINE (test_3, PARAM_DOUBLE, 3.15, -10, 10);
PARAM_DEFINE_STR (test_str, 10, "abcdef");

extern void ShowAllParam(void);
extern void SaveAllParam(void);
extern int InitAllParam(void);
extern int ResetParam(void);

extern int ModifyByName(const char *pszName, const void *pNewData);

int main ()
{
  uint32_t data = 16;
  double fdata = 7.25;

  uint8_t databuf[8];

  InitAllParam();

  ShowAllParam();

  printf("\nModify: \n");

  // 通过接口修改(参数范围限制)
  memcpy(databuf, &data, sizeof(data));
  ModifyByName("test_2", databuf);

  memcpy(databuf, &fdata, sizeof(fdata));
  ModifyByName("test_3", databuf); 

  ShowAllParam();
  
  // 在模块文件中定义的参数可以选择直接修改(没有参数范围限制)
  test = 5;
  test_3 = 8.26; 

  ShowAllParam();

  printf("\nReset: \n");
  ResetParam();
  ShowAllParam();

  return 0;
}
