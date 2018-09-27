
#include "../include/cthread.h"
#include "../include/support.h"
#include "../include/cdata.h"

#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>


#define STACKSIZE 8192

int static tid=0;
int static primeiraInit=1;

FILA2 static filaAlta;
FILA2 static  filaMedia;
FILA2 static  filaBaixa;
FILA2 static  bloqueados;
TCB_t static  executando;


PFILA2 static PfilaAlta=&filaAlta;
PFILA2 static PfilaMedia=&filaMedia;
PFILA2 static PfilaBaixa=&filaBaixa;
PFILA2 static Pbloqueados=&bloqueados;
TCB_t* Pexecutando=NULL;

// Estruturas e variaveis da cjoin
TCB_t *ThreadAtual; // Papel parecido com a Pexecutando, mas preciso do NULL pra diferenciar preempcao de termino de execucao
struct s_BloqJoin {
	int tidBloqueadora;
	TCB_t *ThreadBloqueada;
};
typedef struct s_BloqJoin	BloqCJoin;
typedef struct s_BloqJoin *	PBloqCJoin;
FILA2 joined;
PFILA2 static Pjoined=&joined;
// Final das estruturas e variaveis da cjoin

ucontext_t Tscheduler;
char scherdulerStack[STACKSIZE];


void firstThread();
void swapThread();
int inserePrioridade();

int tidBloqueando(int tid); // Testa se o tid esta bloqueando alguma thread
int tidNaFila(int tid, PFILA2 fila); // Testa se o tid em questao esta na fila passada
int desbloqueiaThread(int tid); // Desbloqueio de threads, para uso no escalonador

/******************************************************************************
Parâmetros:

Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
void scheduler()
{
  printf("Estou dentro da scheduler");

  if(Pexecutando==NULL)
  {
  printf("Entrei na primeira vez");
  firstThread();
  Pexecutando->state=PROCST_EXEC;
  setcontext(&(Pexecutando->context));
  return;
  }
  
  if(ThreadAtual != NULL) // Entra apenas se a execucao de uma thread foi finalizada
  {
	desbloqueiaThread(ThreadAtual->tid);
  {
  swapThread();
  ThreadAtual = Pexecutando;
  setcontext(&(Pexecutando->context));

}

/*******************************************************************************

********************************************************************************/
int iniciaFilas()
{

int e1=CreateFila2(&filaAlta);
int e2=CreateFila2(&filaMedia);
int e3=CreateFila2(&filaBaixa);
int e5=CreateFila2(&bloqueados);


if(e1!=0||e2!=0||e3!=0||e5!=0)
return -1;

  printf("Sai da IniciaFilas e: %d %d %d %d \n",e1,e2,e3,e5);
return 0;
}

TCB_t tMain;

/*******************************************************************************

********************************************************************************/
void saveMain()
{
  printf("Entrei na savemain \n");
  //Alocamos a thread
  //Definimos a prioridade e o id da thread
  tMain.prio=2;
  tMain.tid=0;
  tMain.state=PROCST_CRIACAO;
  //Obtemos o molde do contexto
  getcontext(&(tMain.context));
  tMain.state=PROCST_APTO;
  printf("Sai da savemain \n");

}

TCB_t* buscaFilaAlta()
{
  FirstFila2(PfilaAlta);
  TCB_t* retornaAlta = (TCB_t*)GetAtIteratorFila2(PfilaAlta);
  if(retornaAlta == NULL){
    return NULL;
  }
  else
  {
  DeleteAtIteratorFila2(PfilaAlta);
   return retornaAlta;
 }
}

TCB_t* buscaFilaMedia()
{
  FirstFila2(PfilaMedia);
  TCB_t* retornaMedia = (TCB_t*)GetAtIteratorFila2(PfilaMedia);
  if(retornaMedia == NULL){
    return NULL;
  }
  else
  {
    DeleteAtIteratorFila2(PfilaMedia);
    return retornaMedia;
  }
}
TCB_t* buscaFilaBaixa()
{
  FirstFila2(PfilaBaixa);
  TCB_t* retornaBaixa = (TCB_t*)GetAtIteratorFila2(PfilaBaixa);
  if(retornaBaixa == NULL){
    return NULL;
  }
  else
  {
    DeleteAtIteratorFila2(PfilaBaixa);
    return retornaBaixa;
  }
}
void saveScheduler()
{
  getcontext(&Tscheduler);
  Tscheduler.uc_link=0;
  Tscheduler.uc_stack.ss_sp=scherdulerStack;
  Tscheduler.uc_stack.ss_size=STACKSIZE;
  makecontext(&Tscheduler,(void(*)(void))scheduler,0);
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
  printf("Entrei na cccreate %d\n",&scheduler);
  int ok=0;
  if(primeiraInit)
  {
    ok = iniciaFilas();
    saveMain();
    saveScheduler();
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
  //Obtemos o molde do contexto
  getcontext(&(novaThread->context));
  printf("Thread criada\n");
  //Alocamos uma nova stack para o contexto.
  char* stack =(char*)malloc(STACKSIZE);
  novaThread->context.uc_stack.ss_size=STACKSIZE;
  novaThread->context.uc_stack.ss_sp=stack;
  //PROVAVELMENTE SERÁ MODIFICADO O UC_LINK
  //novaThread->context.uc_link=&(novaThread->context);
  novaThread->context.uc_link=&Tscheduler;
  //Definimos o novo contexto
  makecontext(&(novaThread->context),(void(*)(void))start,1,arg);
  novaThread->state=PROCST_APTO;
  printf("Contexto foi salvo na nova Thread\n");
  printf("%d",tMain.tid);
  printf("AAA");
  inserePrioridade(novaThread);
  printf("Inseri na prioridade\n");
  //Inserimos na fila de prioridade correta
  printf("%d",tMain.tid);
  printf("AAA\n");
//  setcontext(&(tMain.context));
scheduler();




return 0;



}

int inserePrioridade(TCB_t* novaThread)
{
  printf("AA");
  int resultado;
  novaThread->state=PROCST_APTO;
  switch(novaThread->prio)
  {
  case 0:resultado=AppendFila2(PfilaAlta,novaThread);
  break;
  case 1:resultado=AppendFila2(PfilaMedia,novaThread);
  break;
  case 2:resultado=AppendFila2(PfilaBaixa,novaThread);
  break;
  }
  return resultado;

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
	inserePrioridade(Pexecutando);
	ThreadAtual = NULL; // utilizado para informar ao escalonador que essa thread ainda nao terminou o processamento
	// acho que o swap eh necessario para que fique salvo o contexto desse exato momento, para quando a thread voltar a ser executada
	swapcontext(&Pexecutando->context, &Tscheduler);
	return 0;
}


void swapThread()
{

  getcontext(&(Pexecutando->context));
  if(Pexecutando->prio==0)
  return;

  TCB_t* thread = buscaFilaAlta();
  if(thread!=NULL)
  {
    inserePrioridade(Pexecutando);
    Pexecutando=thread;
    return;
  }
  if(Pexecutando->prio==1)
  return;
  thread=buscaFilaMedia();
  if(thread!=NULL)
  {
    inserePrioridade(Pexecutando);
    Pexecutando=thread;
    return;
  }
  if(Pexecutando->prio==2)
  return;
  thread=buscaFilaBaixa();
  if(thread!=NULL)
  {
    inserePrioridade(Pexecutando);
    Pexecutando=thread;
    return;
  }
}

void firstThread()
{

  TCB_t* thread = buscaFilaAlta();
  if(thread!=NULL)
  {
    Pexecutando=thread;
    return;
  }
  thread=buscaFilaMedia();
  if(thread!=NULL)
  {
    Pexecutando=thread;
    return;
  }
  thread=buscaFilaBaixa();
  if(thread!=NULL)
  {
    Pexecutando=thread;
    return;
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
  Pexecutando->prio=prio;
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
	if(tidBloqueando(tid) == 0) // teste se o tid informado ja esta bloqueando alguem
	{
		return -1;
	}
	// Testa se a thread do tid informado ainda exite
	if(tidNaFila(tid, PfilaAlta) != 0 && tidNaFila(tid, PfilaMedia) != 0 && tidNaFila(tid, PfilaBaixa) != 0 && tidNaFila(tid, Pbloqueados) != 0)
	{
		return -1;
	}
	
	PBloqCJoin novoBloqueio = (PBloqCJoin)malloc(sizeof(BloqCJoin));
	novoBloqueio->tidBloqueadora = tid;
	novoBloqueio->ThreadBloqueada = ThreadAtual;
	novoBloqueio->ThreadBloqueada->state = PROCST_BLOQ;
	
	AppendFila2(&bloqueados,novoBloqueio->ThreadBloqueada); // colocando nas filas de bloqueio e na especifica de joined
	AppendFila2(Pjoined,novoBloqueio);						// necessaria para podermos verificar por qual tid a thread espera
	
	ThreadAtual = NULL; // utilizado para informar ao escalonador que essa thread ainda nao terminou o processamento
	
	swapcontext(&novoBloqueio->ThreadBloqueada->context, &Tscheduler); // TROCAR POR scheduler(); ?
	return 0;
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
		if(FirstFila2(sem->fila) == 0)
		{
			TCB_t *ThreadTemp = (TCB_t *)GetAtIteratorFila2(sem->fila); // EM CASO DE PROBLEMA, COMENTE ISSO FORA. era pra funcionar.
			while(Thread->prio <= ThreadTemp->prio && endofqueue == 0)
			{
				if(NextFila2(sem->fila) == NXTFILA_ENDQUEUE)
					endofqueue = 1;
				else
					ThreadTemp = (TCB_t *)GetAtIteratorFila2(sem->fila);
			}
			if(endofqueue == 0)
				InsertBeforeIteratorFila2(sem->fila, Thread);
			else
				AppendFila2(sem->fila,Thread);
		}
		else
			AppendFila2(sem->fila,Thread);
		ThreadAtual = NULL; // utilizado para informar ao escalonador que essa thread ainda nao terminou o processamento
		scheduler(); //MUDAR PARA SWAPCONTEXT()?
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
		ThreadAtual = NULL; // utilizado para informar ao escalonador que essa thread ainda nao terminou o processamento
		scheduler();//MUDAR PARA SWAPCONTEXT()?
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

//Verifica se alguma thread espera pelo final da thread com o tid fornecido
// Retorna 0 se exite alguma, -1 se não exite;
int tidBloqueando(int tid)
{
	if(FirstFila2(Pjoined) == 0)
	{
		do
		{
			PBloqCJoin Temp = (PBloqCJoin)GetAtIteratorFila2(Pjoined);
			if(Temp->tidBloqueadora == tid)
				return 0;
		}while(NextFila2(Pjoined) == 0);
	}
	return -1;
}


//Verifica se a thread com o tid fornecido exite na fila
// Retorna 0 se exite alguma, -1 se não exite;
int tidNaFila(int tid, PFILA2 fila)
{
	if(FirstFila2(fila) == 0)
	{
		do
		{
			TCB_t * Temp = (TCB_t *)GetAtIteratorFila2(fila);
			if(Temp->tid == tid)
				return 0;
		}while(NextFila2(fila) == 0);
	}
	return -1;
}


// Função de desbloqueio de thread
// Quando uma thread termina, essa funcao recebe o tid dela
// Ela entao busca se tem alguma thread sendo bloqueada por esse tid
// caso sim, desbloqueia e retorna 0
// caso nao, segue o baile e retorna -1
int desbloqueiaThread(int tid)
{
	TCB_t * threadDesbloq = NULL;
	if(FirstFila2(Pjoined) == 0) // garante que a fila de joined nao e vazia
	{
		int exitsearch = 0;
		do // Varre a fila de joineds
		{
			PBloqCJoin bloqueio = (PBloqCJoin)GetAtIteratorFila2(Pjoined);
			if(bloqueio->tid == tid)
			{
				threadDesbloq = bloqueio->ThreadBloqueada;
				DeleteAtIteratorFila2(Pjoined);
				exitsearch = 1;
			}
		}while(NextFila2(Pjoined) == 0 && exitsearch == 0);
		if (threadDesbloq != NULL) // testa se foi encontrada alguma thread bloqueada pelo tid recebido
		{
			int exitsearch2 = 0;
			do
			{
				TCB_t* threadNavegacao = (TCB_t*)GetAtIteratorFila2(Pbloqueados);
				if(threadNavegacao->tid == threadDesbloq->tid)
				{
					DeleteAtIteratorFila2(Pbloqueados); // Retira da fila de bloqueados, pois a thread voltara para os aptos
					exitsearch2 = 1;
				}
			}while(NextFila2(Pbloqueados) == 0 && exitsearch2 == 0);
			
			inserePrioridade(threadDesbloq);
			return 0;
		}
	}
	return -1;
}