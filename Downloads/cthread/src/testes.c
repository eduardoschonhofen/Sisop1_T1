
#include <stdio.h>
#include "../include/cthread.h"

void printa()
{
  printf("AAAAA");

}

int main()
{
  printf("Entrei na main\n");
  ccreate((void*)printa,NULL,0);
  printf("Sai da main\n");

  return 0;
}
