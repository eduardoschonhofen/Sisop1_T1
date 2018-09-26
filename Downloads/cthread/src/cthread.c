
#include "../include/cthread.h"
#include "../include/support.h"
#include "../include/cdata.h"

#include <stdio.h>


#define STACKSIZE 8192

int static tid=0;
int static primeiraInit=1;

FILA2 static filaAlta;
FILA2 static filaMedia;
FILA2 static filaBaixa;
FILA2 static bloqueados;
FILA2 static executando;


PFILA2 static PfilaAlta=&filaAlta;
PFILA2 static PfilaMedia=&filaMedia;
PFILA2 static PfilaBaixa=&filaBaixa;
PFILA2 static Pbloqueados=&bloqueados;
PFILA2 static Pexecutando=&executando;


/*******************************************************************************

********************************************************************************/
int iniciaFilas()
{
  printf("Entrei na IniciaFilas \n");

int e1=CreateFila2(PfilaAlta);
printf("Criei fila alta com ponteiro \n");


int e2=CreateFila2(PfilaMedia);
int e3=CreateFila2(PfilaBaixa);
int e5=CreateFila2(Pbloqueados);
int e6=CreateFila2(Pexecutando);


if(e1!=0||e2!=0||e3!=0||e5!=0||e6!=0)
return -1;

  printf("Sai da IniciaFilas e: %d %d %d %d %d \n",e1,e2,e3,e5,e6);
return 0;
}


/*******************************************************************************

********************************************************************************/
saveMain()
{
  printf("Entrei na savemain \n");
  //Alocamos a thread
  TCB_t *novaThread = (TCB_t*)malloc(sizeof(TCB_t));
  //Definimos a prioridade e o id da thread
  novaThread->prio=2;
  novaThread->tid=0;
  novaThread->state=PROCST_CRIACAO;
  //Obtemos o molde do contexto
  getcontext(&novaThread->context);
  novaThread->state=PROCST_APTO;
  AppendFila2(&filaBaixa,novaThread);

  FirstFila2(&filaBaixa);
  TCB_t *ThreadNew = (TCB_t*)GetAtIteratorFila2(&filaBaixa);

  printf("Valor da main: %d = %d --- %d = %d --- %d = %d\n ",novaThread->prio,ThreadNew->prio,novaThread->tid,ThreadNew->tid,novaThread->state,ThreadNew->state);

  printf("Sai da savemain \n");

}


/******************************************************************************
Parâmetros:
	start:	ponteiro para a função que a thread executará.
	arg:	um parâmetro que pode ser passado para a thread na sua criação.
	prio:	NÃO utilizado neste semestre, deve ser sempre zero.
Retorno:
	Se correto => Valor positivo, que representa o identificador da thread criada
	Se erro	   => Valor negativo.
******************************************************************************/
int ccreate (void* (*start)(void*), void *arg, int prio)
{

  printf("Entrei na cccreate\n");

  int ok=0;
  if(primeiraInit)
  {
    ok = iniciaFilas();
    saveMain();
    primeiraInit=0;
  }
  if(ok!=0)
  return -1;

  printf("Sai do if(PrimeiraInit)\n");


  printf("Aloquei a nova thread\n");

  //Alocamos a thread
  TCB_t *novaThread = (TCB_t*)malloc(sizeof(TCB_t));
  //Definimos a prioridade e o id da thread
  novaThread->prio=prio;
  novaThread->tid=tid;
  tid++;

  //Definidos o estado inicial da Thread para criação
  novaThread->state=PROCST_CRIACAO;

  printf("Thread foi criada com sucesso \n");




  //Obtemos o molde do contexto
  getcontext(&novaThread->context);

  //Alocamos uma nova stack para o contexto.
  char* stack =(char*)malloc(STACKSIZE);
  novaThread->context.uc_stack.ss_size=STACKSIZE;
  novaThread->context.uc_stack.ss_sp=stack;
  //PROVAVELMENTE SERÁ MODIFICADO O UC_LINK
//  novaThread->context.uc_stack.uc_link=novaThread->context;

  //Definimos o novo contexto
  makecontext(&novaThread->context,start,arg);
  novaThread->state=PROCST_APTO;

  printf("Contexto foi salvo na nova Thread \n");


 //Acredito que está errado
  //Alocamos um nodo para inserir na fila
  //PNODE2* nodo = (PNODE2*)malloc(sizeof(PNODE2));

  //Criamos o nodo da Thread para inserir na fila
  //nodo->node=&novaThread;

  printf("Entrei no switch\n");
  //Inserimos na fila de prioridade correta
  switch(novaThread->prio)
  {
  case 0:AppendFila2(PfilaAlta,novaThread);
  break;
  case 1:AppendFila2(PfilaMedia,novaThread);
  break;
  case 2:{printf("Entrei no 2\n");
    AppendFila2(PfilaBaixa,novaThread);
  printf("Sai do 2\n");}
  break;
  }
  printf("Entrei na escalona\n");
  escalona();

return 0;
}

/******************************************************************************
Parâmetros:
	Sem parâmetros
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
int cyield(void)
{



}


/******************************************************************************
Parâmetros:

Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
void escalona()
{
  int* e1 = FirstFila2(&executando);
  printf("Estou na escalona %d",e1);
  if(FirstFila2(&executando)!=0)
  {
    if(FirstFila2(&filaAlta) != 0)
    {


      if (FirstFila2(&filaMedia) != 0)
      {
        if(FirstFila2(&filaBaixa) !=0)
        {
            return -1;
        }
        else
        {
          TCB_t *ThreadNew = (TCB_t*)GetAtIteratorFila2(&filaBaixa);
          ThreadNew->state=PROCST_EXEC;
          AppendFila2(&executando,ThreadNew);
          if(DeleteAtIteratorFila2(&filaBaixa) == 0)
          {
            NextFila2(&filaBaixa);
            setcontext(&ThreadNew->context);

          }
        }
      }
      else
      {
        TCB_t *ThreadNew = (TCB_t*)GetAtIteratorFila2(&filaMedia);
        ThreadNew->state=PROCST_EXEC;
        AppendFila2(&executando,ThreadNew);
        if(DeleteAtIteratorFila2(&filaMedia) == 0)
        {
          NextFila2(&filaMedia);
          setcontext(&ThreadNew->context);

        }
      }
    }
    else
    {
      TCB_t *ThreadNew = (TCB_t*)GetAtIteratorFila2(&filaAlta);
      ThreadNew->state=PROCST_EXEC;
      AppendFila2(&executando,ThreadNew);
      if(DeleteAtIteratorFila2(&filaAlta) == 0)
      {
        NextFila2(&filaAlta);
        setcontext(&ThreadNew->context);

      }

    }
  }
  TCB_t *Thread = (TCB_t*)GetAtIteratorFila2(&executando);
  getcontext(&Thread->context);

  if(Thread->prio==0)
    return 0;

  printf("Prioridade:%d",Thread->prio);

  if(FirstFila2(&filaAlta) != 0)
  {
    if(Thread->prio==1)
    return 0;

    if (FirstFila2(&filaMedia) != 0)
    {
      if(Thread->prio==2)
      return 0;
      if(FirstFila2(&filaBaixa) !=0)
      {
          return -1;
      }
      else
      {

        TCB_t *ThreadNew = (TCB_t*)GetAtIteratorFila2(&filaBaixa);
        ThreadNew->state=PROCST_EXEC;
        AppendFila2(&executando,ThreadNew);
        if(DeleteAtIteratorFila2(&filaBaixa) == 0)
        {
          NextFila2(&filaBaixa);
          setcontext(&ThreadNew->context);

        }
      }
    }
    else
    {
      TCB_t *ThreadNew = (TCB_t*)GetAtIteratorFila2(&filaMedia);
      ThreadNew->state=PROCST_EXEC;
      AppendFila2(&executando,ThreadNew);
      if(DeleteAtIteratorFila2(&filaMedia) == 0)
      {
        NextFila2(&filaMedia);
        setcontext(&ThreadNew->context);

      }
    }
  }
  else
  {
    TCB_t *ThreadNew = (TCB_t*)GetAtIteratorFila2(&filaAlta);
    ThreadNew->state=PROCST_EXEC;
    AppendFila2(&executando,ThreadNew);
    if(DeleteAtIteratorFila2(&filaAlta) == 0)
    {
      NextFila2(&filaAlta);
      setcontext(&ThreadNew->context);

    }

  }


  Thread->state=PROCST_APTO;
    switch(Thread->prio)
    {
    case 0:AppendFila2(&filaAlta,Thread);
    break;
    case 1:AppendFila2(&filaMedia,Thread);
    break;
    case 2:AppendFila2(&filaBaixa,Thread);
    break;
    }

}

/******************************************************************************
Parâmetros:
	tid: identificador da thread cuja prioridade será alterada (deixar sempre esse campo como NULL em 2018/02)
	prio: nova prioridade da thread.
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
int csetprio(int tid, int prio)
{
  FirstFila2(&executando);
  TCB_t *Thread = (TCB_t*)GetAtIteratorFila2(&executando);
  Thread->prio=prio;
}

/******************************************************************************
Parâmetros:
	tid:	identificador da thread cujo término está sendo aguardado.
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
int cjoin(int tid)
{


}

/******************************************************************************
Parâmetros:
	sem:	ponteiro para uma variável do tipo csem_t. Aponta para uma estrutura de dados que representa a variável semáforo.
	count: valor a ser usado na inicialização do semáforo. Representa a quantidade de recursos controlados pelo semáforo.
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
int csem_init(csem_t *sem, int count)
{
	sem = malloc(sizeof(csem_t));
	sem->count = count;
	return CreateFila2(sem->fila);
}

/******************************************************************************
Parâmetros:
	sem:	ponteiro para uma variável do tipo semáforo.
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
int cwait(csem_t *sem)
{
	sem->count = sem->count - 1;
	if(sem->count < 0)
	{
		// thread bloqueada, entra na fila do semaforo
		TCB_t *Thread;
		getcontext(&Thread->context);
		Thread->state=PROCST_BLOQ;
		int endofqueue = 0;
		// Posiciona a thread na fila por ordem de prioridade e então de idade
		// Vou dar uma simplificada nessa busca pq tem coisa repetida, mas nao to conseguindo mais
		if(FirstFila2(sem->fila) == 0)
		{
			while(Thread->prio <= (TCB_t *)GetAtIteratorFila2(sem->fila)->prio && endofqueue == 0) 
			{
				if(NextFila2(sem->fila) == NXTFILA_ENDQUEUE)
					endofqueue = 1;
			}
			if(endofqueue == 0)
				InsertBeforeIteratorFila2(sem->fila, Thread);
			else
				AppendFila2(sem->fila,Thread);
		}
		else
			AppendFila2(sem->fila,Thread);
		escalona();
	}
}

/******************************************************************************
Parâmetros:
	sem:	ponteiro para uma variável do tipo semáforo.
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
int csignal(csem_t *sem)
{
	sem->count = sem->count + 1;
	if(sem->count >= 0)
	{
		// Libera a thread de maior prioridade e idade da fila do semaforo para estado apto
		FirstFila2(sem->fila);
		TCB_t *ThreadNew = (TCB_t*)GetAtIteratorFila2(sem->fila);
		DeleteAtIteratorFila2(sem->fila);
		ThreadNew->state=PROCST_APTO;
		escalona();
	}
}

/******************************************************************************
Parâmetros:
	name:	ponteiro para uma área de memória onde deve ser escrito um string que contém os nomes dos componentes do grupo e seus números de cartão.
		Deve ser uma linha por componente.
	size:	quantidade máxima de caracteres que podem ser copiados para o string de identificação dos componentes do grupo.
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
int cidentify (char *name, int size)
{


}
