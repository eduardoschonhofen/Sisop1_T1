
#include <stdio.h>
#include "../include/cthread.h"

void printa()
{
  printf("Entrei na thread printa\n");

}

void printa2()
{
  printf("Entrei na thread printa2\n");

}

int main()
{
  printf("Entrei na main\n");
  ccreate((void*)printa,NULL,2);
ccreate((void*)printa2,NULL,2);
  printf("Sai da main\n");

  return 0;
}
