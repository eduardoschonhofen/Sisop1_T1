
#include "../include/cthread.h"
#include "../include/support.h"
#include "../include/cdata.h"

#include <stdio.h>


#define STACKSIZE 8192

int static tid=0;
int static primeiraInit=1;
PFILA2 static filaAlta;
PFILA2 static filaMedia;
PFILA2 static filaBaixa;
PFILA2 static bloqueados;
PFILA2 static executando;


/*******************************************************************************

********************************************************************************/
int iniciaFilas()
{


int e1=CreateFila2(filaAlta);
int e2=CreateFila2(filaMedia);
int e3=CreateFila2(filaBaixa);
int e5=CreateFila2(bloqueados);
int e6=createFila2(executando);

if(e1||e2||e3||e5||6!=0)
return -1;
return 0;
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
  int ok=0;
  if(primeiraInit)
  {
    ok = iniciaFilas();
    primeiraInit=0;
  }
  if(ok!=0)
  return -1;

  //Alocamos a thread
  TCB_t *novaThread = (TCB_t*)malloc(sizeof(TCB_t));
  //Definimos a prioridade e o id da thread
  novaThread->prio=prio;
  novaThread->tid=tid;
  tid++;

  //Definidos o estado inicial da Thread para criação
  novaThread->state=PROCST_CRIACAO;




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


/* Acredito que está errado
  //Alocamos um nodo para inserir na fila
  PNODE2* nodo = (PNODE2*)malloc(sizeof(PNODE2));

  //Criamos o nodo da Thread para inserir na fila
  //nodo->node=&novaThread;
  */

  //Inserimos na fila de prioridade correta
  switch(novaThread->prio)
  {
  case 0:AppendFila2(filaAlta,novaThread);
  break;
  case 1:AppendFila2(filaMedia,novaThread);
  break;
  case 2:AppendFila2(filaBaixa,novaThread);
  break;
  }
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
  FirstFila2(executando);
  TCB_t *Thread = (TCB_t*)GetAtIteratorFila2(executando);
  getcontext(&Thread->context);


  if(FirstFila2(filaAlta) != 0)
  {

    if (FirstFila2(filaMedia) != 0)
    {
      if(FirstFila2(filaBaixa) !=0)
      {
          return -1;
      }
      else
      {
        TCB_t *ThreadNew = (TCB_t*)GetAtIteratorFila2(filaBaixa);
        AppendFila2(executando,ThreadNew);
        if(DeleteAtIteratorFila2(filaBaixa) == 0)
        {
          NextFila2(filaBaixa);
          setcontext(&ThreadNew->context);
          return 0;
        }
      }
    }
    else
    {
      TCB_t *ThreadNew = (TCB_t*)GetAtIteratorFila2(filaMedia);
      AppendFila2(executando,ThreadNew);
      if(DeleteAtIteratorFila2(filaMedia) == 0)
      {
        NextFila2(filaMedia);
        setcontext(&ThreadNew->context);
        return 0;
      }
    }
  }
  else
  {
    TCB_t *ThreadNew = (TCB_t*)GetAtIteratorFila2(filaAlta);
    AppendFila2(executando,ThreadNew);
    if(DeleteAtIteratorFila2(filaAlta) == 0)
    {
      NextFila2(filaAlta);
      setcontext(&ThreadNew->context);
      return 0;
    }

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
  FirstFila2(executando);
  TCB_t *Thread = (TCB_t*)GetAtIteratorFila2(executando);
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
