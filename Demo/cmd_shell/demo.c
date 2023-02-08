
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
  memcpy(databuf, &data, sizeof(data));
  ModifyByName("test_2", databuf);

  memcpy(databuf, &fdata, sizeof(fdata));
  ModifyByName("test_3", databuf);

  ShowAllParam();

  printf("\nReset: \n");
  ResetParam();
  ShowAllParam();

  return 0;
}
