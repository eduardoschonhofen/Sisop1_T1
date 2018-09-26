
#include <stdio.h>
#include "../include/cthread.h"

void printa()
{
  printf("AAAAA");

}

int main()
{
  ccreate((void*)printa,NULL,0);

  return 0;
}
