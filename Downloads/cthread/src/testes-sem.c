
#include <stdio.h>
#include "../include/cthread.h"

csem_t semaforo;
//csem_t * sem = &semaforo;

void printa3()
{
  printf("Primeiro print da printa3\n");
  cwait(&semaforo);
  printf("Area Critica da print3\n");
  cyield();
  printf("Area Critica da print3 pos cyield\n");
  csignal(&semaforo);

printf("fora da area critica da print3\n");


}


void printa2()
{
  printf("Primeiro print da printa2\n");
ccreate((void*)printa3,NULL,0);
  printf("pos criacao printa3\n");
  cwait(&semaforo);
  printf("Area Critica da print2\n");
  cyield();
  printf("Area Critica da print2 pos cyield\n");
  csignal(&semaforo);

printf("fora da area critica da print2\n");


}

void printa1()
{
printf("Primeiro print da printa1\n");
  if(csem_init(&semaforo, 1) != 0)
	{
		printf("FUCK");
		return;
	}
else
printf("NOICE\n");
  ccreate((void*)printa2,NULL,1);
  printf("Area Critica da print1 antes do cwait\n");
  cwait(&semaforo);
  cyield();
  cyield();
  printf("Area Critica da print1\n");
  csignal(&semaforo);
//cyield();
printf("fora da area critica da print1\n");

}
int main()
{
  printf("Entrei na main\n");
  ccreate((void*)printa1,NULL,1);

  printf("Sai da main\n");

  return 0;
}
