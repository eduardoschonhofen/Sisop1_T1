
#include <stdio.h>
#include "../include/cthread.h"
#include "cthread.c"
#include "../include/support.h"
#include "../include/cdata.h""

void printa()
{
  printf("AAAAA");

}

int main()
{
  printf("Entrei na main\n");
  ccreate((void*)printa,NULL,2);
  printf("Sai da main\n");

  return 0;
}
