
#include <stdio.h>
#include "../include/cthread.h"


void printa2()
{
  printf("Primeiro print da printa2\n");
printf("Segundo print da printa2\n");
cyield();
printf("Terceiro print da printa2\n");

}

void printa1()
{
  printf("Primeiro print da printa1\n");
  ccreate((void*)printa2,NULL,2);

cyield();
printf("Segundo print da printa1\n");

}
int main()
{
  printf("Entrei na main\n");
  ccreate((void*)printa1,NULL,2);
  cyield();

  printf("Sai da main\n");

  return 0;
}
