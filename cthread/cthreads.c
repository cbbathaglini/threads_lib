#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#include "cthread.h"
#include "support.h"
#include "cdata.h"

/* Funções auxiliares */
TCB_t* searchTID(PFILA2 pFila, int tid);

/* Variáveis Globais */
int tid = 0;

/* Filas */

FILA2 aptoAltaPrior
PFILA2 APTO_ALTA = &aptoAltaPrior
FILA2 aptoMediaPrior
PFILA2 APTO_MEDIA = &aptoMediaPrior
FILA2 aptoBaixaPrior
PFILA2 APTO_BAIXA = &aptoBaixaPrior
FILA2 filaBloqueado
PFILA2 BLOQUEADO = &filaBloqueado
FILA2 filaExecutando
PFILA2 F_EXECUTANDO = &filaExecutando
FILA2 filaJoin
PFILA2 JOIN = &filaJoin


/* Ponteiro para o TCB em execução */
TCB_t *EXECUTANDO;

/* Variável u_context para facilitar o retorno */
ucontext_t r_context;


int ccreate (void *(*start) (void *), void *arg, int prio)
{
	TCB_t* new_thread;
	// Fazer tratamento caso o tid seja 0, que é a thread main;
	/* Nesse caso, criar as filas, alocar um tcb para main, colocá-la no executando
	"Para a criação desse contexto devem  ser utilizadas as mesmas chamadas
	getcontext() e makecontext(), usadas na criação de threads com a ccreate."
	*/

	new_thread = (TCB_t*) malloc(sizeof(TCB_t));
	
	new_thread->tid = tid;
	new_thread->prio = prio; // a prioridade da thread foi passada como parâmetro 
	
	
	
	/* Making thread context */
	getcontext(&new_thread->context);
	new_thread->context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
	new_thread->context.uc_stack.ss_size = SIGSTKSZ;
	new_thread->context.uc_link = &control.ended_thread;
	makecontext(&new_thread->context, (void (*)(void))start, 1, arg);


	/* Put it into all_treads and able_threads */
	if (!rb_insert(control.all_threads, new_thread->tid, new_thread))
		return FALSE;
	if (!rb_able_insert(new_thread->tid))
		return FALSE;


	return new_thread->tid;
}

// Cedência voluntária:
int cyield(void){
	return 0;
}

// Setar prioridade:
int csetprio(int tid, int prio){
		//Aparentemente é pra deixar tid em null em 2018/2, então o csetprio funciona apenas na thread corrente.
		if(prio >= 0 && prio <= 2){
			if(EXECUTANDO->prio > prio){
			// Executar caso o processo tenha baixado a prioridade, executar o escalonador
			}
			else{
				EXECUTANDO->prio = prio;
			}
		}
		else //valor de prioridade é inválido, deve estar entre [0,2].
			return -1;
}

int cjoin(int tid);

int csem_init(csem_t *sem, int count);

int cwait(csem_t *sem);

int csignal(csem_t *sem);

int cidentify (char *name, int size);

TCB_t* searchTID(PFILA2 pFila, int tid)
 /*Procura numa fila se existe o processo de tid e retorna | um ponteiro para o TCB caso positivo
  																												 | NULL caso contrário*/
{
    TCB_t* tcb;

    if (FirstFila2(pFila))
        return NULL;

    while((tcb=GetAtIteratorFila2(pFila)) != NULL){
        if(tcb->tid == tid)
            return tcb;
        else
            if (NextFila2(pFila))
                return NULL;
    }
	return NULL;
}
