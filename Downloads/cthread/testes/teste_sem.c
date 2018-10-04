
/*
 *	Programa de exemplo de uso da biblioteca cthread
 *
 *	Versão 1.0 - 14/04/2016
 *
 *	Sistemas Operacionais I - www.inf.ufrgs.br
 *
 *
 *	Modificado para demonstracao da funcionalidade de Semaforo
 *	03/10/2018
 */

#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>
csem_t semaforo;


void* func1(void *arg) {
	printf("4 - func1 - Inicio da func1\n");
	cwait(&semaforo);
	printf("6 - func1 - Area Critica da func1\n");
	csignal(&semaforo);
	printf("7 - func1 - apos Area Critica da func1\n");
	return;
}

void* func0(void *arg) {
	printf("1 - func0 - Inicio da func0, antes do ccreate da func1\n");
	int i;
	int id1 = ccreate(func1, (void *)&i, 0);
	csem_init(&semaforo, 1);
	printf("2 - func0 -  Semaforo iniciado\n");
	cwait(&semaforo);
	printf("3 - func0 - Area Critica da func0 - pre yield\n");
	cyield();
	printf("5 - func0 - Area Critica da func0 - pos yield\n");
	csignal(&semaforo);
	printf("8 - func0 - apos Area Critica da func0\n");
	cjoin(id1);
	return;
}

int main(int argc, char *argv[]) {

	int id0;
	int i;

	printf("0 - main - Inicio da main, antes do primeiro ccreate\n");
	id0 = ccreate(func0, (void *)&i, 0);
	

	printf("9 - main - main após a criação de ID0 e ID1\n");

	cjoin(id0);

	printf("10 - main - main voltando para terminar o programa\n");
}

